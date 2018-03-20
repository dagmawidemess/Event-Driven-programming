#include "Game.h"

#include <stdio.h>
#include <string.h>

#include "Player.h"

#define GAME_MAX_VERSIONS 100

struct RoomVersionInfo {
  char description[GAME_MAX_ROOM_DESC_LENGTH + 1];
  uint8_t itemsRequirement;
  uint8_t itemsContained;
  uint8_t northExitRoom;
  uint8_t eastExitRoom;
  uint8_t southExitRoom;
  uint8_t westExitRoom;
};

static uint8_t activeVersion = 0;

struct Room {
  char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
  uint8_t numberOfVersions;
  struct RoomVersionInfo* versions[GAME_MAX_VERSIONS];
};

struct RoomVersionInfo* GameReadRoomVersionInfo(FILE *filePointer, uint8_t decryptionKey);
struct Room* GameLoadRoom(uint8_t roomNumber);
void DestroyRoom(struct Room *room);
void Decrypt(uint8_t *data, uint8_t length, uint8_t decryptionKey);

static struct Room *currentRoom = NULL;

/**
* These function transitions between rooms. Each call should return SUCCESS if the current room has
* an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
* otherwise.
* @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
*         succeeded.
*/
int GameGoNorth(void) {
  if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS){
    struct Room *nextRoom = GameLoadRoom(currentRoom->versions[activeVersion]->northExitRoom);
    int version = 0;
    for(; version < nextRoom->numberOfVersions; version++) {
      if (FindInInventory(nextRoom->versions[version]->itemsRequirement) == SUCCESS) {
        DestroyRoom(currentRoom);
        currentRoom = nextRoom;
        activeVersion = version;
        return SUCCESS;
      }
    }
    return STANDARD_ERROR;
    
  }
  return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void) {
  if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS){
    struct Room *nextRoom = GameLoadRoom(currentRoom->versions[activeVersion]->eastExitRoom);
    int version = 0;
    for(; version < nextRoom->numberOfVersions; version++) {
      if (FindInInventory(nextRoom->versions[version]->itemsRequirement) == SUCCESS) {
        DestroyRoom(currentRoom);
        currentRoom = nextRoom;
        activeVersion = version;
        return SUCCESS;
      }
    }
    return STANDARD_ERROR;
    
  }
  return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void) {
  if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS){
    struct Room *nextRoom = GameLoadRoom(currentRoom->versions[activeVersion]->southExitRoom);
    int version = 0;
    for(; version < nextRoom->numberOfVersions; version++) {
      if (FindInInventory(nextRoom->versions[version]->itemsRequirement) == SUCCESS) {
        DestroyRoom(currentRoom);
        currentRoom = nextRoom;
        activeVersion = version;
        AddToInventory(nextRoom->versions[version]->itemsContained);
        return SUCCESS;
      }
    }
    return STANDARD_ERROR;
    
  }
  return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void) {
  if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS){
    struct Room *nextRoom = GameLoadRoom(currentRoom->versions[activeVersion]->westExitRoom);
    int version = 0;
    for(; version < nextRoom->numberOfVersions; version++) {
      if (FindInInventory(nextRoom->versions[version]->itemsRequirement) == SUCCESS) {
        DestroyRoom(currentRoom);
        currentRoom = nextRoom;
        activeVersion = version;
        return SUCCESS;
      }
    }
    return STANDARD_ERROR;
    
  }
  return STANDARD_ERROR;
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void) {
  currentRoom = GameLoadRoom(STARTING_ROOM);
  return SUCCESS;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title) {
  strcpy(title, currentRoom->title);
  return (int)strlen(title);
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc) {
  struct RoomVersionInfo *roomVersionInfo = currentRoom->versions[activeVersion];
  strcpy(desc, roomVersionInfo->description);
  return (int)strlen(desc);
}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room->
 */
