#include "CAR.h"

Car::Car(std::ifstream& file)
{
	file.getline(mark, m);
	file.getline(number, m);
	file.getline(FIO, m);
}

Passenger::Passenger(std::ifstream& file) : Car(file)
{
	file.getline(body_type, m);
	file.getline(model, m);
	file.getline(color, m);
	file.ignore();
	if (!file.eof())
	{
		std::string delimline{};
		getline(file, delimline);
	}
}

void Passenger::print_type_car()
{
	std::cout << "Тип: легковой" << '\n';
}

void Passenger::print()
{
	Passenger::print_type_car();
	Car::print();

	std::cout << "Тип кузова: " << body_type << '\n';
	std::cout << "Модель: " << model << '\n';
	std::cout << "Цвет: " << color << '\n';
	std::cout << '\n';
}

Cargo::Cargo(std::ifstream& file) : Car(file)
{
	file >> tonnage;
	file.ignore();
	file.getline(view, m);
	file.ignore();
	if (!file.eof())
	{
		std::string delimline{};
		getline(file, delimline);
	}
}

void Cargo::print_type_car()
{
	std::cout << "Тип: грузовой" << '\n';
}

void Cargo::print()
{
	Cargo::print_type_car();
	Car::print();

	std::cout << "Тоннаж: " << tonnage << '\n';
	std::cout << "Вид: " << view << '\n';
	std::cout << '\n';
}
