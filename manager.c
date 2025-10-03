#include "defs.h"

/* ---- entry comparator -------------------------------------------
   Order: room name ASC, then type ASC by #define value, then timestamp ASC
   Returns <0 if a<b, >0 if a>b, 0 if equal.
----------------------------------------------------------------------------- */
int entry_cmp(const LogEntry *a, const LogEntry *b) {
    return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}

/* ---- rooms_find ------------------------------------------------------------
   Purpose: Find a room by name.
   Params:
    - rc (in): room collection
    - room_name (in): C-string room name
   Returns: pointer to room or NULL if not found or on error
----------------------------------------------------------------------------- */
Room* rooms_find(RoomCollection *rc, const char *room_name) {
//personal reminder: return type is room*, not int.
//
//THIS IS SMTH THAT CHAT ADDED:
//if (rc == NULL || room_name == NULL) {
//  return NULL;
//}
  for (i=0; i < rc->size; i++){
    if (strcmp(room_name, rc->rooms[i].name) == 0){
      return &rc->rooms[i];     //return the duplicate room
    }
  }
    return NULL;
}

/* ---- rooms_add -------------------------------------------------------------
   Purpose: Add a room if it does not already exist.
   Params:
     - rc (in/out): room collection
     - room_name (in): C-string room name
   Returns: C_ERR_OK, C_ERR_NULL_PTR, C_ERR_DUPLICATE, C_ERR_FULL_ARRAY
----------------------------------------------------------------------------- */
int rooms_add(RoomCollection *rc, const char *room_name) {

//THIS IS SMTH CHAT ADDED
//  if (rc == NULL || room_name == NULL) {
//      return C_ERR_NULL_PTR;
//  }
    //At capacity
    if (rc->size >= MAX_ARR) {
        return C_ERR_FULL_ARRAY;
    }

    //Duplicate
    if (rooms_find(rc, room_name) != NULL) {
        return C_ERR_DUPLICATE;
    }

    // Add new room
    Room *new_room = &rc->rooms[rc->size];
    //Need help trying to find &rc->rooms[rc->size]:
    //rc->size: the number of rooms already in the collection (0 means empty).
    //rc->rooms[rc->size]: the next available slot (index) where the new room should be stored.
    //&rc->rooms[rc->size]: takes the address of that slot to give a Room * pointer to it.
    strncpy(new_room->name, room_name, MAX_STR - 1);    //from textbook
    new_room->name[MAX_STR - 1] = '\0'; // ensure null-termination
    new_room->size = 0;

    rc->size++;

    return C_ERR_OK;
    return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}

/* ---- entries_create -----------------------------------------------------------
   Purpose: Create a log entry and place it in the global entries (sorted),
            and attach a pointer to it in the owning room (append or sorted, either works)
   Params:
     - ec (in/out): entry collection (owns LogEntry storage)
     - room (in/out): room to attach entry to (must already exist)
     - type (in): TYPE_TEMP|TYPE_DB|TYPE_MOTION
     - value (in): union payload for reading
     - timestamp (in): simple int timestamp
   Returns: C_ERR_OK, C_ERR_NULL_PTR, C_ERR_FULL_ARRAY, C_ERR_INVALID
----------------------------------------------------------------------------- */
int entries_create(EntryCollection *ec, Room *room, int type, ReadingValue value, int timestamp) {
    return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}

/* ---- entry_print -----------------------------------------------------------
   Purpose: Print one entry in a formatted row.
   Params:
     - e (in): entry to print
   Returns: C_ERR_OK, C_ERR_NULL_PTR if e is NULL, C_ERR_INVALID if room is NULL
----------------------------------------------------------------------------- */
int entry_print(const LogEntry *e) {
    return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}

/* ---- room_print ------------------------------------------------------------
   Purpose: Print a room header and all of its entries (already sorted).
   Params:
     - r (in): room to print
   Returns: C_ERR_OK, C_ERR_NULL_PTR if r is NULL
----------------------------------------------------------------------------- */
int room_print(const Room *r) {
    return C_ERR_NOT_IMPLEMENTED; // Remove once implemented
}
