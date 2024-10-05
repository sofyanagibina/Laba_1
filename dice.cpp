// dice.cpp

#include "dice.h"

unsigned General_Dice::roll() {
    return 10; // Default implementation
}

Dice::Dice(unsigned max, unsigned seed) : max(max), dstr(1, max), reng(seed) { }

unsigned Dice::roll() {
    return dstr(reng);
}

ThreeDicePool::ThreeDicePool(unsigned max, unsigned seed_1, unsigned seed_2, unsigned seed_3) 
    : d1(max, seed_1), d2(max, seed_2), d3(max, seed_3) { }

unsigned ThreeDicePool::roll() {
    return d1.roll() + d2.roll() + d3.roll();
}

PenaltyDice::PenaltyDice(General_Dice& object) : object(object) { }

unsigned PenaltyDice::roll() {
    unsigned roll1 = object.roll();
    unsigned roll2 = object.roll();
    return std::min(roll1, roll2);
}

BonusDice::BonusDice(General_Dice& object) : object(object) { }

unsigned BonusDice::roll() {
    unsigned roll1 = object.roll();
    unsigned roll2 = object.roll();
    return std::max(roll1, roll2);
}

DoubleDiceNaslednik::DoubleDiceNaslednik(Dice& d) : PenaltyDice(d), BonusDice(d) { }

unsigned DoubleDiceNaslednik::roll() {
    return PenaltyDice::roll() + BonusDice::roll();
}

DoubleDice::DoubleDice(Dice& d) : bonus(d), penalty(d) { }

unsigned DoubleDice::roll() {
    return bonus.roll() + penalty.roll();
}

double expected_value(General_Dice& tdp, unsigned number_of_rolls) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += tdp.roll();
    return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

double value_probability(unsigned value, General_Dice& d, unsigned number_of_rolls) {
    unsigned counter = 0;
    for (int i = 0; i < number_of_rolls; i++) {
        if (d.roll() == value) {
            counter++;
        }
    }
    return static_cast<double>(counter) / static_cast<double>(number_of_rolls);
}
