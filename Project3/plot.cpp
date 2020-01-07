#include <iostream>
#include "grid.h"
#include <cctype>
#include <cassert>
#include <string>
using namespace std;

const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;
const int MAXROW = 20;
const int MAXCOL = 30;

int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
bool findDistance(string commandString, int stringpos, int& distance, int& posError, int& countOfDigits);
bool checkCharacter(string commandString, int spot, char& character);
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
bool returnUpdatedRow(int distance, int& updatedRow);
bool returnUpdatedColumn(int distance, int& updatedColumn);

int main()
{
	setSize(20, 30);
	char currentChar = '*';
	int currentMode = FG;
	for (;;)
	{
		cout << "Enter a command string: ";
		string cmd;
		getline(cin, cmd);
		if (cmd == "")
			break;
		int position;
		int status = performCommands(cmd, currentChar, currentMode, position);
		switch (status)
		{
		case 0:
			draw();
			break;
		case 1:
			cout << "Syntax error at position " << position + 1 << endl;
			break;
		case 2:
			cout << "Cannot perform command at position " << position + 1 << endl;
			break;
		default:
			cerr << "performCommands returned " << status << "!" << endl;
		}
	}
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
	if (commandString == "")
	{
		return 0;
	}
int countOfDigits = 0;
int dir;
int row = 1;
int column = 1;
int distance;
bool PositionErrorHappened = false; //flag used to indicate whether there is a plotting command that goes outside the plotting grid
	for (int k = 0;  k != commandString.size(); k++) //Goes through the command string looking for the letters that initiate plotting commands
	{
		if (isalpha(commandString[k])) // checks for letters
		{
			switch (commandString[k]) // depending on if it is a c, f, b, h, or v (lower or upper case) goes to perform the associated command
			{
			case 'C':
			case 'c': //clears grid, resetting the character, mode, and row/column position
				clearGrid();
				plotChar = '*';
				mode = FG;
				row = 1;
				column = 1;
				if (!isalpha(commandString[k + 1]) && (k+1)!=commandString.size()) //if there isn't a letter and c isn't the only command, error
				{
					badPos = k + 1;
					return 1;
				}
				continue;
			case 'F':
			case 'f':
			case 'B':
			case 'b':
				if (k+1 == commandString.size()) //If b/B or f/F don't have any value following them: Error!
				{
					badPos = k+1;
					return 1;
				}
				if (toupper(commandString[k]) == 'B') //set mode to background if B/b was inputted
				{
					mode = BG;
				}
				else if (toupper(commandString[k]) == 'F') //set mode to foreground if F/f was inputted
				{
					mode = FG;
				}
				if (!checkCharacter(commandString, k, plotChar)) //if the character after the F/f B/b is not an acceptable character, Error!
				{
					badPos = k + 1;
					return 1;
				}
				else //move to the digit after the F/f B/b (which will iterate again in the for loop to after the place after this plotting command)
				{
					k++;
					continue;
				}
				continue;
			case 'H':
			case 'h':
				if (1 == commandString.size()) //if H is the command, Error!
				{
					badPos = 1;
					return 1;
				}
				dir = HORIZ;
				if (!findDistance(commandString, k, distance, badPos, countOfDigits)) //If the characters after the H have any incorrect characters for the plotting command, Error!
				{
					badPos = badPos;
					return 1;
				}
				if ((column + distance > MAXCOL || column + distance < 1) && PositionErrorHappened == false) // checks if the distance found will plot outside the grid + turns the flag to true
				{
						badPos = k; //will not return this error if the command is not syntactically valid
						PositionErrorHappened = true;
				}
				if (countOfDigits == 1) //depending on the amount of characters after the plotting command, will iterate to skip those when going through the for loop
				{
					k += 1;
				}
				if (countOfDigits == 2)
				{
					k += 2;
				}
				if (countOfDigits == 3)
				{
					k += 3;
				}
				if (!plotLine(row, column, distance, dir, plotChar, mode)) //if plotLine returns false, it won't plot the grid but it will continue the loop to check if the syntax is correct
				{
					continue;
				}
				else
				{
					returnUpdatedColumn(distance, column); //if plotline is true, this will return the updated position of the column (passed by reference)
				}
				continue;
			case 'V':
			case 'v': //same functionality as above but will do it for the vertical plotting command - I did not combine Hh and Vv as I felt it would become more complicated
				if (1 == commandString.size())
				{
					badPos = 1;
					return 1;
				}
				dir = VERT;
				if (!findDistance(commandString, k, distance, badPos,countOfDigits))
				{
					badPos = badPos;
					return 1;
				}
				if ((row + distance > MAXROW || row + distance < 1) && PositionErrorHappened == false)
				{
						badPos = k;
						PositionErrorHappened = true;
				}
				if (countOfDigits == 1)
				{
					k += 1;
				}
				if (countOfDigits == 2)
				{
					k += 2;
				}
				if (countOfDigits == 3)
				{
					k += 3;
				}
				if (!plotLine(row, column, distance, dir, plotChar, mode))
				{
					continue;
				}
				else
				{
					returnUpdatedRow(distance, row); // if plotLine is true, returns updated row (passed by reference)
				}
				continue;
			default:
				badPos = k; //if it hits a letter other than the ones above, it is not a plotting command and therefore Error!
				return 1;
				break;
			}
		}
		else if(isdigit(commandString[k])) //if a digit is found outside the scope of a plotting command, Error!
		{
			badPos = k;
			return 1;
		}
		if (!isprint(commandString[k])) //if there is an invalid character, error!
		{
			badPos = k;
			return 1;
		}
		if (isprint(commandString[k]) && !isalpha(commandString[k])) //if a character is found outside the scope of a plotting command and it isn't a letter, Error!
		{
			badPos = k;
			return 1;
		}
	}
	if (PositionErrorHappened) //if the command string was syntactically valid but there was a command whose distance went outside the grid, Error!
	{
		return 2;
	}
return 0; //if there was no error, success!
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
	if (!isprint(plotChar)) //makes sure plotChar is a valid character
	{
	return false;
	}
	if (fgbg != FG && fgbg != BG) //makes sure mode is foreground or background
	{
		return false;
	}
	if (dir != HORIZ && dir != VERT) //makes sure direction is either horizontal or vertical
	{
		return false;
	}
	if (distance >= 0) //if distance is not negative
	{
		for (int p = 0; p <= distance; p++) //iterate through, adding to the column or row
		{
			if (dir == HORIZ)
			{
				if (c + distance > MAXCOL || c + distance < 1) //if distance will plot outside grid, Error!
				{
					return false;
				}
				else if (fgbg == FG) //if mode is foreground - replace what is there with your character
				{
					setChar(r, c + p, plotChar);
				}
				else if (fgbg == BG && getChar(r, c + p) == ' ') //if mode is background and the character at the spot is a space, replace it with your character
				{
					setChar(r, c + p, plotChar);
				}
				else if (fgbg == BG && isprint(getChar(r, c + p))) //if the character at the spot is not a space, keep that character there
				{
					setChar(r, c + p, getChar(r, c + p));
				}
			}
			else if (dir == VERT) // the same as above, but if the direction is vertical
			{
				if (r + distance > MAXROW || r + distance < 1)
				{
					return false;
				}
				else if (fgbg == FG)
				{
					setChar(r + p, c, plotChar);
				}
				else if (fgbg == BG && getChar(r + p, c) == ' ')
				{
					setChar(r + p, c, plotChar);
				}
				else if (fgbg == BG && isprint(getChar(r + p, c)))
				{
					setChar(r, c + p, getChar(r + p, c));
				}
			}
		}
	}
	else if (distance < 0)// if distance is negative
	{
		for (int p = 0; p <= -1*distance; p++) //make the distance positive and subtract it from each spot rather than adding it
		{
			if (dir == HORIZ) // the same as for positive distance except you are subtracting from the columns
			{
				if (c + distance > MAXCOL || c + distance < 1)
				{
					return false;
				}
				else if (fgbg == FG)
				{
					setChar(r, c - p, plotChar);
				}
				else if (fgbg == BG && getChar(r, c - p) == ' ')
				{
					setChar(r, c - p, plotChar);
				}
				else if (fgbg == BG && isprint(getChar(r, c - p)))
				{
					continue;
				}
			}
			else if (dir == VERT) // the same as for positive distance except you are subtracting from the row
			{
				if (r + distance > MAXROW || r + distance < 1)
				{
					return false;
				}
				else if (fgbg == FG)
				{
					setChar(r - p, c, plotChar);
				}
				else if (fgbg == BG && getChar(r - p, c) == ' ')
				{
					setChar(r - p, c, plotChar);
				}
				else if (fgbg == BG && isprint(getChar(r - p, c)))
				{
					continue;
				}
			}
		}
	}
	return true; // if function works correctly
}

