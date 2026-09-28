#include <iostream>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <future>

const size_t N{ 11 };
const size_t M{ 9 };
const size_t COUNT{ N * M };
const size_t NTHREAD{ 4 };

const int A{ -50 };
const int B{ 50 };

const size_t number{ 1 };

int min_el(int matrix[][M], size_t left, size_t right)
{
	int local_min_el{};

	for (size_t i{ left }; i < right; ++i)
	{
		int cur_el = matrix[i / M][i % M];
		int el = abs(cur_el);
		while (el / 10 > 0)
			el /= 10;
		if (el == number && (local_min_el > cur_el || local_min_el == 0))
		{
			local_min_el = cur_el;
		}
	}

	return local_min_el;
}

int min_parallel(int matrix[][M])
{
	std::future<int> ftr[NTHREAD - 1]{}; 
	size_t chunk{ COUNT / NTHREAD };
	int result{};
	for (int i{}; i < NTHREAD - 1; ++i)
	{
		ftr[i] = std::async(std::launch::async, min_el, matrix, chunk * i, chunk * (i + 1));
	}
	int global_min{min_el(matrix, chunk * (NTHREAD - 1), COUNT)};
	
	for (size_t i{}; i < NTHREAD - 1; ++i)
	{
		result = ftr[i].get();
		if (result < global_min || global_min == 0)
			global_min = result;
	}
	return global_min;

}



void print_matrix(int matrix[][M])
{
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
			std::cout << std::setw(5) << matrix[i][j];
		std::cout << "\n";
	}

}

void init_matrix(int matrix[][M])
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

	std::cout << "Nonparallel min = " << min_el(matrix, 0, COUNT) << "\n";
	std::cout << "parallel min = " << min_parallel(matrix) << "\n";

	std::cin.ignore().get();
	return 0;
}
