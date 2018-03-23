#include "Player.h"

#include <stdio.h>

static uint8_t inventory[INVENTORY_SIZE];

static uint8_t currentInventorySize = 0;

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item) {
  if (item == 0) {
    return SUCCESS;
  }
  
  if(currentInventorySize >= INVENTORY_SIZE) return STANDARD_ERROR;
  inventory[currentInventorySize++] = item;
  return SUCCESS;
}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item) {
  if (item == 0) {
    return SUCCESS;
  }
  int i;
  for (i = 0; i < currentInventorySize; i++) {
    if (inventory[i] == item) {
      return SUCCESS;
    }
  }
  return STANDARD_ERROR;
}
