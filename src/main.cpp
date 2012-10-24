


// In order to not launch GUI, but only test algorithms in the console,
// uncomment the following define.
//
// In order to run GUI, comment it.
#define ALGORITHM_TESTING


#ifndef ALGORITHM_TESTING

#include <QtGui/QApplication>
#include "mainwindow.h"

#else // ALGORITHM_TESTING

#include <iostream>
#include "domino_problem_input.h"
#include "domino_problem.h"
#include "domino_problem_solver.h"
#include "simple_list.h"

#endif // ALGORITHM_TESTING

// needed by all_chars()
#include <ostream>
#include <limits>
#include <iomanip>

void all_chars(std::ostream& s);

int main(int argc, char **argv)
{
   //all_chars(std::cout);

#ifndef ALGORITHM_TESTING

   QApplication app(argc, argv);
   MainWindow w;
   w.show();
   return app.exec();

#else // ALGORITHM_TESTING

   std::cout << "Vanishing Domino Problem" << std::endl;
   domino_problem_input input("problem2.txt");
#ifdef DEBUG
   std::cout << input << std::endl;
#else

#ifdef RELEASE
   std::cout << std::endl << "Example input:" << std::endl;
   std::cout << input.board_str() << std::endl << std::endl;
#endif // RELEASE
#endif // DEBUG

   domino_problem prob(input);
   std::cout << "Problem defined!" << std::endl;

   domino_problem_solver solver(prob);
   std::cout << "Solver initialized!" << std::endl;

   solver.execute();

   std::cout << "Building graph done!" << std::endl;
   simple_list<domino_problem,size_t> frst = solver.find_first_best_solution();
   std::cout << std::endl << "Best solution:" << std::endl;
   for(simple_list<domino_problem,size_t>::elem i = frst.first(); i; ++i)
      std::cout << i.value_ref().board_str() << std::endl << std::endl;

   //   simple_list<simple_list<domino_problem,size_t>,size_t> all = solver.find_all_best_solutions();
   //   std::cout << "List of all best solutions:" << std::endl;
   //   size_t count = 0;
   //   for(simple_list<simple_list<domino_problem,size_t>,size_t>::elem list = all.first();
   //       list; ++list, ++count)
   //      std::cout << std::endl << "Solution:" << std::endl << list << std::endl;

   return 0;

#endif // ALGORITHM_TESTING

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