uint8_t GameGetCurrentRoomExits(void) {
  struct RoomVersionInfo *roomVersionInfo = currentRoom->versions[activeVersion];
  // NORTH, EAST, SOUTH, and WEST
  uint8_t ret = 0;
  if(roomVersionInfo->northExitRoom) {
    ret |= GAME_ROOM_EXIT_NORTH_EXISTS;
  }
  
  if(roomVersionInfo->eastExitRoom) {
    ret |= GAME_ROOM_EXIT_EAST_EXISTS;
  }
  
  if(roomVersionInfo->southExitRoom) {
    ret |= GAME_ROOM_EXIT_SOUTH_EXISTS;
  }
  
  if(roomVersionInfo->westExitRoom) {
    ret |= GAME_ROOM_EXIT_WEST_EXISTS;
  }
  
  return  ret;
}

void Decrypt(uint8_t *data, uint8_t length, uint8_t decryptionKey) {
  int i = 0;
  for(;i < length; i++) {
    data[i] = data[i] ^ decryptionKey;
  }
}

struct Room* GameLoadRoom(uint8_t roomNumber) {
  struct Room *room = malloc(sizeof(struct Room));
  
  char fileName[255];
  sprintf(fileName,"RoomFiles/room%d.txt", roomNumber);
  FILE *filePointer = fopen(fileName,"r");
  
  uint8_t titleLength = fgetc(filePointer);
  Decrypt(&titleLength, 1, roomNumber + DECRYPTION_BASE_KEY);
  
  fread(room->title, titleLength, 1, filePointer);
  Decrypt((uint8_t *)room->title, titleLength, roomNumber + DECRYPTION_BASE_KEY);
  room->title[titleLength] = '\0';
  
  int numberOfVersions = 0;
  while(!feof(filePointer)) {
    struct RoomVersionInfo *roomVersionInfo = GameReadRoomVersionInfo(filePointer, roomNumber+DECRYPTION_BASE_KEY);
    if(roomVersionInfo == NULL) break;
    room->versions[numberOfVersions++] = roomVersionInfo;
    room->numberOfVersions = numberOfVersions;
  }
  
  fclose (filePointer);
  return room;
}

struct RoomVersionInfo* GameReadRoomVersionInfo(FILE *filePointer, uint8_t decryptionKey) {
  struct RoomVersionInfo *roomVersionInfo = malloc(sizeof(struct RoomVersionInfo));
  
  int itemRequirement = fgetc(filePointer);
  if(itemRequirement == EOF) {
    free(roomVersionInfo);
    return NULL;
  }
  roomVersionInfo->itemsRequirement = itemRequirement;
  Decrypt(&roomVersionInfo->itemsRequirement, 1, decryptionKey);
  if(roomVersionInfo->itemsRequirement) {
    roomVersionInfo->itemsRequirement = fgetc(filePointer);
    Decrypt(&roomVersionInfo->itemsRequirement, 1, decryptionKey);
  }
  
  uint8_t descriptionLength = fgetc(filePointer);
  Decrypt(&descriptionLength, 1, decryptionKey);
  
  fread(roomVersionInfo->description, descriptionLength, 1, filePointer);
  Decrypt((uint8_t *)roomVersionInfo->description, descriptionLength, decryptionKey);
  roomVersionInfo->description[descriptionLength] = '\0';
  
  roomVersionInfo->itemsContained = fgetc(filePointer);
  Decrypt(&roomVersionInfo->itemsContained, 1, decryptionKey);
  if(roomVersionInfo->itemsContained) {
    roomVersionInfo->itemsContained = fgetc(filePointer);
    Decrypt(&roomVersionInfo->itemsContained, 1, decryptionKey);
  }
  
  roomVersionInfo->northExitRoom = fgetc(filePointer);
  Decrypt(&roomVersionInfo->northExitRoom, 1, decryptionKey);
  roomVersionInfo->eastExitRoom = fgetc(filePointer);
  Decrypt(&roomVersionInfo->eastExitRoom, 1, decryptionKey);
  roomVersionInfo->southExitRoom = fgetc(filePointer);
  Decrypt(&roomVersionInfo->southExitRoom, 1, decryptionKey);
  roomVersionInfo->westExitRoom = fgetc(filePointer);
  Decrypt(&roomVersionInfo->westExitRoom, 1, decryptionKey);
  return roomVersionInfo;
}

void DestroyRoom(struct Room *room) {
  if(room == NULL) return;
  
  int version = 0;
  for(; version < room->numberOfVersions; version++) {
    free(room->versions[version]);
  }
  free(room);
}



