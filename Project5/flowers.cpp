#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable:6262)

#include "utilities.h"
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

const char WORDFILENAME[] = "D:/CS31/Project5/words.txt"; 
const int MAXWORDS = 10000; //sets max possible words in the list

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);
void findFlowersAndBees(char TRIAL_WORD[MAXWORDLEN + 1], char MYSTERY_WORD[MAXWORDLEN + 1], int TRIAL_LENGTH, int MYSTERY_LENGTH, int* FLOWERS, int* BEES);

int main()
{
	char wordList[MAXWORDS][MAXWORDLEN + 1]; //creates an array of c-strings
	int nWords = getWords(wordList, MAXWORDS, WORDFILENAME); // returns the number of words filled in the c-string array
	
	if (nWords < 1) //If nWords is less than 1, return error 
	{
		cout << "No words were loaded, so I can't play the game.";
		return -1;
	}

	cout << "How many rounds do you want to play? "; //asks the user for the number of rounds and stores that value
	int NUM_ROUNDS; 
	cin >> NUM_ROUNDS;
	cin.ignore(10000, '\n');
	
	if (NUM_ROUNDS < 1) //If the number of rounds is not positive, return error
	{
		cout << "The number of rounds must be positive.";
		return -1;
	}
	cout.setf(ios::fixed);
	cout.precision(2); //when the average is printed out, it has two significant figures after decimal point

	double TOTAL_TRIES = 0;
	double AVERAGE_TRIES = 0;
	int MIN_TRIES = 0;
	int MAX_TRIES = 0;
	char MYSTERY_WORD[MAXWORDLEN + 1] = {};

		for (int round = 1; round <= NUM_ROUNDS; round++) //loops through the specified number of rounds
		{
			cout << '\n' << "Round " << round << endl; //tells the user what round they are on
			
			int wordnum = randInt(0, nWords - 1); //returns a random int that is within the bounds of the number of words in the array
			strcpy(MYSTERY_WORD, wordList[wordnum]);
			cout << "The mystery word is " << strlen(MYSTERY_WORD) << " letters long." << endl; //outputs the length of the mystery word

			int TRIES = playOneRound(wordList, nWords, wordnum); //calls the playOneRound function to return the user's tries after they play
			
			if (TRIES == -1) //checks if playOneRound is passed bad arguments
			{
				cout << "A bad argument was passed to the playOneRound function.";
					return -1;
			}
			if (round == 1) //if the round is 1, set the min and max tries to the number of tries
			{
				MIN_TRIES = TRIES;
				MAX_TRIES = TRIES;
			}
			else if (TRIES > MAX_TRIES) //update the max value
				MAX_TRIES = TRIES;
			else if (TRIES < MIN_TRIES) //update the min value
				MIN_TRIES = TRIES;
			
			TOTAL_TRIES += TRIES; //adds the score to the total tries each round
			AVERAGE_TRIES = (TOTAL_TRIES) / round; //finds average tries each round

			if (TRIES == 1)
				cout << "You got it in 1 try." << endl; 
			else
				cout << "You got it in " << TRIES << " tries." << endl; //when the user takes more than one try

			cout << "Average: " << AVERAGE_TRIES << ", minimum: " << MIN_TRIES << ", maximum: " << MAX_TRIES << endl; //average, min and max
		}
}

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
	if (nWords < 1 || wordnum < 0 || wordnum >= nWords) //if nWords is not positive, wordnum is negative, or wordnum is greater or equal to nWords, error!
		return -1;

	int tries = 0;
	char MYSTERY_WORD[MAXWORDLEN + 1] = {};
	strcpy(MYSTERY_WORD, words[wordnum]); 

	for (;;) //repeatedly asking for a trial word until user gets it right
	{
		cout << "Trial word: "; //asks the user to input a trial word
		char TRIAL_WORD[100] = { '\0' };
		cin.getline(TRIAL_WORD, 100);

		if (strcmp(TRIAL_WORD, MYSTERY_WORD) == 0) //if the user gets it right, add one to the score and leave the loop
		{
			tries++;
			break;
		}

		if ((strlen(TRIAL_WORD) > MAXWORDLEN) || (strlen(TRIAL_WORD) < MINWORDLEN)) //if trial word is smaller or bigger than the range 4-6 letters, error
		{
			cout << "Your trial word must be a word of 4 to 6 letters." << endl;
			continue;
		}

		bool INCORRECT_CHARACTER = false; //flag for finding an incorrect letter
		for (int k = 0; k != strlen(TRIAL_WORD); k++) //checks if the trial word has any characters that are not lower case letters
		{
			if (isupper(TRIAL_WORD[k]) || (isprint(TRIAL_WORD[k]) && isalpha(TRIAL_WORD[k]) == false))
			{
				cout << "Your trial word must be a word of 4 to 6 letters." << endl; //if it does, return error and change flag to true
				INCORRECT_CHARACTER = true;
				break;
			}
		}
		if (INCORRECT_CHARACTER == true) //if there is a non-lower case letter, repeat the loop
		{
			continue;
		}

		bool WORD_IS_NOT_PRESENT = true; //flag for finding an unknown word
		for (int i = 0; i != nWords; i++) 
		{
			if (strcmp(TRIAL_WORD, words[i])==0) //checks the trial word against list of words
			{
				WORD_IS_NOT_PRESENT = false; //if it's present, change the flag to false and break out
				break;
			}
		}
		if (WORD_IS_NOT_PRESENT == true) //if the flag is true, return the error and restart loop
		{
			cout << "I don't know that word." << endl;
			continue;
		}
		int numberOfFlowers = 0;
		int numberOfBees = 0;

		findFlowersAndBees(TRIAL_WORD, MYSTERY_WORD, strlen(TRIAL_WORD), strlen(MYSTERY_WORD), &numberOfFlowers, &numberOfBees); //passes number of flowers and bees back
		cout << "Flowers: " << numberOfFlowers << ", Bees: " << numberOfBees << endl; //tells the user how many flowers and bees were found in their trial word

		tries++; //add to the score
		continue;
	}
	return tries; //returns the score to the main function
}

