#pragma once
#pragma comment(lib, "psapi.lib")
#ifndef TOOLKIT_PROGRAM_INFO_H
#define TOOLKIT_PROGRAM_INFO_H

//#include "MBdev_QtTools_global.h"
//#include <string>
#include <ostream>

#ifdef WINDOWS
#define NOMINMAX // to avoid min/max macro problems
#include <windows.h> //to get avalilable and used memory, auxiliary include
#include "psapi.h" //to get avalilable and used memory
#endif //WINDOWS

namespace toolkit
{

class /*MBDEV_QTTOOLSSHARED_EXPORT*/ program_info
{
public:
   class /*MBDEV_QTTOOLSSHARED_EXPORT*/ program_environment
   {
   private:
      static const size_t bits = 8;
   public:
      class /*MBDEV_QTTOOLSSHARED_EXPORT*/ variable_sizes
      {
      public:
         static const size_t bits_short = sizeof(short) * bits;
         static const size_t bits_int = sizeof(int) * bits;
         static const size_t bits_long = sizeof(long) * bits;
         static const size_t bits_ptr = sizeof(ptrdiff_t) * bits;
         static const size_t bits_long_long = sizeof(long long) * bits;
         // return type of sizeof
         static const size_t bits_size_t = sizeof(size_t) * bits;
      public:
         static bool equals(size_t bits_short, size_t bits_int, size_t bits_long,
                            size_t bits_ptr, size_t bits_long_long);
      };
   public:
      static const variable_sizes sizes;
   public:
      static const char* c_str();
      friend std::ostream& operator<<(std::ostream& os, const program_environment& env);
   };

public:
   const program_environment environment;
public:
   program_info()
      : environment(program_environment()) { }
public:
   static unsigned long long physical_memory_used();
   static unsigned long long physical_memory_available();
   static unsigned long long virtual_memory_used();
   static unsigned long long virtual_memory_available();
   friend std::ostream& operator<<(std::ostream& os, const program_info& info);
};

}

#endif // TOOLKIT_PROGRAM_INFO_H
