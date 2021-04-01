#include <stdio.h>
#include "philos.h"
#include <string.h>
#include <stdlib.h>
int main()
{
	int seed;
	printf("Random seed: ");
	scanf("%u", &seed);
	srandom(seed);
	int players;
	printf("How many players? ");
	scanf("%u", &players);
	
	typedef struct {
		char name[50];
		int cash;
	       	int pos;
			}variables ;
	variables ppl[players];

	int i;
	for (i =0; i<players; i++){ //Create an array of data		
		strcpy(ppl[i].name, philosophers[i]);
		ppl[i].cash = 3;
		ppl[i].pos = i;
		}		
	

	int check(){ //Check if the game should keep going
		int i;
		int cashtest = 0;
		for (i=0; i<players; i++){
			if (ppl[i].cash > 0) //if only one person has cash then cashtest =1
			    cashtest = cashtest +1;   	
		if (cashtest ==1){
			return 1;}
		}

	int diceroll(){
			int r = rand() % 6;
			return r;
			}


	while(check() ==1)
		int a;
		for (a=0; a<players; a++){
	
			if (ppl[a].cash == 0){	
				//pass
			else if (ppl[a].cash ==2){
				//roll 2 dice
				int rando = diceroll();
			

			else if (ppl[a].cash ==1){
				//roll 1 dice
			else{
				//roll 3 dice
			
	
	typedef enum faciem { PASS, LEFT, RIGHT, CENTER } faces;
        faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };


}
/*
	//returns the position of the player on the left
	//pos: Position of the current player
	//players: The number of players in the game.

	//The following two lines are from the asgn1.pdf lab1 documentation.
static inline uint8_t left (unit8_t pos, unit8_t players) {
	return ((pos + players - 1) % players);

	//returns the position of the player on the right
        //The following two lines are from the asgn1.pdf lab1 documentation.
static inline unit8_t right(unit8_t pos, unit8_t players) {
	return ((pos + 1) % players);

        //The following two lines are from the asgn1.pdf lab1 documentation
	////typedef enum faciem { PASS, LEFT, RIGHT, CENTER } faces;
	faces die[] = { LEFT, RIGHT, CENTER, PASS, PASS, PASS };
*/	        ////typedef enum faciem { PASS, LEFT, RIGHT, CENTER } faces;
        
