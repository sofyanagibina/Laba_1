#include <random>
#include <iostream>


class General_Dice {
public:
  virtual unsigned roll() {
    return 10;
  }
};


 class Dice: public General_Dice {
 public:
   Dice(unsigned max, unsigned seed) :
     max(max), dstr(1, max), reng(seed) { }

     unsigned roll() override {
     return dstr(reng);
     
  }

 private:
   unsigned max;
   std::uniform_int_distribution<unsigned> dstr;
   std::default_random_engine reng;

};

 class ThreeDicePool : public General_Dice {
 public:
   ThreeDicePool(unsigned max,
     unsigned seed_1, unsigned seed_2, unsigned seed_3) :
     d1(max, seed_1), d2(max, seed_2), d3(max, seed_3) { }

     unsigned roll() override {
       return d1.roll() + d2.roll() + d3.roll();

  }

 private:
   Dice d1, d2, d3;

 };

 class PenaltyDice : public General_Dice {
 public:
   PenaltyDice(General_Dice& object) : object(object) { }
   unsigned roll() override {
     unsigned roll1 = object.roll();
     unsigned roll2 = object.roll();
     return std::min(roll1, roll2);
   }

 private:
   General_Dice &object;
 };

 class BonusDice : public General_Dice {
 public:
   BonusDice(General_Dice& object) : object(object) { }
   unsigned roll() override {
     unsigned roll1 = object.roll();
     unsigned roll2 = object.roll();
     return std::max(roll1, roll2);
   }

 private:
   General_Dice &object;
 };

 class DoubleDiceNaslednik : public BonusDice, public PenaltyDice {
 public:
   DoubleDiceNaslednik(Dice& d) : PenaltyDice(d), BonusDice(d) { }

   unsigned roll() override {
     return PenaltyDice::roll() + BonusDice::roll();
   }
 };

 class DoubleDice : public General_Dice {
 public:
   DoubleDice(Dice& d) : bonus(d), penalty(d) { }
   unsigned roll() override {
     return bonus.roll() + penalty.roll();
   }
 private:
   BonusDice bonus;
   PenaltyDice penalty;
 };

 double expected_value(General_Dice& tdp, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
      accum += tdp.roll();
    return
      static_cast<double>(accum) / static_cast<double>(number_of_rolls);
 }

 double value_probability(unsigned value, General_Dice& d, unsigned number_of_rolls = 1) {
   unsigned counter = 0;
   for (int i = 0; i < number_of_rolls; i++) {
     if (d.roll() == value) {
       counter++;
     }
   }
   return static_cast<double>(counter) / static_cast<double>(number_of_rolls);
 }

 int main() {
   Dice object(100, 511);
   DoubleDice d(object);
   for (int i = 1; i < 201; i++) {
     std::cout << i << ' ' << value_probability(i, d, 1000) << ' ';
   }
 }


