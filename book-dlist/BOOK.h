//сортирую по авторам
#pragma once
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <string>
#include <cstring>
#include <functional>
//III.«Научная книга»
//Структура записи :
// Автор
// Название
// Год издания
// Количество страниц
// Специальность

const int m = 100;
class Book
{
private:
	char FIO[m]{};
	char book_name[m]{};
	int year{};
	size_t pages{};
	char specialization[m]{};
public:
	static std::string special_task;
	Book() {};
	Book(std::ifstream& file);
	char* get_FIO() { return FIO; };
	char* get_book_name() { return book_name; };
	char* get_specialization() { return specialization; };

	friend std::ostream& operator << (std::ostream& os, const Book& arr);
	friend std::istream& operator >> (std::istream& in, Book& arr);

	int compare(const Book& other) const;
	bool operator > (const Book& other) const;
	bool operator >= (const Book& other) const;
	bool operator < (const Book& other) const;
	bool operator <= (const Book& other) const;
	bool operator == (const Book& other) const;
	bool operator != (const Book& other) const;
};