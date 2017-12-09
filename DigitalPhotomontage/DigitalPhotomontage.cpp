// DigitalPhotomontage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Tests.h"
#include "Test2.h"
#include<iostream>

int main(int argc, char** argv)
{
	std::cout << "Choose the test to launch between: \n\t1.Interactive digital photomontage\n\t2.Maximum likelyhood\n";
	int a;
	std::cin >> a;
	if (a == 1)
		test1(argv[1], argv[2]);
	else 
		if (a == 2)
		test2(argv[1], argv[2]);
	else
		std::cout << "Incorrect input" << std::endl;
	return 0;
}

