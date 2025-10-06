#include "defs.h"

// Static declares that this function can only be found in this file and not during linking
static void print_menu(int* choice);

int main(void) {
  RoomCollection  rooms   = { .size = 0 };
  EntryCollection entries = { .size = 0 };
  int choice;

}

  do {
    print_menu(&choice);

    if (choice == 1) {    //loading sample data
        load_sample(&rooms, &entries);

        for (int i = 0; i < entries.size; i++) {
            LogEntry *e = &entries.entries[i];
            const char *type_label;
            if (e->data.type == TYPE_TEMP)
                type_label = "TEMP";
            else if (e->data.type == TYPE_DB)
                type_label = "DB";
            else if (e->data.type == TYPE_MOTION)
                type_label = "MOTION";
            else
                type_label = "UNKNOWN";
    }

        printf("Loaded sample data.\n");
    }

    else if (choice == 2) {   //Printing entries
       printf("%-15s  %-10s  %-8s  %-10s\n", "Room", "Timestamp", "Type", "Value");
       for (int i = 0; i < entries.size; i++) {
            entry_print(&entries.entries[i]);
        }
    }

    else if (choice == 3) {   //Printing rooms
      for (int i = 0; i < rooms.size; i++) {
          room_print(&rooms.rooms[i]);
    }
  }

    else if (choice == 4) {   //Adding rooms
      char name[MAX_STR];
      printf("Enter room name: ");
      scanf("%s", name);
      printf("Added room '%s\n'", name);

    }

    else if (choice == 5) {   //Supposed to be adding entries
      printf("I give up")
    }

    else if (choice == 6) {    // Testing order
        printf("Testing entry order\n");
        int result = loader_test_order(&entries, 1);
        if (result == C_ERR_OK)
            printf("Entries order is CORRECT.\n");
        else
            printf("Entries order is INCORRECT.\n");
    }

    else if (choice == 7) {   //Testing room entries
      printf("Testing room Correctness\n");
      int result = loader_test_rooms(&entries, &rooms, 1);
      if (result == C_ERR_OK)
          printf("Room correctness is CORRECT.\n");
      else
          printf("Room correctness is INCORRECT.\n");
    }
    else if (choice == 0) {   //EXIT
        printf("Goodbye!\n");
    }

  return 0;
}

void print_menu(int* choice) {
  int c = -1;
  int rc = 0;
  const int num_options = 7;

  printf("\nMAIN MENU\n");
  printf("  (1) Load sample data\n");
  printf("  (2) Print entries\n");
  printf("  (3) Print rooms\n");
  printf("  (4) Add room\n");
  printf("  (5) Add entry\n");
  printf("  (6) Test order\n");
  printf("  (7) Test room entries\n");
  printf("  (0) Exit\n\n");

  do {
    printf("Please enter a valid selection: ");
    // Check if they entered a non-integer
    rc = scanf("%d", &c);
    while (getchar() != '\n');
  } while (rc < 1 || c < 0 || c > num_options);

  *choice = c;
}
