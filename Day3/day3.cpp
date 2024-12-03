#pragma once

#include <iostream>
#include <cstdlib>
#include <vector>

#include "../_utils/utils.h"

enum class State { None = 0, Mul, Enable, Disable };

struct Instruction
{
	enum class Response {Fail = 0, Match, Invoke};
	char m_value[16] = { '\0' };
	size_t m_offset = 0;
	size_t m_size = 0;
	bool enabled = true;

	Instruction(const char* value) 
	{
		m_size = strlen(value);
		strncpy_s(m_value, sizeof(m_value), value, m_size);
	}

	~Instruction()
	{
	}

	Response Check(const char value)
	{
		if (enabled && m_value[m_offset] == value)
		{
			m_offset++;
			if (m_offset == m_size)
			{
				m_offset = 0;
				return Response::Invoke;
			}
			else return Response::Match;
		}

		m_offset = 0;
		return Response::Fail;
	}

	void Enable() { enabled = true; }
	void Disable() { enabled = false;  }
};


int main(int argc, char * argv[])
{
	//const char* ins_mul = "mul(\0";
	//const char* ins_enable = "do()\0";
	//const char* ins_disable = "don't()\0";

	std::vector<Instruction*> instructions;
	instructions.push_back(new Instruction("mul("));
	instructions.push_back(new Instruction("do("));
	instructions.push_back(new Instruction("don't("));


	State state = State::None;
		
	char* buffer = nullptr;
	size_t size = LoadFileToBuffer("day3.txt", &buffer);
	if (size == 0) return EXIT_FAILURE;

	unsigned total = 0;
	size_t offset = 0;
	for (size_t i = 0; i < size; i += 1 + (offset > 0 ? offset-1 : 0))
	{
		offset = 0;
		state = State::None;

		// check instruction
		while (true)
		{
			Instruction::Response res = Instruction::Response::Fail;
			for (size_t j = 0; j < instructions.size(); ++j)
			{
				auto curr_res = instructions[j]->Check(buffer[i + offset]);
				if (curr_res == Instruction::Response::Invoke)
				{
					res = Instruction::Response::Invoke;
					switch (j)
					{
					case 0:
						state = State::Mul;
						break;
					case 1:
						state = State::Enable;
						break;
					case 2:
						state = State::Disable;
						break;
					default:
						break;
					}
				}
				else if (curr_res == Instruction::Response::Match && res == Instruction::Response::Fail) res = Instruction::Response::Match;
			}

			if (res == Instruction::Response::Match) ++offset;
			else break;
		}

		switch (state)
		{
		case State::None:
			break;
		case State::Mul:
		{
			offset++;
			unsigned value = 0;
			unsigned first = 0;
			while (true)
			{
				if (buffer[i + offset] >= '0' && buffer[i + offset] <= '9')
				{
					value = 10 * value + static_cast<unsigned>(buffer[i + offset] - '0');
				}
				else if (buffer[i + offset] == ',')
				{
					first = value;
					value = 0;
				}
				else if (buffer[i + offset] == ')' && first > 0)
				{
					total += first * value;
					break;
				}
				else break;

				offset += 1;
			}
		}
			break;
		case State::Enable:
			instructions[0]->Enable();
			break;
		case State::Disable:
			instructions[0]->Disable();
			break;
		default:
			break;
		}
	}


	for (Instruction* ins : instructions)
	{
		delete ins;
	}


	std::cout << "Solution Day 3: " << total << std::endl;
	return EXIT_SUCCESS;
}