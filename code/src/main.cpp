#include "lib/account.hpp"
#include "lib/edge.hpp"
#include "lib/node.hpp"
#include "lib/trafficCondition.hpp"
#include "lib/vehicle.hpp"
#include <iostream>

void testAllIncludes()
{
  std::cout << "Hello World!" << std::endl;
  printEdge();
  printNode();
  printAccount();
  printVehicle();
  printTrafficCondition();
}

int main(int argc, char *argv[])
{

  testAllIncludes();

  return 0;
}
