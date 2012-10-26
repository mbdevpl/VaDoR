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
   args_t::elem e = args.find(command);
   if(e.valid())
   {
      e.remove();
      return true;
   }
   return false;
}
