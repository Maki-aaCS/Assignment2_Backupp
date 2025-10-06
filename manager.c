#include "defs.h"

/* ---- entry comparator -------------------------------------------
   Order: room name ASC, then type ASC by #define value, then timestamp ASC
   Returns <0 if a<b, >0 if a>b, 0 if equal.
----------------------------------------------------------------------------- */
int entry_cmp(const LogEntry *a, const LogEntry *b) {

  //Room names (Alphebtical: A to Z)
  int room_cmp = strcmp(a->room->name, b->room->name);
  if (room_cmp != 0)
      return room_cmp;

  //Type (from TEMP then DB then MOTION)
  if (a->data.type < b->data.type)
      return -1;
  else if (a->data.type > b->data.type)
      return 1;

  //Timestamp (ascending order: least recent to most recent)
  if (a->timestamp < b->timestamp)
      return -1;
  else if (a->timestamp > b->timestamp)
      return 1;

  //If everything is equal
  return 0;
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
  if (rc == NULL || room_name == NULL) {
      return NULL;
  }

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
  if (rc == NULL || room_name == NULL) {
      return C_ERR_NULL_PTR;
  }
  //Max capacity rached
  if (rc->size >= MAX_ARR) {
      return C_ERR_FULL_ARRAY;
  }

  //Duplicate
  if (rooms_find(rc, room_name) != NULL) {
      return C_ERR_DUPLICATE;
  }

  //Adding the new room
  Room *new_room = &rc->rooms[rc->size];
  //Explanation to understand &rc->rooms[rc->size]:
  //rc->size: the number of rooms already in the collection (0 means empty).
  //rc->rooms[rc->size]: the next available slot (index) where the new room should be stored.
  //&rc->rooms[rc->size]: takes the address of that slot to give a Room * pointer to it.
  strncpy(new_room->name, room_name, MAX_STR - 1);    //from textbook
  new_room->name[MAX_STR - 1] = '\0'; // ensure null-termination
  new_room->size = 0;

  rc->size++;

  return C_ERR_OK;
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
  if (ec == NULL || room == NULL)
      return C_ERR_NULL_PTR;

  if (type != TYPE_TEMP && type != TYPE_DB && type != TYPE_MOTION)
      return C_ERR_INVALID;

  if (ec->size >= MAX_ARR || room->size >= MAX_ARR)
      return C_ERR_FULL_ARRAY;

  LogEntry new_entry;
  new_entry.data.type = type;
  new_entry.data.value = value;
  new_entry.timestamp = timestamp;
  new_entry.room = room;

  //Finding position in entry
  int insert_pos = 0;
  while (insert_pos < ec->size) {
    if (entry_cmp(&new_entry, &ec->entries[insert_pos]) < 0)
      break;
    insert_pos++;
}

  //Shift entries to the right to make room
  for (int i = ec->size; i > insert_pos; i--) {
      LogEntry *src = &ec->entries[i - 1];
      LogEntry *dst = &ec->entries[i];

      *dst = *src;  // Copy struct by value

      // Retarget the room’s pointer to this new location
      for (int j = 0; j < dst->room->size; j++) {
          if (dst->room->entries[j] == src) {
              dst->room->entries[j] = dst;
              break;
          }
      }
  }

  ec->entries[insert_pos] = new_entry;
  ec->size++;

  LogEntry *inserted = &ec->entries[insert_pos];

  int room_insert_pos = 0;
  while (room_insert_pos < room->size) {
      if (entry_cmp(inserted, room->entries[room_insert_pos]) < 0)
          break;
      room_insert_pos++;
  }

  for (int i = room->size; i > room_insert_pos; i--) {
      room->entries[i] = room->entries[i - 1];
  }

  room->entries[room_insert_pos] = inserted;
  room->size++;

  return C_ERR_OK;
}



/* ---- entry_print -----------------------------------------------------------
   Purpose: Print one entry in a formatted row.
   Params:
     - e (in): entry to print
   Returns: C_ERR_OK, C_ERR_NULL_PTR if e is NULL, C_ERR_INVALID if room is NULL
----------------------------------------------------------------------------- */
int entry_print(const LogEntry *e) {
  if (e == NULL) return C_ERR_NULL_PTR;
  if (e->room == NULL) return C_ERR_INVALID;

  char *type_str = [17];
  char value_str[64];

  if (e->data.type == TYPE_TEMP) {
      type_str = "TEMP";
      sprintf(value_str, "%.2f°C", e->data.value.temperature);

  } else if (e->data.type == TYPE_DB) {
      type_str = "DB";
      sprintf(value_str, "%d dB", e->data.value.decibels);

  } else if (e->data.type == TYPE_MOTION) {
      type_str = "MOTION";
      sprintf(value_str, "[%d,%d,%d]",
              e->data.value.motion[0],
              e->data.value.motion[1],
              e->data.value.motion[2]);

  } else {
      return C_ERR_INVALID;
  }

  printf("%-15s  %-10d  %-8s  %-10s\n", e->room->name, e->timestamp, type_str, value_str);

  return C_ERR_OK;
}



/* ---- room_print ------------------------------------------------------------
   Purpose: Print a room header and all of its entries (already sorted).
   Params:
     - r (in): room to print
   Returns: C_ERR_OK, C_ERR_NULL_PTR if r is NULL
----------------------------------------------------------------------------- */
int room_print(const Room *r) {
    if (r == NULL) 
        return C_ERR_NULL_PTR;

    printf("\nRoom: %s (entries=%d)\n", r->name, r->size);
    printf("%-15s  %-10s  %-8s  %-10s\n", "Room", "Timestamp", "Type", "Value");

    for (int i = 0; i < r->size; i++) {
        entry_print(r->entries[i]);
    }

    return C_ERR_OK;
}
