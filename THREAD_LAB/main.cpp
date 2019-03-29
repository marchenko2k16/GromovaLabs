#include <queue>
#include <thread>
#include <string>
#include <regex>

#include <SFML/Graphics.hpp>

 std::queue<int> FIRST_SIDE_QUEUE;
 std::queue<int> SECOND_SIDE_QUEUE;
 std::queue<int> THIRD_SIDE_QUEUE;
 std::queue<int> MAIN_QUEUE;

template<class T>
void fillTheQueue(std::queue<T>& _q, int quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		_q.push(1);
	}
}

void mainQueueThreadFunction(
	std::queue<int>& mainQ,
	std::queue<int>& firsSideQ,
	std::queue<int>& secondSideQ,
	std::queue<int>& thirdSideQ
	)
{
	while (!mainQ.empty())
	{
		firsSideQ.push(mainQ.front());
		mainQ.pop();

		secondSideQ.push(mainQ.front());
		mainQ.pop();

		thirdSideQ.push(mainQ.front());
		mainQ.pop();
	}
}

void sideQueueThreadFunction(std::queue<int>& sideQueue, std::queue<int>& mainQ)
{
	while (!sideQueue.empty())
	{
		mainQ.push(sideQueue.front());
		sideQueue.pop();
	}
}


int main()
{
	fillTheQueue(FIRST_SIDE_QUEUE, 1000);
	fillTheQueue(SECOND_SIDE_QUEUE, 1000);
	fillTheQueue(THIRD_SIDE_QUEUE, 1000);
	fillTheQueue(MAIN_QUEUE, 100000);

	//sf::Font font;
	//font.loadFromFile("Pixel-Miners.otf");
	//
	//sf::Text mainQueue;
	//sf::Text firstQueue;
	//sf::Text secondQueue;
	//sf::Text thirdQueue;
	//
	//mainQueue.setPosition(0, 0);
	//mainQueue.setCharacterSize(35);
	//mainQueue.setFont(font);
	//std::string mainQueueStr("MAIN QUEUE " + std::to_string(MAIN_QUEUE.size()));
	//
	//firstQueue.setPosition(0, 100);
	//firstQueue.setCharacterSize(20);
	//firstQueue.setFont(font);
	//std::string firstQueueStr("FIRST QUEUE " + std::to_string(FIRST_SIDE_QUEUE.size()));
	//
	//secondQueue.setPosition(0, 200);
	//secondQueue.setCharacterSize(20);
	//secondQueue.setFont(font);
	//std::string secondQueueStr("SECOND QUEUE " + std::to_string(SECOND_SIDE_QUEUE.size()));
	//
	//thirdQueue.setPosition(0, 300);
	//thirdQueue.setCharacterSize(20);
	//thirdQueue.setFont(font);
	//std::string thirdQueueStr("THIRD QUEUE " + std::to_string(THIRD_SIDE_QUEUE.size()));

	
	std::thread mainThread(
		mainQueueThreadFunction,
		std::ref(MAIN_QUEUE),
		std::ref(FIRST_SIDE_QUEUE),
		std::ref(SECOND_SIDE_QUEUE),
		std::ref(THIRD_SIDE_QUEUE)
	);
	std::thread firstSideThread(
		sideQueueThreadFunction, 
		std::ref(FIRST_SIDE_QUEUE),
		std::ref(MAIN_QUEUE)
		);
	std::thread secondSideThread(
		sideQueueThreadFunction, 
		std::ref(SECOND_SIDE_QUEUE),
		std::ref(MAIN_QUEUE)
	);
	std::thread thirdSideThread(
		sideQueueThreadFunction, 
		std::ref(THIRD_SIDE_QUEUE),
		std::ref(MAIN_QUEUE)
	);

	//mainThread.join();
	//firstSideThread.join();
	//secondSideThread.join();
	//thirdSideThread.join();

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

		//mainQueue.setString(mainQueueStr);
		//firstQueue.setString(firstQueueStr);
		//secondQueue.setString(secondQueueStr);
		//thirdQueue.setString(thirdQueueStr);

		wnd.clear(sf::Color::Black);

		//wnd.draw(mainQueue);
		//wnd.draw(firstQueue);
		//wnd.draw(secondQueue);
		//wnd.draw(thirdQueue);
		wnd.display();
		//
		//mainQueueStr = std::regex_replace(mainQueueStr, replaceCurrentNumber, std::to_string(MAIN_QUEUE.size()));
		//firstQueueStr = std::regex_replace(firstQueueStr, replaceCurrentNumber, std::to_string(FIRST_SIDE_QUEUE.size()));
		//secondQueueStr = std::regex_replace(secondQueueStr, replaceCurrentNumber, std::to_string(SECOND_SIDE_QUEUE.size()));
		//thirdQueueStr = std::regex_replace(thirdQueueStr, replaceCurrentNumber, std::to_string(THIRD_SIDE_QUEUE.size()));
	}
	wnd.close();
	
	return 0;
}