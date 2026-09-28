#include "vector3D.h"

enum class MenuOption
{
    FROM_CONSOLE = 1, FROM_FILE = 2, EXIT = 3
};

enum class Input
{
    SUM_V = 1, DIFFERENCE_V, SCALAR_PRODUCT_V, VECTOR_PRODUCT_V, PRODUCT_ON_SCALAR, LENGTH_V, COMPARE_V, COMPARE_LENGTH_V
};

void errorInp()
{
    std::cout << "Ошибка ввода\n";
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
}

//выбор задачи
Input userInput()
{
    int choice{};
    std::cout << "\n1. Найти сумму векторов\n";
    std::cout << "2. Найти разность векторов\n";
    std::cout << "3. Найти скалярное произведение векторов\n";
    std::cout << "4. Найти векторное произведение векторов\n";
    std::cout << "5. Найти произведение вектора на скаляр\n";
    std::cout << "6. Найти длину вектора\n";
    std::cout << "7. Сравнить векторы\n";
    std::cout << "8. Сравнить длины векторов\n";

    do
    {
        std::cout << "Введите нужный пункт меню: ";
        std::cin >> choice;
        if (choice >= static_cast<int>(Input::SUM_V) && choice <= static_cast<int>(Input::COMPARE_LENGTH_V))
        {
            std::cout << '\n';
            return static_cast<Input>(choice);
        }
        else
            errorInp();
    } while (true);
}

//Выбор способа ввода
MenuOption userChoice()
{
    int choice{};
    std::cout << "1. Решить задачу, вводя данные из консоли\n";
    std::cout << "2. Решить задачу, вводя данные из файла\n";
    std::cout << "3. Выход\n";
    do
    {
        std::cout << "Введите нужный пункт меню: ";
        std::cin >> choice;
        if (choice >= static_cast<int>(MenuOption::FROM_CONSOLE) && choice <= static_cast<int>(MenuOption::EXIT))
        {
            return static_cast<MenuOption>(choice);
        }
        else
            errorInp();
    } while (true);
}

//ввод числа
int16_t input_int()
{
    int16_t num{};
    do
    {
        std::cin >> num;
        if (std::cin.good())
        {
            return num;
        }
        else errorInp();
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
    size_t word_length = word.length();
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
            errorInp();
        }
    } while (true);

}


//ввод скаляра
bool input_scalar(MenuOption option, int16_t& scalar)
{
    bool flag = true;
    switch (option)
    {
    case MenuOption::FROM_CONSOLE:

        scalar = input_int();
        flag = true;
        break;
    case MenuOption::FROM_FILE:
    {
        std::cout << "Введите имя файла, в котором хранится скаляр: \n";
        std::string file_name = input_file();
        std::ifstream file;
        file.open(file_name);
        if (file)
        {
            file >> scalar;
            if (!file.good())
            {
                flag = false;
                std::cout << "Неверные данные!\n";
            }
        }
        else
        {
            std::cout << "Файл не найден!\n";
        }
        file.close();
        break;
    }
    }
    return flag;
}

//ввод одного вектора
bool input_one_vector(MenuOption option, vector3D& v1)
{
    bool flag{};
    switch (option) 
    {
    case MenuOption::FROM_CONSOLE:

        v1.fill_vector_from_console();
        flag = true;
        break;
    case MenuOption::FROM_FILE:
    {
        std::string file_name = input_file();
        std::ifstream file;
        file.open(file_name);
        if (file)
        {
            v1.fill_vector_from_file(file, flag);
        }
        else
        {
            std::cout << "Файл не найден!\n";
        }
        file.close();
        break;
    }
    }
    return flag;
}


//ввод двух векторов
bool input_two_vectors(MenuOption option, vector3D& v1, vector3D& v2)
{
    bool flag{};
    switch (option)
    {
    case MenuOption::FROM_CONSOLE:
        v1.fill_vector_from_console();
        v2.fill_vector_from_console();
        flag = true;
        break;
    case MenuOption::FROM_FILE:
    {
        std::ifstream file;
        std::string file_name = input_file();
        file.open(file_name);
        if (file)
        {
            v1.fill_vector_from_file(file, flag);
            if (flag == true)
            {
                v2.fill_vector_from_file(file, flag);
            }
        }
        else
        {
            std::cout << "Файл не найден!\n";
        }
        file.close();
        break;
    }
    }
    return flag;
}


//решение задачи
void task_list(MenuOption option, Input user_input)
{
    vector3D v1;
    vector3D v2;

    switch (user_input)
    {
    case Input::SUM_V:
        if (input_two_vectors(option,v1,v2))
        {
            std::cout << "Сумма векторов равна вектору: ";
            (v1+v2).print_vector(std::cout); // v1+v2 - перегрузка
            std::cout << '\n';
        }
        break;
    case Input::DIFFERENCE_V:
        if (input_two_vectors(option,v1,v2))
        {
            std::cout << "Разность векторов равна вектору: ";
            (v1-v2).print_vector(std::cout);  //v1-v2 - перегрузка
            std::cout << '\n';
        }
        break;
    case Input::SCALAR_PRODUCT_V:
    {
        if (input_two_vectors(option, v1, v2))
        {
            std::cout << "Скалярное произведение векторов равно : " << v1.scalar_product(v2) << '\n';
        }

        break;
    }
    case Input::VECTOR_PRODUCT_V:
        if (input_two_vectors(option,v1,v2))
        {
            std::cout << "Векторное произведение векторов равно вектору: ";
            v1.vector_product(v2).print_vector(std::cout);
            std::cout << '\n';
        }
        break;
    case Input::PRODUCT_ON_SCALAR:
    {
        int16_t scalar;
        if (input_one_vector(option, v1) && input_scalar(option, scalar))
        {
            std::cout << "Произведение вектора на скаляр равно: ";
            v1.multiplication_by_scalar(scalar).print_vector();
            std::cout << '\n';
        }
        break;
    }
    case Input::LENGTH_V:
        if (input_one_vector(option,v1))
            std::cout << "Длина вектора равна: " << v1.vector_length() << '\n';
        break;
    case Input::COMPARE_V:
        if (input_two_vectors(option,v1,v2))
        {
            if (!v1.compare_vectors(v2))
                std::cout << "Векторы не равны\n";
            else
                std::cout << "Векторы равны\n";
        }
        break;
    case Input::COMPARE_LENGTH_V:
    {
        if (input_two_vectors(option,v1,v2))
        {
            int16_t compare_length_vectors = v1.compare_length_vectors(v2);

            switch (compare_length_vectors)
            {
            case 1:
                std::cout << "Длина вектора "; v1.print_vector(std::cout);
                std::cout << "больше длины вектора "; v2.print_vector(std::cout);
                std::cout << '\n';
                break;
            case 0:
                std::cout << "Длины векторов равны\n";
                break;
            case -1:
                std::cout << "Длина вектора "; v1.print_vector(std::cout);
                std::cout << "меньше длины вектора "; v2.print_vector(std::cout);
                std::cout << '\n';
                break;
            }
        }

        break;
    }
    }

}



int main()
{
    SetConsoleOutputCP(1251);

    MenuOption user_choice{};
    Input user_input;
    do
    {
        user_choice = userChoice();
        if (user_choice != MenuOption::EXIT)
        {
            user_input = userInput();
            task_list(user_choice, user_input);
        }

    } while (user_choice != MenuOption::EXIT);

    //std::cin.ignore().get();
    return 0;
}
