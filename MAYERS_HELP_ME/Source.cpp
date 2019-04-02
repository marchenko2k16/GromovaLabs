#include <iostream>
#include <thread>
#include <string>

#include <queue>


int apex;
auto lambda = [](decltype(apex) a) {return std::string("sanya h sosi").append(std::to_string(a)); };


void qLamb1(std::queue<int>& q1, std::queue<int>&q2)
{
	while (!q1.empty())
	{
		//std::cout << "FILLING QQQ" << std::endl;
		q2.push(q1.front());
		std::cout << q1.size() << std::endl;
		q1.pop();
	}
	if (q1.empty())
	{
		std::cout << "IS EMPTY" << std::this_thread::get_id() << std::endl;
	}
};

template<class T>
void fillTheQueue(std::queue<T>& _q, const int& quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		_q.push(1);
	}
}

void main()
{
	std::queue<int> QQQ;
	std::queue<int> QQ1;
	fillTheQueue(QQQ, 666);
	fillTheQueue(QQ1, 666);

	std::thread thread1(qLamb1, std::ref(QQQ), std::ref(QQ1));
	thread1.detach();
	std::thread thread2(qLamb1, std::ref(QQ1), std::ref(QQQ));
	thread2.detach();
	
	
	std::cin.get();
}