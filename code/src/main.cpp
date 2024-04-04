#include <iostream>
#include "lib/edge.hpp"
#include "lib/node.hpp"
#include "lib/account.hpp"
#include "lib/vehicle.hpp"
#include "lib/trafficCondition.hpp"


void testAllIncludes()
{
  std::cout << "Hello World!" << std::endl;
  printEdge();
  printNode();
  printAccount();
  printVehicle();
  printTrafficCondition();

}

int main(int argc, char *argv[]) {

  testAllIncludes();
  
  return 0;
}
