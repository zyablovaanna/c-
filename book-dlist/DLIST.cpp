#include "DLIST.h"
#include "BOOK.h"



DLIST::DLIST(const char* file_name, std::function<bool(TInfo, TInfo)> compare)
{
	std::ifstream file(file_name);
	TInfo elem = new Book(file);
	first_node(elem);

	auto find_place = [this, compare](TInfo elem)
		{
			ptrNODE ptr{ head };
			while (ptr && compare(ptr->info, elem))
				ptr = ptr->next;
			return ptr;
		};

	ptrNODE place{};
	while (!file.eof())
	{
		elem = new Book(file);
		place = find_place(elem);
		if (place)
			insert_before(place, elem);
		else insert_after(tail, elem);

	}
	file.close();
}

DLIST::DLIST(const DLIST& other)/* : head(other.head), tail(other.tail)*/
{
	std::cout << "Copy contructor" << '\n';

	ptrNODE ptr{ other.head };
	TInfo elem = new Book(*(ptr->info));
	first_node(elem);
	ptr = ptr->next;
	while (ptr)
	{
		elem = new Book(*(ptr->info));
		insert_after(tail, elem);
		ptr = ptr->next;
	}

}

DLIST& DLIST::operator=(const DLIST& other)
{
	std::cout << "Copy operator\n";
	if (this != &other)
	{
		while (!empty())
		{
			remove(head);
		}
		//head = other.head;
		//tail = other.tail;

		ptrNODE ptr{ other.head };
		TInfo elem = new Book(*(ptr->info));
		first_node(elem);
		while (ptr)
		{
			elem = new Book(*(ptr->info));
			insert_after(tail, elem);
			ptr = ptr->next;
		}

	}
	//++cnt;
	return *this;
	
}

DLIST::DLIST(DLIST&& tmp) : head(tmp.head), tail(tmp.tail)
{
	std::cout << "Move constructor\n";
	tmp.head = nullptr;
	tmp.tail = nullptr;

}

DLIST& DLIST::operator=(DLIST&& tmp)
{
	if (this != &tmp)
	{
		std::cout << "Move operator\n";
		while (!empty())
		{
			remove(head);
		}
		head = tmp.head;
		tail = tmp.tail;
		tmp.head = nullptr;
		tmp.tail = nullptr;
	}
	return *this;

}


DLIST::~DLIST()
{
	while (!empty())
	{
		remove(head);
	}
}


void DLIST::first_node(TInfo elem)
{
	head = tail = new NODE(elem);
}

bool DLIST::empty()
{
	return head == nullptr;
}

void DLIST::insert_after(ptrNODE ptr, TInfo elem)
{
	ptrNODE p = new NODE(elem, ptr->next, ptr);
	if (ptr == tail)
		tail = p;
	else ptr->next->prev = p;
	ptr->next = p;
}

void DLIST::insert_before(ptrNODE ptr, TInfo elem)
{
	ptrNODE p = new NODE(elem, ptr, ptr->prev);
	if (ptr == head)
		head = p;
	else ptr->prev->next = p;
	ptr->prev = p;
}


void DLIST::remove(ptrNODE& ptr)
{
	ptrNODE p = ptr;
	if (ptr == head)
	{
		head = p->next;
		if (p == tail) tail = p->prev;
		else p->next->prev = p->prev;
		ptr = p->next;
	}
	else
		if (ptr == tail)
		{
			tail = p->prev;
			p->prev->next = p->next;
			ptr = p->prev;
		}
		else
		{
			p->next->prev = p->prev;
			p->prev->next = p->next;
			ptr = p->next;
		}
	delete p->info;
	delete p;

}

void DLIST::print(std::ostream& stream)
{
	ptrNODE ptr = head;
	while (ptr)
	{
		stream << *(ptr->info);
		ptr = ptr->next;
	}
	std::cout << '\n';
}


