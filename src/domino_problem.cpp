#include "domino_problem.h"
/*
domino_problem::domino_problem(const domino_problem_input& input)
   : elements(input.elements), width(input.width), height(input.height), board(input.board),
     on_board(input.elements), possible(), removed(), unresolved(input.elements),
     checked(), invalid()
{
   resolve_elements();
   scan_board();
}
void domino_problem::resolve_elements()
{
   for(elements_t::elem i = unresolved.first(); i; ++i)
   {
      domino_elem_located* e = i.value_ref();
      domino_elem_value_t v1 = e->h1.value;
      domino_elem_value_t v2 = e->h2.value;
      if(e->is_vertical)
      {
         // up & down
         // left & right
         if(v1 > y && v2 >= height - y
               && (v1 > x || v2 > x) && (v1 >= width - x || v2 >= width - x) )
            invalid.append(e);
         else
            checked.append(e);
      }
      else
      {
         // left & right
         // up & down
         if(v1 > x || v2 >= width - x
               && (v1 > y || v2 > y) && (v1 >= height - y || v2 >= height - y) )
            invalid.append(e);
         else
            checked.append(e);
      }
      elements_t::elem i2 = i.copy();
      i.forward();
      i2.remove();
      i.back();
   }
}

void domino_problem::scan_board()
{

   for(elements_t::elem i = checked.first(); i; ++i)
   {
      domino_elem_located* e = i.value_ref();
      else if(can_be_removed(e))
         possible.append(e);
   }
}

bool domino_problem::can_be_removed(domino_elem_located* e)
{
   if(e->is_vertical)
   {
      if(distance(e->x, e->y, board, up) == )
         return false;
      distance(e->x, e->y+1, board, down);
   }
   else
   {
      distance(e->x, e->y, board, left);
      distance(e->x+1, e->y, board, right);
   }
   distance(e->x, e->y, board, up);
}

void domino_problem::removeAt(size_t x, size_t y, whole_board& b,
                              elem_loc_ptr_list& backing_list)
{
   half_elem*& e = b[x][y];
   elem_loc_ptr_list::elem found = backing_list.find(e->owner);
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

void domino_problem::removePiece(domino_elem_located* piece, whole_board& b,
                                 elem_loc_ptr_list& backing_list)
{
   removeAt(piece->x, piece->y, b, backing_list);
}

simple_list<domino_problem::elem_list,size_t> domino_problem::all_best_soluions()
{
   simple_list<domino_problem::elem_list,size_t> solutions;
   //simple_tree<whole_board,size_t> partial(board);

   size_t problem_size = width * height;

   for(size_t i=0; i < problem_size; ++i)
   {
      elem_loc_ptr_list moves = find_all_possible_moves(board);
      //solutions

      for(size_t i=0; i < problem_size; ++i)
      {

      }
   }

   return solutions;
}

#ifdef DEBUG

void domino_problem::demo_solution()
{
   std::cout << find_all_possible_moves(board) << std::endl;
   //std::cout << str() << std::endl;

   removeAt(2, 0, board, elements);

   std::cout << find_all_possible_moves(board) << std::endl;
   //std::cout << str() << std::endl;

   removeAt(1, 1, board, elements);

   std::cout << find_all_possible_moves(board) << std::endl;
   //std::cout << str() << std::endl;
}
*/
//#endif // DEBUG
