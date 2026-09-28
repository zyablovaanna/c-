#include "CAR_PARK.h"

Car_park::Car_park(const char* file_name, const std::string& name)
{
	this->name = name;
	std::ifstream file(file_name);
	char type{};
	while (file >> type)
	{
		file.ignore();
		if (type == 'P')
			car_park.push_back(std::move(std::make_unique<Passenger>(file)));
		else 
			if (type == 'C')
				car_park.push_back(std::move(std::make_unique<Cargo>(file)));
	}
	file.close();
}


void Car_park::sort_car(std::function<bool(const std::unique_ptr<Car>&, const std::unique_ptr<Car>&) > compare)
{
	car_park.sort(compare);
}
