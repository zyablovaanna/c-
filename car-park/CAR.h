#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <fstream>
#include <cstring>
#include <string>
const int m = 100;
class Car
{
public:
	Car() {};
	Car(std::ifstream& file);
	static std::string specified_type;
	static std::string specified_color;
	virtual void print_type_car() = 0;
	virtual ~Car() = default;
	virtual void print()
	{
		std::cout << "Марка: " << mark << '\n';
		std::cout << "Гос номер: " << number << '\n';
		std::cout << "ФИО водителя: " << FIO << '\n';
	};
protected:
	char FIO[m]{};
	char mark[m]{};
	char number[m]{};
};

class Passenger : public Car
{
public:
	Passenger() {};
	Passenger(std::ifstream& file);
	void print_type_car() override;
	void print() override;
	char* get_body_type() { return body_type; };
	char* get_color() { return color; };
private:
	char body_type[m]{};
	char model[m]{};
	char color[m]{};
};

class Cargo : public Car
{
public:
	Cargo() {};
	Cargo(std::ifstream& file);
	void print_type_car() override;
	void print() override;
private:
	size_t tonnage{};
	char view[m]{};
};


