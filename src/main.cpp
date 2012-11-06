#include "program_args.h"

#include <QtGui/QApplication>
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

#include "binary_hash.h"

int main_cmd(program_args& args);
void all_chars(std::ostream& s);

/*!
  \brief main function

  \param argc number of command line options
  \param argv command line options

  output/gui related options:

  -cmd : runs application in text mode
  -nocout : no text output is generated (nonsense when combined with -cmd)
  -fullsol : prints all boards of the final solution, instead of ordered sequence of removed
     pieces and the final board

  algorithm related options:

  -approx=# : currently not working; in the future it will launch approximate algorithm number #
  -depthfirst : depth-first search done through left branch first
  -depthlast : depth-first search done through right branch first (option is ignored
     if combined with -depthfirst or -approx)
  -depthgreedy : depth-first search done through most promising branch first, i.e. the branches with most
     possibilities are explored first
     (option is ignored if combined with any of above 3 options)
  -purge : tree of (ignored if search is not depth-first i.e. if neither -depthfirst
     nor -depthlast nor -depthgreedy are set)
  -delay=### : ### should be a positive number, means that report is printed each ### scanned states

  command line options used for testing:

  -testmem
  -testloop
  */
int main(int argc, char **argv)
{
   //all_chars(std::cout);
   //simple_list<int,size_t>::test();

   program_args args(argc, argv);

   if(args.pop("-cmd"))
   {
      try
      {
      if(args.pop("-testmem"))
      {
         long long* ptr;
         for(int i=0;i<930;++i) ptr = new long long[262144];
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
      std::cout << "execute \"vador.exe -cmd file.xml\" to solve file.xml in text mode "
                << "(also accepts *.txt)" << std::endl;
      QApplication app(argc, argv);
      MainWindow w;
      w.show();
      return app.exec();
   }

   return 0;
}

int main_cmd(program_args& args)
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
