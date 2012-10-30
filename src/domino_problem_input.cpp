#include "domino_problem_input.h"

domino_problem_input::domino_problem_input()
//   : alloc(true), elements(new elements_t()), width(0), height(0), board(),
//     invalid(new elements_t()), unresolved(new elements_t()), checked() { }
   : alloc(true), elements(nullptr), width(0), height(0), board(),
     invalid(nullptr), unresolved(nullptr), checked() { }

domino_problem_input::domino_problem_input(const domino_problem_input& input)
   : elements(input.elements), width(input.width), height(input.height), board(input.board),
     invalid(input.invalid), unresolved(input.unresolved), checked(input.checked)
{
   alloc = false;
}

domino_problem_input::domino_problem_input(const std::string& path)
   : alloc(true), elements(new elements_t()), width(0), height(0), board(),
     invalid(new elements_t()), unresolved(new elements_t()), checked()
{
   if(path.substr(path.find_last_of(".") + 1) == "xml")
      read_xml(path);
   else if (path.substr(path.find_last_of(".") + 1) == "txt")
      read_txt(path);
   unresolved->initFrom(*elements);
   resolve_elements();
}

domino_problem_input::~domino_problem_input()
{
   if(alloc)
      release(); // do not dealloc unless this object created the variables
}

void domino_problem_input::release()
{
   if(invalid)
   {
      delete invalid;
      invalid = nullptr;
   }
   if(unresolved)
   {
      delete unresolved;
      unresolved = nullptr;
   }
   if(elements)
   {
      for(elements_t::elem e = elements->first(); e; ++e)
         delete *e;
      delete elements;
      elements = nullptr;
   }
   //for(board_t::elem col = board.first(); col; ++col)
   //   for(column_t::elem half = (*col).first(); half; ++half)
   //      if(true) ;
}

void domino_problem_input::read_txt(const std::string &path)
{
   static const int MAX_CHARS_PER_LINE = 128;
   static const int MAX_TOKENS_PER_LINE = 5;
   static const char* const DELIMITER = ";";

   size_t x = 0;
   size_t y = 0;
   bool is_vertical = false;
   domino_elem_value_t value1 = 0;
   domino_elem_value_t value2 = 0;

   // create a file-reading object
   std::ifstream fin;
   fin.open(path); // open a file
   if (!fin.good())
      throw std::runtime_error("Error: *.txt file not found");

   // read each line of the file
   while (!fin.eof())
   {
      // read an entire line into memory
      char buf[MAX_CHARS_PER_LINE];
      fin.getline(buf, MAX_CHARS_PER_LINE);
      // parse the line into blank-delimited tokens
      int n = 0; // a for-loop index
      // array to store memory addresses of the tokens in buf
      const char* token[MAX_TOKENS_PER_LINE] = {0}; // initialize to 0
      // parse the line
      token[0] = strtok(buf, DELIMITER); // first token
      if (token[0]) // zero if line is blank
      {
         for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
         {
            token[n] = strtok(0, DELIMITER); // subsequent tokens
            if (!token[n]) break; // no more tokens
         }
      }

      if (n == 5)
      {
         if (token[0][0] == 'v') is_vertical = true;
         else if (token[0][0] == 'h') is_vertical = false;
         value1 = std::atoi(token[1]);
         value2 = std::atoi(token[2]);
         x = std::atoi(token[3]);
         y = std::atoi(token[4]);
         elements->append(new domino_elem_located(value1, value2, is_vertical, x, y));
      } else if (n == 2)
      {
         this->width = atoi(token[0]);
         this->height = atoi(token[1]);
      }
   }
   init_board();
}

void domino_problem_input::read_xml(const std::string &path)
{
#ifdef DEBUG
   bool verbose = false;
#endif // DEBUG
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
   if(verbose)
      std::cout << node->name() << ": ";
#endif // DEBUG
   for (rapidxml::xml_attribute<> *attr = node->first_attribute();
        attr; attr = attr->next_attribute())
   {
      // reading attributes of main node
#ifdef DEBUG
      if(verbose)
         std::cout << "  " << attr->name() << "=" << attr->value();
#endif // DEBUG
      if(!attr_width.compare(attr->name()))
         width = std::atoi(attr->value());
      else if(!attr_height.compare(attr->name()))
         height = std::atoi(attr->value());
      else
         throw std::runtime_error("wrong XML attribute: width/height expected");
   }
#ifdef DEBUG
   if(verbose)
      std::cout << std::endl;
#endif // DEBUG
   if(width == 0 || height == 0)
      throw std::runtime_error("invalid XML data: width or height is zero");

   // reading domino pieces
   for( rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
   {
#ifdef DEBUG
      if(verbose)
         std::cout << "  " << n->name() << ": ";
#endif // DEBUG
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
         if(verbose)
            std::cout << "  " << att->name() << "=" << att->value();
#endif // DEBUG
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
      if(verbose)
         std::cout << std::endl;
#endif // DEBUG
      // add new domino element
      elements->append(new domino_elem_located(value1, value2, is_vertical, x, y));
   }
   init_board();
}

void domino_problem_input::init_board()
{
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
         elements_t::elem it = elements->first();
         while(it)
         {
            domino_elem_located& el = *(it.value_ref());
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
         }

      }
   }
}

void domino_problem_input::resolve_elements()
{
   if(unresolved->length() == 0)
      return;
   for(elements_t::elem i = unresolved->first(); i; i.removeAndForward())
   {
      domino_elem_located* e = i.value_ref();
      size_t& x = e->x;
      size_t& y = e->y;
      domino_elem_value_t v1 = e->h1.value;
      domino_elem_value_t v2 = e->h2.value;

      if(e->is_vertical)
      {
         // up & down
         // left & right
         if(v1 > y && v2 >= height - y
               && (v1 > x || v2 > x) && (v1 >= width - x || v2 >= width - x) )
            invalid->append(e);
         else
            checked.append(e);
      }
      else
      {
         // left & right
         // up & down
         if(v1 > x && v2 >= width - x
               && (v1 > y || v2 > y) && (v1 >= height - y || v2 >= height - y) )
            invalid->append(e);
         else
            checked.append(e);
      }
   }
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
            s << fill << +(h_elem->value) << fill;
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
