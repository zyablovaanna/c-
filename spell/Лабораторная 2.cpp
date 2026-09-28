// Зяблова Анна 9 гр МОиАИС 2к (4)
//«Заклинание» представляет собой структуру с полями : название, краткое описание,
//стихия(перечисление), затрачиваемая мана, сила и продолжительность эффекта,
//минимальный уровень чародея для использования, время суток «с» и «по» которое
//оно может быть применено(использовать одну из предлагаемых языком структур).
//Выборка заклинаний производится путем указания пользователем подходящего
//интервала по одному из критериев : название(например, от «Mutilatio»
//до «Somnium» включительно) и расход маны.Также выборка всех заклинаний,
//доступных в указанное время, выборка всех заклинаний, относящихся к указанной
//стихии, выборка всех заклинаний, доступных на указанном уровне.Шаблонный
//класс должен иметь дополнительный метод сортировки.Добавить в меню
//возможность отсортировать заклинания по минимальному уровню по возрастанию
//и по названию по алфавиту

import std;
import Module;
#include <Windows.h>

enum class Choice
{
    LOAD_FROM_FILE = 1,//загрузка из файла
    ADD_FROM_CONSOLE,//добавление одного заклинания с консоли
    ADD_FROM_FILE,//добавление нескольких заклинаний из файла
    ERASE_BY_INDEX,//удаление по индексу
    ERASE_BY_VALUE, //удаление по значению
    CHANGE_BY_INDEX,//изменение по индексу
    //выборка
    SELECT_BY_NAME_RANGE,//по диапазону названий
    SELECT_BY_MANA_RANGE,//по диапазону маны
    SELECT_BY_ELEMENT, //по стихии
    SELECT_BY_LEVEL, //по уровню
    SELECT_BY_TIME,//по времени
    //сортировка
    SORT_BY_NAME, //по названию
    SORT_BY_LEVEL,//по уровню
    //вывод
    PRINT_TO_SCREEN,//на экран
    PRINT_TO_FILE,//в файл
    EXIT
};



//печать сообщения и очистка потока
void error_input()
{
    std::cout << "Ошибка ввода\n";
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
}

//ввод числа
int input_number(const std::string& message = "")
{
    int number{};
    std::cout << message;
    do
    {
        std::cin >> number;
        if (std::cin.good())
        {
            return number;
        }
        else
            error_input();
    } while (true);
}


//ввод стихии 
Element input_element() {
    while (true) {
        std::println("\nВыберите стихию:");
        std::println("1 - Вода");
        std::println("2 - Земля");
        std::println("3 - Воздух");
        std::println("4 - Огонь");
        std::print("Ваш выбор: ");

        int choice;
        std::cin >> choice;

        if (std::cin.good() && choice >= 1 && choice <= 4) {
            return static_cast<Element>(choice);
        }

        error_input();
    }
}

//ввод времени
std::chrono::minutes input_time() {
    std::println("Введите время в формате HH:MM: ");
    while (true) {
        int hours, minutes;
        char colon;

        std::cin >> hours >> colon >> minutes;

        if (std::cin.good() && colon == ':' &&
            hours >= 0 && hours <= 23 &&
            minutes >= 0 && minutes <= 59) {
            return  std::chrono::minutes(hours*60 + minutes);
        }

        error_input();
    }
}

namespace spell_filters //компараторы и предикаты для выборок (используются в зависимости от выбора пользователя)
{
    //компараторы
    auto by_name = [](const Spell& a, const Spell& b) { return a.name < b.name; };
    auto by_level = [](const Spell& a, const Spell& b) { return a.min_level < b.min_level; };

    //предикаты (использую оболочки для них)
    auto nameRange(const std::string& from, const std::string& to) {
        return [=](const Spell& s) { return s.name >= from && s.name <= to; };
    }

    auto manaRange(int min, int max) {
        return [=](const Spell& s) { return s.mana >= min && s.mana <= max; };
    }

    auto byElement(Element elem) {
        return [=](const Spell& s) { return s.elem == elem; };
    }

    auto minLevel(int level) {
        return [=](const Spell& s) { return s.min_level <= level; };
    }

    auto atTime(std::chrono::minutes time) {
        return [=](const Spell& s) { return s.is_available_at_time(time); };
    }
}


