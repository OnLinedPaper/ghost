#include "engine.h"
int main(int argc, char **argv) {
  if(argc || argv) { /*squash compiler warning*/ }

  engine e;
  e.play();

  return 0;
}
