#include <autotools_mvp/uuid.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
  com::autotools_mvp::UUID uuid;
  std::cout << uuid << std::endl;
  return EXIT_SUCCESS;
}