//для ввода значения в диапазоне (min_val, max_val)
template <typename T>
T get_valid_input(const std::string& message, T min_val, T max_val)
{
    T value;
    do
    {
        std::cout << message << " (" << min_val << " - " << max_val << "): ";
        if (std::cin >> value && value >= min_val && value <= max_val) {
            return value;
        }
        else
            error_input();
    } while (true);
}
//ввод заклинания из консоли
Spell input_spell_from_console(const std::string & message)
{
    std::cout << message;
    Spell spell;
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());

    std::cout << "Введите название: ";
    std::getline(std::cin, spell.name);
    while (spell.name.empty()) {
        std::cout << "Название не может быть пустым. Повторите: ";
        std::getline(std::cin, spell.name);
    }
    std::cout << "Введите описание: ";
    std::getline(std::cin, spell.description);

    spell.elem = input_element();
    spell.mana = get_valid_input("Мана", 0, 10000);
    spell.power = get_valid_input("Сила", 0, 1000);
    int duration = get_valid_input("Продолжительность", 0, 3600);
    spell.duration = spell.duration = 
        std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::minutes(duration)));
    spell.min_level = get_valid_input("Минимальный уровень", 1, 100);

    while (true) {
        std::cout << "Время начала (HH:MM): ";
        spell.start = input_time();
        std::cout << "Время окончания (HH:MM): ";
        spell.end = input_time();   

        if (spell.start > spell.end) {
            std::println("Внимание! Время начала позже времени окончания");
            std::println("Это значит, что заклинание действует через полночь");
            std::print("Вы уверены? (y/n): ");
            char confirm;
            std::cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') break;
        }
        else {
            break;
        }
    }

    return spell;
}

//выбор задачи
Choice user_choice()
{
    int choice{};
    std::cout << "\n1. Загрузка данных из файла\n";
    std::cout << "2. Добавление элемента (из консоли)\n";
    std::cout << "3. Добавление элемента (из файла)\n";
    std::cout << "4. Удаление элемента (по индексу)\n";
    std::cout << "5. Удаление элемента (по значению)\n";
    std::cout << "6. Изменение элемента (по индексу)\n";
    std::cout << "7. Выборка (по названиям)\n";
    std::cout << "8. Выборка (по мане)\n";
    std::cout << "9. Выборка (по стихии)\n";
    std::cout << "10.Выборка (по уровню)\n";
    std::cout << "11.Выборка (по времени)\n";
    std::cout << "12.Сортировка (по названиям)\n";
    std::cout << "13.Сортировка (по уровню)\n";
    std::cout << "14.Печать (в консоль)\n";
    std::cout << "15.Печать (в файл)\n";
    std::cout << "16.Выход\n";

    do
    {
        std::cout << "Введите нужный пункт меню: ";
        std::cin >> choice;
        if (choice >= static_cast<int>(Choice::LOAD_FROM_FILE) && choice <= static_cast<int>(Choice::EXIT))
        {
            std::cout << '\n';
            return static_cast<Choice>(choice);
        }
        else
            error_input();
    } while (true);
}


//ввод имени файла
std::string input_file()
{
    static std::string word = ".txt";
    std::cout << "Введите имя файла: ";
    std::string file_name{};
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());


    size_t file_length{};
    int word_length = word.length();
    do
    {
        std::getline(std::cin, file_name);
        file_length = file_name.length();
        if (file_length > word_length && word.compare(file_name.substr(file_length - word_length)) == 0)
        {
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            return file_name;
        }
        else
        {
            error_input();
        }
    } while (true);

}



using SpellContainerSp = SpellContainer<Spell>;


void handle_filter_spells(SpellContainerSp& container,std::function<bool(const Spell&)> predicate) //!! 
{
    if (container.size() != 0)
    {
        auto select = container.select(predicate);
        if (!select.empty()) {
            std::println("Найдены заклинания:");

            for (const auto& spell : select)
                std::cout << spell;
        }
        else
            std::println("Данные не найдены");
    }
    else
        std::cout << "Контейнер не содержит ни одного элемента\n";
}


