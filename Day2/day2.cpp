#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>

#include "../_utils/utils.h"

#define UNSET_VALUE -1
#define UNSAFE_LINE -2
#define ALLOWED_ERRORS 1

inline bool InRange(int value) { return !(value == 0 || value < -3 || value > 3); }

bool CheckVector(std::vector<int> &values)
{
	std::vector<int> diffs;
	for (size_t i = 1; i < values.size(); ++i) diffs.push_back(values[i] - values[i - 1]);

	int sign = -1;
	if ((diffs[0] > 0 ? 1 : 0) + (diffs[1] > 0 ? 1 : 0) + (diffs[2] > 0 ? 1 : 0) >= 3 - ALLOWED_ERRORS) sign = 1;

	unsigned errors = 0;

	size_t i = 1U;
	if (diffs[0] * sign < 0 || !InRange(diffs[0]))
	{
		errors++;
		if (errors > ALLOWED_ERRORS) return false;

		int combined = diffs[0] + diffs[1];
		bool valid_alone    = (diffs[1] * sign > 0 && InRange(diffs[1]));
		bool valid_combined = (combined * sign > 0 && InRange(combined));

		if (!valid_alone && !valid_combined) return false;
		i = 2U;
	}

	for (; i < diffs.size()-1U; ++i)
	{
		if (diffs[i] * sign < 0 || !InRange(diffs[i]))
		{
			errors++;
			if (errors > ALLOWED_ERRORS) return false;

			diffs[i + 1] += diffs[i];
		}
	}

	if (diffs[diffs.size() - 1U] * sign < 0 || !InRange(diffs[diffs.size() - 1U]))
	{
		errors++;
		if (errors > ALLOWED_ERRORS) return false;
	}

	return true;
}

int main(int argc, char * argv[])
{
	char* buffer = nullptr;
	size_t size = LoadFileToBuffer("day2.txt", &buffer);
	if (size == 0) return EXIT_FAILURE;

	std::vector<int> values;

	unsigned total_safe = 0;

	int value = 0;
	int previous = UNSET_VALUE;
	int sign = 0;
	int before = UNSET_VALUE;

	int error_count = 0;

	for (size_t i = 0; i < size; ++i)
	{
		if (buffer[i] == ' ')
		{
			values.emplace_back(value);
			value = 0;
		}
		else if (buffer[i] == '\n')
		{
			values.emplace_back(value);
			value = 0;

			if (CheckVector(values)) total_safe++;
			values.clear();
		}
		else if (buffer[i] >= '0' && buffer[i] <= '9') value = 10 * value + (buffer[i] - '0');
	}

	if (!values.empty())
	{
		values.emplace_back(value);			
		if (CheckVector(values)) total_safe++;
	}

	ReleaseBuffer(&buffer);

	std::cout << "Solution day 2: " << total_safe << std::endl;
	return EXIT_SUCCESS;
}