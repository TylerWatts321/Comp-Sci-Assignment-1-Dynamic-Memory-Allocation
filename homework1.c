        /* COP 3502C Assignment 1
This program is written by: Gordon Tyler Watts */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

typedef struct monster{
    char *name;
    char *element;
    int population;
}monster;

typedef struct region{
    char *name;
    int nmonsters;
    int total_population;
    monster **monsters;
}region;

typedef struct itinerary{
    int nregions;
    region **regions;
    int captures;
}itinerary;

typedef struct trainer{
    char *name;
    itinerary *visits;
}trainer;

monster* createMonster(char *name, char *element, int population){
    /*This function is repsonsible for dynamically allocating memory each individual monster
    that exists in the monsters double pointer. After we do this, we then assign each value to its respective data container in
    the monster struct.*/

    // Dynamically allocate memory for the individual monster.
    monster * specificmonster = (monster*)malloc(sizeof(monster));
    
    // Retrieves the string lengths of the monster's name and element.
    int nameSize = strlen(name) + 1;
    int elementSize = strlen(element) + 1;

    // Assign the data to its respective data container in the monster struct.
    specificmonster->element = (char*)calloc(elementSize, sizeof(char));
    specificmonster->name = (char*)calloc(nameSize, sizeof(char));
    strcpy(specificmonster->element, element);
    strcpy(specificmonster->name, name);
    specificmonster->population = population;

    return specificmonster; // Return the specific monster pointer.
}

monster** readMonsters(FILE* infile, int *monsterCount){
    /*This function is repsonsible for scanning in the details of each monster from the in.txt file.
    After determining the number of unique monsters that exists, we are able to go through each monster and
    scan in the necessary data. This function also dynamically allocates memory for the entire group of monsters as whole.*/

    fscanf(infile, "%d", monsterCount); // Retrieves the number of unique monsters that can exist.
    int population;
    char name[50];
    char element[50];

    // Dynamically allocate memory for the entire mem block of monsters that can exist.
    monster** monsters = (monster**)malloc(*monsterCount * sizeof(monster *));

    fscanf(infile, "%*s"); // Skips the string "monsters"; it is not necessary.

    for(unsigned int i = 0; i < *monsterCount; i++)
    {
        fscanf(infile, "%s", name); // Scans in the monster name.
        fscanf(infile, "%s", element); // Scans in the element type.
        fscanf(infile, "%d", &population); // Scans in the number of that specific monster that can exist in each region.
        monsters[i] = createMonster(name, element, population); // Calls the "createMonster" function using the info we just scanned in from in.txt.
                                                                // Assigns the pointer return value to monsters[i].
    }

    return monsters; // Returns our double pointer we just created.
}

region* createRegion(FILE * infile, monster ** monsters, int * monsterCount, char * regionname, int * nmonsters){
    
    /*This function is repsonsible for dynamically allocating memory each individual region
    that exists in the region double pointer. After we do this, we then assign each value to its respective data container in
    the region struct.*/

    char monstername[50];
    int total_population = 0;
    
    // Dynamically allocates memory for the specific region.
    region * specificregion = (region *)malloc(sizeof(region));

    // Retrieves the string length of the region's name. And then copies the name and number of monsters to their respective data containers. 
    int nameSize = strlen(regionname) + 1;
    specificregion->name = (char *)calloc(nameSize, sizeof(char));
    strcpy(specificregion->name, regionname);
    specificregion->nmonsters = *nmonsters;

    // Sets the monsters in the respective region to the dynamically allocated memory for amount of unique monsters in that region.
    specificregion->monsters = (monster **)malloc(sizeof(monster*) * *(monsterCount));

    int k = 0;

    do
    {
        fscanf(infile, "%s", monstername); // Scans the monster name.
        for(unsigned int j = 0; j < *monsterCount; j++) // Iterates "monsterCount" times.
        {
            if ((strcmp(monstername, monsters[j]->name)) == 0) // If the scanned monster name is the same as the monster's name in the "monsters".
            {
                    total_population += monsters[j]->population; // Increment the total population by that determined monster's population.
                    specificregion->monsters[k] = monsters[j]; // Set that regions monster to that determined monster.
                    k++;
                    break;
            }
        }
            
    }while(k < *(nmonsters)); // Iterates "# of unique monster" times.

    specificregion->total_population = total_population; // Now that we have accumulated the number of the total population in that region,
                                                         // we set the total_population data container to total_population's value.
    return specificregion; // Return that specfic region.
}

