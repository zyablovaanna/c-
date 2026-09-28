#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <stdint.h>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
class vector3D
{
private:
	int16_t vector[3];
	int16_t input_coordinate(const int coord);
	
public:
	vector3D() {};
	vector3D(int16_t x, int16_t y, int16_t z);
	void fill_vector_from_file(std::ifstream& file, bool& flag);
	void fill_vector_from_console();

	void print_vector(std::ostream& stream = std::cout) const;

	int16_t get_x() const { return vector[0]; };
	int16_t get_y() const { return vector[1]; };
	int16_t get_z() const { return vector[2]; };

	void set_x(int16_t value) { vector[0] = value; };
	void set_y(int16_t value) { vector[1] = value; };
	void set_z(int16_t value) { vector[2] = value; };

	

	float vector_length() const;
	/*vector3D sum_vectors(const vector3D& vector2);
	vector3D difference_vectors(const vector3D& subtrahend);*/
	int16_t scalar_product(const vector3D& vector2);
	vector3D vector_product(const vector3D& vector2);
	vector3D multiplication_by_scalar(const int16_t scalar);
	bool compare_vectors(const vector3D& vector2);
	int16_t compare_length_vectors(const vector3D& vector2);
};

vector3D operator - (const vector3D& v_1, const vector3D& v_2);
vector3D operator + (const vector3D& v_1, const vector3D& v_2);