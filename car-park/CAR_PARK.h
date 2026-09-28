#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <Windows.h>
#include <memory>
#include <algorithm>
#include <functional>
#include <cstring>
#include "CAR.h"

using ptrCar = std::unique_ptr<Car>;

class Car_park
{
private:
	std::string name;
	std::list<ptrCar> car_park;
public:
	Car_park() {};
	Car_park(const char* file_name, const std::string&);
	std::list<ptrCar>& get_park() { return car_park; };
	void print()
	{
		std::cout << "Название парка: " << name << '\n';
		std::for_each(car_park.begin(), car_park.end(), std::mem_fn(&Car::print));
	};
	void sort_car(std::function<bool(const std::unique_ptr<Car>&, const std::unique_ptr<Car>&)> compare);
};