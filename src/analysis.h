#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <iostream>

void analysis(std::string &str, int *var);
bool is_num(char c);
bool is_number(char c);
bool is_var(char c);
bool is_oper(char c);
bool other(char c);
void help();

void analysis(std::string &str, int *var)
{
    int size = str.length(), i = 0, j = 0, tempi;
    bool flag_corr = true, flag = false;
    int counter1 = 0, loop = 0;
    int maxvar = 1, temp;
    while(flag_corr)
    {
        flag_corr = false;
        while(i < size)
        {
            if(!is_number(str[i]) && !is_oper(str[i]) && !is_var(str[i]) && !other(str[i]))
            {
                std::cout << "The " << i + 1 << " symbol is not suitable!\n";
                std::cout << "Try to enter formula again\n";
                getline(std::cin, str);
                flag_corr = true;
                break;
            }
            if(str[i] == '(') ++loop;
            if(str[i] == ')') --loop;
            ++i;
        }
        if(loop != 0)
        {
            std::cout << "There is an missing loop!\n";
            std::cout << "Try to enter formula again\n";
            getline(std::cin, str);
            flag_corr = true;
        }
    }

    i = 0;
    while(i < size)
    {
        if(str[i] == '!')
        {
            counter1 = tempi = i;
            while(1)
            {
                ++counter1;
                ++i;
                if(str[i] == ' ' || str[i] == '!') continue;
                else break;
            }
            if(counter1 - tempi > 1)
            {
                if((counter1 - tempi) % 2 == 0) for(int j = tempi; j < counter1; ++j) str[j] = ' ';
                else for(int j = tempi; j < counter1 - 1; ++j) str[j] = ' ';
                flag = true;
            }
            counter1 = 0;
            i = tempi;
        }
        else if(is_var(str[i]))
        {
            ++i;
            temp = 1;
            if(!is_number(str[i]))
            {
                ++i;
                continue;
            }
            j = i;
            if(is_number(str[j]) && str[j] > '0')
            {
                temp -= 1;
                while(is_number(str[j]))
                {
                    temp += (str[j] - 48);
                    temp *= 10;
                    ++j;
                }
                temp /= 10;
            }
            if(maxvar < temp) maxvar = temp;
        }

        ++i;
    }

    *var = maxvar;

    if(flag)
    {
        std::cout << "This is more correct expression:\n";
        for(int i = 0; i < size; ++i) if(str[i] != ' ') std::cout << str[i];
        std::cout << "\n";
    }
}

bool is_number(char c)
{
    return c >= 48 && c <= 57;
}

bool is_var(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool is_oper(char c)
{
    return c == '^' || c == '~' || c == '>' || c == '<' || c == '|' || c == '&';
}

bool other(char c)
{
    return c == '!' || c == '(' || c == ')' || c == '=' || c == ' ';
}

void help()
{
    std::cout << "___There is manual for my work___\n";
    std::cout << "You can use all of these operations, boolean constants and variables\n";
    std::cout << "Variables should contain index otherwise it will be first variable\n";
    std::cout << "\"!\" is negation or element \"not\"\n";
    std::cout << "\"&\" is conjunction or element \"and\"\n";
    std::cout << "\"|\" is disjunction or element \"or\"\n";
    std::cout << "\"~\" is equality or element \"xnor\"\n";
    std::cout << "\"^\" is exclusive \"or\" or element \"xor\"\n";
    std::cout << "\">\" is implication\n";
    std::cout << "\"<\" is reversed implication\n";
    std::cout << "For example string: \"!x&x2~x1=\"\n\n";
}

#endif // ANALYSIS_H
