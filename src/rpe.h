#ifndef RPE_H
#define RPE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include "stack.h"

class BoolRPE
{
private:
    int SIZE;
    int numb_of_var;
    std::vector<bool> *X_array;
    std::vector<bool> answer;
    std::string example;
    std::string bool_example;
    bool classes[5] {};
    bool classP = false;
    int prior(char c);
    bool is_num(char c);
    bool is_number(char c);
    bool is_var(char c);
    bool is_oper(char c);
    bool is_unar_oper(char c);
    bool calc(bool second, char oper, bool first);

public:
    BoolRPE(int size = 1, int vars = 1);
    void setExample(std::string expr);
    void setX();
    void setXdec();
    void setNumbOfVar(int vars);
    void setBitSize(int size);
    void randomSetX();
    void getX();
    void getAnswer();
    void calculate_expression();
    void truth_table();//full-cased testing
    void classification();
    ~BoolRPE();
};



BoolRPE::BoolRPE(int size, int vars)
{
    example = "";
    bool_example = "";
    SIZE = size;
    numb_of_var = vars;
    X_array = new std::vector<bool> [numb_of_var];
}

void BoolRPE::setExample(std::string expr)
{
    example = expr;
}

void BoolRPE::setX()
{
    bool temp;
    std::cout << "Enter through the space every variable:\n";
    for(int l = 0; l < numb_of_var; ++l)
    {
        std::cout << l + 1 << " variable = ";
        for(int i = 0; i < SIZE; ++i)
        {
            std::cin >> temp;
            X_array[l].push_back(temp);
        }
    }
}

void BoolRPE::setXdec()
{
    int *array = new int [numb_of_var];
    int max = 1, temp, counter = 0, i = 0, j = 0;
    std::cout << "Enter through the enter every variable (decimal):\n";
    for(int l = 0; l < numb_of_var; ++l)
    {
        std::cout << l + 1 << " variable = ";
        std::cin >> array[l];
        temp = array[l];
        while(temp > 0)
        {
            temp /= 2;
            ++counter;
        }
        if(counter > max) max = counter;
        counter = 0;
    }
    SIZE = max;

    for(int i = 0; i < numb_of_var; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            X_array[i].push_back(0);
        }
    }

    while(i < numb_of_var)
    {
        j = SIZE - 1;
        while(array[i] > 0 && j >= 0)
        {
            if(array[i] % 2)
            {
                X_array[i][j--] = 1;
            }
            else X_array[i][j--] = 0;
            array[i] /= 2;
        }

        ++i;
    }
    delete [] array;
}

void BoolRPE::randomSetX()
{
    srand(time(NULL));
    for(int l = 0; l < numb_of_var; ++l)
    {
        for(int i = 0; i < SIZE; ++i)
        {
            X_array[l].push_back((rand() % 17) % 2);
        }
    }
}

void BoolRPE::setNumbOfVar(int vars)
{
    numb_of_var = vars;
    X_array = (std::vector<bool>*) calloc(numb_of_var, sizeof(std::vector<bool>));
}

void BoolRPE::setBitSize(int size)
{
    SIZE = size;
}

void BoolRPE::getX()
{
    std::cout << "Variables:\n";
    for(int l = 0; l < numb_of_var; ++l)
    {
        std::cout << l + 1 << " variable = ";
        for(int i = 0; i < SIZE; ++i)
        {
            std::cout << X_array[l][i];
        }
        std::cout << "\n";
    }
}

void BoolRPE::getAnswer()
{
    std::cout << "Answer is ";
    for(int i = 0; i < SIZE; ++i)
        std::cout << answer[i];
    std::cout << "\n";
}

void BoolRPE::calculate_expression()
{
    Stack<char> stk_c;
    Stack<bool> stk_b;
    int l = 0, i = 0;
    int temp = 1;

    while(l < SIZE)
    {
        while(i < example.length())
        {
            if(is_num(example[i]))
            {
                bool_example += example[i];
                bool_example += ' ';
            }
            else if(is_oper(example[i]) || is_unar_oper(example[i]))
            {
                while(!stk_c.empty() && prior(stk_c.top()) >= prior(example[i]))
                {
                    bool_example += stk_c.pop();
                    bool_example += ' ';
                }
                stk_c.push(example[i]);
            }
            else if(is_var(example[i]))//отсчет начинается с 1, вместо "x1" можно писать "x"
            {
                ++i;
                if(is_number(example[i]) && example[i] > '0')
                {
                    temp -= 1;
                    while(is_number(example[i]))
                    {
                        temp += (example[i] - 48);
                        temp *= 10;
                        ++i;
                    }
                    temp /= 10;
                }
                bool_example += (X_array[temp - 1][l] + 48);
                bool_example += ' ';
                temp = 1;
                --i;
            }
            else if(example[i] == '(')
            {
                stk_c.push(example[i]);
            }
            else if(example[i] == ')')
            {
                while(stk_c.top() != '(')
                {
                    bool_example += stk_c.pop();
                    bool_example += ' ';
                }
                if(stk_c.top() == '(')
                {
                    stk_c.pop();
                }
            }

            if((example[i] == '=' || i + 1 == example.length()) && !stk_c.empty())
            {
                while(!stk_c.empty())
                {
                    bool_example += stk_c.pop();
                    bool_example += ' ';
                }
                break;
            }

            ++i;
        }

        i = 0;

        while(i < bool_example.length())
        {
            if(is_num(bool_example[i]))
            {
                stk_b.push(bool_example[i] - 48);
            }
            else if(is_oper(bool_example[i]))
            {
                stk_b.push(calc(stk_b.pop(), bool_example[i], stk_b.pop()));
            }
            else if(is_unar_oper(bool_example[i]))
            {
                stk_b.push(!stk_b.pop());
            }
            ++i;
        }

        i = 0;
        bool_example = "";
        answer.push_back(stk_b.pop());
        ++l;
    }
}

