#include "defs.h"

int main(int argc, char *argv[]) {
    // Choose if you would like to choose the tools the hunters hunt with
    // or if it should be random
    int isRandomStart = argc == 2 && strcmp(argv[1], "random_start") == 0;

    // Initialize the random number generator
    srand(time(NULL));

    HouseType *house;
    GhostType *ghost;

    // Initialize the house and populate it with rooms
    initHouse(&house);
    populateRooms(house);

    // Initialize the ghost and add it to the house 
    initGhost(house, &ghost);

    HunterListType *hunterList = createHunterList();

    // Initialize the hunters
    char hunterName[MAX_STR];
    int ev;
    // Used to give each hunter a unique id
    int id = NUM_HUNTERS;
    // Used as an array to hold the current taken enum values for evidence
    unsigned char takenEvidence = 0;
    EvidenceListType *evList;
    
    // Asking for a user specified evidence changes the program so it is bonus only
    if(isRandomStart) {
        evList = createEvidenceList();
        addEvidence(evList, FINGERPRINTS, C_TRUE);
        addEvidence(evList, EMF, C_TRUE);
        addEvidence(evList, SOUND, C_TRUE);
        addEvidence(evList, TEMPERATURE, C_TRUE);
    }
    
    // Loop to get information about the hunters 
    while(id > 0) {
        HunterType *currentHunter;
        // Collect their name
        printf("Please enter the hunters name: ");
        scanf("%s", hunterName);
        
        if(!isRandomStart) {
            // Evidence has strict restrictions so loop until they are met 
            while(1) {
                printf("Please enter their evidence type (0 - EMF, 1 - TEMPERATURE, 2 - FINGERPRINTS, 3 - SOUND): ");
                scanf("%d", &ev);
                // Check if it is a valid choice 
                if(!(ev >= 0) || !(ev < EV_COUNT)) {
                    printf("That is not a valid evidence type!\n");
                    continue;
                }

                // Check if the bit at the enum int value is set 
                if(getBit(takenEvidence, ev)) {
                    printf("Please enter an evidence type that has not been taken!\n");
                } else { 
                    // Set the bit at the given enum value
                    takenEvidence = setBit(takenEvidence, ev);
                    break;
                }
            }
        } else {
            // Pick a random evidence from the remaining choices
            ev = randomEvidence(evList);
            // Remove it after since we can't have the same type twice 
            removeEvidence(evList, ev);
        }
    
        // Init the hunter and add it to our list
        initHunter(&currentHunter, house, hunterName, &id, (EvidenceType) ev);
        addHunter(hunterList, currentHunter);
    }
    
    // If we create an evidence list then free it
    if(isRandomStart) free(evList);
    
    // Reuse the hunter list for house
    house->hunterList = hunterList;
    
    // Start the threads
    pthread_t* ghostThread = startGhostThread(ghost);
    pthread_t* hunter1Thread = startHunterThread(hunterList->hunters[0]);
    pthread_t* hunter2Thread = startHunterThread(hunterList->hunters[1]);
    pthread_t* hunter3Thread = startHunterThread(hunterList->hunters[2]);
    pthread_t* hunter4Thread = startHunterThread(hunterList->hunters[3]);

    // Wait for the threads to finish
    pthread_join(*ghostThread, NULL);
    pthread_join(*hunter1Thread, NULL);
    pthread_join(*hunter2Thread, NULL);
    pthread_join(*hunter3Thread, NULL);
    pthread_join(*hunter4Thread, NULL);

    // Cleanup the threads
    free(ghostThread);
    free(hunter1Thread);
    free(hunter2Thread);
    free(hunter3Thread);
    free(hunter4Thread);

    l_gameComplete(ghost, house->hunterList, house->evidence);

    // Cleanup the ghost
    cleanupGhost(ghost);

    // Cleanup the house
    cleanupHouse(house);

    return 0; 
}

