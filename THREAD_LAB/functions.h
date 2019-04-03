#pragma once

#include <Windows.h>
#include <random>


std::queue<long> MAIN_QUEUE;
std::queue<long> FIRST_SIDE_QUEUE;
std::queue<long> SECOND_SIDE_QUEUE;
std::queue<long> THIRD_SIDE_QUEUE;

unsigned int SUM_OF_ELEMENTS;

bool stopMain = false;
bool stopFirst = false;
bool stopSecond = false;
bool stopThird = false;
bool stopAll = false;

sf::Text mainQueueText;
sf::Text firstQueueText;
sf::Text secondQueueText;
sf::Text thirdQueueText;

//sf::Color mainQueueRgb(sf::Color(0, 0, 0));
//sf::Color firstQueueRgb(sf::Color(0, 0, 0));
//sf::Color secondQueueRgb(sf::Color(0, 0, 0));
//sf::Color thirdQueueRgb(sf::Color(0, 0, 0));

sf::Font font;

std::mutex MUTEX;

namespace functions
{

	int random(unsigned int from, unsigned int to)
	{
		std::random_device randomDevice;
		std::mt19937_64 mt(randomDevice());
		std::uniform_real_distribution<> distribution(from, to);
		return distribution(mt);
	}

	void makeBeautiful()
	{
		mainQueueText.setFillColor(sf::Color(random(0,256), random(0, 256), random(0, 256)));
		firstQueueText.setFillColor(sf::Color(random(0, 256), random(0, 256), random(0, 256)));
		secondQueueText.setFillColor(sf::Color(random(0, 256), random(0, 256), random(0, 256)));
		thirdQueueText.setFillColor(sf::Color(random(0, 256), random(0, 256), random(0, 256)));
	}

	void initText(
		sf::Text& txt,
		int posX,
		int posY,
		int size,
		const sf::Font& font,
		std::string name)
	{
		txt.setPosition(posX, posY);
		txt.setCharacterSize(size);
		txt.setFont(font);
		std::string mainQueueStr(name + std::to_string(0));
	}

	void clearCmd()
	{
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { 0, 0 };
	
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;
	
		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
		cellCount = csbi.dwSize.X *csbi.dwSize.Y;
	
		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR) ' ',
			cellCount,
			homeCoords,
			&count
		)) return;
	
		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) return;
	
		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, homeCoords);
	
	}

	void buttonPressed(sf::Event e)
	{
		if (e.type == e.KeyPressed)
		{
			switch (e.key.code)
			{
			case (sf::Keyboard::Num1):
				stopFirst ? stopFirst = false : stopFirst = true;
				break;
			case (sf::Keyboard::Num2):
				stopSecond ? stopSecond = false : stopSecond = true;
				break;
			case (sf::Keyboard::Num3):
				stopThird ? stopThird = false : stopThird = true;
				break;
			case (sf::Keyboard::M):
				stopMain ? stopMain = false : stopMain = true;
				break;
			case (sf::Keyboard::Num0):
				stopAll ? stopAll = false : stopAll = true;
				break;
			default:
				break;
			}

		}
	}
}