#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "tiles.h"


int debug_enabled = 0;


/*
 * Prints sandpile as vector
 */
void printBoard(struct Board_square *b){
  for (int i = 0; i < b->height; i++){
    for (int j = 0; j < b->width; j++){
      if (j == 0)
        printf("[");
      printf(" %d ", b->tiles[j][i].value);
      if (j != (b->width - 1))
        printf(",");
    }
    printf("]\n");
  }
  printf("\n");
}


/*
 * Overflows a square
 */
void overflowSq(struct Board_square *b, int x, int y){
  // increment squares in y-dir
  if (y == 0){
    b->tiles[x][y + 1].value++;
  } else if (y == (b->height - 1)){
    b->tiles[x][y - 1].value++;
  } else {
    b->tiles[x][y - 1].value++;
    b->tiles[x][y + 1].value++;
  }

  // increment squares in x-dir
  if (x == 0){
    b->tiles[x + 1][y].value++;
  } else if (x == (b->width - 1)){
    b->tiles[x - 1][y].value++;
  } else {
    b->tiles[x - 1][y].value++;
    b->tiles[x + 1][y].value++;
  }

  b->tiles[x][y].value -= 4;
  if (b->tiles[x][y].value < 0)
    b->tiles[x][y].value = 0;
}


/*
 * Handles overflows
 */
bool checkOverflowSq(struct Board_square *b){
  bool overflow_happened = FALSE;

  for (int i = 0; i < b->height; i++){
    for (int j = 0; j < b->width; j++){
      if (b->tiles[j][i].value > b->threshold){
        overflow_happened = TRUE;
        overflowSq(b, j, i);
      }
    }
  }

  if (debug_enabled)
    printBoard(b);

  return overflow_happened;
}


/*
 * Adds two sandpiles together
 */
void addBoards(struct Board_square *b1, struct Board_square *b2){
  bool overflow = TRUE;

  if ((b1->height * b1->width) != (b2->height * b2->width)){
    printf("Sandpiles not same size\n");
    return;
  }

  for (int i = 0; i < b1->height; i++){
    for (int j = 0; j < b1->width; j++){
      b1->tiles[j][i].value += b2->tiles[j][i].value;
    }
  }

  while (overflow == TRUE){
    overflow = checkOverflowSq(b1);
  }
}


/*
 * Initialize board tiles with values given in an array
 */
void initializeBoard(struct Board_square *b, int grid[3][3]){
  for (int i = 0; i < b->height; i++){
    for (int j = 0; j < b->width; j++){
      b->tiles[j][i].value = grid[j][i];
    }
  }
}


/*
 * 
 */
int main(int argc, char *argv[]){
  struct Board_square grid1;
  struct Board_square grid2;
  int initValues1[3][3] = { { 2, 2, 2 }, 
                            { 2, 3, 2 }, 
                            { 2, 2, 2 } };
  int initValues2[3][3] = { { 1, 1, 1 }, 
                            { 1, 3, 1 }, 
                            { 1, 1, 1 } };
  int width = 3;
  int height = 3;
  int threshold = 3;
  int opt;

  opterr = 0;
  while ((opt = getopt(argc, argv, "dht")) != -1){
    switch (opt){
      case 'd': // debug
        debug_enabled = 1;
        break;
      case 'h': // help
      default:
        printf("USAGE: %s\n", argv[0]);
        printf("OPTIONS: -dh\n");
        printf("\t-d: debug mode\n");
        printf("\t-h: help\n");
        return 0;
    }
  }
      


  // allocate 2D arrays
  grid1.tiles = (struct Tile_square **)malloc(sizeof(struct Tile_square *) * height);
  for (int i = 0; i < height; i++)
    grid1.tiles[i] = (struct Tile_square *)malloc(sizeof(struct Tile_square) * width);
  grid2.tiles = (struct Tile_square **)malloc(sizeof(struct Tile_square *) * height);
  for (int i = 0; i < height; i++)
    grid2.tiles[i] = (struct Tile_square *)malloc(sizeof(struct Tile_square) * width);
  // initialize boards
  grid1.width = width;
  grid2.width = width;
  grid1.height = height;
  grid2.height = height;
  grid1.threshold = threshold;
  grid2.threshold = threshold;
  initializeBoard(&grid1, initValues1);
  initializeBoard(&grid2, initValues2);
  printf("--- Board 1 ---\n");
  printBoard(&grid1);
  printf("--- Board 2 ---\n");
  printBoard(&grid2);
  printf("--- Add Boards ---\n");

  // add boards
  addBoards(&grid1, &grid2);
  printf("--- Board ---\n");
  printBoard(&grid1);

}