void findFlowersAndBees(char TRIAL_WORD[MAXWORDLEN + 1], char MYSTERY_WORD[MAXWORDLEN + 1], int TRIAL_LENGTH, int MYSTERY_LENGTH, int* FLOWERS, int* BEES )
{
	int BIGGER_LENGTH; //will be the length of the larger c-string
	int SMALLER_LENGTH; //will be the length of the smaller c-string
	char BIGGER_WORD[MAXWORDLEN + 1] = { '\0' }; //creates a temporary c-string larger word
	char SMALLER_WORD[MAXWORDLEN + 1] = { '\0' }; //creates a temporary c-string for smaller word
	
	if (TRIAL_LENGTH >= MYSTERY_LENGTH) //if the trial word is larger than the mystery word - assign it to the "big" c-string and vice versa for the mystery word
	{
		BIGGER_LENGTH = TRIAL_LENGTH;
		SMALLER_LENGTH = MYSTERY_LENGTH;
		strcpy(BIGGER_WORD, TRIAL_WORD);
		strcpy(SMALLER_WORD, MYSTERY_WORD);
	}
	else // if the TRIAL_LENGTH will be < MYSTERY_LENGTH - do the same as above but with the mystery word as the "big" c-string
	{
		BIGGER_LENGTH = MYSTERY_LENGTH;
		SMALLER_LENGTH = TRIAL_LENGTH;
		strcpy(BIGGER_WORD, MYSTERY_WORD);
		strcpy(SMALLER_WORD, TRIAL_WORD);
	}

	for (int T = 0; T != BIGGER_LENGTH; T++) //starts looping with the longer word
	{
		for (int M = 0; M != SMALLER_LENGTH; M++) //loops through smaller word
		{
			if (BIGGER_WORD[T] == '!') //if larger c-string's character is !, means there was a flower - go to the next trial word letter
				break;
			if (SMALLER_WORD[M] == '!') //if smaller c-string's character is !, means there was a flower - continue checking the other letters
				continue;
			if (M == T && (SMALLER_WORD[M] == BIGGER_WORD[T])) //if letter is at same spot in both words - there was a flower
			{
				(*FLOWERS)++; //this value is added to the numberOfFlowers variable in the playOneRound function as it is a pointer
				BIGGER_WORD[T] = '!'; //change the characters in both c-strings to ! marks
				SMALLER_WORD[M] = '!';
				continue;
			}
			else //if not, keep checking
				continue;
		}
	}
	for (int T = 0; T != BIGGER_LENGTH; T++) //checks for bees after checking for flowers as flowers take priority
	{
		for (int M = 0; M != SMALLER_LENGTH; M++)
		{
			if (BIGGER_WORD[T] == '!')
				break;
			if (SMALLER_WORD[M] == '!')
				continue;
			if (SMALLER_WORD[M] == BIGGER_WORD[T]) //if a letter is present in both words and isnt already a flower, increment bees
			{
				(*BEES)++; ////this value is added to the numberOfBees variable in the playOneRound function as it is a pointer
				BIGGER_WORD[T] = '!'; //change letters in both c-strings to ! marks
				SMALLER_WORD[M] = '!';
				continue;
			}
			else
				continue;
		}
	}
}
