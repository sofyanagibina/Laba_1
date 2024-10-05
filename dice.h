
#ifndef DICE_H
#define DICE_H

#include <random>
#include <algorithm>

class General_Dice {
public:
    virtual unsigned roll();
};

class Dice : public General_Dice {
public:
    Dice(unsigned max, unsigned seed);

    unsigned roll() override;

private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};

class ThreeDicePool : public General_Dice {
public:
    ThreeDicePool(unsigned max, unsigned seed_1, unsigned seed_2, unsigned seed_3);
    
    unsigned roll() override;

private:
    Dice d1, d2, d3;
};

class PenaltyDice : public General_Dice {
public:
    PenaltyDice(General_Dice& object);
    
    unsigned roll() override;

private:
    General_Dice& object;
};

class BonusDice : public General_Dice {
public:
    BonusDice(General_Dice& object);
    
    unsigned roll() override;

private:
    General_Dice& object;
};

class DoubleDiceNaslednik : public BonusDice, public PenaltyDice {
public:
    DoubleDiceNaslednik(Dice& d);
    
    unsigned roll() override;
};

class DoubleDice : public General_Dice {
public:
    DoubleDice(Dice& d);
    
    unsigned roll() override;

private:
    BonusDice bonus;
    PenaltyDice penalty;
};

double expected_value(General_Dice& tdp, unsigned number_of_rolls = 1);
double value_probability(unsigned value, General_Dice& d, unsigned number_of_rolls = 1);

#endif 
