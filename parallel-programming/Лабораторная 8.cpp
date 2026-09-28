#include <thread>
#include <mutex>
#include <queue>
#include <Windows.h>
#include <stack>
#include <iostream>
#include <iomanip>
#include <fstream>
const size_t N{ 11 };
const size_t M{ 13 };
const size_t COUNT{ N * M };
const size_t NTHREAD{ 4 };

const int A{ -50 };
const int B{ 50 };

const size_t number{ 1 };

struct Pair
{
	size_t left, right;
};


class ThreadSafeStack
{
private:
	std::mutex mutex;
	std::stack<Pair> stack;
public:
	ThreadSafeStack() {};
	~ThreadSafeStack() {};
	void push(Pair elem)
	{
		stack.push(elem);
	}
	bool try_pop(Pair& elem)
	{
		bool result{};
		if (!stack.empty())
		{
			std::lock_guard<std::mutex> guard(mutex);
			if (!stack.empty())
			{
				result = true;
				elem = stack.top();
				stack.pop();
			}

		}
		return result;
	}
	bool empty()
	{
		return stack.empty();
	}
};


ThreadSafeStack TSQ;
std::mutex mutex_task;
std::thread::id MainID = std::this_thread::get_id();
void min_el(int matrix[][M], int& global_min)
{
	Pair pair{};
	int local_min_el{};
	auto check = [](int cur_el)
		{
			cur_el = abs(cur_el);
			while (cur_el / 10 > 0)
				cur_el /= 10;
			return cur_el == number;
		};

	while (TSQ.try_pop(pair))
	{
		{
			std::lock_guard<std::mutex> guard(mutex_task);
			std::cout << "Id : " << std::this_thread::get_id() << ": (" << pair.left << " " <<pair.right << ") \n";
		}

		for (size_t i{ pair.left }; i < pair.right; ++i)
		{
			int cur_el = matrix[i / M][i % M];
			if (check(cur_el) && (local_min_el > cur_el || local_min_el == 0))
			{
				local_min_el = cur_el;
			}
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}


	{
		std::lock_guard<std::mutex> guard(mutex_task);
		if (local_min_el!= 0 && (local_min_el < global_min || global_min == 0))
		{
			global_min = local_min_el;
		}
		std::cout << "Id : " << std::this_thread::get_id() << ": " << global_min << "\n";
	}

}


int min_parallel(int matrix[][M])
{
	std::thread thr[NTHREAD - 1]{};
	size_t chunk{10};
	size_t left{}, right{};
	while (right != COUNT)
	{
		left = right;
		right = (right + chunk < COUNT) ? right + chunk : COUNT;
		TSQ.push(Pair{ left, right });
	}
	int global_min{};
	for (int i{}; i < NTHREAD - 1; ++i)
	{
		thr[i] = std::thread(min_el, matrix, std::ref(global_min));
	}

	min_el(matrix, global_min);
	for (size_t i{}; i < NTHREAD - 1; ++i)
	{
		thr[i].join();
	}

	return global_min;
}

void print_matrix(int matrix[N][M])
{
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
			std::cout << std::setw(5) << matrix[i][j];
		std::cout << "\n";
	}

}

void init_matrix(int matrix[N][M])
{
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
			matrix[i][j] = A + rand() % (B - A);
	}
}


int min_nonparallel(int matrix[][M])
{
	int sum{};
	int min_el{};
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
		{
			int cur_el = matrix[i][j];
			int el = abs(cur_el);
			while (el / 10 > 0)
				el /= 10;
			if (el == number && (min_el > cur_el || min_el == 0))
			{
				min_el = cur_el;
			}
		}
	}
	return min_el;
}

int main()
{
	srand(GetTickCount64());
	int matrix[N][M];
	init_matrix(matrix);
	print_matrix(matrix);
	int min_nonp{ min_nonparallel(matrix) };
	std::cout << "Nonparallel min = " << min_nonp << "\n";
	std::cout << "MainID : " << MainID << '\n';
	std::cout << "parallel min = " << min_parallel(matrix) << "\n";

	std::cin.ignore().get();
	return 0;
}