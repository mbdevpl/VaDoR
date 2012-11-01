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

int main(int argc, char **argv)
{
   //all_chars(std::cout);
   //simple_list<int,size_t>::test();

   program_args args(argc, argv);

   if(args.pop("-cmd"))
   {
      //for(int i=0;i<930;++i) long long* a = new long long[262144];
      //while(true)
      main_cmd(args);
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
   if(output)
      std::cout << "Vanishing Domino Problem" << std::endl;
   domino_problem_input input(args.last());

   if(output)
   {
      std::cout << std::endl << "Input:" << std::endl;
      std::cout << input.board_str() << std::endl << std::endl;
   }

   domino_problem prob(input);
   if(output)
      std::cout << "Problem defined!" << std::endl;

   domino_problem_solver solver(prob);

   if(output)
      std::cout << "Solver initialized!" << std::endl;

   solver.execute(output);

   if(output)
      std::cout << "Building graph done!" << std::endl;
   domino_problem::solution_t frst = solver.find_first_best_solution();
   if(output)
   {
      std::cout << std::endl << "Best solution:" << std::endl;
      for(domino_problem::solution_t::elem i = frst.first(); i; ++i)
         std::cout << i.value_ref().board_str() << std::endl << std::endl;
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
