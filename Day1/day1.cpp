#pragma once

#include <iostream>
#include <cstdlib>
#include <queue>
#include <vector>

#include "../_utils/utils.h"

#define STD_MIN_HEAP std::priority_queue<int, std::vector<int>, std::greater<int>>

bool ParseFile(STD_MIN_HEAP &heap_a, STD_MIN_HEAP &heap_b)
{
	char* buffer = nullptr;
	size_t size = LoadFileToBuffer("day1.txt", &buffer);
	if (size == 0) return false;

	bool reading = false;

	int value = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (reading && buffer[i] == ' ')
		{
			heap_a.push(value);
			value = 0;
			reading = false;
		}
		else if (buffer[i] == '\n')
		{
			heap_b.push(value);
			value = 0;
			reading = false;
		}
		else if (buffer[i] >= '0' && buffer[i] <= '9')
		{
			reading = true;
			value = 10 * value + (buffer[i] - '0');
		}
	}
	if (value != 0) heap_b.push(value);

	ReleaseBuffer(&buffer);
	return true;
}


int main(int argc, char * argv[])
{
	STD_MIN_HEAP heap_a;
	STD_MIN_HEAP heap_b;

	if (!ParseFile(heap_a, heap_b)) return EXIT_FAILURE;
	if (heap_a.size() != heap_b.size())
	{
		std::cerr << "Error: Heap size mismatch\n";
		return EXIT_FAILURE;
	}
 
	/*
	int distance = 0;
	while (!heap_a.empty())
	{
		int value = heap_b.top() - heap_a.top();
		distance += (value >= 0 ? value : -value);

		heap_a.pop();
		heap_b.pop();
	}
	std::cout << "Part 1 Solution: " << distance << std::endl;
	*/

	unsigned long long score = 0;
	
	int repetition = 0;
	int key = heap_a.top();

	while (!heap_a.empty() && !heap_b.empty())
	{
		int A = heap_a.top();
		int B = heap_b.top();

		if (heap_b.top() < heap_a.top())
		{
			heap_b.pop();
		}
		else if (heap_b.top() == heap_a.top())
		{
			repetition++;
			heap_b.pop();
		}
		else
		{
			score += static_cast<unsigned long long>(heap_a.top()) * static_cast<unsigned long long>(repetition);
			
			heap_a.pop();

			if (heap_a.top() != key)
			{
				key = heap_a.top();
				repetition = 0;
			}
		}
	}

	std::cout << "Part 2 Solution: " << score << std::endl;
	return EXIT_SUCCESS;
}