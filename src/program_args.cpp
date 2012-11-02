#include "program_args.h"

program_args::program_args(int argc, char **argv)
   : argc(argc), argv(argv), args()
{
#ifdef DEBUG
   std::cout << "command line arguments:\n";
   if(argc > 0) {
      for(int i = 0; i < argc; i++)
         std::cout << "  arg" << i << "=" << argv[i] << "\n";
   } else
      std::cout << "  none\n";
   std::cout.flush();
#endif
   restore();
}

void program_args::restore()
{
   args.clear();
   for(int i = 0; i < argc; i++) {
      std::string arg(argv[i]);
      args.append(arg);
   }
}

bool program_args::pop(const arg_t& command)
{
   if(command.empty())
      return false;
   args_t::elem e = args.find(command);
   if(e.valid())
   {
      e.remove();
      return true;
   }
   return false;
}

long long program_args::pop_number(const arg_t& command)
{
   if(command.empty())
      return false;
   args_t::elem e = args.first();
   while(e)
   {
      std::string& s = *e;
      if(s.length() > command.length()+1 && !s.substr(0,command.length()).compare(command))
      {
         std::string val = s.substr(command.length()+1);
         e.remove();
         return std::stoll(val);
      }
      ++e;
   }
   return 0;
}
