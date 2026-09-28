#include "CAR_PARK.h"
#include "CAR.h"


bool task(Car_park& car_p, std::function<bool(const std::unique_ptr<Car>&) > predicate) //вызываем после сортировки
{
	bool flag{};
	std::list<ptrCar>::iterator it{ car_p.get_park().begin() };

	while (it != car_p.get_park().end() && predicate(*it))
	{
		(*it)->print();
		++it;
		flag = 1;
	}
	return flag;
}


std::string Car::specified_color = "синий";
std::string Car::specified_type = "купе";


int main()
{
	auto is_good = [](const std::unique_ptr<Car>& a)
		{
			bool flag{};
			if (Passenger* ptr = dynamic_cast<Passenger*>(a.get()))
			{
				if (Car::specified_type.compare(ptr->get_body_type()) == 0 && Car::specified_color.compare(ptr->get_color()) == 0)
					flag = 1;

			}
			return flag;
		};

	auto compare_l = [is_good](const std::unique_ptr<Car>& a, const std::unique_ptr<Car>& b)
		{
			Passenger* ptr_a = dynamic_cast<Passenger*>(a.get());
			Passenger* ptr_b = dynamic_cast<Passenger*>(b.get());
			bool res{ };
			bool compare_a = is_good(a);
			bool compare_b = is_good(b);

			if (ptr_a && ptr_b)
			{
				if (compare_a && !compare_b) res = true;
			}
			else
			{
				if (ptr_a && !ptr_b) res = true;
			}

			return res;
		};


	SetConsoleOutputCP(1251);

	Car_park p1("text.txt", "Боравто");
	p1.print();
	p1.sort_car(compare_l);
	std::cout << "----------------\n";
	p1.print();
	std::cout << "-----------------------------\n";
	bool flag = task(p1, is_good);
	if (!flag)
		std::cout << "Автомобили с заданными свойствами отсутствуют\n";
	std::cin.ignore().get();
	return 0;
}