bool findDistance(string commandString, int stringpos, int& distance, int& posError, int& countOfDigits) //returns an true and false and passes the distance value back by reference
{
	countOfDigits = 0; //will be passed back to the performCommands function in order to skip the digits this function parses
	
	if (stringpos + 1 == commandString.size()) //if there is no value after the H/V, error!
		{
			posError = stringpos + 1;
			return false;
		}
	
	stringpos++; //goes to the character after the H/V
	string distanceString = ""; //creates string to store digits
	if (!isprint(commandString[stringpos])) //if the character after H is not a valid character, Error
	{
		posError = stringpos;
		return false;
	}
	if ((isprint(commandString[stringpos]) && commandString[stringpos] != '-') && !isdigit(commandString[stringpos])) //if the value after the H/V is a letter and the character is not a - sign, error!
	{
		posError = stringpos;
		return false;
	}
	if (commandString[stringpos] == '-' && (!isdigit(commandString[stringpos + 1]) && !isalpha(commandString[stringpos+1]) && commandString[stringpos+1]!= '-'))// if first character after H/V was 
		//a - sign, and there isn't a letter or number after, error!
	{
		posError = stringpos+1;
		return false;
	}
	if (commandString[stringpos + 1] == '-') // if there is a minus sign that isn't right after the H/V, error!
	{
		posError = stringpos + 1;
		return false;
	}
if (commandString[stringpos] == '-')// if there is a - sign after H/V
{
	countOfDigits++;
	stringpos++;
	if (isalpha(commandString[stringpos])) //if is H-(letter) or V-(Letter) - error!
	{
		posError = stringpos;
		return false;
	}
	if (isalpha(commandString[stringpos + 1]) && toupper(commandString[stringpos+1]) != 'F' 
		&& toupper(commandString[stringpos + 1]) != 'B' && toupper(commandString[stringpos + 1]) != 'H' && toupper(commandString[stringpos + 1]) != 'V' 
		&& toupper(commandString[stringpos + 1]) != 'C') // if H-1(letter) or V-1(Letter) and that letter is not one of the plotting command letters, error!
	{
		posError = stringpos + 1;
		return false;
	}
	if ((isdigit(commandString[stringpos]) && !isdigit(commandString[stringpos + 1])) || (isdigit(commandString[stringpos]) && stringpos + 1 == commandString.size())) //if there is one digit after H-/V-
	{
		distanceString += commandString[stringpos]; //add digit to this string
		countOfDigits++;
		distance = atoi(distanceString.c_str()); //convert it to an int
		distance = -1 * distance; //multiply it by negative one 
		return true;
	}
	if (isdigit(commandString[stringpos]) && isdigit(commandString[stringpos + 1])) //if there are two digits after H-/V-
	{
		for (int m = stringpos; m != stringpos + 2; m++) //add both to the string
		{
			distanceString += commandString[m];
			countOfDigits++;
		}
		distance = atoi(distanceString.c_str()); //convert to int
		distance = -1 * distance; //multiply by negative one
		return true;
	}
}
else //if there isn't a minus sign after the H/V
{
	if ((isdigit(commandString[stringpos]) && !isdigit(commandString[stringpos + 1])) || (isdigit(commandString[stringpos]) && stringpos+1 == commandString.size())) //if there is only one digit after
	{
		distanceString += commandString[stringpos]; //add digit to string
		countOfDigits++;
		distance = atoi(distanceString.c_str()); //convert to int
		return true;
	}
	if (isdigit(commandString[stringpos]) && isdigit(commandString[stringpos + 1])) //if there are two digits after
	{
		for (int m = stringpos; m != stringpos+ 2; m++) // add both to string
		{
			distanceString += commandString[m];
			countOfDigits++;
		}
		distance = atoi(distanceString.c_str()); //conver to int
		return true;
	}
}
	posError = stringpos + 1; //if none of the other paths apply, must be invalid, therefore error!
	return false;
}

bool checkCharacter(string commandString, int spot, char& character) //checks if the character after the F/f or B/b is a valid character and returns it back by reference
{
	spot++;
	if (isprint(commandString[spot]))
	{
		character = commandString[spot];
		return true;
	}
	else
	{
		return false;
	}
}

bool returnUpdatedRow(int distance, int& updatedRow) // adds distance to row and passes it back by reference
{
	int r = distance + updatedRow;
	updatedRow = r;
	return true;

}

bool returnUpdatedColumn(int distance, int& updatedColumn) // adds distance to column and passes it back by reference
{
	int c = distance + updatedColumn;
	updatedColumn = c;
	return true;

}
