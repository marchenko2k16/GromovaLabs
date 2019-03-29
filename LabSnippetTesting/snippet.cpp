#include <iostream>
#include <queue>
#include <thread>
#include <string>


std::queue<int> FIRST_SIDE_QUEUE;
std::queue<int> SECOND_SIDE_QUEUE;
std::queue<int> THIRD_SIDE_QUEUE;
std::queue<int> MAIN_QUEUE;

void fillTheQueue(std::queue<int>& _q, int quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		_q.push(1);
	}
}

void mainQueueThreadFunction()
{
	while (!::MAIN_QUEUE.empty())
	{
		FIRST_SIDE_QUEUE.push(::MAIN_QUEUE.front());
		::MAIN_QUEUE.pop();

		SECOND_SIDE_QUEUE.push(::MAIN_QUEUE.front());
		::MAIN_QUEUE.pop();

		THIRD_SIDE_QUEUE.push(::MAIN_QUEUE.front());
		::MAIN_QUEUE.pop();

		std::cout << "MAIN SIZE " << MAIN_QUEUE.size() << std::endl;
		std::cout << "first SIZE " << FIRST_SIDE_QUEUE.size() << std::endl;
		std::cout << "second SIZE " << SECOND_SIDE_QUEUE.size() << std::endl;
		std::cout << "third SIZE " << THIRD_SIDE_QUEUE.size() << std::endl;
	}
}

void sideQueueThreadFunction(std::queue<int>& sideQueue)
{
	while (!sideQueue.empty())
	{
		::MAIN_QUEUE.push(sideQueue.front());
		sideQueue.pop();
	}
}

int main()
{
	fillTheQueue(MAIN_QUEUE, 100000);
	fillTheQueue(FIRST_SIDE_QUEUE, 10000);
	fillTheQueue(SECOND_SIDE_QUEUE, 10000);
	fillTheQueue(THIRD_SIDE_QUEUE, 10000);
	//
	std::thread mainThread(mainQueueThreadFunction);
	std::thread firstSideThread(sideQueueThreadFunction, std::ref(FIRST_SIDE_QUEUE));
	std::thread secondSideThread(sideQueueThreadFunction, std::ref(SECOND_SIDE_QUEUE));
	std::thread thirdSideThread(sideQueueThreadFunction, std::ref(THIRD_SIDE_QUEUE));

	mainThread.join();
	firstSideThread.join();
	secondSideThread.join();
	thirdSideThread.join();

	std::cin.get();
	return 0;
}