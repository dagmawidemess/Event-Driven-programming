#include "BOARD.h"
#include "LinkedList.h"
#include <stdlib.h>

/**
 * This function starts a new linked list. Given an allocated pointer to data it will return a
 * pointer for a malloc()ed ListItem struct. If malloc() fails for any reason, then this function
 * returns NULL otherwise it should return a pointer to this new list item. data can be NULL.
 *
 * @param data The data to be stored in the first ListItem in this new list. Can be any valid 
 *             pointer value.
 * @return A pointer to the malloc()'d ListItem. May be NULL if an error occured.
 */
ListItem *LinkedListNew(char *data)
{
    ListItem *newword;
    newword = malloc(sizeof (ListItem));
    if (newword == NULL) {
        return NULL;
    }
    newword->previousItem = NULL;
    newword->nextItem = NULL;
    newword->data = data;
    return newword;
}

/**
 * This function will remove a list item from the linked list and free() the memory that the
 * ListItem struct was using. It doesn't, however, free() the data pointer and instead returns it
 * so that the calling code can manage it.  If passed a pointer to NULL, LinkedListRemove() should
 * return NULL to signal an error.
 *
 * @param item The ListItem to remove from the list.
 * @return The data pointer from the removed item. May be NULL.
 */
char *LinkedListRemove(ListItem *item)
{
    if (item == NULL) {
        return NULL;
    }
    if (item->previousItem == NULL) {
        item->nextItem->previousItem = NULL;
    } else if (item->nextItem == NULL) {
        item->previousItem->nextItem = NULL;
    } else if (item->previousItem != NULL && item->nextItem != NULL) {
        item->previousItem->nextItem = item->nextItem;
        item->nextItem->previousItem = item->previousItem;
    }

    char *ret = item->data;
    free(item);
    return ret;
}

/**
 * This function returns the total size of the linked list. This means that even if it is passed a
 * ListItem that is not at the head of the list, it should still return the total number of
 * ListItems in the list. A NULL argument will result in 0 being returned.
 *
 * @param list An item in the list to be sized.
 * @return The number of ListItems in the list (0 if `list` was NULL).
 */
int LinkedListSize(ListItem *list)
{
    ListItem *temp = list;
    int count = 0;

    while (temp->nextItem != NULL) {
        count++;
        temp = temp->nextItem;
    }

    temp = list;
    while (temp->previousItem != NULL) {
        count++;
        temp = temp->previousItem;
    }
    return count + 1;
}

/**
 * This function returns the head of a list given some element in the list. If it is passed NULL,
 * it will just return NULL. If given the head of the list it will just return the pointer to the
 * head anyways for consistency.
 *
 * @param list An element in a list.
 * @return The first element in the list. Or NULL if provided an invalid list.
 */
ListItem *LinkedListGetFirst(ListItem *list)
{
    if(list == NULL ){
        return NULL;
    }
    while (list->previousItem != NULL) {
        list = list->previousItem;
    }
    return list;
}
/**
 * This function allocates a new ListItem containing data and inserts it into the list directly
 * after item. It rearranges the pointers of other elements in the list to make this happen. If
 * passed a NULL item, InsertAfter() should still create a new ListItem, just with no previousItem.
 * It returns NULL if it can't malloc() a new ListItem, otherwise it returns a pointer to the new
 * item. The data parameter is also allowed to be NULL.
 *
 * @param item The ListItem that will be before the newly-created ListItem.
 * @param data The data the new ListItem will point to.
 * @return A pointer to the newly-malloc()'d ListItem.
 */
//linkedlist already generated

ListItem *LinkedListCreateAfter(ListItem *item, char *data)
{
    ListItem *newItem = LinkedListNew(data);
    
    if(item->nextItem == NULL) {
        item->nextItem = newItem;
        newItem->previousItem = item;
    } else {
        newItem->nextItem= item->nextItem;
        item->nextItem = newItem;
        newItem->nextItem->previousItem = newItem;
        newItem->previousItem = item;
    }
    return newItem;
}

/**
 * LinkedListSwapData() switches the data pointers of the two provided ListItems. This is most
 * useful when trying to reorder ListItems but when you want to preserve their location. It is used
 * within LinkedListSort() for swapping items, but probably isn't too useful otherwise. This
 * function should return STANDARD_ERROR if either arguments are NULL, otherwise it should return
 * SUCCESS. If one or both of the data pointers are NULL in the given ListItems, it still does
 * perform the swap and returns SUCCESS.
 *
 * @param firstItem One of the items whose data will be swapped.
 * @param secondItem Another item whose data will be swapped.
 * @return SUCCESS if the swap worked or STANDARD_ERROR if it failed.
 */
int LinkedListSwapData(ListItem *firstItem, ListItem *secondItem)
{
    if(firstItem == NULL || secondItem == NULL){
        return STANDARD_ERROR;
    }
    
    char *temp = firstItem->data;
    firstItem->data = secondItem->data;
    secondItem->data = temp;
    
    return SUCCESS;
}


/**
 * LinkedListSort() performs a selection sort on list to sort the elements into decending order. It
 * makes no guarantees of the addresses of the list items after sorting, so any ListItem referenced
 * before a call to LinkedLis+tSort() and after may contain different data as only the data pointers
 * for the ListItems in the list are swapped. This function sorts the strings in ascending order
 * first by size (with NULL data pointers counting as 0-length strings) and then alphabetically
 * ascending order. So the list [dog, cat, duck, goat, NULL] will be sorted to [NULL, cat, dog,
 * duck, goat]. LinkedListSort() returns SUCCESS if sorting was possible. If passed a NULL pointer
 * for either argument, it will do nothing and return STANDARD_ERROR.
 *
 * @param list Any element in the list to sort.
 * @return SUCCESS if successful or STANDARD_ERROR is passed NULL pointers.
 */
int LinkedListSort(ListItem *list) {
    if(list == NULL) {
        return STANDARD_ERROR;}
    
    ListItem *ref = list;
    while(ref->nextItem != NULL) {
       ListItem *secondItem = ref->nextItem;
       while(secondItem != NULL) {
           if(strcmp(ref->data, secondItem->data) > 0) {
               LinkedListSwapData(ref, secondItem);
           }
           secondItem = secondItem->nextItem;
       }
       ref = ref->nextItem;
    }
    return SUCCESS;
}

/**
 * LinkedListPrint() prints out the complete list to stdout. This function prints out the given
 * list, starting at the head if the provided pointer is not the head of the list, like "[STRING1,
 * STRING2, ... ]" If LinkedListPrint() is called with a NULL list it does nothing, returning
 * STANDARD_ERROR. If passed a valid pointer, prints the list and returns SUCCESS.
 *
 * @param list Any element in the list to print.
 * @return SUCCESS or STANDARD_ERROR if passed NULL pointers.
 */
int LinkedListPrint(ListItem *list) {
   if(list == NULL) 
   {
       return STANDARD_ERROR;
   }
   
   ListItem *item = LinkedListGetFirst(list);
   printf("[");
   while(item) {
       printf("%s", item->data);
       if(item->nextItem != NULL) {
           printf(", ");
       }
       item = item->nextItem;
   }
   printf("]");
   return SUCCESS;
}
