#pragma once

#include "Deck.h"

constexpr int NUM_FLOP_CARDS = 3;
constexpr int NUM_POOL_CARDS = 5;

constexpr char FLOP1_FLAG = 0x1;
constexpr char FLOP2_FLAG = 0x2;
constexpr char FLOP3_FLAG = 0x4;
constexpr char TURN_FLAG = 0x8;
constexpr char RIVER_FLAG = 0x10;

constexpr char POOL_BIT_PACK[5] = { FLOP1_FLAG, FLOP2_FLAG, FLOP3_FLAG, TURN_FLAG, RIVER_FLAG };

/*Representation of a players hand*/
struct Hand
{
	Card cards[2];

	/*Deal a random hand from the deck*/
	Hand(Deck& deck) : cards{deck.getNextHandCard(), deck.getNextHandCard()} {}

	/*Create a specific, desired hand*/
	Hand(const Card* initHand) : cards{ initHand[0], initHand[1] } {}
	Hand(const CardEnum* initHand) : cards{ { getCardsNumber(initHand[0]), getCardsSuit(initHand[0]) },
									  { getCardsNumber(initHand[1]), getCardsSuit(initHand[1]) } } {}
	Hand() : cards() {}

	/*Different ways to change the cards in a given hand*/
	void setHand(const Card* newHand) {
		cards[0] = newHand[0];
		cards[1] = newHand[1];
	}

	void setHand(const Card newCard1, const Card newCard2) {
		cards[0] = newCard1;
		cards[1] = newCard2;
	}

	void setHand(const CardEnum* newHand) {
		cards[0].number = getCardsNumber(newHand[0]);
		cards[1].number = getCardsNumber(newHand[1]);

		cards[0].suit = getCardsSuit(newHand[0]);
		cards[1].suit = getCardsSuit(newHand[1]);
	}
	
	/*Deal a new random hand from the deck*/
	void newHand(Deck& deck) 
	{
		//Make sure that resetEntireDeck was called before this
		cards[0] = deck.getNextHandCard();
		cards[0] = deck.getNextHandCard();
	}
};

/*A different representation of the pools*/
struct PoolCardNames
{
	Card flop[3];
	Card turn;
	Card river;
};

struct Pool {
	/*Union makes it possible to access pool cards sequentially or by name*/
	union
	{
		PoolCardNames names;
		Card cards[5];
	};

	/*Deal a random pool from the deck*/
	Pool(Deck& d) : cards{ d.getNextPoolCard(), d.getNextPoolCard(),
						  d.getNextPoolCard(), d.getNextPoolCard(), d.getNextPoolCard() } {}

	/*Use a very specific pool*/
	Pool(const Card* initCards) : cards{initCards[0], initCards[1], initCards[2], initCards[3], initCards[4] } {}
	Pool(const CardEnum* initCards) 
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i].number = getCardsNumber(initCards[i]);
			cards[i].suit = getCardsSuit(initCards[i]);
		}
	}

	Pool(const Pool& obj) 
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = obj.cards[i];
		}
	}

	Pool& operator=(const Pool& obj) 
	{
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = obj.cards[i];
		}
		return *this;
	}

	Pool() : cards() {}

	/*Put the old pool back into the deak then deal a new pool from the deck*/
	void newPool(Deck& deck) {
		deck.resetPool();
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = deck.getNextPoolCard();
		}
	}
		
	/*Set a new pool of cards from a desired list*/
	void setCards(const CardEnum* newCards) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i].number = getCardsNumber(newCards[i]);
			cards[i].suit = getCardsSuit(newCards[i]);
		}
	}

	void setCards(const Card* newCards) {
		for (int i = 0; i < NUM_POOL_CARDS; i++) {
			cards[i] = newCards[i];
		}
	}
};

/*Print the string representation of the pool*/
inline void printPool(const Pool& pool) 
{
	std::cout << "Pool cards: ";
	for (int i = 0; i < NUM_FLOP_CARDS + 1; i++)
	{
		std::cout << getCardsString(pool.cards[i]) << ", ";
	}
	std::cout << getCardsString(pool.names.river) << std::endl;
}

/*Print the string representation for a hand*/
inline void printHand(const Hand& hand)
{
	std::cout << "Hand: ";
	std::cout << getCardsString(hand.cards[0]) << ", ";
	std::cout << getCardsString(hand.cards[1]) << std::endl;;
}
