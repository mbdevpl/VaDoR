#include "program_timer.h"

program_timer::program_timer(bool local)
   : local(local), running(false), start_time(), stop_time() { }

void program_timer::start()
{
#ifdef WINDOWS
   if(local) GetLocalTime(&start_time);
   else GetSystemTime(&start_time);
#endif // WINDOWS
   running = true;
}

void program_timer::stop()
{
#ifdef WINDOWS
   if(local) GetLocalTime(&stop_time);
   else GetSystemTime(&stop_time);
#endif // WINDOWS
   running = false;
}

std::string program_timer::str(const std::string& format)
{
   std::stringstream s;
   s << "start=";
#ifdef WINDOWS
   s << systemtime_str(format, start_time);
#endif // WINDOWS
   if(!running)
   {
      s << " stop=";
#ifdef WINDOWS
      s << systemtime_str(format, stop_time);
#endif // WINDOWS
      s << " diff=";
#ifdef WINDOWS
      s << delta(start_time, stop_time) / 10000000 << "s";
#endif // WINDOWS
   }
   return s.str();
}

#ifdef WINDOWS

_int64 program_timer::delta(const SYSTEMTIME& t1, const SYSTEMTIME& t2)
{
   union timeunion
   {
      FILETIME fileTime;
      ULARGE_INTEGER ul;
   };

   timeunion ft1;
   timeunion ft2;

   SystemTimeToFileTime(&t1, &ft1.fileTime);
   SystemTimeToFileTime(&t2, &ft2.fileTime);

   return ft2.ul.QuadPart - ft1.ul.QuadPart;
}

std::string program_timer::systemtime_str(const std::string& format, const SYSTEMTIME& t)
{
   static std::string commands("YMDhms"); // special characters
   std::stringstream s;
   for(unsigned int i = 0; i < format.length(); ++i)
   {
      char c = format.at(i);
      if(commands.find_first_of(c) == std::string::npos)
      {
         s << format.substr(i,1);
         continue;
      }
      switch(c)
      {
      case 'Y': s << std::setw(4) << std::setfill('0') << t.wYear; break;
      case 'M': s << std::setw(2) << std::setfill('0') << t.wMonth; break;
      case 'D': s << std::setw(2) << std::setfill('0') << t.wDay; break;
      case 'h': s << std::setw(2) << std::setfill('0') << t.wHour; break;
      case 'm': s << std::setw(2) << std::setfill('0') << t.wMinute; break;
      case 's': s << std::setw(2) << std::setfill('0') << t.wSecond; break;
      }
   }
   return s.str();
}

#endif // WINDOWS
