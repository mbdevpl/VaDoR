#include "domino_problem_input.h"


domino_problem_input::domino_problem_input()
   : elements(), width(0), height(0), board()
{

}

domino_problem_input::domino_problem_input(const std::string& path)
   : elements(), width(0), height(0), board()
{
   std::ifstream t(path);
   std::stringstream buffer;
   buffer << t.rdbuf();
   std::string s = buffer.str();
   char* text = (char*)s.c_str();

   rapidxml::xml_document<> doc;
   doc.parse<0>(text);

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
         width = std::atoi(attr->value());
      else if(!attr_height.compare(attr->name()))
         height = std::atoi(attr->value());
      else
         throw std::runtime_error("wrong XML attribute: width/height expected");
   }
#ifdef DEBUG
   std::cout << std::endl;
#endif
   if(width == 0 || height == 0)
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
      elements.append(new domino_elem_located(value1, value2, is_vertical, x, y));
   }
   for(size_t x = 0; x < width; ++x)
   {
      column_t col;
      for(size_t y = 0; y < height; ++y)
         col.append(0);
      board.append(col);
   }

   for(size_t x = 0; x < width; ++x)
   {
      for(size_t y = 0; y < height; ++y)
      {
         elements_t::elem it = elements.first();
         //elem_loc_list::elem_const loc_it = dominos.first();
         while(it)
         {
            //domino_elem& el = *(it.value_ref());
            domino_elem_located& el = *(it.value_ref());
            //const domino_elem_located& loc_el = loc_it.value_ref_const();
            if(el.x == x && el.y == y)
            {
               // found element that starts at current point
               board[x][y] = &(el.h1);
               half_direction& dir = el.h1.direction;
               if(dir == up)
                  board[x][y-1] = &(el.h2);
               else if(dir == right)
                  board[x+1][y] = &(el.h2);
               else if(dir == down)
                  board[x][y+1] = &(el.h2);
               else if(dir == left)
                  board[x-1][y] = &(el.h2);
               break;
            }
            ++it;
            //++loc_it;
         }

      }
   }
   //initial_board = board;
}

std::string domino_problem_input::board_str() const
{
   //size_t len = elements.length();
   std::stringstream s;

   char fill = -79;

   //above board
   s << '+';
   for(size_t i = 0; i < width; ++i)
      s << "---+";
   s << "\n";

   for(size_t y = 0; y < height; ++y)
   {
      // above domino number
      s << '|';
      for(size_t x = 0; x < width; ++x)
      {
         half_elem* h_elem = board[x][y];
         if(h_elem)
         {
            s << fill << fill << fill;
            if(h_elem->direction == right)
               s << fill;
            else
               s << '|';
         }
         else
            s << "    ";
      }
      s << "\n";

      // domino number
      s << '|';
      for(size_t x = 0; x < width; ++x)
      {
         half_elem* h_elem = board[x][y];
         if(h_elem)
         {
            s << fill << h_elem->value << fill;
            if(h_elem->direction == right)
               s << fill;
            else
               s << '|';
         }
         else
            s << "    ";
      }
      s << "\n";

      // below domino number
      s << '|';
      for(size_t x = 0; x < width; ++x)
      {
         half_elem* h_elem = board[x][y];
         if(h_elem)
         {
            s << fill << fill << fill;
            if(h_elem->direction == right)
               s << fill;
            else
               s << '|';
         }
         else
            s << "    ";
      }
      s << "\n";

      // line between dominos
      s << '+';
      for(size_t x = 0; x < width; ++x)
      {
         half_elem* h_elem = board[x][y];
         if(h_elem)
         {
            if(h_elem->direction == down)
               s << fill << fill << fill;
            else
               s << "---";
         }
         else
            s << "   ";
         s << '+';
      }
      s << "\n";
   }
   return s.str();
}
