#include <stdio.h>

#define TILE_SIZE 65 * 65
#define MASK_SIZE 256 * 256

enum TerrainChildren {
  TC_SW = 1,                    // 2^0, bit 0
  TC_SE = 2,                    // 2^1, bit 1
  TC_NW = 4,                    // 2^2, bit 2
  TC_NE = 8                     // 2^3, bit 3
};

class TerrainTile {
public:
  TerrainTile() {};

  TerrainTile(FILE *fp) {
    unsigned char bytes[2];
    int count = 0;

    while ( count < TILE_SIZE && fread(bytes, 2, 1, fp) != 0) {
      /* adapted from
         <http://stackoverflow.com/questions/13001183/how-to-read-little-endian-integers-from-file-in-c> */
    
      mHeights[count++] = bytes[0] | (bytes[1]<<8);
    }
  
    if ( fread(&(mChildren), 1, 1, fp) != 1) {
      throw 1;
    }

    if ( fread(mMask, 1, MASK_SIZE, fp) != MASK_SIZE ) {
      throw 2;
    }
  }

  void print() {
    int count;
    for (count = 0; count < TILE_SIZE; count++) {
      printf("height: %d\n", mHeights[count]);
    }

    for (count = 0; count < MASK_SIZE; count++) {
      printf("mask: %d\n", mMask[count] & 0xFF);
    }
  }

  void writeFile(FILE *fp) {
      fwrite(&mHeights, TILE_SIZE * 2, 1, fp);
      fwrite(&mChildren, 1, 1, fp);
      fwrite(mMask, MASK_SIZE, 1, fp);
  }

  /* for a discussion on bitflags see
     <http://www.dylanleigh.net/notes/c-cpp-tricks.html#Using_"Bitflags"> */
  inline bool hasChildSW() {
    return ((mChildren & TC_SW) == TC_SW);
  }
  inline bool hasChildSE() {
    return ((mChildren & TC_SE) == TC_SE);
  }
  inline bool hasChildNW() {
    return ((mChildren & TC_NW) == TC_NW);
  }
  inline bool hasChildNE() {
    return ((mChildren & TC_NE) == TC_NE);
  }
  
private:
  short int mHeights[TILE_SIZE];
  char mChildren;
  char mMask[MASK_SIZE];
};
