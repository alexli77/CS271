/**********************************
*Author: Hualong Li
*Date 1/10/18
*Description:  The program should now ask people to enter
*favorite animal, and after do it, it will print out 
*the favorite animal is the whatever typed.
***********************************/




#include <iostream>
#include <string>

//a simple example program
int main()
{
	std::string faveAnimal;
	std::cout << "Please enter your favorite animal." << std::endl;
	std::cin >> faveAnimal;
	std::cout << "Your favorite animal is the " << faveAnimal;
	std::cout << "." << std::endl;

	return 0;
}










