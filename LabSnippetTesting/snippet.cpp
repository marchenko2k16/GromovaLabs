#include <iostream>
#include <queue>
#include <thread>
#include <string>
#include <mutex>

#include <chrono>

int main()
{
	int i{};
	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
	while (true)
	{
		std::chrono::time_point<std::chrono::steady_clock> next = std::chrono::steady_clock::now();
		if (next - start > std::chrono::seconds(1))
		{
			i++;
			std::cout << "second elapsed " << i << std::endl;
			start = next;
		}
	}

	std::cin.get();
	return 0;
}