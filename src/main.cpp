#include <iostream>
#include <ostream>
#include <limits>
#include <iomanip>

#include <string>
#include <fstream>
#include <streambuf>

#include <rapidxml.hpp>

#include "domino_problem.h"
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

raw_domino_problem read_xml(const std::string& path)
{
   std::ifstream t(path);
   std::stringstream buffer;
   buffer << t.rdbuf();
   std::string s = buffer.str();
   char* text = (char*)s.c_str();

   rapidxml::xml_document<> doc;
   doc.parse<0>(text);

   raw_domino_problem results;
   results.width = 0;
   results.height = 0;

   std::string node_domino_board("domino_board");
   std::string attr_width("width");
   std::string attr_height("height");
   std::string node_domino_piece("piece");
   std::string attr_x("x");
   std::string attr_y("y");
   std::string attr_orientation("orientation");
   std::string attr_orientation_vertical("vertical");
   std::string attr_orientation_horizontal("horizontal");
   std::string attr_value1("value1");
   std::string attr_value2("value2");

   rapidxml::xml_node<> *node = doc.first_node();
   if(!node || node_domino_board.compare(node->name()))
      throw std::runtime_error("wrong XML tag: domino_board expected");
#ifdef DEBUG
   std::cout << node->name() << ": ";
#endif
   for (rapidxml::xml_attribute<> *attr = node->first_attribute();
        attr; attr = attr->next_attribute())
   {
      // reading attributes of main node
#ifdef DEBUG
      std::cout << "  " << attr->name() << "=" << attr->value();
#endif
      if(!attr_width.compare(attr->name()))
         results.width = std::atoi(attr->value());
      else if(!attr_height.compare(attr->name()))
         results.height = std::atoi(attr->value());
      else
         throw std::runtime_error("wrong XML attribute: width/height expected");
   }
#ifdef DEBUG
   std::cout << std::endl;
#endif
   if(results.width == 0 || results.height == 0)
      throw std::runtime_error("invalid XML data: width or height is zero");

   // reading domino pieces
   for( rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
   {
#ifdef DEBUG
      std::cout << "  " << n->name() << ": ";
#endif
      if(node_domino_piece.compare(n->name()))
         throw std::runtime_error("wrong XML tag: piece expected");

      size_t x = 0;
      size_t y = 0;
      bool is_vertical = false;
      domino_elem_value_t value1 = 0;
      domino_elem_value_t value2 = 0;
      for (rapidxml::xml_attribute<> *att = n->first_attribute();
           att; att = att->next_attribute())
      {
#ifdef DEBUG
         std::cout << "  " << att->name() << "=" << att->value();
#endif
         if(!attr_x.compare(att->name()))
            x = std::atoi(att->value());
         else if(!attr_y.compare(att->name()))
            y = std::atoi(att->value());
         else if(!attr_orientation.compare(att->name()))
         {
            if(!attr_orientation_vertical.compare(att->value()))
               is_vertical = true;
            else if(!attr_orientation_horizontal.compare(att->value()))
               is_vertical = false;
            else
               throw std::runtime_error("wrong XML attribute value: " \
                                        "vertical/horizontal expected");

         }
         else if(!attr_value1.compare(att->name()))
            value1 = std::atoi(att->value());
         else if(!attr_value2.compare(att->name()))
            value2 = std::atoi(att->value());
         else
            throw std::runtime_error("wrong XML attribute, expected one of these: x," \
                                     " y, orientation, value1, value2");
      }
#ifdef DEBUG
      std::cout << std::endl;
#endif
      // add new domino element
      results.elements.append(domino_elem_located(value1, value2, is_vertical, x, y));
   }

   return results;
}

int main()
{
#ifdef DEBUG
   //simple_tree<int,size_t> x;
#endif // DEBUG

   //all_chars(std::cout);
   std::cout << "Vanishing Domino Problem" << std::endl;

   raw_domino_problem pr = read_xml("problem1.xml");
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

