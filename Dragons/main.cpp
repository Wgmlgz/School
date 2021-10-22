#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>

#define COLOR(k) SetConsoleTextAttribute(hConsole, k)
#define RED COLOR(4)
#define WHITE COLOR(15)
#define GREEN COLOR(2)

class Attacker {
 public:
  void attack(Attacker* target);
  bool isAlive();

 protected:
  int health_ = 0;
  int attack_ = 0;
};

void Attacker::attack(Attacker* target) { target->health_ -= this->attack_; }

bool Attacker::isAlive() { return this->health_ >= 0; }

class Enemy : public Attacker {
 public:
  virtual bool isEnemy() { return true; }
};

class Hero : public Attacker {
 public:
  Hero(std::string name, int health, int attack);
  std::string name();
  int getAttack();
  void setAttack(int);

  int getExp();
  void setExp(int);

  std::string getName() { return name_; }

 private:
  int experience_ = 0;
  std::string name_ = "Habib";
};

Hero::Hero(std::string name, int health, int attack) {
  this->name_ = name;
  this->attack_ = attack;
  this->health_ = health;
}

int Hero::getAttack() { return this->attack_; }

void Hero::setAttack(int value) { this->attack_ = value; }

int Hero::getExp() { return this->experience_; }

void Hero::setExp(int value) { this->experience_ = value; }

int64_t xorshf96() {
  static unsigned long xx = 123456789, yy = 362436069, zz = 521288629;
  unsigned long t;
  xx ^= xx << 16;
  xx ^= xx >> 5;
  xx ^= xx << 1;

  t = xx;
  xx = yy;
  yy = zz;
  zz = t ^ xx ^ yy;

  return zz;
}

class Dragon : public Enemy {
 public:
  Dragon(std::string name, int health, int attack);
  bool checkAnswer(int answer);
  virtual std::string question() = 0;
  std::string getName() { return color_; }

 protected:
  void setAnswer(int answer);
  std::string color_;
  int answer_;
  std::string line_ = "";
};

Dragon::Dragon(std::string name, int health, int attack) {
  this->color_ = name;
  this->health_ = health;
  this->attack_ = attack;
}

bool Dragon::checkAnswer(int answer) { return answer_ == answer; }

void Dragon::setAnswer(int answer) { answer_ = answer; }

class RedDragon : public Dragon {
 public:
  virtual std::string question() override;
  RedDragon(std::string name, int health, int attack)
      : Dragon(name, health, attack) {}
};

std::string RedDragon::question() {
  int a1 = xorshf96() % 100, a2 = xorshf96() % 100;
  this->line_ =
      "What is " + std::to_string(a1) + " - " + std::to_string(a2) + "?\n\r";
  this->answer_ = a1 - a2;
  return this->line_;
}

class GreenDragon : public Dragon {
 public:
  virtual std::string question() override;
  GreenDragon(std::string name, int health, int attack)
      : Dragon(name, health, attack) {}
};

std::string GreenDragon::question() {
  int a1 = xorshf96() % 100, a2 = xorshf96() % 100;
  this->line_ =
      "What is " + std::to_string(a1) + " + " + std::to_string(a2) + "?\n\r";
  this->answer_ = a1 + a2;
  return this->line_;
}

class BlackDragon : public Dragon {
 public:
  virtual std::string question() override;
  BlackDragon(std::string name, int health, int attack)
      : Dragon(name, health, attack) {}
};

std::string BlackDragon::question() {
  int a1 = xorshf96() % 100, a2 = xorshf96() % 100;
  this->line_ =
      "What is " + std::to_string(a1) + " * " + std::to_string(a2) + "?\n\r";
  this->answer_ = a1 * a2;
  return this->line_;
}

class Troller : public Dragon {
 public:
  virtual std::string question() override;
  Troller(std::string name, int health, int attack)
      : Dragon(name, health, attack) {}
};

std::string Troller::question() {
  int a1 = xorshf96() % 100, a2 = xorshf96() % 100;
  this->line_ =
      "Welcome to the Troller casino! Guess random number 1..5\n\r";
  this->answer_ = xorshf96() % 5 + 1;
  return this->line_;
}

int main() {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  std::cout << "hi\n";

  Hero hero("Habib", 54, 10);
  std::vector<Dragon*> dragons = {new RedDragon("Red dragon", 10, 8),
                                  new Troller("Big Black Troller", 2, 5),
                                  new GreenDragon("Green dragon", 32, 10),
                                  new BlackDragon("Black dragon", 48, 12)};

  int exp = 2;
  for (auto i : dragons) {
    do {
      std::cout << i->getName() << " asks you: " << i->question();
      int ans;
      std::cin >> ans;
      if (i->checkAnswer(ans)) {
        GREEN;
        std::cout << "Cool!" << std::endl;
        hero.attack(i);
        hero.setExp(hero.getExp() + exp);
        WHITE;
      } else {
        RED;
        i->attack(&hero);
        if (hero.isAlive()) {
          std::cout << "You are damaged, but still alive" << std::endl;
          WHITE;
        } else {
          std::cout << "You died :(" << std::endl;
          WHITE;
          return 0;
        }
      }
    } while (i->isAlive());
    GREEN;
    std::cout << "You defeated the " + i->getName() +"!!!" << std::endl;
    exp += 5;
    WHITE;
  }
  GREEN;
  std::cout << "gg" << std::endl;
  WHITE;
}