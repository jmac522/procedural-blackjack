/* John McCarthy
CIT-120 Final Project
Blackjack Game
*/
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <random>		// for default_random_engine
#include <stdlib.h> // srand, rand
#include <time.h>		// get current time for unique random seeds
#include <functional> // for greater objects
using namespace std;

// function prototypes
void printMainMenu();
void shuffleDeck(vector<int>&); // function to shuffle a deck
string printCard(int); // function to print a card as a string
void dealHand(vector<int>&, vector<int>&, vector<int>&, int &); // Function to deal an inital hand
void playGame(); // main game function
void showRoundMenu(); // Display the Hit/Stand options
void playRound(vector<int>&, bool &); // function for a signle round of the game
int getCardValue(int, int &, int &); // Determine numerical value of a given card
int getHandTotal(vector<int>&, int &); // Find total value of a hand
void playerRound(vector<int>&, vector<int>&, int &, bool &, int); // Players turn after initial hand is dealt
void dealPlayerCard(vector<int>&, vector<int>&, int &, bool &, int); // Deal a single card in player rounds
void dealerRound(vector<int>&, vector<int>&, int &, bool &); // Handles dealer after player has finished
bool playAgain(); // Ask user if they want to play another hand
void checkWinner(int, int);

// assign integer values to card type constants
const int ace = 100,
deuce = 200,
three = 300,
four = 400,
five = 500,
six = 600,
seven = 700,
eight = 800,
nine = 900,
ten = 1000,
jack = 1100,
queen = 1200,
king = 1300;

// Create a vector constand for a deck to use for building a deck
const vector<int> DECK = { 100, 100, 100, 100, 200, 200, 200, 200, 300, 300,
300, 300, 400, 400, 400, 400, 500, 500, 500, 500,
600, 600, 600, 600, 700, 700, 700, 700, 800, 800,
800, 800, 900, 900, 900, 900, 1000, 1000, 1000, 1000,
1100, 1100, 1100, 1100, 1200, 1200, 1200, 1200,
1300, 1300, 1300, 1300 };

// string constant for formatting
const string INDENT = "\t\t\t\t\t";

void main()
{
	// variable for holding menu choice
	int menu_choice;

	// display the main menu and get choice from user
	printMainMenu();
	cout << "\t\tSelection: ";
	cin >> menu_choice;

	// validate user input
	while (menu_choice < 1 || menu_choice > 2) {
		cout << "Please make a valid selection: ";
		cin >> menu_choice;
	}

	// Start game or exit program based on selection
	switch (menu_choice) {
	case 1:
	{
		// NEW GAME

		// Clear the console
		system("cls");

		// Start the game
		playGame();
		break;
	}
	case 2:
		// QUIT
		cout << "Thanks for playing!!\n";
		break;
	}

	system("pause");
}

void shuffleDeck(vector<int>& deck) {
	// Generator for shuffle function
	auto randomNumberGenerator = default_random_engine{};

	// Use current time as seed to prevent same output everytime program is run
	srand(time(NULL));

	// Generate number between 1 and 100 for number of times to shuffle the deck
	int shuffle_multiplier = rand() % 100 + 1;

	// shuffle multiple times to avoid same output
	for (int i = 0; i < shuffle_multiplier; i++)
		shuffle(begin(deck), end(deck), randomNumberGenerator);
}

string printCard(int card) {
	string card_type;
	switch (card) {
	case ace:
		card_type = "A";
		break;
	case deuce:
		card_type = "2";
		break;
	case three:
		card_type = "3";
		break;
	case four:
		card_type = "4";
		break;
	case five:
		card_type = "5";
		break;
	case six:
		card_type = "6";
		break;
	case seven:
		card_type = "7";
		break;
	case eight:
		card_type = "8";
		break;
	case nine:
		card_type = "9";
		break;
	case ten:
		card_type = "10";
		break;
	case jack:
		card_type = "J";
		break;
	case queen:
		card_type = "Q";
		break;
	case king:
		card_type = "K";
		break;
	}

	return card_type;
}

