#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <mutex>

std::queue<long> FIRST_SIDE_QUEUE;
std::queue<long> SECOND_SIDE_QUEUE;
std::queue<long> THIRD_SIDE_QUEUE;
std::queue<long> MAIN_QUEUE;

//std::mutex MUTEX;

void fillTheQueue(std::queue<long>& _q, const long& quantity)
{
	for (long i = 0; i < quantity; ++i)
	{
		_q.push(1);
	}
}

void mainQueueThreadFunction(
	std::queue<long>& mainQ,
	std::queue<long>& firsSideQ,
	std::queue<long>& secondSideQ,
	std::queue<long>& thirdSideQ
)
{
	while (!mainQ.empty())
	{
		std::cout << "MAIN SIZE " << MAIN_QUEUE.size() << std::endl;
		//	MUTEX.lock();
		if (!mainQ.empty())
		{
			firsSideQ.push(mainQ.front());
			mainQ.pop();
			std::cout << "first SIZE " << FIRST_SIDE_QUEUE.size() << std::endl;
			//		MUTEX.unlock();
		}


		if (!mainQ.empty())
		{
			secondSideQ.push(mainQ.front());
			mainQ.pop();
			std::cout << "second SIZE " << SECOND_SIDE_QUEUE.size() << std::endl;
	//		MUTEX.unlock();
		}


		if (!mainQ.empty())
		{
			thirdSideQ.push(mainQ.front());
			mainQ.pop();
			std::cout << "third SIZE " << THIRD_SIDE_QUEUE.size() << std::endl;
	//		MUTEX.unlock();
		}
	//	MUTEX.unlock();
	}
}

void sideQueueThreadFunction(
	std::queue<long>& sideQueue,
	std::queue<long>& mainQ
)
{
	while (!sideQueue.empty())
	{
		//MUTEX.lock();
		mainQ.push(sideQueue.front());
		sideQueue.pop();


		//MUTEX.unlock();
	}
}

int main()
{
	fillTheQueue(MAIN_QUEUE, 10);
	fillTheQueue(FIRST_SIDE_QUEUE, 10000);
	fillTheQueue(SECOND_SIDE_QUEUE, 10000);
	fillTheQueue(THIRD_SIDE_QUEUE, 10000);
	
	std::thread mainThread(
		mainQueueThreadFunction,
		std::ref(MAIN_QUEUE),
		std::ref(FIRST_SIDE_QUEUE),
		std::ref(SECOND_SIDE_QUEUE),
		std::ref(THIRD_SIDE_QUEUE)
	);
	if(mainThread.joinable())
		mainThread.join();

	std::thread firstSideThread(
		sideQueueThreadFunction,
		std::ref(FIRST_SIDE_QUEUE),
		std::ref(MAIN_QUEUE)
	);
	if (firstSideThread.joinable())
		firstSideThread.join();

	std::thread secondSideThread(
		sideQueueThreadFunction,
		std::ref(SECOND_SIDE_QUEUE),
		std::ref(MAIN_QUEUE)
	);
	if (secondSideThread.joinable())
		secondSideThread.join();

	std::thread thirdSideThread(
		sideQueueThreadFunction,
		std::ref(THIRD_SIDE_QUEUE),
		std::ref(MAIN_QUEUE)
	);
	if (thirdSideThread.joinable())
		thirdSideThread.join();


	std::cin.get();
	return 0;
}