//лаба
#include <queue>
#include <thread>
//флоу контрол
#include <mutex>
//#include <chrono>
//графен
#include <SFML/Graphics.hpp>
//отрисовка и корректировка
#include <string>
#include <regex>
///////////////////////
#include <iostream>
#include "functions.h"
///////////////////////


void buttonPressed(sf::Event e)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		stopFirst ? stopFirst = false : stopFirst = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		stopSecond ? stopSecond = false : stopSecond = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		stopThird ? stopThird = false : stopThird = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		stopMain ? stopMain = false : stopMain = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
	{
		stopAll ? stopAll = false : stopAll = true;
	}
}

template<class T>
void fillTheQueue(std::queue<T>& _q, const long& quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		_q.push(1);
	}
}

void mainQueueThreadFunction(
	std::queue<long>& mainQ,
	std::queue<long>& firsSideQ,
	std::queue<long>& secondSideQ,
	std::queue<long>& thirdSideQ,
	const bool& stop 
)
{
	while (!mainQ.empty())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (!stop && mainQ.size() > 1 && !::stopAll)
		{

			MUTEX.lock();
			if (!mainQ.empty())
			{
				firsSideQ.push(mainQ.front());
				mainQ.pop();
			}

			MUTEX.unlock();
		}
		if (!stop && mainQ.size() > 1 && !::stopAll)
		{

			MUTEX.lock();
			if (!mainQ.empty())
			{
				secondSideQ.push(mainQ.front());
				mainQ.pop();
			}

			MUTEX.unlock();
		}
		if (!stop && mainQ.size() > 1 && !::stopAll)
		{

			MUTEX.lock();
			if (!mainQ.empty())
			{
				thirdSideQ.push(mainQ.front());
				mainQ.pop();
			}

			MUTEX.unlock();
		}
	}
}

void sideQueueThreadFunction(
	std::queue<long>& sideQueue,
	std::queue<long>& mainQ,
	const bool& stop
)
{

	while (!sideQueue.empty())
	{
		if (!stop && sideQueue.size() > 1 && !::stopAll)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			MUTEX.lock();
			mainQ.push(sideQueue.front());
			sideQueue.pop();
	
	
			MUTEX.unlock();
		}
	}
}


int main()
{
	fillTheQueue(FIRST_SIDE_QUEUE, 1000);
	fillTheQueue(SECOND_SIDE_QUEUE, 1000);
	fillTheQueue(THIRD_SIDE_QUEUE, 1000);
	fillTheQueue(MAIN_QUEUE, 1000);
	
	//SUM_OF_ELEMENTS = FIRST_SIDE_QUEUE.size() + SECOND_SIDE_QUEUE.size() + THIRD_SIDE_QUEUE.size() + MAIN_QUEUE.size();
	font.loadFromFile("Pixel-Miners.otf");
	
	std::string mainQueueStr("MAIN QUEUE " + std::to_string(MAIN_QUEUE.size()));
	functions::initText(mainQueueText, 0, 0, 35, font, mainQueueStr);

	std::string firstQueueStr("FIRST QUEUE " + std::to_string(FIRST_SIDE_QUEUE.size()));
	functions::initText(firstQueueText, 0, 100, 25, font, mainQueueStr);
	
	std::string secondQueueStr("SECOND QUEUE " + std::to_string(SECOND_SIDE_QUEUE.size()));
	functions::initText(secondQueueText, 0, 200, 25, font, secondQueueStr);
	
	std::string thirdQueueStr("THIRD QUEUE " + std::to_string(THIRD_SIDE_QUEUE.size()));
	functions::initText(thirdQueueText, 0, 300, 25, font,  thirdQueueStr);


	std::thread mainThread(
		mainQueueThreadFunction,
		std::ref(MAIN_QUEUE),
		std::ref(FIRST_SIDE_QUEUE),
		std::ref(SECOND_SIDE_QUEUE),
		std::ref(THIRD_SIDE_QUEUE),
		std::ref(stopMain)
	);
	if (mainThread.joinable())
		mainThread.detach();

	std::thread firstSideThread(
		sideQueueThreadFunction,
		std::ref(FIRST_SIDE_QUEUE),
		std::ref(MAIN_QUEUE),
		std::ref(stopFirst)
	);
	if (firstSideThread.joinable())
		firstSideThread.detach();

	std::thread secondSideThread(
		sideQueueThreadFunction,
		std::ref(SECOND_SIDE_QUEUE),
		std::ref(MAIN_QUEUE),
		std::ref(stopSecond)
	);
	if (secondSideThread.joinable())
		secondSideThread.detach();

	std::thread thirdSideThread(
		sideQueueThreadFunction,
		std::ref(THIRD_SIDE_QUEUE),
		std::ref(MAIN_QUEUE),
		std::ref(stopThird)
	);
	if (thirdSideThread.joinable())
		thirdSideThread.detach();


	sf::RenderWindow wnd(sf::VideoMode(600, 400), "LABWORK 2");
	std::regex replaceCurrentNumber("\\d+");
	sf::Event e;
	wnd.pollEvent(e);

	while (wnd.isOpen())
	{
		while (wnd.pollEvent(e))
		{
			if (e.type == sf::Event::EventType::Closed)
			{
				wnd.close();
			}
			functions::buttonPressed(e);
			{
				functions::clearCmd();
				//////////////////////////////////////////////////////////////////////////////
				std::cout << "stopFirst " << stopFirst << std::endl;
				std::cout << "stopSecond " << stopSecond << std::endl;
				std::cout << "stopThird " << stopThird << std::endl;
				std::cout << "stopMain " << stopMain << std::endl;
				std::cout << "stopAll " << stopAll << std::endl;
				std::cout << std::endl;
				//////////////////////////////////////////////////////////////////////////////
			}
		}
		mainQueueText.setString(mainQueueStr);
		firstQueueText.setString(firstQueueStr);
		secondQueueText.setString(secondQueueStr);
		thirdQueueText.setString(thirdQueueStr);

		wnd.clear(sf::Color::Black);

		mainQueueStr	= std::regex_replace(mainQueueStr, replaceCurrentNumber, std::to_string(MAIN_QUEUE.size()));
		firstQueueStr	= std::regex_replace(firstQueueStr, replaceCurrentNumber, std::to_string(FIRST_SIDE_QUEUE.size()));
		secondQueueStr	= std::regex_replace(secondQueueStr, replaceCurrentNumber, std::to_string(SECOND_SIDE_QUEUE.size()));
		thirdQueueStr	= std::regex_replace(thirdQueueStr, replaceCurrentNumber, std::to_string(THIRD_SIDE_QUEUE.size()));

		functions::makeBeautiful();
		wnd.draw(mainQueueText);
		wnd.draw(firstQueueText);
		wnd.draw(secondQueueText);
		wnd.draw(thirdQueueText);
		wnd.display();
	}
	return 0;
}