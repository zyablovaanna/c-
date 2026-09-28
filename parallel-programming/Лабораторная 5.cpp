#include <iostream>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <thread>

const size_t N{ 11 };
const size_t M{ 9 };
const size_t COUNT{ N * M };
const size_t NTHREAD{ 4 };

const int A{ -50 };
const int B{ 50 };

const size_t number{ 1 };



void min_el(int matrix[][M], size_t left, size_t right, int& result)
{
	int local_min_el{};

	for (size_t i{left }; i < right; ++i)
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

	result = local_min_el;
	std::cout << "ID from C++API " << std::this_thread::get_id() << ": " << result << "\n";
}

int min_parallel(int matrix[][M])
{
	std::thread thr[NTHREAD - 1]{};
	int results[NTHREAD]{};
	size_t chunk{ COUNT / NTHREAD };
	for (int i{}; i < NTHREAD-1; ++i)
	{
		thr[i] = std::thread(min_el, matrix, chunk * i, chunk * (i + 1), std::ref(results[i]));
	}

	int global_min{};
	min_el(matrix, chunk*(NTHREAD-1), COUNT, global_min);

	for (size_t i{}; i < NTHREAD - 1; ++i)
	{
		thr[i].join();
		if (results[i] < global_min || global_min == 0)
			global_min = results[i];

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
	int min_nonparallel{};
	min_el(matrix, 0, COUNT, min_nonparallel);
	std::cout << "Nonparallel min = " << min_nonparallel << "\n";
	std::cout << "parallel min = " << min_parallel(matrix) << "\n";

	std::cin.ignore().get();
	return 0;
}
