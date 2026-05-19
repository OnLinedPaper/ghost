#include <iostream>
#include <string>
#include "engine.h"
int main(int argc, char **argv) {
  if(argc || argv) { /*squash compiler warning*/ }

  std::cout << "hello, world! (type \"x\" and press enter to attempt to load SDL)" << std::endl;
  char wait;
  std::cin >> wait;
  
  engine e;
  e.play();

  return 0;
}
