#include <iostream>
#include <string>
using namespace std;

int main() {
	cout.setf(ios::fixed);
	cout.precision(1); // Makes sure that there is only one significant figure following the decimal point

	string NAME; // Getting the input about the defendant name from the user
	cout << "Defendant: ";
	getline(cin, NAME);

	if (NAME == "") // In case the name entered is an empty string, returning the specified response, & ending the program
	{
		cout << "---" << endl << "You must enter a defendant name." << endl;
		return 1;
	}

	double AMTPAID; // Getting the input about the amount paid from the user 
	cout << "Amount paid (in thousands): ";
	cin >> AMTPAID;
	cin.ignore(10000, '\n');

	if (AMTPAID < 0) // In case the amtPaid is a negative number, returning the specified response, & ending the program
	{
		cout << "---" << endl << "The amount paid must not be negative." << endl;
		return 1;
	}

	string ATHTYPE; // Getting the input about whether the user paid for faking athletic credentials
	cout << "Fake athlete? (y/n): ";
	getline(cin, ATHTYPE);

	if ((ATHTYPE != "y") && (ATHTYPE != "n")) // In case the athType wasn't "y" or "n", returning the specified response, & ending the program
	{
		cout << "---" << endl << "You must enter y or n." << endl;
		return 1;
	}

	cout << "---" << endl; // Returns the three hyphens that precede the last line of the program
	double BASEFEE = 46.4, ADDEDFEE{}; //BASEFEE accounts for the fee for the first 40,000 paid 

	if (AMTPAID > 250) // For when the amtPaid is greater than 250, the program goes to this block of code
	{
		if (ATHTYPE == "n") // When the user didn't pay for faking athletic credentials
		{
			ADDEDFEE = ((AMTPAID - 250) * .14) + 21;
		}
		else if (ATHTYPE == "y") // When the user did pay for faking athletic credentials
		{
			ADDEDFEE = ((AMTPAID - 250) * .14) + 46.2;
		}
	}
	else if (AMTPAID <= 40) /* When the amtPaid wasn't greater than 250, the program goes to this block of code
		This if statement is if the amtPaid was less than or equal to 40 */
	{
		ADDEDFEE = (AMTPAID * .66) + 20;
		cout << "The suggested fine for " << NAME << " is $" << ADDEDFEE << " thousand." << endl; // outputs total fee for this condition only
		return 1;
	}
	else if (((AMTPAID - 40) <= 210) && (ATHTYPE == "n")) /* When the amtPaid wasn't greater than 250 and WAS greater than
		40, but the user didn't pay for faking athletic credentials, the program goes to this block of code */
	{
		ADDEDFEE = (AMTPAID - 40) * .1;
	}
	else if (((AMTPAID - 40) <= 210) && (ATHTYPE == "y")) /* When the amtPaid wasn't greater than 250 and WAS greater than
		 40, but the user did pay for faking athletic credentials, the program goes to this block of code */
	{
		ADDEDFEE = (AMTPAID - 40) * .22;
	}
	cout << "The suggested fine for " << NAME << " is $" << ADDEDFEE + BASEFEE << " thousand." << endl; /* This outputs the total fee
	 for each of the conditions above except for when the amtPaid was less than or equal to 40 */
}
