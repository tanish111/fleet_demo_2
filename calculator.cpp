#include <iostream>

int main() {
    std::cout << "Welcome to the CLI Calculator!" << std::endl;
    std::cout << "Enter an expression (e.g., 5 + 3): ";
    // In a real calculator, you would parse the input here.
    // For now, we'll just acknowledge the input.
    std::string input;
    std::getline(std::cin, input);
    std::cout << "You entered: " << input << std::endl;
    return 0;
}