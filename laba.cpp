#include <fstream>
#include <random>
#include <iostream>
#include <ctime>
 
// Абстрактный класс, общий для всех видов кубиков
class Dice
{
public:
  virtual unsigned roll() = 0;
 
  virtual unsigned min()
  {
    return _min;
  };
 
  virtual unsigned max()
  {
    return _max;
  }
 
  Dice(unsigned min, unsigned max) :
    _min(min),
    _max(max)
  {
  }
 
private:
  unsigned _max;
  unsigned _min;
};
 
double expected_value(Dice &d, unsigned number_of_rolls = 1) {
  auto accum = 0llu;
  for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
    accum += d.roll();
  return static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}
 
// Простой кубик
class SimpleDice : public Dice
{
public:
  SimpleDice(unsigned max, unsigned seed) :
    Dice(1, max),
    dstr(1, max), 
    reng(seed)
  {}
 
  virtual unsigned roll() { return dstr(reng); }
 
private:
  std::uniform_int_distribution<unsigned> dstr;
  std::default_random_engine reng;
};
 
class ThreeDicePool : public Dice
{
public:
  ThreeDicePool(unsigned max, unsigned seed_1, unsigned seed_2, unsigned seed_3):
    Dice(3, max * 3), // три кубика не могут дать меньше 3 и больше 18 в сумме 
    dice_1(max, seed_1),
    dice_2(max, seed_2),
    dice_3(max, seed_3)
  {}
 
  virtual unsigned roll() 
  { 
    return dice_1.roll() + dice_2.roll() + dice_3.roll(); 
  }
 
private:
  SimpleDice dice_1, dice_2, dice_3;
};
 
class PenaltyDice : virtual public Dice
{
public:
  PenaltyDice(Dice& dice):
    Dice(dice.min(), dice.max()),
    dice(dice)
  {}
 
  virtual unsigned roll() 
  { 
    std::cout << "min\n";
    return std::min(dice.roll(), dice.roll());
  }
 
private:
  Dice& dice;
};
 
class BonusDice : virtual public Dice
{
public:
  BonusDice(Dice& dice):
    Dice(dice.min(), dice.max()),
    dice(dice)
  {}
 
  virtual unsigned roll() 
  { 
    std::cout << "max\n";
    return std::max(dice.roll(), dice.roll());
  }
 
private:
  Dice& dice;
};
 
class DoubleDice : virtual public BonusDice, virtual public PenaltyDice
{
public:
  DoubleDice(Dice& dice):
    Dice(dice.min(), dice.max()),
    BonusDice(dice),
    PenaltyDice(dice)
  {}
 
  virtual unsigned roll()
  {
    counter++;
    if (counter % 2 == 0)
      return dynamic_cast<BonusDice*>(this)->roll();
    else
      return dynamic_cast<PenaltyDice*>(this)->roll();
  }
 
private:
  unsigned counter = 0;
};
 
double value_probability(unsigned value, Dice &d, unsigned number_of_rolls = 1) {
  unsigned number_of_occurences = 0;
 
  for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
  {
    auto rolled = d.roll();
    if (rolled == value)
      number_of_occurences++;
  }
 
  return static_cast<double>(number_of_occurences) / static_cast<double>(number_of_rolls);
}

 
void part_1_demonstration()
{
  std::cout << "Part 1.\n";
  SimpleDice simple_dice(6, time(NULL));
  ThreeDicePool three_dice_pool(6, time(NULL) + 1, time(NULL) + 2, time(NULL) + 3);
 
  std::cout << "E(simple_dice) = " << expected_value(simple_dice, 100000) << "\n";
  std::cout << "E(three_dice_pool) = " << expected_value(three_dice_pool, 100000) << "\n";
}
 
void calculate_probabilities_csv(Dice& d, const char* filename, unsigned number_of_rolls = 1)
{
  std::ofstream fout(filename);
 
  fout << "value,probability" << "\n";
 
  for (unsigned value = d.min(); value <= d.max(); value++)
  {
    fout << value << "," << value_probability(value, d, number_of_rolls) << "\n";
  }
}
 
void part_2_demonstration()
{
  std::cout << "Part 2.\n";
  SimpleDice simple_dice(100, time(NULL));
  ThreeDicePool three_dice_pool(6, time(NULL) + 1, time(NULL) + 2, time(NULL) + 3);
 
  calculate_probabilities_csv(simple_dice, "simple_dice.csv", 1000);
  calculate_probabilities_csv(three_dice_pool, "three_dice_pool.csv", 1000);
}
 
void part_3_demonstration()
{
  std::cout << "Part 3.\n";
  SimpleDice simple_dice(100, time(NULL));
  DoubleDice double_dice(simple_dice);
 
  std::cout << "E(simple_dice) = " << expected_value(simple_dice, 1) << "\n";
  std::cout << "E(penalty_dice) = " << expected_value(dynamic_cast<PenaltyDice&>(double_dice), 1) << "\n";
  std::cout << "E(bonus_dice) = " << expected_value(dynamic_cast<BonusDice&>(double_dice), 1) << "\n";
  std::cout << "E(double_dice) = " << expected_value(double_dice, 1) << "\n";
}
 
int main()
{
  part_1_demonstration();
  part_2_demonstration();
  part_3_demonstration();
}

