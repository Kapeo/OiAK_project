#include <iostream>
#include "my_fixedpoint.h"


int main() 
{
	myfixedpoint mf1(10240, 64);
	myfixedpoint mf2(10240, 64);
	std::cin >> mf1;
	std::cin >> mf2;
	std::cout << "\n oto liczba: \n" << mf1;
	std::cout << "\n oto liczba: \n" << mf2;
	
	myfixedpoint mf3 = mf1 + mf2;
	std::cout << "\n oto suma: \n" << mf3;
	
	myfixedpoint mf4 = mf1 - mf2;
	std::cout << "\n oto roznica: \n" << mf4;
	
	myfixedpoint mf5 = mf1 * mf2;
	std::cout << "\n oto iloczyn: \n" << mf5;
	
	myfixedpoint mf6 = mf1 / mf2;
	std::cout << "\n oto iloraz: \n" << mf6;
	
	
	return 0;
}
