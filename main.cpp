#include <iostream>

#include "SeaBattleExecutor.h"
#include <stdexcept>

int main() {
    try {
        SeaBattleExecutor::RunApplication();
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }
    
    return 0;
}
