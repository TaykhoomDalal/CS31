#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int appendToAll(string a[], int n, string value)
{
	if (n < 0) //negative n value = error
		return -1;
	for (int k = 0; k < n; k++)
	{
		a[k] += value; //append the value to each of the elements in the array
	}
	return n;
}

int lookup(const string a[], int n, string target)
{
	if (n <= 0)//negative n value or 0 elements in array = error
		return -1;
	for (int k = 0; k < n; k++)
	{
		if (a[k] == target) //if the target is inside the array, return the spot of the target 
		{
			return k;
		}		
	}
	return -1;
}

int positionOfMax(const string a[], int n)
{
	if (n <= 0)//negative n value or 0 values in the array to check = error
		return -1;
	int highest = 0; //set highest value to 0
	for (int k = 0; k < n; k++)
	{
		if (a[k] > a[highest])//compares each value against the highest value, if it is greater, then set it to the highest value
			highest = k;
	}
	return highest;
}

int rotateLeft(string a[], int n, int pos)
{
	if (n <= 0) //negative n value = error
		return -1;
	if (pos >= n || pos < 0) //error if position is less than 0 or bigger than or equal to the n value
		return -1;
	string position = a[pos]; //store the position that is going to be switched
	for (int k = pos; k < n-1 ; k++) 
	{
		a[k] = a[k + 1]; //move all the values back one
	}
	a[n - 1] = position; //replace the last value with the stored value of the specified position
	return pos;
}

int countRuns(const string a[], int n)
{
	if (n < 0) //negative n value = error
		return -1;
	if (n == 0) //equals 0 sequences
		return 0;
	int sequences = 0; //counter of how many sequences are present in the array
	for (int k = 0; k < n; k++)
	{
		if (k + 1 == n) //in case the program is going to go to the while loop but it is at the n-1 element it needs to break otherwise the while loop will access outside the array size
		{
			sequences++;
			break;
		}
		while (a[k] == a[k + 1])//if there are multiple of the same string in the array, it should cycle through them and not add to the number of sequences
		{
			k++;
			if (k + 1 == n)//makes sure that when it goes into the while loop again it doesn't access past n
				break;
			else
				continue;
		}
		sequences++; //adds to the sequence every time there is one or more terms in a row
	}
	return sequences;
}

int flip(string a[], int n)
{
	if (n < 0) //negative n value = error
		return -1;
	int theEnd = n-1; //the last term in the specified array
	string temporaryStorage = ""; //storage for when swapping the values
	for (int start = 0; start <= theEnd; start++)
	{
		temporaryStorage = a[start]; //puts the starting value into storage
		a[start] = a[theEnd];//sets the end value to to the start position
		a[theEnd] = temporaryStorage; //sets the start value into the end position
		theEnd--; //keeps going back through the array
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0) //negative n value = error
		return -1;
	if(n1 == 0 && n2 == 0) //if both are 0, both arrays are empty and equal up to the last point but you can't return a number that represents an empty string, so error
		return -1;
	if (n1 == n2) //if both arrays are of the same size
	{
		for (int k = 0; k < n1; k++) //since both are the same size, the k value can be less than either n values
		{
			if (a1[k] == a2[k]) //do nothing if the values are the same throughout the arrray
				continue;
			else
				return k; //return the position if there is a difference in values
		}
		return n1; //gets here if the arrays have no differences up until n1 runs out, since both n values are the same size, doesn't matter which is returned
	}
	else if (n1 > n2) //if the first array is bigger than the second array
	{
		for (int k = 0; k < n2; k++) //since n2 is smaller than n1, the for loop needs to end at the smaller loop
		{
			if (a1[k] == a2[k])
				continue;
			else
				return k;
		}
		return n2; //gets here if the arrays have no differences up until n1 runs out, thus returning n2 as it is less than n1
	}
	else //if the second array is bigger than the first array
	{
		for (int k = 0; k < n1; k++) //since n1 is smaller than n2, the for loop needs to end at the smaller loop
		{
			if (a1[k] == a2[k]) 
				continue;
			else
				return k;
		}
		return n1; //gets here if the arrays have no differences up until n1 runs out, thus returning n1 as it is less than n2
	}
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0) //negative n value = error
		return -1;
	if (n2 > n1)
		return -1;
	if (n2 == 0)
		return 0;
	int pos = 0;
	int correctSub = 0;
	for (int k = 0; k < n1; k++)
	{
		if (a2[0] == a1[k])//checking if the first value of the second array is present anywhere in the first array
		{
			pos = k; //sets the position of where first match up between the two arrays is
			for (int i = 1; i < n2; i++) //now goes through the full length of the second array
			{
				if (a2[i] == a1[pos + i])//checks if the values after the first position in the second array are continguously present in the first array
				{
					correctSub++; // if they are, add one to correct subsequence
					continue;
				}
				else //otherwise set correct subsequence to -1 and break
				{
					correctSub = -1;
					break;
				}
			}
			if (correctSub == n2 - 1) //if all elements of second array are present in first array, correctSub should have incremented to n2 -1
				return k; //thus return the position of where the first position of the second array was found in the first array
		}	
		else //if not continue cycling through, looking for the sequence
			continue;
	}
	return -1; //only gets here if a1 does not contain a2 as a contigous sequence
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 <= 0 || n2 <= 0) //negative n value = error
		return -1;
	for (int k = 0; k < n1; k++) //goes through length of first array
	{
		for (int i = 0; i < n2; i++) //goes through length of second array
		{
			if (a1[k] == a2[i]) //compares value of first array to second array, iterating through second array
				return k; //returning first spot where they are equal
			else 
				continue; //otherwise continuing through the sequence
		}
	}
	return -1; //if no element of a1 is equal to any element of a2, error
}

int separate(string a[], int n, string separator)
{
	if (n < 0) //negative n value = error
		return -1;
	if (n == 0)
		return 0;
	int count = 0;
	for (int k = 0; k < n; k++) //goes through the array
	{
		if (separator > a[k])
			count++; //counts how many values are smaller than the separator
		else
			continue;
	}
	if (count == 0) //if all the values are greater than the separator, the 0th position string is the first element that is not less than the separator
		return 0;
	else if (count == n) //if all the valuesa are smaller than the separator, return n
		return n;
	else //if the above statements are false, then count must be greater than 0
	{
		string tempStorage = ""; //temporary storage variable
		for (int k = 0; k < count; k++) //goes through all values in the array that areless than the spot where the separation will be
		{
			if (a[k] >= separator) //if the value here is greater than or equal to the separator
			{
				for (int i = count; i < n; i++) //go through values that are past where the separation will be
				{
					if (a[i] < separator) // check for values that are smaller than the separator and swap them with the value of a[k] 
					{
						tempStorage = a[i];
						a[i] = a[k];
						a[k] = tempStorage;
						break;
					}
					else
						continue;
						
				}
			}
		}
		if (lookup(a, n, separator) == -1) //if the separator string is not present in the array
		{
			return count; //returns the position of where the string is not less than separator
		}
		else //if the separator string IS in the array
		{
			int temporaryCount = count; //make a temporary count
			for (int k = 0; k < n; k++) //loop through the array
			{
				if (a[k] == separator)//if the separator is found, swap it to separation point
				{
					tempStorage = a[temporaryCount];
					a[temporaryCount] = a[k];
					a[k] = tempStorage;
					temporaryCount++; //and add one from the count in case separator is present multiple times in the array
				}
			}
		}
	return count;
	}
}