region** readRegions(FILE* infile, int *regionCount, int *monsterCount, monster **monsters){
    /*This function is repsonsible for scanning in the details of each region from the in.txt file.
    After determining the number of unique regions that exists, we are able to go through each region and
    scan in the necessary data. This function also dynamically allocates memory for the entire group of regions as whole.*/

    monster ** temp = monsters;
    int nmonsters;
    char regionname[50];
    
    fscanf(infile, "%d", regionCount); // Retrieves the number of unique regions that can exist. 
    fscanf(infile, "%*s"); // Skips the string "regions"; it is not necessary.

    // Dynamically allocate memory for the all the regions that can exist.
    region** regions = (region**)malloc(*regionCount * sizeof(region *));


    for(unsigned int i = 0; i < *regionCount; i++)
    {
        fscanf(infile, "%s", regionname); // Scans in the region name.
        fscanf(infile, "%d", &nmonsters); // Scans in the number of unique monsters that can exist.
        fscanf(infile, "%*s"); // Skips over the word monsters yet again.

        regions[i] = createRegion(infile, monsters, monsterCount, regionname, &nmonsters); // Calls the "createRegion" function using the info we just scanned in from in.txt.
                                                                                           // Assigns the pointer return value to regions[i].
    }

    return regions; // Returns double pointer "regions".
}


trainer* createTrainer(FILE* infile, char *trainername, int *ncaptures, int *regionCount, int *nregions, region** regions)
{
    /*This function is repsonsible for dynamically allocating memory each individual trainer
    that exists in the trainer double pointer. After we do this, we then assign each value to its respective data container in
    the trainer struct.*/
    char regionname[50];

    // Dynamically allocates memory for the specific region.
    trainer * specifictrainer = (trainer *)malloc(sizeof(trainer));

    // Retrieves the string length of the trainer's name. 
    // Dynamically allocates for that name, and then copies the name to its respective data container.
    int nameSize = strlen(trainername) + 1;
    specifictrainer->name = (char *)calloc(nameSize, sizeof(char));
    strcpy(specifictrainer->name, trainername);

    // Sets the visits pointer in the trainer struct to the dynamically allocated memory for the itinerary
    // that belongs to each individual trainer.
    specifictrainer->visits = (itinerary *)malloc(sizeof(itinerary));

    // Dynamically allocates memory for each regions the trainer has been to.
    specifictrainer->visits->regions = (region **)malloc(*regionCount * sizeof(region*));

    // Sets the data in the itinerary struct we allocated memory for equal to the their respective values.
    specifictrainer->visits->nregions = *nregions;
    specifictrainer->visits->captures = *ncaptures;


    unsigned int k=0;
    do
    {
        fscanf(infile, "%s", regionname); // Scan in the region's name
        for (unsigned int i = 0; i < *regionCount; i++) // Iterates "the # of regions that exist" times.
        {
            if ((strcmp(regionname, regions[i]->name)) == 0) // If the scanned region name is the same as the region's name in the "regions"...
            {
                specifictrainer->visits->regions[k] = regions[i]; // Set that visted region to that determined region.
                k++;
                break;
            }
        }
    }while (k < *(nregions)); // Iterates "# of unique region visits" times.

    return specifictrainer; // Return the specifictrainer pointer.
}

trainer** readTrainers(FILE* infile, int *trainerCount, int *regionCount, monster **monsters, region** regions){
    /*This function is repsonsible for scanning in the details of each trainer from the in.txt file.
    After determining the number of unique trainers that exists, we are able to go through each trainer and
    scan in the necessary data. This function also dynamically allocates memory for the entire group of trainers as whole.*/

    char trainername[50];
    int ncaptures;
    int nregions;
    region ** temp = regions;
    
    fscanf(infile, "%d", trainerCount); // Retrieves the number of unique trainers that can exist. 
    fscanf(infile, "%*s"); // Skips the word "Trainers"; again not necessary.

    // Dynamically allocate memory for the all the trainers that can exist.
    trainer** trainers = (trainer**)malloc(*trainerCount * sizeof(trainer *));

    for(unsigned int i = 0; i < *trainerCount; i++) // Iterates "*trainerCount" times
    {
        fscanf(infile, "%s", trainername); // Scans in the Trainer's name
        fscanf(infile, "%d", &ncaptures);  // Scans in the number of unique captures the trainer has.
        fscanf(infile, "%*s");             // Skips the word "captures".
        fscanf(infile, "%d", &nregions);   // Scans in the number of unique regions visited by the trainer.
        fscanf(infile, "%*s");             // Skips the word "regions".

        trainers[i] = createTrainer(infile, trainername, &ncaptures, regionCount, &nregions, temp); // Calls the "createRegion" function using the info we just scanned in from in.txt.
                                                                                                    // Assigns the pointer return value to regions[i].
    }
    return trainers; // Returns double pointer "trainers".
}

