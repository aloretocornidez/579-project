#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include "CommonUtilities.h"
#include <random>

class Parser
{
public:
  typedef struct params_s
  {
    unsigned int startId;
    unsigned int goalId;
    CostType costType;
    unsigned int hr;
    unsigned int min;

  } Parameters;

  enum parseArgsRetVal
  {
    DEFAULT_ARGS,
    USER_ARGS,
    ERROR_ARGS,
    HELP_ARGS,

  };

  Parameters getParams() { return params_; }
  unsigned int parseArguments(int argc, char *argv[]);

  void printUsage();
  void PrintParams();

private:
  Parameters params_;
};

#endif // !PARSE_ARGS_H
