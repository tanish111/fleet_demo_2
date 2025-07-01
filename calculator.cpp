#include <iostream>

int main() {
    std::cout << "Welcome to the CLI Calculator!" << std::endl;
    std::cout << "Enter an expression (e.g., 5 + 3): ";
    double num1, num2;
    char op;
    std::cin >> num1 >> op >> num2;

    double result;
    switch (op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
            } else {
                std::cout << "Error: Division by zero!" << std::endl;
                return 1;
            }
            break;
        default:
            std::cout << "Error: Invalid operator!" << std::endl;
            return 1;
    }
    std::cout << "Result: " << result << std::endl;
    return 0;
}