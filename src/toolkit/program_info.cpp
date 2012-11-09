#include "program_info.h"

namespace toolkit
{

unsigned long long program_info::physical_memory_used()
{
#ifdef WINDOWS
   PROCESS_MEMORY_COUNTERS pmc;
   //PROCESS_MEMORY_COUNTERS_EX pmc;
   GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
   SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
   return physMemUsedByMe;
#endif // WINDOWS
   return 0;
}

unsigned long long program_info::physical_memory_available()
{
   return 0;
}

unsigned long long program_info::virtual_memory_used()
{
#ifdef WINDOWS
   return 0;
//   PROCESS_MEMORY_COUNTERS pmc;
//   //PROCESS_MEMORY_COUNTERS_EX pmc; // cannot convert from PROCESS_MEMORY_COUNTERS_EX* to PROCESS_MEMORY_COUNTERS*
//   GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
//   SIZE_T virtualMemUsedByMe = pmc.PrivateUsage; // error: C2039: 'PrivateUsage' : is not a member of '_PROCESS_MEMORY_COUNTERS'
//   return virtualMemUsedByMe;
#endif // WINDOWS
   return 0;
}

unsigned long long program_info::virtual_memory_available()
{
   return 0;
}

std::ostream& operator<<(std::ostream& os, const program_info& info)
{
   os << info.environment.c_str();
   return os;
}

//// Methods of program_environment:

bool program_info::program_environment::variable_sizes::equals(
      size_t bits_short, size_t bits_int, size_t bits_long, size_t bits_ptr,
      size_t bits_long_long)
{
   if(variable_sizes::bits_short == bits_short
         && variable_sizes::bits_int == bits_int
         && variable_sizes::bits_long == bits_long
         && variable_sizes::bits_ptr == bits_ptr
         && variable_sizes::bits_long_long == bits_long_long)
      return true;
   return false;
}

const char* program_info::program_environment::c_str()
{
   //ILP32LL, ILP32LL64
   if(variable_sizes::equals(16,32,32,32,64))
      return "ILP32LL64";

   // LLP64, IL32LLP64
   if(variable_sizes::equals(16,32,32,64,64))
      return "IL32LLP64";

   // LP64, I32LP64
   if(variable_sizes::equals(16,32,32,64,64))
      return "I32LP64";

   // ILP64
   if(variable_sizes::equals(16,32,32,64,64))
      return "ILP64";

   // SILP64
   if(variable_sizes::equals(64,64,64,64,64))
      return "SILP64";

   //IP16,
   //IP16L32,
   //I16LP32,
   //ILP32,
   // and other unspecified platforms
   return "OTHER";
}

std::ostream& operator<<(std::ostream& os, const program_info::program_environment& env)
{
   return os << env.c_str();
}

}