void printMainMenu() {
	const int WIDTH = 40;
	cout << INDENT << char(201) << string(39, 205) << char(187) << "\n";
	cout << INDENT << char(186) << "\tWelcome to C++ Blackjack\t" << char(186) << "\n";
	cout << INDENT << char(200) << string(39, 205) << char(188) << "\n";
	cout << INDENT << "\t\t1. New Game\n";
	cout << INDENT << "\t\t2. Quit\n";
}

void dealHand(vector<int>& deck, vector<int>& player_hand, vector<int>& dealer_hand, int &player_total) {
	system("cls");
	cout << INDENT << char(201) << string(25, 205) << char(187) << "\n";
	cout << INDENT << char(186) << "     Dealing hand...     " << char(186) << "\n";
	cout << INDENT << char(200) << string(25, 205) << char(188) << "\n";
	// Deal 2 cards to each player
	for (int i = 0; i < 2; i++) {
		player_hand.push_back(deck.back());

		deck.pop_back(); // Remove card from deck
		dealer_hand.push_back(deck.back());
		deck.pop_back();
	}

	cout << INDENT << "The player has " << printCard(player_hand[0])
		<< " and  " << printCard(player_hand[1]) << " totaling "
		<< getHandTotal(player_hand, player_total) << endl;
	cout << INDENT << "The dealer is showing " << printCard(dealer_hand[1]) << endl;
}

void playGame() {
	// create a deck for the game and shuffle it
	vector<int> game_deck = DECK;
	shuffleDeck(game_deck);

	// boolean flag for tracking if user wishes to continue playing. True by default
	bool continue_game = true;

	while (continue_game) {
		if (game_deck.size() < 10) {
			// generate new deck if not enough cards for a new round
			cout << "\n\n";
			cout << INDENT << char(201) << string(25, 205) << char(187) << "\n";
			cout << INDENT << char(186) << "    Shuffling Deck...    " << char(186) << "\n";
			cout << INDENT << char(200) << string(25, 205) << char(188) << "\n";
			game_deck = DECK;
			shuffleDeck(game_deck);
		}
		playRound(game_deck, continue_game);
		// continue_game = false; // temporary flag change to avoid infinite loop
	}
	// system("cls");
}

void showRoundMenu() {
	cout << INDENT << "\t1. Hit\n";
	cout << INDENT << "\t2. Stand\n";
}

void playRound(vector<int>& game_deck, bool &continue_game) {
	// Vectors for player's hand and dealer's hand
	vector<int> player_hand, dealer_hand;

	// initialize counters for player and dealer hand totals
	int player_total = 0, dealer_total = 0;

	// Bust flags to keep track of if player or dealer busts
	bool bust = false;
	bool dealer_bust = false;
	bool play_again;

	// Deal initial hand
	dealHand(game_deck, player_hand, dealer_hand, player_total);

	// Go through players turn
	playerRound(player_hand, game_deck, player_total, bust, dealer_hand[1]);

	if (!bust) {
		// Go through dealer's turn
		dealerRound(dealer_hand, game_deck, dealer_total, dealer_bust);
		// Determine winner
		if (!dealer_bust) {
			checkWinner(player_total, dealer_total);
		}
	}

	if (!playAgain()) {
		continue_game = false;
	}
}

int getCardValue(int card, int &hand_total, int &ace_counter) {
	if (card >= deuce && card <= ten) {
		// if card is between 2 or 10, its value is what is listed on the card
		hand_total += card / 100;
	}
	else if (card > ten) {
		// face cards all have the value of 10
		hand_total += 10;
	}
	else {
		// If the card is an ace, add 11 and increment ace counter for later bust check
		hand_total += 11;
		ace_counter += 1;
	}

	return hand_total;
}

