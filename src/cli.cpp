#include "cli.hpp"

namespace bcalc
{

BooleanCalculatorCli::BooleanCalculatorCli ( int argc, char ** argv )
{}

//----------------------------------------------------------------------------//

int BooleanCalculatorCli::run ()
{
    BoolRPN expr;
    std::string expr_string;
    char answer = 'n';

    help();

    std::cout << "Enter your function\n";
    std::getline( std::cin, expr_string );

    expr = BoolRPN( expr_string );

    InputManagerCLI iManager;
    OutputManagerCLI oManager;
    Classificator classificator;

    do
    {
        expr.set( iManager );
        expr.calculateExpression();
        expr.get( oManager );
            
        classificator.get(
                expr.getVariables()
            ,   expr.getAnswer()
            ,   [ answer ]() mutable -> bool
                {
                    std::cout <<
                        "Do you want to see the classification"
                        " of boolean function? (y/n) ";
                    
                    std::cin >> answer;
                    
                    return std::toupper( answer ) == 'Y';
                }
            ,   []( std::string_view const sv )
                {
                    std::cout << sv << std::endl;
                }
        );

        std::cout << "Do you want to continue? (y/n) ";
        std::cin >> answer;
    }
    while ( std::toupper( answer ) == 'Y' );

    return 0;
}

//----------------------------------------------------------------------------//

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
