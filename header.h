//
// Created by User on 09.04.2020.
//

#ifndef FISHMAN_HEADER_H
#define FISHMAN_HEADER_H

#include <string>
#include <utility>
#include <random>
#include <tuple>
#include <iostream>
#include <ctime>
#include <exception>

class Number
{
private:
    std::string _digs; // цифры хранятся в обратном порядке, n -- количество цифр в числе [=_digs.size()]
public:
    void remove_zeroes()
    {
        if (!_digs.empty())
            for (size_t i = _digs.size() - 1; i > 0 && _digs[i] == '0'; --i)
                _digs.pop_back();

    }

    std::string to_str() const
    {
        std::string str;
        if (_digs.size()) {
            size_t i = _digs.size() - 1;
            for (; i > 0 && _digs[i] == 0; --i);
            for(; i > 0; --i)
                str += std::to_string(int(_digs[i]));
            str += std::to_string(int(_digs[0]));
        }

        return str;
    }

    static void print(const Number& num)
    {
        if (num.size()) {
            size_t i = num.size() - 1;
            for (; i > 0 && num[i] == 0; --i);
            for(; i > 0; --i)
                std::cout << int(num._digs[i]);
            std::cout << int(num._digs[0]);
        }
    }

    static inline Number make_number_raw(std::string str)
    {
        for (char& c: str)
            c -= '0';
        return {str};
    }

    static Number make_number(std::string str)
    {
        std::reverse(str.begin(), str.end());
        return make_number_raw(str);
    }

    Number(const std::string& digits)
    {
        _digs = digits;
    }

    char operator[](size_t i) const { return _digs[i];}

    char& operator[](size_t i) { return _digs[i];}

    std::pair<Number, Number> split() const
    {
        return {
                {{_digs.begin(),                      _digs.begin() + (_digs.size() / 2)}}, // копирование первой половины [от 0 до n/2] числа в другую деку и создание числа
                {{_digs.begin() + (_digs.size() / 2), _digs.end()}} // тоже самое со с второй половиной [от n/2 до n]
        };
    }

    Number& shift(size_t power) // *= 10^power
    {
        std::string str;
        str.append(power, 0);
        _digs = str + _digs;


        return *this;
    }

    size_t size() const { return _digs.size();}

    Number& operator+=(const Number& other)
    {
        if (_digs.size() < other.size())
            _digs.append(other.size() - _digs.size(), 0);

        int ostatok = 0;

        for (size_t i = 0; i < other.size(); ++i)
        {
            int sum = _digs[i] + ostatok + other[i];
            _digs[i] = sum % 10;
            ostatok = sum / 10;
        }

        for (size_t i = other.size(); i < _digs.size() && ostatok != 0; ++i)
        {
            int sum = _digs[i] + ostatok;
            _digs[i] = sum % 10;
            ostatok = sum / 10;
        }

        if (ostatok != 0)
            _digs.push_back(ostatok);

        return *this;
    }

    Number operator+ (const Number& other) const
    {
        return Number{other} += *this; // копирование other чтобы можно было использовать неконстантный метод
    }

    Number& operator-= (const Number& other)
    {
        if (_digs.size() < other.size())
            _digs.resize(other.size(), 0);

        int ostatok = 0;
        for (size_t i = 0; i < other.size(); ++i)
        {
            _digs[i] += ostatok - other[i];
            if (_digs[i] < 0)
            {
                _digs[i] += 10;
                ostatok = -1;
            }
            else
                ostatok = 0;
        }
        for (size_t i = other.size(); i < _digs.size() && ostatok != 0; ++i)
        {
            _digs[i] += ostatok;
            if (_digs[i] < 0)
            {
                _digs[i] += 10;
                ostatok = -1;
            }
            else
                ostatok = 0;
        }

        return *this;
    }

    Number operator-(const Number& other) const
    {
        return Number{*this} -= other;
    }

    bool operator== (const Number& other)
    {
        return _digs == other._digs;
    }

    bool operator!= (const Number& other)
    {
        return _digs != other._digs;
    }

    void resize(size_t m)
    {
        _digs.resize(m, 0);
    }
};

class Multiplicator
{
public:
    class MultiplicatorError : std::exception
    {
    protected:
        Number m1, m2, res1, res2, res3;
    public:
        MultiplicatorError(const Number& mult1 = Number("0"), const Number& mult2 = Number("0"),
                           const Number& result1 = Number("0"), const Number& result2 = Number("0"), const Number& result3 = Number("0"))
                : m1(mult1), m2(mult2), res1(result1), res2(result2), res3(result3)
        {
        }

