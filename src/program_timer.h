#ifndef PROGRAM_TIMER_H
#define PROGRAM_TIMER_H

#include <string>
#include <sstream>
#include <iomanip>
#ifdef WINDOWS
#define NOMINMAX // to avoid min/max macro problems
#include "windows.h"
#endif // WINDOWS

class program_timer
{
private:
   bool local;
   bool running;
#ifdef WINDOWS
   SYSTEMTIME start_time;
   SYSTEMTIME stop_time;
#endif // WINDOWS
public:
   program_timer(bool local = true);
   void start();
   void stop();
   std::string str(const std::string& format = std::string("h:m:s"));
private:
#ifdef WINDOWS
   static _int64 delta(const SYSTEMTIME& t1, const SYSTEMTIME& t2);
   static std::string systemtime_str(const std::string& format, const SYSTEMTIME& t);
#endif // WINDOWS
};

#endif // PROGRAM_TIMER_H
