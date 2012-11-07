#ifndef PROGRAM_ARGS_H
#define PROGRAM_ARGS_H

#include "simple_list.h"
#include <string>
#include <cstdlib>

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
   bool popAll() { if(args.length() == 0) return false; args.clear(); return true; }
   long long pop_number(const arg_t& command);
   size_t count() { return args.length(); }
   arg_t first() { if(args.first()) return *(args.first()); else return std::string(); }
   arg_t last() { if(args.last()) return *(args.last()); else return std::string(); }
   arg_t element(size_t index) { if(args.element(index)) return *(args.element(index)); else return std::string(); }
};

#endif // PROGRAM_ARGS_H
