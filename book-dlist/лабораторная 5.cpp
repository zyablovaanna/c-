//III.«Научная книга»
//Структура записи :
// Автор
// Название
// Год издания
// Количество страниц
//4. Сформировать список, содержащий книги авторов, издавших наибольшее количество книг по заданной специальности.
#include "DLIST.h"


DLIST task(DLIST& list)
{
	DLIST result;
	ptrNODE ptr{ list.get_head() };
	ptrNODE ptr_pred_fio = list.get_head();
	std::string pred_fio{ list.get_head()->info->get_FIO() }, t_fio{ ptr->info->get_FIO() };
	std::string t_spec{ ptr->info->get_specialization() };
	size_t max_cnt{}, cnt{};

	auto lambda = [&pred_fio, &max_cnt, &result, &t_fio](size_t cnt, ptrNODE ptr_pred_fio)
		{
			if (cnt > max_cnt)
			{
				max_cnt = cnt;

				while (!result.empty())
				{
					ptrNODE h = result.get_head();
					result.remove(h);
				}
			}
			if (cnt >= max_cnt && cnt > 0)
			{
				while (ptr_pred_fio && ptr_pred_fio->info->get_FIO() == pred_fio && Book::special_task.compare(ptr_pred_fio->info->get_specialization()) == 0)
				{
					TInfo new_book = new Book(*(ptr_pred_fio->info));
					if (!result.empty())
					{
						ptrNODE t = result.get_tail();
						result.insert_after(t, new_book);
					}
					else
						result.first_node(new_book);
					ptr_pred_fio = ptr_pred_fio->next;
				}
			}

		};

	while (ptr && t_spec.compare(Book::special_task) == 0)
	{
		t_fio = ptr->info->get_FIO();
		t_spec = ptr->info->get_specialization();

		if (pred_fio.compare(t_fio) == 0)
		{
			cnt += 1;
		}
		else
		{
			lambda(cnt, ptr_pred_fio);
			pred_fio = t_fio;
			ptr_pred_fio = ptr;
			cnt = 1;
		}


		ptr = ptr->next;
	}
    lambda(cnt, ptr_pred_fio);



	return result;
}

std::string Book::special_task = "Исторический роман";
int main()
{
	SetConsoleOutputCP(1251);
	std::ifstream file("text.txt");

	DLIST list("text2.txt", [](TInfo ptr_1, TInfo ptr_2) {return *ptr_1 >= *ptr_2; });
	list.print(std::cout);
	std::cout << "-------------------------------------\n";
	DLIST list2 = std::move(task(list));//конструктор перемещения
	list2.print(std::cout);
	std::cout << "---------------------------------------------------\n";

	DLIST list3 = list2; //копирующий конструктор
	list3.print(std::cout);
	std::cout << "--------------------------------\n";

	list3 = std::move(task(list)); //оператор присваивания перемещением
	//list3.print(std::cout);
	std::cout << "-----------------------------\n";

	DLIST copy;
	copy = list2;
	//copy.print(std::cout);
	//list3 = list2;//оператор присваивания копированием
	//list3.print(std::cout);
	std::cout << "-----------------------------\n";
	//list.print(std::cout);
	//DLIST::print_count();


	std::cin.ignore().get();
	return 0;
}