#include "BOOK.h"

Book::Book(std::ifstream& file)
{
	file.getline(FIO, m);
	file.getline(book_name, m);
	file >> year >> pages;
	file.ignore();
	file.getline(specialization, m);
	
	file.ignore();
	if (!file.eof())
	{
		std::string delimline{};
		getline(file, delimline);
	}
}

int Book::compare(const Book& other) const
{
	int fl1{ special_task.compare(specialization)}, fl2{ special_task.compare(other.specialization)};
	int res{};
	if (fl1 == 0 && fl2 == 0)
	{
		res = strcmp(FIO, other.FIO);
	}
	else
	{
		if (fl1 == 0 && fl2!=0)
		{
			res = 1;
		}
		else
		{
			if (fl1!=0 && fl2==0)
			{
				res = -1;
			}
			else
			{
				res = strcmp(FIO, other.FIO);
			}
		}
	}
	return res;
}

bool Book::operator>(const Book& other) const
{
	return compare(other) > 0;
	//return strcmp(FIO, other.FIO) > 0;
}

bool Book::operator>=(const Book& other) const
{
	return compare(other) >= 0;
	//return strcmp(FIO, other.FIO) >= 0;
}

bool Book::operator<=(const Book& other) const
{
	return compare(other) <= 0;
	//return strcmp(FIO, other.FIO)<=0;
}

bool Book::operator<(const Book& other) const
{
	return compare(other) < 0;
	//return strcmp(FIO, other.FIO) < 0;
}

bool Book::operator==(const Book& other) const
{
	return compare(other) == 0;
	//return strcmp(FIO, other.FIO) == 0;
}

bool Book::operator!=(const Book& other) const
{
	return compare(other) != 0;
	//return strcmp(FIO, other.FIO) != 0;
}


std::ostream& operator<<(std::ostream& os, const Book& book)
{
	os << book.FIO << '\n';
	os << book.book_name << '\n';
	os << book.year << '\n';
	os << book.pages << '\n';
	os << book.specialization << '\n';
	os << "-----------------\n";
	return os;
}

std::istream& operator>>(std::istream& in, Book& book)
{
	in.getline(book.FIO, m);
	in.getline(book.book_name, m);
	in >> book.year >> book.pages;
	in.ignore();
	in.getline(book.specialization, m);

	in.ignore();
	if (!in.eof())
	{
		std::string delimline{};
		getline(in, delimline);
	}
	return in;
}


//bool operator == (const Counter& counter) const
//{
//	return value == counter.value;
//}