#include "rpe.h"
#include "analysis.h"

int main()
{
    BoolRPE expression;
    int bits, variables;
    char answer = 'y';
    bool flag = false;
    std::string expr_string;

    help();

    std::cout << "Enter your function\n";
    getline(std::cin, expr_string);

    while(answer == 'y' || answer == 'Y')
    {
        analysis(expr_string, &variables);
        expression.setNumbOfVar(variables);
        std::cout << "Do you want to add elements manually? (y/n) ";
        std::cin >> answer;
        if(answer != 'y' && answer != 'Y')
        {
            std::cout << "Select the option (r - random / t - truth table) ";
            std::cin >> answer;
            if(answer == 'r' || answer == 'R')
            {
                std::cout << "Enter the digit depth of variables ";
                std::cin >> bits;
                expression.setBitSize(bits);
                expression.randomSetX();
            }
            else
            {
                expression.truth_table();
                std::cout << "Do you want to see classification of boolean function? (y/n) ";
                std::cin >> answer;
                if(answer == 'y' || answer == 'Y') flag = true;
            }
        }
        else
        {
            std::cout << "Decimal or bitwise enter? (d/b) ";
            std::cin >> answer;
            if(answer == 'b' || answer == 'B')
            {
                std::cout << "Enter the digit depth of variables ";
                std::cin >> bits;
                expression.setBitSize(bits);
                expression.setX();
            }
            else expression.setXdec();
        }
        expression.getX();
        expression.setExample(expr_string);
        expression.calculate_expression();
        expression.getAnswer();
        if(flag) expression.classification();
        flag = false;
        std::cout << "Do you want to continue? (y/n) ";
        std::cin >> answer;
    }

    return 0;
}

