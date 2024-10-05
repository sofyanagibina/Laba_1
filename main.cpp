#include <iostream>
#include "dice.h"

int main() {
    Dice object(100, 511);
    DoubleDice d(object);
    for (int i = 1; i < 201; i++) {
        std::cout << i << ' ' << value_probability(i, d, 1000) << ' ';
    }
    return 0;
}