//выполнение задачи в зависимости от выбора пользователя
void completing_task(SpellContainerSp& container, Choice user_input)
{
    switch (user_input)
    {
    case Choice::LOAD_FROM_FILE:
    {
        std::string file_name{ input_file() };
        std::ifstream file(file_name);
        container.load_from_file(file_name);
        if (container.size()!=0)
        {
            std::cout << "Контейнер заполнен успешно\n";
        }
        else
            std::cout << "Произошла ошибка\n";
        break;
    }
    case Choice::ADD_FROM_CONSOLE:
    {
        container.add(input_spell_from_console("Введите данные для заклинания:\n"));
        std::cout << "Успешно\n";
        break;
    }
    case Choice::ADD_FROM_FILE:
    {
        std::string file_name{ input_file() };
        std::ifstream file(file_name);
        Spell s;
        if (file >> s)
        {
            container.add(s);
            std::cout << "Успешно\n";
        }
        else 
            std::cout << "Произошла ошибка\n";
        break;
    }
    case Choice::ERASE_BY_INDEX:
    {
        if (container.size() != 0)
        {
            std::cout << "Введите целое число в диапазоне от 0 до " << container.size()-1 << '\n';
            int index = input_number();
            if (container.remove_to_index(index))
                std::cout << "Успешно\n";
            else
                std::cout << "Произошла ошибка\n";
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }
    case Choice::ERASE_BY_VALUE:
    {
        if (container.size() != 0)
        {
            Spell s = input_spell_from_console("Введите заклинание для удаления");
            if (container.remove_to_value(s))
                std::cout << "Успешно\n";
            else
                std::cout << "Произошла ошибка\n";
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }
    case Choice::CHANGE_BY_INDEX:
    {
        if (container.size() != 0)
        {
            std::cout << "Введите целое число в диапазоне от 0 до " << container.size() - 1 << '\n';
            int index = input_number();
            Spell s = input_spell_from_console("Введите новое заклинание ");
            if (container.change_elem(index, s))
                std::cout << "Успешно\n";
            else
                std::cout << "Произошла ошибка";
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }
    case Choice::SELECT_BY_NAME_RANGE:
    {
        std::cin.clear();
        std::cin.ignore(std::cin.rdbuf()->in_avail());
        std::string from, to;

        std::println("Введите название заклинания (from): ");
        std::getline(std::cin, from);

        std::println("Введите название заклинания (to): ");
        std::getline(std::cin, to);

        handle_filter_spells(container, spell_filters::nameRange(from, to));
        break;
    }
    case Choice::SELECT_BY_MANA_RANGE:
    {
        int from = input_number("Введите начальное значение маны (from): ");
        int to = input_number("Введите конечное значение маны (to): ");
        if (from > to)
        {
            std::println("Данные некорректны");
            break;
        }
        handle_filter_spells(container, spell_filters::manaRange(from, to));

        break;
    }
    case Choice::SELECT_BY_ELEMENT:
    {
        Element e = input_element();
        handle_filter_spells(container, spell_filters::byElement(e));
        break;
    }
    case Choice::SELECT_BY_LEVEL:
    {
        int level = input_number("Введите значение уровня: ");
        handle_filter_spells(container, spell_filters::minLevel(level));
        break;
    }
    case Choice::SELECT_BY_TIME:
    {
        std::chrono::minutes time = input_time();
        handle_filter_spells(container, spell_filters::atTime(time));
        break;
    }
    case Choice::SORT_BY_NAME:
    {
        if (container.size() != 0)
        {
            container.sort(spell_filters::by_name);
            std::println("Сортировка успешна");
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }
    case Choice::SORT_BY_LEVEL:
    {
        if (container.size() != 0)
        {
            container.sort(spell_filters::by_level);
            std::println("Сортировка успешна");
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }
    case Choice::PRINT_TO_SCREEN:
    {
        if (container.size() != 0)
        {
            container.print(std::cout);
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }
    case Choice::PRINT_TO_FILE:
    {
        std::string file_name{ input_file() };
        std::ofstream file(file_name);
        if (container.size() != 0)
        {
            container.print(file);
        }
        else
            std::cout << "Контейнер не содержит ни одного элемента\n";
        break;
    }

    

    }
}

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    Choice choice_task{};
    SpellContainerSp container;
    do
    {
        choice_task = user_choice();
        completing_task(container, choice_task);

    } while (choice_task != Choice::EXIT);

    return 0;
}
