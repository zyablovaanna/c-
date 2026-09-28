#include <iostream>
#include <Windows.h>
#include <process.h>
#include <random>
#include <iomanip>

const size_t N{ 11 };
const size_t M{ 9 };
const size_t COUNT{ N*M };
const size_t NTHREAD{ 4 };

const int A{ -50 };
const int B{ 50 };

const size_t number{1};

struct INFORM
{
	int (*matrix)[M];
	size_t left, right;
	int min_el;
};

DWORD MainID = ::GetCurrentThreadId();

unsigned __stdcall min_el(void* param) 
{
	int local_min_el{};
	INFORM* inform = (INFORM*)param; 

	for (size_t i{ inform->left }; i < inform->right; ++i)
	{
		int cur_el = inform->matrix[i / M][i % M];
		int el = abs(cur_el);
		while (el / 10 > 0)
			el /= 10;
		if (el == number && (local_min_el > cur_el || local_min_el == 0))
		{
			local_min_el = cur_el;
		}
	}

	inform->min_el = local_min_el;
	DWORD currentID = ::GetCurrentThreadId();
	std::cout << "ID from WinAPI " << currentID << ": " << inform->min_el << "\n";
	if (currentID != MainID)
		_endthreadex(0); 
	return 0;
}

int min_parallel(int matrix[][M])
{
	/*HANDLE thr[NTHREAD - 1]{};
	INFORM informs[NTHREAD]{};
	size_t chunk{ COUNT / NTHREAD };
	for (int i{}; i < NTHREAD; ++i)
	{
		informs[i].matrix = matrix;
		informs[i].left = chunk * i;
		informs[i].min_el = 0;
		if (i == NTHREAD - 1)
			informs[i].right = COUNT;
		else
		{
			informs[i].right = chunk * (i + 1);
			thr[i] = (HANDLE)_beginthreadex(nullptr, 0, &min_el, &informs[i], 0,      nullptr);
		}
	}

	min_el(informs + NTHREAD - 1);

	int global_min{ informs[NTHREAD - 1].min_el };
	for (size_t i{}; i < NTHREAD - 1; ++i)
	{
		WaitForSingleObject(thr[i], INFINITE);
		CloseHandle(thr[i]);
		if (informs[i].min_el < global_min || global_min == 0)
			global_min = informs[i].min_el;

	}

	return global_min;*/

	HANDLE thr[NTHREAD - 1]{};
	INFORM inf[NTHREAD]{};
	int chunk{ COUNT / NTHREAD };

	for (int i{}; i < NTHREAD; ++i)
	{
		inf[i].matrix = matrix;
		inf[i].left = chunk * i;
		inf[i].min_el = 0;
		if (i == NTHREAD - 1)
			inf[i].right = COUNT;
		else
		{
			inf[i].right = chunk * (i + 1);
			thr[i] = _beginthreadex(nullptr, 0, &min_el, &inf[i], 0, nullptr);
		}

	}

	min_el(INFORM + NTHREAD - 1);

	int global_min{ inf[NTHREAD - 1].min_el };
	for (int i{}; i < NTHREAD - 1; ++i)
	{
		WaitForSingleObject(thr[i], INFINITE);
		CloseHandle(thr[i]);
		//решение задачки

	}
	return global_min;
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
	std::cout << 2 % 7 << "\n";
	srand(GetTickCount64());
	int matrix[N][M];
	init_matrix(matrix);
	print_matrix(matrix);
	std::cout << "Nonparallel min = " << min_nonparallel(matrix) << "\n";
	std::cout << "parallel min = " << min_parallel(matrix) << "\n";

	std::cin.ignore().get();
	return 0;
}
