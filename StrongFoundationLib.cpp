// StrongFoundationLib.cpp : Defines the entry point for the application.
//

#include "StrongFoundationLib.h"
#include "src/ThreadPool.h"

#include <chrono>
#include <format>

using namespace std;


int sum(int a, int b)
{
	int c = a + b;
	std::cout << std::format("sum({}, {}) = {}\n", a, b, c);
	return c;
}

int main()
{
	sf::ThreadPool pool(20);

	std::mutex mu;

	for (int i = 0; i < 100; ++i)
	{
		pool.addTask([i, &mu]()
			{
				for (int count = 0; count < 1000000; ++count)
				{
					std::lock_guard lock(mu);
					std::cout << "count " << count << " from thread id " << std::this_thread::get_id() << std::endl;
				}
			});
	}

	pool.addTask(sum, 10, 20);


	return 0;
}
