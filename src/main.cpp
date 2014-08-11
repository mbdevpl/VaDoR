#include "toolkit/program_args.h"

#include <QApplication>
#include "mainwindow.h"

#include <iostream>
#include "domino_problem_input.h"
#include "domino_problem.h"
#include "domino_problem_solver.h"
#include "simple_list.h"

// needed by all_chars()
#include <ostream>
#include <limits>
#include <iomanip>

//#include "binary_hash.h"

int main_cmd(toolkit::program_args& args);
void all_chars(std::ostream& s);

/*!
  \brief main function

  \param argc number of command line options
  \param argv command line options, use "-help" option
   to see a list of all available options
  */
int main(int argc, char **argv)
{
   //all_chars(std::cout);

   toolkit::program_args args(argc, argv);

   if(args.count() == 1)
      std::cout << "use \"vador.exe -help\" to display list of available command line options";

   if(args.pop("-help"))
   {
      std::cout << "output/gui related options:" << std::endl;

      std::cout
            << " -help : displays this text, all other options are ignored" << std::endl
            << "     if this option is present" << std::endl
            << " -cmd : runs application in text mode, all below options are ignored" << std::endl
            << "     if this option is not present" << std::endl
            << " -fullsol : prints all boards on the way to the final solution, instead of" << std::endl
            << "     only ordered sequence of removed pieces and the final board" << std::endl
            << " -delay=# : # should be a positive number, means that report is printed" << std::endl
            << "     each # scanned states" << std::endl
            << " ##.xml : solves the problem defined in path ##, given in XML format" << std::endl
            << " ##.txt : solves the problem defined in path ##, given in a properly formatted" << std::endl
            << "     text file" << std::endl
            << std::endl;

      std::cout << "algorithm related options:" << std::endl;

      std::cout
            << " -approx : it launches approximate algorithm by Mateusz Bysiek" << std::endl
            << "     (all below options related to the algorithm are ignored if this option is present)" << std::endl
               //<< " -approx=# :  all currently not working; in the future it will launch approximate" << std::endl
               //<< "     algorithm number #" << std::endl
            << " -depthfirst : depth-first search done through left branch first" << std::endl
            << " -depthlast : depth-first search done through right branch first (option is ignored" << std::endl
            << "     if combined with -depthfirst)" << std::endl
            << " -depthsort : depth-first search done through most promising branch first," << std::endl
            << "     i.e. the branches with most possibilities are explored first" << std::endl
            << "     (option is ignored if combined with any of above 2 options)" << std::endl
            << " -purge : tree of scanned states is cleared from useless states, i.e. the branches" << std::endl
            << "     that are fully explored and do not contain current best solution are removed" << std::endl
            << "     (option is ignored if search is not depth-first i.e. if neither -depthfirst" << std::endl
            << "     nor -depthlast nor -depthsort are set)" << std::endl
            << std::endl;

      std::cout << "command line options used for testing:" << std::endl;

      std::cout
            << " -nocout : no text output is generated (nonsense unless you inspect variables)" << std::endl
            << " -testmem : fills most of the memory to test behaviour in case of low memory" << std::endl
            << " -testloop : launches solver for given parameters in endless loop" << std::endl
            << std::endl;

      std::cout << "examples of sets of command line options (order does not matter):" << std::endl;

      std::cout
            << " vador.exe -delay=100000 -depthsort -purge file.xml : typical command used to solve" << std::endl
            << "     the most difficult problems" << std::endl
            << " vador.exe -delay=500 -approx file.xml : typical command used to solve" << std::endl
            << "     the any problem using approximate algorithm" << std::endl
            << " vador.exe file.xml : solves the problem using breadth-first search," << std::endl
            << "     no defined delay is the same as -delay=100000" << std::endl
            << std::endl;

      return 0;
   }

   if(args.pop("-cmd"))
   {
      try
      {
         if(args.pop("-testmem"))
         {
            long long* ptr;
            for(int i=0;i<1600;++i) ptr = new long long[131072]; // 131072 * 64 bits = 1MB
            ptr = nullptr;
         }
         if(args.pop("-testloop"))
            while(true)
               main_cmd(args);
         else
            main_cmd(args);
      }
      catch(std::runtime_error& er)
      {
         std::cout << er.what() << std::endl;
      }
   }
   else
   {
      args.popAll();
      QApplication app(argc, argv);
      MainWindow w;
      w.show();
      return app.exec();
   }

   return 0;
}

int main_cmd(toolkit::program_args& args)
{
   static bool output = !args.pop("-nocout");
   static bool fullsol = args.pop("-fullsol");
   static bool approx = args.pop("-approx");
   static bool purge = approx ? false : args.pop("-purge");
   static bool depthfirst = approx ? false : args.pop("-depthfirst");
   static bool depthlast = (depthfirst || approx) ? false : args.pop("-depthlast");
   static bool depthsort = (depthfirst || depthlast || approx) ? false : args.pop("-depthsort");
   static long long delay = args.pop_number("-delay");
   if(delay <= 0)
      delay = 100000;
   if(output)
      std::cout << "Vanishing Domino Problem" << std::endl;
   domino_problem_input input(args.last());

   if(output)
   {
      std::cout << std::endl << "Input:" << std::endl;
      std::cout << input.board_str() << std::endl << std::endl;

      std::cout << " pieces that cannot be removed: " << input.invalid_length() << std::endl;
      if(input.invalid_length())
         std::cout /*<< ""*/ << input.invalid_str() << std::endl;
      std::cout << " pieces that can be removed: " << input.checked_length() << std::endl;
      if(input.invalid_length())
         std::cout /*<< ""*/ << input.checked_str() << std::endl;
   }

   domino_problem prob(input);
   if(output)
   {
      std::cout << "Problem defined!" << std::endl;
   }

   domino_problem_solver solver(prob);

   if(output)
      std::cout << "Solver initialized!" << std::endl;

   size_t mode = depthfirst ? DEPTH_FIRST : ( depthlast ? DEPTH_LAST : (depthsort ? DEPTH_SORT : BREADTH) );

   solver.execute(output, delay, approx, mode, purge);

   if(output)
      std::cout << "Building graph done!" << std::endl;
   domino_problem::solution_t frst = solver.find_first_best_solution();
   if(output)
   {
      std::cout << std::endl << "Best solution:" << std::endl;
      if(fullsol)
         for(domino_problem::solution_t::elem i = frst.first(); i; ++i)
            std::cout << (*i).board_str() << std::endl << std::endl;
      else
      {
         domino_problem::solution_t::elem i = frst.last();
         std::cout << (*i).removed_str() << std::endl
                   << (*i).board_str() << std::endl << std::endl;
      }
   }

   //   simple_list<simple_list<domino_problem,size_t>,size_t> all = solver.find_all_best_solutions();
   //   std::cout << "List of all best solutions:" << std::endl;
   //   size_t count = 0;
   //   for(simple_list<simple_list<domino_problem,size_t>,size_t>::elem list = all.first();
   //       list; ++list, ++count)
   //      std::cout << std::endl << "Solution:" << std::endl << list << std::endl;

   return 0;
}

void all_chars(std::ostream& s)
{
   char min = std::numeric_limits<char>::min();
   char max = std::numeric_limits<char>::max();
   int i = +min;
   size_t n = 0;
   for(char ch = min; true; ++ch, ++i, ++n)
   {
      if(n != 0 && n%9 != 0)
         s << "  ";
      s << std::setw(4) << std::setfill(' ') << i << ": " << ch;
      if((n+1)%9 == 0 || ch == max)
         s << "\n";
      if(ch == max)
         break;
   }
}
