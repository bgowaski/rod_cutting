// HW5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Ben Gowaski EECE 7205 HW5
//#include "pch.h"
#include <iostream>
#include <stdio.h>    
#include <time.h>       
#include <math.h>
#include <float.h>
#include <limits.h>
#include <array>

using namespace std;

// Array of all prices of lengths 0-50
int price[50];

// Function prototypes
int get_recursive_max_price(int len, int * price_array, clock_t runTime);
int get_dynamic_max_price(int len, int * temp, int *price);


// Simple textbook max function
int max(int a, int b) { 
	return (a > b) ? a : b; 
}


// Get optimal price for rod cuts recursively
int get_recursive_max_price(int len, int *price, clock_t runTime) {
	clock_t clkCurr = clock();
	// error/null check
	if (len <= 0)
		return 0;
	// initialize max price to some very large and in this case
	// obscure number in order to find error cases and escape
	int maxPrice = 0xDEADBEEF;
	// Check timeout if takes too long and spins foreverrrrrrrr
	// Recursively cut the rod in different pieces and compare different  
	// configurations 
	for (int i = 0; i < len; i++)
	{
		// current clock time if iteration
		clkCurr = clock();
		maxPrice = max(maxPrice, price[i] + get_recursive_max_price(len - i - 1, price, runTime));
		double totalRunTime = (double)(clkCurr - runTime) / CLOCKS_PER_SEC;
		// Give it 3 minutes to run and timeout and see how far we get.....
		if (totalRunTime > 180)
		{
			//printf("MAX_TIMEOUT!!!");
			return 0xDEADBEEF;
		}
	}
	// return the best/optimal price found!
	return maxPrice;

}

// Get the optimal revenue for rod cuts dynamically
// temp is an array for the known best prices of all previous lengths
// up to the current length
int get_dynamic_max_price(int len, int * temp, int *price) {
	// null check for temp lengthArray to be used
	// in case there is already a best price stored there
	if (temp[len] >= 0) {

		return temp[len];
	}
	// initialize dynamic lengthArray for input of length L
	int *lengthArray = new int[len];

	// get the price from all of the previos max prices and add in price of current length cut
	for (int i = 0; i < len; i++) {
		lengthArray[i] = price[i] + get_dynamic_max_price(len - i - 1, temp, price);
	}
	// initialize the best/largest price to zero dollars
	int maxPrice = 0;
	// iterate over full array and get the best price and set to maxPrice
	for (int i = 0; i < len; i++) {
		if (lengthArray[i] > maxPrice) {
			maxPrice = lengthArray[i];
		}
	}
	// set the new known best price for length 'len' in the temporary array
	temp[len] = maxPrice;
	// delete array for next pass of new length 'len'
	delete[] lengthArray;
	// return current maxPrice for length 'len'
	return maxPrice;

}

int main(){

		// fill price[50] with prices from step function for

		for (int len = 0; len <= 50; len++)
		{
			if (len == 1)
			{
				price[len] = 2;
			}
			else if ((len > 1) && (len < 50))
			{
				price[len] = floor(len*2.5);
			}
			else if (len == 50)
			{
				price[len] = (len*2.5) - 1;
			}
			cout << "Price of rod length " << len << " is $" << price[len] << "\n";
		}

		// initialize current optimal price for cuts
		int optimalPrice = 0;
		// initialize array for storing all known best prices to use 
		// for dynamic programming (memoization) in hopes of 
		// running a lit faster!
		int bestPriceArray[51];

		for (int i = 0; i < 51; i++) {
			bestPriceArray[i] = 0xDEADBEEF;
		}

		cout << "Rod	 Recursive	   Recursive	 Dynamic    Dynamic\n";
		cout << "Size	   Time		  Max Revenue	  Time	   Max Revenue\n";
		cout << "--------------------------------------------------------------------\n";
		// recursive 
		for (int len = 0; len <= 30; len = len+5)
		{
			// Recursive Solution
			// Start time to run problem
			clock_t begin = clock();
			optimalPrice = get_recursive_max_price(len, price, begin);
			clock_t end = clock();
			double totalRtime = (double)(end - begin) / CLOCKS_PER_SEC;
			cout << len << "\t\t" << totalRtime << "\t\t" << optimalPrice << "\t\t";
			//// dynamic programming
			clock_t begin2 = clock();
			optimalPrice = get_dynamic_max_price(len, bestPriceArray, price);
			clock_t end2 = clock();
			double totalDtime = (double)(end2 - begin2) / CLOCKS_PER_SEC;
			cout << totalDtime << "\t\t" << optimalPrice << "\n";
		}

		return 1;
}
