#include "domino_problem.h"

domino_problem::domino_problem(size_t width, size_t height,
                               const simple_list<domino_elem_located,size_t>& dominos)
   : elements(), width(width), height(height), board(), on_board()
{
#ifdef DEBUG
   simple_list<size_t,size_t>::test();
#endif
   //size_t length = elements.length();

   for(simple_list<domino_elem_located,size_t>::elem_const i = dominos.first(); i; ++i)
   {
      const domino_elem_located& loc_el = *i;
      domino_elem* el = new domino_elem(loc_el.h1.value, loc_el.h2.value, loc_el.is_vertical);
      elements.append(el);
   }
   on_board = elements;

   //elem_loc_list not_added(dominos);

   for(size_t x = 0; x < width; ++x)
   {
      board_column col;
      for(size_t y = 0; y < height; ++y)
         col.append(0);
      board.append(col);
   }

#ifdef DEBUG_DOMINO
   std::cout << dominos << std::endl;
   std::cout << elements << std::endl;
   std::cout << board << std::endl;
   std::cout << str() << std::endl;
#endif // DEBUG_DOMINO

   for(size_t x = 0; x < width; ++x)
   {
      for(size_t y = 0; y < height; ++y)
      {
         elem_ptr_list::elem it = elements.first();
         elem_loc_list::elem_const loc_it = dominos.first();
         while(it)
         {
            domino_elem& el = *(it.value_ref());
            const domino_elem_located& loc_el = loc_it.value_ref_const();
            if(loc_el.x == x && loc_el.y == y)
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
#ifdef DEBUG_DOMINO
               std::cout << board << std::endl;
               std::cout << str() << std::endl;
#endif // DEBUG_DOMINO
               break;
            }
            ++it;
            ++loc_it;
         }

      }
   }
   initial_board = board;
}

void domino_problem::removeAt(size_t x, size_t y, whole_board& b,
                              elem_ptr_list& backing_list)
{
   half_elem*& e = b[x][y];
   elem_ptr_list::elem found = backing_list.find(e->owner);
   found.remove();
   half_direction& dir = e->direction;
   if(dir == up)
      b[x][y-1] = 0;
   else if(dir == right)
      b[x+1][y] = 0;
   else if(dir == down)
      b[x][y+1] = 0;
   else if(dir == left)
      b[x-1][y] = 0;
   e = 0;
}

simple_list<domino_problem::elem_list,size_t> domino_problem::all_best_soluions()
{
   simple_list<domino_problem::elem_list,size_t> solutions;

   std::cout << find_all_possible_moves(board) << std::endl;
   std::cout << str() << std::endl;

   removeAt(2, 0, board, elements);

   std::cout << find_all_possible_moves(board) << std::endl;
   std::cout << str() << std::endl;

   removeAt(1, 1, board, elements);

   std::cout << find_all_possible_moves(board) << std::endl;
   std::cout << str() << std::endl;


   return solutions;
}

std::string domino_problem::str()
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
         half_elem*& h_elem = board[x][y];
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
         half_elem*& h_elem = board[x][y];
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
         half_elem*& h_elem = board[x][y];
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
         half_elem*& h_elem = board[x][y];
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
