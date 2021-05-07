/*
Question 1

You are given a deck containing N cards.  While holding the deck facedown:

1. Deal all the cards facedown onto a table into Y piles like you would if you were playing with a group of people (i.e. card 1 to P1, card 2 to P2, ..., card Y to PY, card Y + 1 to P1, etc).
2. Combine all the piles into a deck by placing P1 onto P2, then P1+P2 onto P3, and so on. This is a round.
3. Pick up the deck from the table and repeat steps 1-2 until the deck is in the original order.
4. For each round, vary the pile count according to a repeating pattern. Start with 3 piles, then 4, then 5, then loop back to 3, then 4 and so on.
	
Write a program to determine how many rounds it will take to put a deck back into the original order.  This will involve creating a data structure to represent the order of the cards. Do not use an array. This program should be written in C only. It should take a number of cards in the deck as a command line argument and write the result to stdout.  Please ensure the program compiles and runs correctly (no pseudo-code).  This isn't a trick question; it should be fairly straightforward.

Bonus: Output how many rounds should be completed before the deck is adequately shuffled from the original deck for a person who is casually playing a game with cards. Provide your methodology in a comment block.
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

#define MAX_TOTAL_PILES (5)
#define NUM_PILES (3)

int pileNumberTable[3] = {3, 4, 5};

/* Linked List node */
typedef struct card
{
	int val;
	struct card* next;
}card_t;

/* Linked list of card_t, or the head of a linked list of cards */
typedef struct pile
{
	card_t* top_card;
}pile_t;

typedef struct deck
{
	int val;
	struct deck* next;
}deck_t;

/* This array only stores the handler of each pile so that they can be
   retrieved for cards each pile hold to form a deck */
pile_t pileArr[MAX_TOTAL_PILES];


/**
 * @brief helper function to cleat/empty piles
*/
static void resetPiles(void)
{
	for(int i = 0; i < MAX_TOTAL_PILES; i++){
		pileArr[i].top_card->val = -1;
		pileArr[i].top_card->next = NULL;
	}
}

/**
 * @brief add all the cards in pile to the deck
 * @param[in] pile pile that contains cards
 * @param[in/out] deck needs to be added with cards
*/
static void addToDeckTail(pile_t* pile, deck_t* deck)
{	
	/* Find last one in current deck */
	deck_t* iter = deck;
	while(iter->next){
		iter = iter->next;
	}
	/* iter is pointing to last node at this point */

	card_t* iter2 = pile->top_card;
	while(iter2){
		deck_t* newDeckNode = (deck_t*)malloc(sizeof(deck_t));
		newDeckNode->val = iter2->val;
		/* add to the end of the deck */
		iter->next = newDeckNode;
		iter = iter->next;
		/* iterate thru pile of cards*/
		iter2 = iter2->next;
	}
}


/**
 *  @brief Taking all cards back from piles and make them in one deck
 *  @param[in] pilesInUse: piles that currently have cards
 *  @return pointer to deck_t, which holds all cards taken from all piles
*/
static deck_t* makeDeck(int pilesInUse)
{

	deck_t* retDeck = (deck_t*)malloc(sizeof(deck_t));
	//first node in deck is dummy node, real code starts at second node
	retDeck->val = -1;
	retDeck->next = NULL;

	/* Link all piles together */
	for(int i = 0; i < pilesInUse; i++){
		addToDeckTail(&pileArr[i], retDeck);
	}

	return retDeck;
}

/**
 * @brief add to the front of the pile
 * @param[in/out] pile pile needs to be added
 * @param[in] cal card val
 */
static void addToPileFront(pile_t* pile, int val)
{
	card_t* newCard = (card_t*)malloc(sizeof(card_t));
	newCard->val = val;
	if(pile->top_card->val == -1){
		pile->top_card = newCard;
		return;
	}
	newCard->next = pile->top_card;
	pile->top_card = newCard;
}

