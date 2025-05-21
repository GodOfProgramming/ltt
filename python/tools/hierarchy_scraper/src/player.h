#include "actor.h"
#include <iostream>

class Player : AActor {
public:
  auto Update() -> void override { std::cout << "Player Update\n"; }
};
