#ifndef PROGRAM_ARGS_H
#define PROGRAM_ARGS_H

#include "simple_list.h"
#include <string>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

class program_args
{
   typedef std::string arg_t;
   typedef simple_list<arg_t,size_t> args_t;
private:
   int argc;
   char** argv;
   args_t args;
public:
   program_args(int argc, char **argv);
   void restore();
   bool pop(const arg_t& command);
   arg_t first() { return *(args.first()); }
   arg_t last() { return *(args.last()); }
   arg_t element(size_t index) { return *(args.element(index)); }
};

#endif // PROGRAM_ARGS_H
