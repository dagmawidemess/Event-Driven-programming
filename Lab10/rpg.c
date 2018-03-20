// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

//CMPE13 Support Library
#include "UNIXBOARD.h"

#include "Player.h"
#include "Game.h"

// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****

int main()
{

  GameInit();
  do {
    char title[GAME_MAX_ROOM_TITLE_LENGTH];
    printf("\33[s\33[2J");
    printf("\33[1;1f");
    
    GameGetCurrentRoomTitle(title);
    printf( "\33[31;47m%s\n",title);
    
    char description[GAME_MAX_ROOM_DESC_LENGTH];
    GameGetCurrentRoomDescription(description);
    printf("\33[0m%s\n", description);
    
    uint8_t exits = GameGetCurrentRoomExits();
    
    printf("\33[10B");
    if(exits & GAME_ROOM_EXIT_NORTH_EXISTS) {
      printf("\33[10C\33[32mNorth\n");
    } else {
      printf("\33[10C\33[31mNorth\n");
    }
    
    if(exits & GAME_ROOM_EXIT_EAST_EXISTS) {
      printf("\33[32mEast");
    } else {
      printf("\33[31mEast");
    }
    
    if(exits & GAME_ROOM_EXIT_WEST_EXISTS) {
      printf("\33[20C\33[32mWest\n");
    }else {
      printf("\33[20C\33[31mWest\n");
    }
    
    if(exits & GAME_ROOM_EXIT_SOUTH_EXISTS) {
      printf("\33[10C\33[32mSouth\n");
    }else {
      printf("\33[10C\33[31mSouth\n");
    }
    
    printf("\33[0mEnter your choice (s, w, e, n or q): ");
    int input;
    do
    {
      input = getchar();
    } while ( input == '\n' );
    
    switch (input) {
      case 's':
      case 'S':
        GameGoSouth();
        break;
      case 'n':
      case 'N':
        GameGoNorth();
        break;
      case 'e':
      case 'E':
        GameGoEast();
        break;
      case 'w':
      case 'W':
        GameGoWest();
        break;
      case 'q':
      case 'Q':
        exit(0);
        break;
      default:
        break;
    }
    
  } while (TRUE);
}