        const char* what() const override
        {
            return (m1.to_str() + " * " + m2.to_str() + " = " + res1.to_str() + " / " + res2.to_str() + " / " + res3.to_str()).data();
        }
    };
    static Number school_grade(const Number& a, const Number& b)
    {
        Number prod = Number::make_number_raw("0");
        for (size_t i = 0; i < a.size(); ++i)
            for (size_t j = 0; j < b.size(); ++j)
            {
                std::string str;
                str.push_back(a[i]*b[j] % 10);
                str.push_back(a[i]*b[j] / 10);
                prod += Number(str).shift(i + j);
            }

        return {prod};
    }

    static Number divide_and_conquer(Number a, Number b)
    {
        if (a.size() <= 10 || b.size() <= 10) // на малых числах для оптимизации используется школьное умножение
            return school_grade(a, b);

        if (a.size() > b.size())
            b.resize(a.size());

        if (b.size() > a.size())
            a.resize(b.size());

        auto a_ = a.split(), b_ = b.split();
        auto a1 = a_.first, a2 = a_.second, b1 = b_.first, b2 = b_.second;
        auto a1b1 = divide_and_conquer(a1, b1);
        auto a1b2_a2b1 = divide_and_conquer(a1, b2) + divide_and_conquer(a2, b1);
        auto a2b2 = divide_and_conquer(a2, b2);
        auto res = a2b2.shift(2 * a1.size()) + a1b2_a2b1.shift(a1.size()) + a1b1;
        res.remove_zeroes();
        return res;
    }

    static Number Karatsuba(Number a, Number b)
    {
        if (a.size() <= 10 || b.size() <= 10) // на малых числах для оптимизации используется школьное умножение
            return school_grade(a, b);

        if (a.size() > b.size())
            b.resize(a.size());

        if (b.size() > a.size())
            a.resize(b.size());

        auto a_ = a.split(), b_ = b.split();
        auto a1b1 = Karatsuba(a_.first, b_.first);
        auto a1_a2__b1_b2 = Karatsuba(a_.first + a_.second, b_.first + b_.second);
        auto a2b2 = Karatsuba(a_.second, b_.second);
        auto res = a2b2.shift(2*a_.first.size()) + (a1_a2__b1_b2 - a1b1 - a2b2).shift(a_.first.size()) + a1b1;
        res.remove_zeroes();
        return res;
    }

    static Number random(size_t size)
    {
        std::random_device rd;  // используем энтропию системы для получения сида
        std::mt19937 gen(rd()); // создаем генератор с полученным сидом
        std::uniform_int_distribution<> dis(0, 9); // задаем диапазон, в котором будем получать числа (цифры 0-9)
        std::string digits;
        for (size_t i = 0; i < size; ++i)
            digits.push_back(dis(gen));

        return {digits};
    }

    static std::vector<std::vector<double>> test_up_to(size_t n, size_t first = 1, size_t step = 1)
    {
        std::vector<std::vector<double>> res((n - first)/step + 1,std::vector<double> {});
        auto it = res.begin();
        for (size_t i = first; i <= n; i += step)
        {   std::cout<<i<<' ';
            double avg1 = 0, avg2 = 0, avg3 = 0;
            for (size_t j = 0; j < 3; ++j)
            {
                Number a = random(i), b = random(i);
                auto start = clock();
                Number res1 = school_grade(a, b);
                auto stop = clock();
                avg1 += (stop - start) / (double) CLOCKS_PER_SEC;
                start = clock();
                Number res2 = divide_and_conquer(a, b);
                stop = clock();
                avg2 += (stop - start) / (double) CLOCKS_PER_SEC;
                start = clock();
                Number res3 = Karatsuba(a, b);
                stop = clock();
                avg3 += (stop - start) / (double) CLOCKS_PER_SEC;
                if (!(res1 == res2 && res2 == res3))
                    throw MultiplicatorError(a, b, res1, res2, res3);
            }
            it->push_back(avg1  / 3);
            it->push_back(avg2  / 3);
            it->push_back(avg3  / 3);
            ++it;
        }
        return res;
    }
};


#endif //FISHMAN_HEADER_H
