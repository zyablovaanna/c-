#pragma once
#include "BOOK.h"

using TInfo = Book*;

class NODE
{
public:
	TInfo info{};
	NODE* next{}, * prev{};
	NODE(TInfo info, NODE* next = nullptr, NODE* prev = nullptr) : info(info), next(next), prev(prev)
	{
	};
	~NODE()
	{
		next = prev = nullptr;
	};
};

using ptrNODE = NODE*;

class DLIST
{
private:
	
	ptrNODE head{}, tail{};
public:
	DLIST() {};
	DLIST(const char* file_name, std::function<bool(TInfo, TInfo)> compare);

	DLIST(const DLIST& other); //конструктор копирования
	DLIST& operator = (const DLIST& other); //присваивание копированием
	DLIST(DLIST&& tmp); //присваивание перемещением (объект переместиться к нам и удалится)
	DLIST& operator=(DLIST&& tmp); //темп исчезнет
	
	~DLIST();

	void first_node(TInfo elem);
	bool empty();
	void insert_after(ptrNODE ptr, TInfo elem);
	void insert_before(ptrNODE ptr, TInfo elem);
	void remove(ptrNODE& ptr);
	void print(std::ostream& stream);
	ptrNODE get_head() { return head; };
	ptrNODE get_tail() { return tail; };
};
