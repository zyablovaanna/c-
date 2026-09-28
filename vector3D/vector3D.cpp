//Создать класс vector3D, задаваемый тройкой координат.Обязательно должны быть
//реализованы : сложение и вычитание векторов, скалярное произведение,
//умножение на скаляр, сравнение векторов, вычисление длины вектора, сравнение
//длины векторов, векторное произведение.
//Зяблова Анна 9 группа МОиАИС 2 курс
#include "vector3D.h"



//ввод координаты вектора 
int16_t vector3D::input_coordinate(const int coord)
{
    int16_t num{};
    char coordinate{};
    switch (coord)
    {
    case 0:
        coordinate = 'x';
        break;
    case 1:
        coordinate = 'y';
        break;
    case 2:
        coordinate = 'z';
        break;
    }

    std::cout << "Введите координату " << coordinate << ' ';
    do
    {
        std::cin >> num;
        if (std::cin.good())
        {
            return static_cast<int16_t>(num);
        }
        else
        {
            std::cout << "Неправильный ввод!\n";
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
        }
    } while (true);
}


vector3D::vector3D(int16_t x, int16_t y, int16_t z)
{
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
}

//ввод координат вектора из файла
void vector3D::fill_vector_from_file(std::ifstream& file, bool& flag)
{
    flag = true;
    int16_t num{};
    for (int i{}; i < 3; ++i)
    {
        file >> num;
        if (file.good())
            vector[i] = num;
        else
        {
            flag = false;
            std::cout << "Неверные данные!\n";
            break;
        }
    }
}

//ввод координат вектора из консоли
void vector3D::fill_vector_from_console()
{
    std::cout << "Введите координаты вектора: \n";
    for (int i{}; i < 3; ++i)
    {
         vector[i] = input_coordinate(i);
    }
}

//печать вектора
void vector3D::print_vector(std::ostream& stream) const
{
    stream << "{x: " << vector[0] << ", y: " << vector[1] << ", z: " << vector[2] << "}\n";
}

//перегрузка +
//vector3D vector3D::operator + (const vector3D& vector2) const
//{
//    vector3D res(vector[0] + vector2.get_x(), vector[1] + vector2.get_y(), vector[2] + vector2.get_z());
//    return res;
//}
////перегрузка -
//vector3D vector3D::operator - (const vector3D& subtrahend) const
//{
//    vector3D res(vector[0] - subtrahend.get_x(), vector[1] - subtrahend.get_y(), vector[2] - subtrahend.get_z());
//    return res;
//}

//длина вектора
float vector3D::vector_length() const
{
    float result{};
    for (int i{}; i < 3; ++i)
    {
        result += vector[i] * vector[i];
    }
    result = sqrt(result);

    return result;
}




//скалярное произведение векторов
int16_t vector3D::scalar_product(const vector3D& vector2)
{
    int16_t result{};

    result += vector[0] * vector2.get_x();
    result += vector[1] * vector2.get_y();
    result += vector[2] * vector2.get_z();

    return result;
}

//векторное произведение векторов
vector3D vector3D::vector_product(const vector3D& vector2)
{
    vector3D result;
    int16_t v2_x = vector2.get_x();
    int16_t v2_y = vector2.get_y();
    int16_t v2_z = vector2.get_z();
   
    result.set_x(vector[1] * v2_z - vector[2] * v2_y);
    result.set_y(vector[2] * v2_x - vector[0] * v2_z);
    result.set_z(vector[0] * v2_y - vector[1] * v2_x);


    return result;
}

//произведение вектора на скаляр
vector3D vector3D::multiplication_by_scalar(const int16_t scalar)
{
    vector3D result(vector[0] * scalar, vector[1] * scalar, vector[2] * scalar);
    return result;
}

//сравнение векторов
bool vector3D::compare_vectors(const vector3D& vector2)
{
    if (get_x() != vector2.get_x())
        return false;
    if (get_y() != vector2.get_y())
        return false;
    if (get_z() != vector2.get_z())
        return false;

    return true;
}


//сравнение длин векторов
int16_t vector3D::compare_length_vectors(const vector3D& vector2)
{
    int16_t length_1 = vector_length();
    int16_t length_2 = vector2.vector_length();

    if (length_1 > length_2)
        return 1;
    if (length_1 == length_2)
        return 0;
    if (length_1 < length_2)
        return -1;

}
//
////сумма векторов
//vector3D vector3D::sum_vectors(const vector3D& vector2)
//{
//    return  this->operator+(vector2);
//}
//
////разность векторов
//vector3D vector3D::difference_vectors(const vector3D& subtrahend)
//{
//    return this->operator-(subtrahend);
//}

vector3D operator-(const vector3D& v_1, const vector3D& v_2)
{
    vector3D res(v_1.get_x() - v_2.get_x(), v_1.get_y() - v_2.get_y(), v_1.get_z() - v_2.get_z());
    return res;
}
vector3D operator+(const vector3D& v_1, const vector3D& v_2)
{
    vector3D res(v_1.get_x() + v_2.get_x(), v_1.get_y() + v_2.get_y(), v_1.get_z() + v_2.get_z());
    return res;
}
