#include <string>
#include <iostream>

#include "Hello.hpp"

int main(int ac, char *av[]) {
  Hello hell;
  std::cout << hell.yo() << " World" << std::endl;
  return 0;
}
