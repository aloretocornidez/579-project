#include "parseArgs.h"
#include "CommonUtilities.h"
#include <cstdlib>
#include <iostream>
#include <string>

unsigned int Parser::parseArguments(int argc, char *argv[])
{
  // Print Help
  // Default Parameters.
  if (argc == 1)
  {
    std::cout << "Using Default Parameters." << std::endl;
    params_.goalId = 52;
    params_.startId = 41;
    params_.costType = CostType::DISTANCE;
    params_.hr = 11;
    params_.min = 50.0;

    return parseArgsRetVal::DEFAULT_ARGS;
  }
  else if (std::string(argv[1]) == "-h")
  {
    return parseArgsRetVal::HELP_ARGS;
  }

  // User Entered Arguments
  else if (argc != 5)
  {

    // Parse Goal IDs.
    unsigned int temp1 = atoi(argv[1]);
    unsigned int temp2 = atoi(argv[2]);
    params_.startId = (temp1 < temp2) ? (temp1) : (temp2);
    params_.goalId = (temp1 < temp2) ? (temp2) : (temp1);

    params_.hr = atoi(argv[4]);
    params_.min = atoi(argv[5]);

    // Parsing the string for the Cost Time
    std::string arg = argv[3];
    if (arg == "DISTANCE")
    {
      params_.costType = CostType::DISTANCE;
    }
    else if (arg == "TIME_WITH_TRAFFIC")
    {
      params_.costType = CostType::TIME_WITH_TRAFFIC;
    }
    else if (arg == "TIME_NO_TRAFFIC")
    {
      params_.costType = CostType::TIME_NO_TRAFFIC;
    }
    else
    {
      std::cout << "Invalid Travel Method Entered: " << arg << std::endl;
      return parseArgsRetVal::ERROR_ARGS;
    }
  }

  return parseArgsRetVal::USER_ARGS;
}

void Parser::PrintParams()
{
  std::cout << "Parameters:" << std::endl;

  std::cout << "{startId}: " << params_.startId << std::endl;
  std::cout << "{goalId}: " << params_.goalId << std::endl;

  std::string arg;

  if (params_.costType == CostType::DISTANCE)
  {
    arg = "DISTANCE";
  }
  else if (params_.costType == CostType::TIME_WITH_TRAFFIC)
  {
    arg = "TIME_WITH_TRAFFIC";
  }
  else if (params_.costType == CostType::TIME_NO_TRAFFIC)
  {
    arg = "TIME_NO_TRAFFIC";
  }
  else
  {
    std::cout << "Invalid Travel Method Entered: " << arg << std::endl;
    exit(0);
  }

  std::cout << "{Cost Type}: " << arg << std::endl;

  std::cout << "{hour}: " << params_.hr << std::endl;
  std::cout << "{minute}: " << params_.min << std::endl;
  std::cout << std::endl;
}

// Parse arguements
void Parser::printUsage()
{
  // Printing Usage
  std::cout << std::endl;
  std::cout << "Usage: {program} [-h] {startId} {goalId} {DISTANCE|TIME_NO_TRAFFIC|TIME_WITH_TRAFFIC} {hour} {minute}" << std::endl;

  // Printing out option descriptions.
  std::cout << "{program}: Program binary name" << std::endl;
  std::cout << "{startId}: Id of the node on the map that you want to start on. Entered as an unsigned integer." << std::endl;
  std::cout << "{goalId}:  Id of the node on the map that you want to end on. Entered as an unsigned integer." << std::endl;
  std::cout << "{DISTANCE|TIME_NO_TRAFFIC|TIME_WITH_TRAFFIC}: Entered as the name of the parameter." << std::endl;
  std::cout << "{hour}: Entered as an unsigned integer." << std::endl;
  std::cout << "{minute}: Entered as an unsigned integer." << std::endl;
  std::cout << std::endl;
}