int BoolRPE::prior(char c)
{
    if(c == '(') return 1;
    else if(c == '^') return 2;
    else if(c == '~') return 3;
    else if(c == '>') return 4;
    else if(c == '<') return 5;
    else if(c == '|') return 6;
    else if(c == '&') return 7;
    else if(c == '!') return 8;
    return 0;
}

bool BoolRPE::is_num(char c)
{
    return c == '0' || c == '1';
}

bool BoolRPE::is_number(char c)
{
    return c >= 48 && c <= 57;
}

bool BoolRPE::is_var(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool BoolRPE::is_oper(char c)
{
    return c == '^' || c == '~' || c == '>' || c == '<' || c == '|' || c == '&';
}

bool BoolRPE::is_unar_oper(char c)
{
    return c == '!';
}

bool BoolRPE::calc(bool second, char oper, bool first)
{
    if(oper == '&') return first & second;
    else if(oper == '|') return first | second;
    else if(oper == '<') return first & !second;
    else if(oper == '>') return !first | second;
    else if(oper == '~') return first == second;
    else if(oper == '^') return first ^ second;
    return 0;
}

void BoolRPE::truth_table()
{
    int tempsize = 1, t1 = 0, t2;

    for(int i = 0; i < numb_of_var; ++i)
    {
        while(!X_array[i].empty())
        X_array[i].pop_back();
    }
    for(int i = 0; i < numb_of_var; ++i)
    {
        tempsize *= 2;
    }
    SIZE = tempsize;
    for(int i = 0; i < numb_of_var; ++i)
    {
        tempsize /= 2;
        t2 = tempsize;
        while(t2 <= SIZE)
        {
            for(int j = t1; j < t2; ++j) X_array[i].push_back(0);
            t1 += tempsize;
            t2 += tempsize;
            for(int j = t1; j < t2; ++j) X_array[i].push_back(1);
            t1 += tempsize;
            t2 += tempsize;
        }
        t1 = 0;
    }
}

void BoolRPE::classification()
{
    bool flag = true, exit_flag = false;
    const bool c0 = answer[0], f = answer[SIZE - 1];
    bool c = answer[0];

    if(answer[0]) classes[0] = 1;

    if(answer[SIZE - 1] != 1) classes[1] = 1;

    for(int i = 0; i < SIZE / 2; ++i)
    {
        if(answer[i] == answer[SIZE - i - 1])
        {
            classes[2] = 1;
            break;
        }
    }

    for(int i = 0; i < SIZE - 1; ++i)
    {
        for(int k = i + 1; k < SIZE; ++k)
        {
            for(int j = 0; j < numb_of_var; ++j)
            {
                if(X_array[j][i] > X_array[j][k])
                {
                    flag = false;
                    break;
                }
            }
            if(flag && answer[i] > answer[k])
            {
                classes[3] = 1;
                exit_flag = true;
                break;
            }
            flag = true;
        }
        if(exit_flag) break;
    }

    int j = 1;
    for(int i = 0; i < numb_of_var; ++i)
    {
        if(answer[j] ^ c0) c ^= 1;
        j *= 2;
    }
    if(c != f) classes[4] = 1;

    classP = true;
    for(int i = 0; i < 5; ++i)
    {
        if(!classes[i])
        {
            classP = false;
            break;
        }
    }

    if(!classes[0]) std::cout << "T0: Zero is constant\n";
    else std::cout << "T0: Zero is NOT constant\n";
    if(!classes[1]) std::cout << "T1: One is constant\n";
    else std::cout << "T1: One is NOT constant\n";
    if(!classes[2]) std::cout << "S:  The function is self-dual\n";
    else std::cout << "S:  The function is NOT self-dual\n";
    if(!classes[3]) std::cout << "M:  The function is monotone\n";
    else std::cout << "M:  The function is NOT monotone\n";
    if(!classes[4]) std::cout << "L:  The function is linear\n";
    else std::cout << "L:  The function is NOT linear\n";
    if(classP) std::cout << "The function is full\n";
    else std::cout << "The function is NOT full\n";
}

BoolRPE::~BoolRPE()
{
    delete [] X_array;
}

#endif // RPE_H
