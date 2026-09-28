#include <iostream>
#include <omp.h>
#include <Windows.h>
#include <iomanip>
const size_t N{ 11 };
const size_t M{ 13 };
const size_t COUNT{ N * M };
const size_t NTHREAD{ 4 };

const int A{ -50 };
const int B{ 50 };

const size_t number{ 1 };


int min_parallel(int matrix[][M])
{
	omp_set_num_threads(NTHREAD); //кол-во потоков
	int global_min{};
	int chunk{ 7 };
	int local_min = 0;
#pragma omp parallel shared(matrix, global_min) firstprivate(local_min)
	{
		auto check = [](int cur_el)
			{
				cur_el = abs(cur_el);
				while (cur_el / 10 > 0)
					cur_el /= 10;
				return cur_el == number;
			};
#pragma omp for schedule(dynamic, chunk)
		for (int i = 0; i < COUNT; ++i)
		{
			int cur_el = matrix[i / M][i % M];
			if (check(cur_el) && (local_min > cur_el || local_min == 0))
			{
				local_min = cur_el;
			}
		}
#pragma omp critical
		{
			if (local_min != 0 && (local_min < global_min || global_min == 0))
			{
				global_min = local_min;
			}
			std::cout << "thread num : " << omp_get_thread_num() << ": " << global_min << "\n";
		}
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

//свойства язык настройка omp включить 
int main()
{
	srand(GetTickCount64());
	int matrix[N][M];
	init_matrix(matrix);
	print_matrix(matrix);
	int min_nonp{ min_nonparallel(matrix) };
	std::cout << "Nonparallel min = " << min_nonp << "\n";
	int res = min_parallel(matrix);
	std::cout << "parallel min = " << res << "\n";

	std::cin.ignore().get();
	return 0;
}