int getHandTotal(vector<int>& hand, int &hand_total) {
	hand_total = 0;
	int ace_counter = 0;
	for (int card : hand) {
		// increment total for each cart in hand thats passed to the function
		getCardValue(card, hand_total, ace_counter);
	}

	if (ace_counter > 0) {
		// adjust ace value from 11 to 1 if 11 will bust the hand
		for(ace_counter; ace_counter > 0; ace_counter--) {
			if (hand_total > 21) {
				hand_total -= 10;
			}
		}
	}
	// return final total
	return hand_total;
}

void playerRound(vector<int>& hand, vector<int>& deck, int &hand_total, bool &bust, int dealer_showing) {
	bool stand = false; // Keep track of if player stands to end turn
	// Menu Choice variable
	int menu_choice;
	while (!bust && !stand) {

		// Show options for hit/stand
		showRoundMenu();

		// Get and validate user choice
		cout << "\t\tChoice: ";
		cin >> menu_choice;
		cout << "\n\n";

		while (menu_choice < 1 || menu_choice > 2) {
			cout << "Please make a valid selection: ";
			cin >> menu_choice;
		}

		switch (menu_choice) {
		case 1:
			// Player hits
			dealPlayerCard(hand, deck, hand_total, bust, dealer_showing);
			break;
		case 2:
			// Player Stands
			stand = true;
			cout << INDENT << "You stand at " << getHandTotal(hand, hand_total) << endl;
			cout << "\n\n";
			break;
		}
	}
}

void dealPlayerCard(vector<int>& hand, vector<int>& deck, int &hand_total, bool &bust, int dealer_showing) {
	hand.push_back(deck.back());
	deck.pop_back();
	// system("cls");
	cout << INDENT << "You are dealt " << printCard(hand.back()) << endl;
	cout << INDENT << "You have ";
	for (int card : hand) {
		cout << printCard(card) << " ";
	}
	cout << "totalling " << getHandTotal(hand, hand_total) << endl;
	if (hand_total > 21) {
		bust = true;
		cout << INDENT << "You busted! Better luck next time.\n\n\n";
	}
	else {
		cout << INDENT << "The dealer is showing " << printCard(dealer_showing) << "\n\n";
	}

}

void dealerRound(vector<int>& hand, vector<int>& deck, int &hand_total, bool &bust) {
	cout << INDENT << "Dealer will play now....\n";
	// Display both dealers cards
	cout << INDENT << "The dealer has ";
	for (int card : hand) {
		cout << printCard(card) << " ";
	}
	cout << "totalling " << getHandTotal(hand, hand_total) << endl;

	// The dealer must hit until his hand is 16 or greater or until busting
	while (!bust && hand_total < 16) {
		hand.push_back(deck.back());
		deck.pop_back();
		cout << INDENT << "The dealer is dealt " << printCard(hand.back()) << endl;
		cout << INDENT << "Their total is " << getHandTotal(hand, hand_total) << endl;
		if (hand_total > 21) {
			bust = true;
			cout << INDENT << "The Dealer busted. You Win!\n";
		}
	}
	cout << "\n\n";
}

bool playAgain() {
	bool play_again;
	int menu_choice;
	cout << INDENT << "Want to play another hand?\n\n";
	cout << INDENT << "\t1. Yes\n";
	cout << INDENT << "\t2. No\n";
	cout << "\t\tChoice: ";
	cin >> menu_choice;

	while (menu_choice < 1 || menu_choice > 2) {
		cout << "Please make a valid selection: ";
		cin >> menu_choice;
	}

	switch (menu_choice) {
	case 1:
		play_again = true;
		break;
	case 2:
		play_again = false;
		cout << "Goodbye...\n";
		break;
	}

	return play_again;
}

void checkWinner(int player_total, int dealer_total) {
	cout << INDENT << "The player has " << player_total << endl;
	cout << INDENT << "The dealer has " << dealer_total << endl;
	cout << "\n\n";
	if (player_total > dealer_total) {
		cout << INDENT << "You win!!\n";
	}
	else if (dealer_total > player_total) {
		cout << INDENT << "The dealer wins. Better luck next time!\n";
	}
	else {
		cout << INDENT << "Push!\n";
	}
	cout << "\n\n";
}
