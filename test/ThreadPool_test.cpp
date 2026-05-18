#include "../src/ThreadPool.h"


#include <gtest/gtest.h>
#include <format>
#include <chrono>


int sum(int a, int b)
{
	int c = a + b;
	std::cout << std::format("sum({}, {}) = {}\n", a, b, c);
	return c;
}

TEST(ThreadPoolTest, CreateThreadPoolAndAsk100Tasks)
{
	sf::ThreadPool pool(20);
	std::mutex mu;

	for (int i = 0; i < 100; ++i)
	{
		pool.addTask([i, &mu]()
			{
				for (int count = 0; count < 10; ++count)
				{
				}
			});
	}

	pool.addTask(sum, 10, 20);

	EXPECT_EQ(1, 1);
}

TEST(ThreadPoolTest, AddTaskAndWaitForResult)
{
	sf::ThreadPool pool(4);

	auto sleepAndSum = [](int a, int b)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return a + b;
		};
	auto result = pool.addTask(sleepAndSum, 100, 1000);
	int value = result.get();
	EXPECT_EQ(value, 1100);
}

