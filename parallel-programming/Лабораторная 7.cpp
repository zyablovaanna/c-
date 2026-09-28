#include <iostream>
#include <Windows.h>
#include <thread>>
#include <iomanip>
#include <fstream>
const size_t N{ 11 };
const size_t M{ 9 };
const size_t COUNT{ N * M };
const size_t NTHREAD{ 4 };

const int A{ -50 };
const int B{ 30 };

const size_t number{ 3 };

volatile long lock{};
void min_el(int matrix[][M], size_t left, size_t right, long volatile& global_min)
{
	int local_min_el{};
	auto check = [](int cur_el)
		{
			cur_el = abs(cur_el);
			while (cur_el / 10 > 0)
				cur_el /= 10;
			return cur_el == number;
		};
	for (size_t i{ left }; i < right; ++i)
	{
		int cur_el = matrix[i / M][i % M];
		if (check(cur_el) && (local_min_el > cur_el || local_min_el == 0))
		{
			local_min_el = cur_el;
		}
	}
	if (local_min_el != 0 && (local_min_el < global_min || global_min == 0))
	{
		while (_InterlockedCompareExchange(&lock, 1, 0) == 1)
			Sleep(0);
		if (local_min_el != 0 && (local_min_el < global_min || global_min == 0))
			global_min = local_min_el;
		_InterlockedCompareExchange(&lock, 0, 1);
	}
	std::cout << "ID from C++API " << std::this_thread::get_id() << ": " << global_min << "\n";
}


int min_parallel(int matrix[][M])
{
	std::thread thr[NTHREAD - 1]{};
	size_t chunk{ COUNT / NTHREAD };
	long volatile global_min{};
	for (int i{}; i < NTHREAD - 1; ++i)
	{
		thr[i] = std::thread(min_el, matrix, chunk * i, chunk * (i + 1), std::ref(global_min));
	}

	min_el(matrix, chunk * (NTHREAD - 1), COUNT, global_min);
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


int main()
{
	srand(GetTickCount64());
	int matrix[N][M];
	init_matrix(matrix);
	print_matrix(matrix);
	long volatile min_nonparallel{};
	min_el(matrix, 0, COUNT, min_nonparallel);
	std::cout << "Nonparallel min = " << min_nonparallel << "\n";
	std::cout << "parallel min = " << min_parallel(matrix) << "\n";

	std::cin.ignore().get();
	return 0;
}