void calculateTrainerStats(monster ** monsters, region ** regions, trainer ** trainers, int *monsterCount, int *regionCount, int *trainerCount, FILE *outfile){
    //1) pick a trainer
    //2) pick i'th region
    //3) retrieve j'th monster's population
    //4) divide that by total_pop in that region
    //5) multiply that number by the number of that trainer's captures
    //6) round
    
    // Along the way, the nested for loops print the corresponding info to out.txt.

    for (unsigned int i = 0; i < *trainerCount; i++)
    {
        fprintf(outfile, "%s\n", trainers[i]->name);
        for(unsigned int j = 0; j < trainers[i]->visits->nregions; j++)
        {
            fprintf(outfile, "%s\n", trainers[i]->visits->regions[j]->name);
            for(unsigned int k = 0; k < trainers[i]->visits->regions[j]->nmonsters; k++)
            {
                int count  = round((1.0*trainers[i]->visits->regions[j]->monsters[k]->population/trainers[i]->visits->regions[j]->total_population) * trainers[i]->visits->captures);
                if(count != 0)
                    fprintf(outfile, "%d %s\n", count, trainers[i]->visits->regions[j]->monsters[k]->name);
            }
        }
        if (i != *trainerCount - 1)
            fprintf(outfile, "\n");
    }
}

void freetrainermem(trainer ** trainers, int *trainerCount)
{
    for (unsigned int i = 0; i < *trainerCount; i++) // Goes through every trainer.
    {
        // Frees all the associated DAM with everything under every trainer...
        free(trainers[i]->visits->regions);
        free(trainers[i]->visits);
        free(trainers[i]->name);

        // Until freeing all the individual trainer itself.
        free(trainers[i]);
    }

    // After freeing each trainer, all of the DAM for the trainers double pointer is freed.
    free(trainers);

}

void freeregionmem(region ** regions, int *regionCount) // Goes through every region.
{
    for(unsigned int i = 0; i < *regionCount; i++)
    {
        // Frees all the associated DAM with everything under every region...
        free(regions[i]->monsters);
        free(regions[i]->name);

        // Until freeing all the individual region itself.
        free(regions[i]);
    }

    // After freeing each region, all of the DAM for the region double pointer is freed.
    free(regions);

}

void freemonstermem(monster ** monsters, int *monsterCount)
{
    for(unsigned int i = 0; i < *monsterCount; i++) // Goes through every monster.
    {
        // Frees all the associated DAM with everything under every monster...
        free(monsters[i]->element);
        free(monsters[i]->name);

        // Until freeing all the individual monster itself.
        free(monsters[i]);
    }

    // After freeing each monster, all of the DAM for the monsters double pointer is freed.
    free(monsters);

}

void freeallocatedmem(monster ** monsters, region ** regions, trainer ** trainers, int *monsterCount, int *regionCount, int *trainerCount)
{
    // The job of this function is to group together all of the functions that deal with freeing memory. The task of freeing
    // all of the dynamically allocated memory is split up into 3 functions:
    monster **tempmonsters = monsters;
    region **tempregions = regions;
    trainer **temptrainers = trainers;

    // This function frees all the dynamically allocated memory associated with the trainers.
    freetrainermem(temptrainers, trainerCount);

    // This function frees all the dynamically allocated memory associated with the regions.
    freeregionmem(tempregions, regionCount);

    // This function frees all the dynamically allocated memory associated with the monsters.
    freemonstermem(tempmonsters, monsterCount);

}

int main(){
    atexit(report_mem_leak); // Necessary for leak_detector_c.c to work.

    // Create pointers of FILE type that will be used in reading in.txt, and writing to out.txt.
    FILE *infile = fopen(/*"C:\\Programming\\C\\CS1\\Assignments\\Assignment 1\\*/"in.txt", "r");
    FILE *outfile = fopen(/*"C:\\Programming\\C\\CS1\\Assignments\\Assignment 1\\*/"out.txt", "w");

    // Create double pointer of struct type "monster" that will hold all the DMA for the monsters in the in.txt file.
    int mcount;
    monster ** monsters = readMonsters(infile, &mcount); // Calls the function "readMonsters". This function takes in infile and mcount as parameters.

    // Create double pointer of struct type "region" that will hold all the DMA for the regions in the in.txt file.
    int rcount;
    region ** regions = readRegions(infile, &rcount, &mcount, monsters); // Calls the function "readRegions". This function takes in infile and rcount as parameters.

    // Create double pointer of struct type "trainer" that will hold all the DMA for the regions in the in.txt file.
    int tcount;
    trainer ** trainers = readTrainers(infile, &tcount, &rcount, monsters, regions); // Calls the function "readTrainers". This function takes in infile and rcount as parameters.

    calculateTrainerStats(monsters, regions, trainers, &mcount, &rcount, &tcount, outfile); // Calls the function responsible for doing all the math, and also outputting info to out.txt.

    freeallocatedmem(monsters, regions, trainers, &mcount, &rcount, &tcount);  // Calls the function responsible for freeing all of our dynamically allocated memory.
    
    // Finally, we close the two files we opened in the beginning.
    fclose(infile);
    fclose(outfile);
}