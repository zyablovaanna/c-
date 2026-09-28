#include <iostream>
#include <fstream>
#include <functional>
#include <random>
#include <string>
#include <numeric>
#include <cmath>
#include <chrono>
export module Module;

export enum class Element
{
	WATER = 1, LAND, AIR, FIRE
};


export struct Spell
{
public:
	std::string name;
	Element elem;
	std::string description;
	int mana;
	int power;
	//храним длительность как точку во времени (отступ от начала эпохи)
	std::chrono::system_clock::time_point duration;
	int min_level;
	std::chrono::minutes start{ 0 };
	std::chrono::minutes end{ 0 };

	bool is_available_at_time(const std::chrono::minutes& time) const;
	std::string time_to_string(std::chrono::minutes minutes) const;
	std::string elem_to_string() const;
	Spell() {};

	bool operator==(const Spell& other) const {
		return name == other.name;
	}

};

export std::ostream& operator << (std::ostream& os, const Spell& spell)
{
	//превращаем time_point обратно в целое число минут
	auto dur_val = std::chrono::duration_cast<std::chrono::minutes>(
		spell.duration.time_since_epoch()
	).count();

	os << spell.name << "\n"
		<< spell.description << "\n"
		<< static_cast<int>(spell.elem) << " "
		<< spell.mana << " "
		<< spell.power << " "
		<< spell.min_level << " "
		<< dur_val << "\n" //выводим число минут
		<< spell.time_to_string(spell.start) << " "
		<< spell.time_to_string(spell.end) << '\n';
	return os;
}

export bool is_valid_time(int h, int m) //провека времени
{
	return (h >= 0 && h < 24) && (m >= 0 && m < 60);
}

export bool parse_time(std::istream& is, std::chrono::minutes& time)
{
	int h, m;
	char colon;
	//пробуем считать формат HH:MM
	if (is >> h >> colon >> m) {
		if (colon == ':' && is_valid_time(h, m)) {
			time = std::chrono::hours(h) + std::chrono::minutes(m);
			return true;
		}
	}
	is.setstate(std::ios::failbit);
	return false;
}


export std::istream& operator >> (std::istream& in, Spell& spell)
{
	if (!std::getline(in >> std::ws, spell.name)) return in;
	if (!std::getline(in, spell.description)) return in;

	int elem_raw;
	long long dur_val;

	if (!(in >> elem_raw >> spell.mana >> spell.power >> spell.min_level >> dur_val)) return in;

	spell.duration = std::chrono::system_clock::time_point(
		std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::minutes(dur_val))
	);

	if (elem_raw < 1 || elem_raw > 4 || spell.mana < 0 || spell.min_level < 1) {
		in.setstate(std::ios::failbit);
		return in;
	}

	spell.elem = static_cast<Element>(elem_raw);

	if (!parse_time(in, spell.start) || !parse_time(in, spell.end)) {
		in.setstate(std::ios::failbit);
	}

	return in;
}



bool Spell::is_available_at_time(const std::chrono::minutes& time) const //проверка, попадает ли время (переведенное в минуты) в диапазон времени
{
	if (start <= end) {
		return time >= start && time <= end;
	}
	else {
		return time >= start || time <= end; //для интервалов через полночь
	}
}


std::string Spell::time_to_string(std::chrono::minutes minutes) const
{
	auto total_min = minutes; //пусть 485 минут
	auto h = std::chrono::duration_cast<std::chrono::hours>(total_min); //8 часов
	auto m = total_min % std::chrono::hours(1); //5 (остаток от деления на час)

	return std::to_string(h.count()) + ":" + (m.count() < 10 ? "0" : "") + std::to_string(m.count()); //если минут меньше 10 то добавляем 0
}

std::string Spell::elem_to_string() const //печать стихии
{
	switch (elem)
	{
	case Element::AIR:
			return "Воздух";
	case Element::LAND:
		return "Земля";
	case Element::FIRE:
		return "Огонь";
	case Element::WATER:
		return "Вода";
	default:
		return "Неизвестное";
	}
}


export template<typename T>
class SpellContainer
{
private:
	std::vector<T> spells;
public:
	size_t size() { return spells.size(); }
	void add(const T& spell); //добавление
	bool remove_to_index(int index); //удаление по индексу
	bool remove_to_value(const T& spell); //удаление по значению
	bool change_elem(int index, const T& new_spell); //замена элемента новым
	//потоковый вывод
	void print(std::ostream& os) const;
	void load_from_file(const std::string& filename);
	
	void sort(std::function<bool(const T&, const T&)> comp);

	template<typename Predicate>
	std::vector<T> select(Predicate pred) const;

};


template<typename T>
void SpellContainer<T>::add(const T& spell)
{
	spells.push_back(spell);
}
template<typename T>
bool SpellContainer<T>::remove_to_index(int index)
{
	if (index >= 0 && index < spells.size())
	{
		spells.erase(spells.begin() + index);
		return true;
	}
	return false;
}

template<typename T>
bool SpellContainer<T>::remove_to_value(const T& spell) //!! find + erase
{
	auto it = std::find(spells.begin(), spells.end(), spell); //возвращает итератор
	if (it != spells.end())
	{
		spells.erase(it);
		return true;
	}
	return false;
}

template<typename T>
bool SpellContainer<T>::change_elem(int index, const T& new_spell)
{
	if (index < spells.size())
	{
		spells[index] = new_spell;
		return true;
	}
	else
		return false;
}

template<typename T>
void SpellContainer<T>::print(std::ostream& os) const
{
	std::copy(
		spells.begin(),
		spells.end(),
		std::ostream_iterator<T>(os, "\n")
	);
}

template<typename T>
void SpellContainer<T>::load_from_file(const std::string& filename) //!! ввод с помощью потокового итератора
{
	std::ifstream file(filename);

	if (file) 
	{
		//очищаем текущий контейнер, если нужно перезаписать данные
		spells.clear();
		std::copy(std::istream_iterator<T>(file), std::istream_iterator<T>(), std::back_inserter(spells));
	}
}

template<typename T>
void SpellContainer<T>::sort(std::function<bool(const T&, const T&)> comp)
{
	std::sort(spells.begin(), spells.end(), comp);
}

template<typename T>
template<typename Predicate>
std::vector<T> SpellContainer<T>::select(Predicate pred) const
{
	std::vector<T> result;
	std::copy_if(spells.begin(), spells.end(),
		std::back_inserter(result), pred);
	return result;
}