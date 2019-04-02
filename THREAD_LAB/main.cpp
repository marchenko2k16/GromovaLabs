#include <queue>
#include <thread>
#include <string>
#include <regex>
#include <mutex>

#include <SFML/Graphics.hpp>
 
#include <iostream>

std::queue<long> MAIN_QUEUE;
std::queue<long> FIRST_SIDE_QUEUE;
std::queue<long> SECOND_SIDE_QUEUE;
std::queue<long> THIRD_SIDE_QUEUE;

bool stopMain	= false;
bool stopFirst	= false;
bool stopSecond	= false;
bool stopThird	= false;
bool stopAll	= false;

std::mutex MUTEX;

void buttonPressed(sf::Event e)
{
	switch (e.key.code)
	{
	case sf::Keyboard::Num1:
		stopMain ? stopMain = false : stopMain = true;
		break;
	case sf::Keyboard::Num2:
		stopFirst ? stopFirst = false : stopFirst = true;
		break;
	case sf::Keyboard::Num3:
		stopSecond ? stopSecond = false : stopSecond = true;
		break;
	case sf::Keyboard::Num4:
		stopThird ? stopThird = false : stopThird = true;
		break;
	case sf::Keyboard::Num0:
		stopAll ? stopAll = false : stopAll = true;
		break;
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
		if (!stop)// && !::stopAll)
		{

			MUTEX.lock();
			if (!mainQ.empty())
			{
				firsSideQ.push(mainQ.front());
				mainQ.pop();
			}

			if (!mainQ.empty())
			{
				secondSideQ.push(mainQ.front());
				mainQ.pop();
			}

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
	//if (sideQueue.size() > 1)
	//{
	//	return;
	//}

	if (!stop && sideQueue.size() > 1)// && !::stopAll)
	{
		while (!sideQueue.empty())
		{
			MUTEX.lock();
			mainQ.push(sideQueue.front());
			sideQueue.pop();
	
	
			MUTEX.unlock();
		}
	}
}

int main()
{

    fillTheQueue(FIRST_SIDE_QUEUE, 100000);
	fillTheQueue(SECOND_SIDE_QUEUE, 100000);
	fillTheQueue(THIRD_SIDE_QUEUE, 100000);
	fillTheQueue(MAIN_QUEUE, 1000000);

	sf::Font font;
	font.loadFromFile("Pixel-Miners.otf");
	
	sf::Text mainQueue;
	sf::Text firstQueue;
	sf::Text secondQueue;
	sf::Text thirdQueue;
	
	mainQueue.setPosition(0, 0);
	mainQueue.setCharacterSize(35);
	mainQueue.setFont(font);
	std::string mainQueueStr("MAIN QUEUE " + std::to_string(MAIN_QUEUE.size()));
	
	firstQueue.setPosition(0, 100);
	firstQueue.setCharacterSize(20);
	firstQueue.setFont(font);
	std::string firstQueueStr("FIRST QUEUE " + std::to_string(FIRST_SIDE_QUEUE.size()));
	
	secondQueue.setPosition(0, 200);
	secondQueue.setCharacterSize(20);
	secondQueue.setFont(font);
	std::string secondQueueStr("SECOND QUEUE " + std::to_string(SECOND_SIDE_QUEUE.size()));
	
	thirdQueue.setPosition(0, 300);
	thirdQueue.setCharacterSize(20);
	thirdQueue.setFont(font);
	std::string thirdQueueStr("THIRD QUEUE " + std::to_string(THIRD_SIDE_QUEUE.size()));

	
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

	while (e.key.code != sf::Keyboard::F)
	{
		wnd.pollEvent(e);
		if (e.type == sf::Event::EventType::Closed)
		{
			wnd.close();
			continue;
		}
			
		buttonPressed(e);

		mainQueue.setString(mainQueueStr);
		firstQueue.setString(firstQueueStr);
		secondQueue.setString(secondQueueStr);
		thirdQueue.setString(thirdQueueStr);

		wnd.clear(sf::Color::Black);

		wnd.draw(mainQueue);
		wnd.draw(firstQueue);
		wnd.draw(secondQueue);
		wnd.draw(thirdQueue);
		wnd.display();
		
		mainQueueStr	= std::regex_replace(mainQueueStr, replaceCurrentNumber, std::to_string(MAIN_QUEUE.size()));
		firstQueueStr	= std::regex_replace(firstQueueStr, replaceCurrentNumber, std::to_string(FIRST_SIDE_QUEUE.size()));
		secondQueueStr	= std::regex_replace(secondQueueStr, replaceCurrentNumber, std::to_string(SECOND_SIDE_QUEUE.size()));
		thirdQueueStr	= std::regex_replace(thirdQueueStr, replaceCurrentNumber, std::to_string(THIRD_SIDE_QUEUE.size()));
	}
	wnd.close();
	
	return 0;
}