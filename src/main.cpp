#include <QtGui/QApplication>
#include <iostream>
#include <ostream>
#include <limits>
#include <iomanip>

//#include <streambuf>

#include "domino_problem.h"
//#include "simple_container.h"
#include "mainwindow.h"
//#include "simple_tree.h"

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

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
/*
#ifdef DEBUG
   //elem_raw<int>::test();
   //simple_tree<int,size_t> x;
   //elem_raw<int,size_t,342> e(256,nullptr);
   //simple_tree<int,size_t>::test();
   //e2.subCount();
#endif // DEBUG

   //all_chars(std::cout);
   std::cout << "Vanishing Domino Problem" << std::endl;

   raw_domino_problem pr;// = read_xml("problem1.xml");
#ifdef DEBUG
   std::cout << pr << std::endl;
#endif // DEBUG

   domino_problem prob(pr.width, pr.height, pr.elements);

#ifdef DEBUG
   prob.demo_solution();
#endif // DEBUG


   simple_list<domino_problem::elem_list,size_t> solutions = prob.all_best_soluions();

   size_t index = 0;
   for(simple_list<domino_problem::elem_list,size_t>::elem_const i = solutions.first(); i; ++i)
   {
      std::cout << index++ << i.value_ref_const() << std::endl;
   }
*/
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



   MainWindow w;
   w.show();

   return app.exec();
}

