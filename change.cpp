/**********************************
*Author: Hualong Li
*Date 01/10/18
*Description:  The program ask the user
for a number of cents and outputs how many
of each type coin would represent that
amount with the fewest total number of coins.
**********************************/

#include <iostream>
using namespace std;

int main()
{
	int change, varQ, varD, varN, varP;

	cout << "Please enter an amount in cents less than a dollar.\n";
	cin >> change;

	varQ = change / 25;
	change = change % 25;
	varD = change / 10;
	change = change % 10;
	varN = change / 5;
	change = change % 5;
	varP = change;

	cout << "Your change will be:\n";
	cout << "Q: " << varQ << endl;
	cout << "D: " << varD << endl;
	cout << "N: " << varN << endl;
	cout << "P: " << varP << endl;

	return 0;
}
