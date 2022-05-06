#include <iostream>
#include <vector>
#include <thread>
#include<mutex>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <execution>
#include <future>
#define TIMES 5

int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	static std::mt19937_64 generator(seed);
	std::uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}



//Task 1

std::once_flag callflag;

void __fastcall once_pcout()
{
	std::cout << "Hello";
}
void pcout()
{
	call_once(callflag, once_pcout);
}




//Task 2

int return_simple()
{
	int x = getRandomNum(1, 1000000);
	size_t cnt{ 0 };
	for (size_t i{ 1 }; i <= x; ++i)
	{
		if (x % i == 0)
			cnt++;
	}
	if (cnt <= 2)
		return x;
	else
		return_simple();
}



//Task 3

std::mutex mut_1;

void insert(std::vector<int>& v)
{
	int elem = getRandomNum(1, 100);
	v.push_back(elem);
	std::lock_guard<std::mutex> lk(mut_1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (auto a : v)
	{
		std::cout << a << " ";
	}
	std::cout << "\n";
}

void steal(std::vector<int>& v)
{
	auto result = std::max_element(v.begin(), v.end());
	std::lock_guard<std::mutex> lk(mut_1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	v.erase(result);
}


int main()
{

	// Task 1

	std::vector<std::thread> v1;

	for (size_t i{ 0 }; i < TIMES; ++i) {
		v1.push_back(std::thread(pcout));
	}
	for (auto& t : v1) { t.join(); }

	std::cout << "\n";


	// Task 2

	int x;

	std::thread t1([&]()
		{
			x = return_simple();
		});
	t1.join();
	
	std::cout << x << "\n";



	//Task 3

	std::vector<int> v2;
	v2.reserve(10);

	for(size_t i{0};i < 10;i++)
	{
		std::thread owner(insert, ref(v2));
		std::thread thief(steal, ref(v2));
		owner.join();
		thief.join();
	}

	system("Pause");

	return 0;
}
