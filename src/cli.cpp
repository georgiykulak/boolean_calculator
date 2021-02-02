#include "cli.hpp"

namespace bcalc
{

BooleanCalculatorCli::BooleanCalculatorCli ( int argc, char ** argv )
{}

int BooleanCalculatorCli::run () // TODO: Make map for more readable code
{
    BoolRPE expr;
    std::string expr_string;
    char answer;
    bool flag = true;

    help();

    std::cout << "Enter your function\n";
    std::getline( std::cin, expr_string );

    expr = BoolRPE( expr_string );

    do
    {
        std::cout << "Do you want to add elements manually? (y/n) ";
        std::cin >> answer;

        if ( std::toupper( answer ) != 'Y' )
        {
            std::cout << "Select the option (r - random / t - truth table) ";
            std::cin >> answer;
            
            if ( std::toupper( answer ) == 'R' )
            {
                std::size_t bits;

                std::cout << "Enter the digit depth of variables ";
                std::cin >> bits;

                expr.setVariablesRandomly( bits );
            }
            else
            {
                expr.setViaTruthTable();

                std::cout << "Do you want to see classification of boolean function? (y/n) ";
                std::cin >> answer;
                
                if ( std::toupper( answer ) == 'Y' )
                    flag = true;
            }
        }
        else
        {
            std::cout << "Decimal or bitwise inputs? (d/b) ";
            std::cin >> answer;
            
            if ( std::toupper( answer ) == 'B' )
            {
                std::size_t bits;
                std::cout << "Enter the digit depth of variables ";
                std::cin >> bits;

                expr.setVariablesBinaries( bits );
            }
            else
                expr.setVariablesDecimals();
        }
        
        expr.getVariables();
        expr.calculateExpression();
        expr.getAnswer();
        
        if ( flag )
            expr.classification();
        
        flag = false;
        
        std::cout << "Do you want to continue? (y/n) ";
        std::cin >> answer;
    }
    while ( std::toupper( answer ) == 'Y' );

    return 0;
}

void BooleanCalculatorCli::help ()
{
    std::cout << "_____There is manual for my work_____\n";
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

} // namespace bcalc