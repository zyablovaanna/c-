#include <thread>
#include <mutex>
#include <stack>
#include <iostream>
#include <condition_variable>
#include <Windows.h>
#include <vector>
const size_t N{ 11 };
const size_t M{ 9 };
const int A{ -50 };
const int B{ 50 };

const size_t number{ 1 };

using Matrix = std::vector<std::vector<int>>;
class ThreadSafeStack
{
private:
	std::mutex mutex;
	std::stack<Matrix> stack;
public:
	ThreadSafeStack() {};
	~ThreadSafeStack() {};
	void print(Matrix elem)
	{
		int cnt{};
		int i{};
		int j{};
		while (cnt < 3 && i < N)
		{
			j = 0;
			while (cnt < 3 && j < M)
			{
				std::cout << elem[i][j] << " ";
				cnt += 1;
				++j;
			}
			++i;
		}
		std::cout << "\n";
	}
	void push(Matrix elem, int ID)
	{
		std::lock_guard<std::mutex> guard(mutex); 
		stack.push(elem);
		std::cout << "P " << ID << " -> "; 
		print(elem);
	}
	bool try_pop(Matrix& elem, int ID)
	{
		bool result{};

		std::lock_guard<std::mutex> guard(mutex);
		if (!stack.empty())
		{
			result = true;
			elem = stack.top();
			stack.pop();
			std::cout << "C " << ID << " -> "; 
			print(elem);
		}
		else
			std::cout << "C " << ID << " sleep" << '\n';

		return result;
	}
	bool empty()
	{
		std::lock_guard<std::mutex> guard(mutex);
		return stack.empty();
	}
};


ThreadSafeStack TSQ;
std::mutex mutex_cv; 
std::condition_variable cv;

volatile int volume_work_producer{ 10 }; 
volatile int volume_work_consumer{ 10 };
std::atomic_int current_work_producer{};
std::atomic_int current_work_consumer{};


void init_matrix(Matrix& matrix, int ID)
{
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
			matrix[i][j] = ID + rand() % (B - A);
	}
}

int min_el(Matrix& matrix)
{
	int sum{};
	int min_el{};
	auto check = [](int cur_el)
		{
			cur_el = abs(cur_el);
			while (cur_el / 10 > 0)
				cur_el /= 10;
			return cur_el == number;
		};
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
		{
			int cur_el = matrix[i][j];
			if (check(cur_el) && (min_el > cur_el || min_el == 0))
			{
				min_el = cur_el;
			}
		}
	}
	return min_el;
}

void task_producer(int ID)
{
	while (current_work_producer.fetch_add(1) < volume_work_producer)
	{
		Matrix elem(N, std::vector<int>(M));
		init_matrix(elem, ID);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		TSQ.push(elem, ID);
		cv.notify_one(); 
	}
}

void task(Matrix& matrix)
{
	int sum{};
	int min_el{};
	int local_min_el{};
	auto check = [](int cur_el)
		{
			cur_el = abs(cur_el);
			while (cur_el / 10 > 0)
				cur_el /= 10;
			return cur_el == number;
		};
	for (int i{}; i < N; ++i)
	{
		for (int j{}; j < M; ++j)
		{
			int cur_el = matrix[i][j];
			if (check(cur_el) && (min_el > cur_el || min_el == 0))
			{
				min_el = cur_el;
			}
		}
	}
}
//void task_consumer(int ID)
//{
//	while (current_work_consumer < volume_work_consumer)
//	{
//		Matrix elem;
//		std::unique_lock<std::mutex> guard(mutex_cv);//гарантирует корректность, mutex нельзя lock руками позваоляет руками изменять mitex на lock
//		cv.wait_for(guard, std::chrono::seconds(5), []() {return !TSQ.empty(); }); //может пробудить ложно поток (без сигнала), поэтому напишем условие проверки
//		//ждем сигнала, что кто-то нам позвонил, крутим фор через каждые 5 секунд и проверяем условие (если получили звонок и проверили что звонок по реальному событию!!)
//		if (current_work_consumer < volume_work_consumer)
//		{
//			if (TSQ.try_pop(elem, ID))
//			{
//				task(elem);
//				std::this_thread::sleep_for(std::chrono::milliseconds(7));
//				current_work_consumer += 1;
//			}
//		}
//	}
//
//}

void task_consumer(int ID)
{
	while (current_work_consumer.load() < volume_work_consumer)
	{
		Matrix elem;
		std::unique_lock<std::mutex> guard(mutex_cv);
		cv.wait_for(guard, std::chrono::seconds(5), []() {return !TSQ.empty(); });

		if (current_work_consumer.fetch_add(1) < volume_work_consumer)
		{
			if (TSQ.try_pop(elem, ID))
			{
				task(elem);
				std::this_thread::sleep_for(std::chrono::milliseconds(7));
			}
			else
				current_work_consumer.fetch_sub(1);
		}
	}
}

int main()
{
	srand(GetTickCount());
	std::thread worker[5];
	for (int i{}; i < 5; ++i)
	{
		if (i < 2)
			worker[i] = std::thread(task_producer, i);
		else
			worker[i] = std::thread(task_consumer, i);
	}
	for (int i{}; i < 5; ++i)
	{
		if (worker[i].joinable())
			worker[i].join();
	}
	std::cin.ignore().get();
	return 0;
}