/**
 * @brief Taking cards from current deck and distribute it back to piles
          the cards will be updated by modifying pileArr
 * @param[in] pilesInUse piles that currently have cards
 * @param[in] deck deck holding cards needs to be distributed
 */
static void distribute(int pilesInUse, deck_t* deck)
{
	deck_t* deckIter = deck;
	uint8_t pileInd = 0;

	while(deckIter){
		if(deckIter->val == -1){
			deckIter = deckIter->next;
			continue;
		}
		addToPileFront(&pileArr[pileInd],deckIter->val);
		deckIter = deckIter->next;
		pileInd = (pileInd+1)%pilesInUse;
	}
}


/**
 * @brief given a card value add it to the deck
 * @param [in] val card value
 * @param[in/out] deck needs to be added with cards
*/
static deck_t* addToDeckTail2(int val, deck_t* deck)
{
	deck_t* newVal = (deck_t*)malloc(sizeof(deck_t));
	newVal->val = val;
	newVal->next = NULL;
	if(deck->val == -1){
		deck = newVal;
		return deck;
	}

	/* Find last one in current deck */
	deck_t* iter = deck;
	while(iter->next){
		iter = iter->next;
	}
	iter->next=newVal;
	return deck;
}

/**
 * @brief printout all the cards in the deck
 *
 * @param[in] deckIn pointer to deck_t
 */
static void printDeck(deck_t* deckIn)
{	
	if(!deckIn){
		printf("empty deck\n");
		return;
	}

	deck_t* iter = deckIn;
	while(iter){
		if(iter->val != -1)
			printf("%d ", iter->val);
		iter = iter->next;
	}
	printf("\n");
}


/**
 * @brief check current check order and return true
 *         if it is original order(0,1,2,3......numCards)
 * @param[in] deck the pointer to deck
 * @return true if ir is original order
*/
static bool checkOrder(deck_t* deck)
{
	deck_t* iter = deck;
	int prev;
	prev = iter->val;
	iter = iter->next;

	while(iter){
		if(prev!= (iter->val)-1)
			return false;
		prev = iter->val;
		iter = iter->next;
	}
	return true;
}

int main(int argv, char**argc)
{
	if(argv != 2){
		printf("Please try again!\n");
		printf("Usage: ./<out file name> <number of cards>\n");
		return 0;
	}
	int numCards = atoi(argc[1]);

	deck_t* deck = (deck_t*)malloc(sizeof(deck_t));
	deck->val = -1;

	/* global data init */
	for(int i = 0; i < MAX_TOTAL_PILES; i++){
		card_t* newCard = (card_t*)malloc(sizeof(card_t));
		pileArr[i].top_card = newCard;
		pileArr[i].top_card->val = -1;
	}

	/* initialize first deck */
	for(int i = 0; i < numCards; i++){
		deck = addToDeckTail2(i, deck);
	}
	printf("original deck order:\n");



	bool originalOrder = false;
	int numRounds = 0;
	int pileNumInd = 0;
	deck_t* curr_deck = deck;
	printf("original deck order:\n");
	printDeck(curr_deck);

	/* Driver logic: repeatively distribute cards into piles and taken them back
	   untill original order is restored */
	while(!originalOrder){

		distribute(pileNumberTable[pileNumInd], curr_deck);

		curr_deck = makeDeck(pileNumberTable[pileNumInd]);

		printf("Current deck order:\n");
		printDeck(curr_deck);

		resetPiles();

		originalOrder = checkOrder(curr_deck);

		numRounds++;
		pileNumInd = (pileNumInd+1)%NUM_PILES;
	}

	printf("Total round needed: %d\n", numRounds);
	return 0;
}



/* Bonus Question: */

/**
It would really depends on the detailed meaning&defenition of "shuffleness", given a clear defenition
of "ramdomness", for eg. some stats relationnship for each adjacent elements, we can computer them and
see if the "adequate shuffleness has been reached and make a note of that."
*/