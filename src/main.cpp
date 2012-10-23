#include <QtGui/QApplication>
#include <iostream>
#include <ostream>
#include <limits>
#include <iomanip>

#include "domino_problem_input.h"
#include "domino_problem.h"
#include "domino_problem_solver.h"
//#include "simple_container.h"
//#include "simple_tree.h"

#include "mainwindow.h"
void all_chars(std::ostream& s);

int main(int argc, char **argv)
{
   //all_chars(std::cout);

   QApplication app(argc, argv);
   //   MainWindow w;
   //   w.show();
   //   return app.exec();

   // Radziu, czy mozesz po prostu odkomentowac swoj kod, a to co ponizej
   // zostawic w spokoju? i tak jak jest "return" to nic dalej sie nie wykona
   //
   // z gory dzieki :)
   //
   std::cout << "Vanishing Domino Problem" << std::endl;
   domino_problem_input input("problem1.xml");
#ifdef DEBUG
   std::cout << input << std::endl;
#endif // DEBUG
   domino_problem prob(input);
   domino_problem_solver solver(prob);
   solver.execute();

   //#ifdef DEBUG
   //   prob.demo_solution();
   //#endif // DEBUG

   //   simple_list<domino_problem::elem_list,size_t> solutions = prob.all_best_soluions();

   //   size_t index = 0;
   //   for(simple_list<domino_problem::elem_list,size_t>::elem_const i = solutions.first(); i; ++i)
   //   {
   //      std::cout << index++ << i.value_ref_const() << std::endl;
   //   }

#ifdef DEBUG
   //elem_raw<int>::test();
   //simple_tree<int,size_t> x;
   //elem_raw<int,size_t,342> e(256,nullptr);
   //simple_tree<int,size_t>::test();
   //e2.subCount();
#endif // DEBUG

   //   domino_problem::elem_loc_list e;
   //   e.append(domino_elem_located(2, 1, true, 0, 0));
   //   e.append(domino_elem_located(3, 2, true, 0, 2));
   //   e.append(domino_elem_located(1, 0, false, 1, 0));
   //   e.append(domino_elem_located(3, 0, true, 1, 1));
   //   e.append(domino_elem_located(0, 0, false, 1, 3));
   //   e.append(domino_elem_located(1, 1, true, 2, 1));
   //   e.append(domino_elem_located(3, 3, false, 3, 0));
   //   e.append(domino_elem_located(0, 2, true, 3, 1));
   //   e.append(domino_elem_located(2, 2, false, 3, 3));
   //   e.append(domino_elem_located(4, 4, true, 4, 1));

   //size_t len = row1.length();

   //   domino_problem prob(5, 4, e);

   /*std::cout << *///prob.all_best_soluions();

   //std::cout << prob.str() << std::endl;
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
