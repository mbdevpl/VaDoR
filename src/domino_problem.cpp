#include "domino_problem.h"

domino_problem::domino_problem()
   : domino_problem_input(), on_board(), possible(), removed(),
     unresolved(), checked(), invalid() { }

domino_problem::domino_problem(const domino_problem& problem)
   : domino_problem_input(problem), on_board(problem.on_board), possible(problem.possible),
     removed(problem.removed),
     unresolved(problem.unresolved), checked(problem.checked), invalid(problem.invalid) { }

domino_problem::domino_problem(const domino_problem_input& input)
   : domino_problem_input(input), on_board(elements), possible(), removed(),
     unresolved(elements), checked(), invalid()
{
   resolve_elements();
   scan_board();
}

void domino_problem::resolve_elements()
{
   if(unresolved.length() == 0)
      return;
   for(elements_t::elem i = unresolved.first(); i; i.removeAndForward())
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
   }
}

void domino_problem::scan_board()
{
   for(elements_t::elem i = checked.first(); i; ++i)
   {
      domino_elem_located* e = i.value_ref();
      size_t& x = e->x;
      size_t& y = e->y;
      if(can_be_removed(x, y) && possible.find(e).empty())
         possible.append(e);
   }
}

void domino_problem::add_possible_outcomes(simple_list<domino_problem,size_t>& outcomes)
{
   if(possible.length() == 0)
      return;
   for (elements_t::elem i = possible.first(); i; ++i)
   {
      outcomes.append(domino_problem(*this));
      domino_problem& pr = outcomes.last().value_ref();
      domino_elem_located* e = i.value_ref();
      pr.remove_at(e->x, e->y);
   }
}

simple_list<domino_problem,size_t> domino_problem::get_possible_outcomes()
{
   typedef simple_list<domino_problem,size_t> results_t;
   results_t outcomes;
   add_possible_outcomes(outcomes);
   return outcomes;
}

bool domino_problem::can_be_removed(size_t x, size_t y)
{
   domino_elem_located* e = board[x][y]->owner;
   size_t dist_up = distance(e->x, e->y, up);
   size_t dist_left = distance(e->x, e->y, left);
   if(e->is_vertical)
   {
      // check distances from the vertical piece
      if(dist_up == e->value1())
         return true;
      if(distance(e->x, e->y+1, down) == e->value2())
         return true;
      if(dist_left == e->value1() && distance(e->x, e->y+1, left) == e->value2())
         return true;
      if(distance(e->x, e->y, right) == e->value1() && distance(e->x, e->y+1, right) == e->value2())
         return true;
   }
   else
   {
      // check distances from the horizontal piece
      if(dist_left == e->h1.value)
         return true;
      if(distance(e->x+1, e->y, right) == e->h2.value)
         return true;
      if(dist_up == e->value1() && distance(e->x+1, e->y, up) == e->value2())
         return true;
      if(distance(e->x, e->y, down) == e->value1() && distance(e->x+1, e->y, down) == e->value2())
         return true;
   }
   //distance(e->x, e->y, up);
   return false;
}

size_t domino_problem::distance(size_t x, size_t y, half_direction dir)
{
   if(dir == up)
   {
      // check up
      if(y == 0)
         return 0;
      size_t yy = 0;
      // looking for closest piece up from the current location
      bool found = false;
      for(yy = y-1; ; --yy)
      {
         if(board[x][yy] != 0)
         {
            found = true;
            break;
         }
         if(yy == 0)
            break;
      }
      if(found)
         ++yy; // to compensate for checking
      // if y == 3, and item was found in yy == 2, then the distance
      //  is not 1, but 0
      return y - yy;
   }
   else if(dir == down)
   {
      //size_t height = board.first().value_ref_const().length();
      // check down
      if(y == height - 1)
         return 0;
      size_t yy = height - 1;
      // looking for closest piece down from the current location
      bool found = false;
      for(yy = y+1; ; ++yy)
      {
         if(board[x][yy] != 0)
         {
            found = true;
            break;
         }
         if(yy == height - 1)
            break;
      }
      if(found)
         --yy; // to compensate for checking, see comments in 'check up'
      return yy - y;
   }
   else if(dir == left)
   {
      // check left
      if(x == 0)
         return 0;
      size_t xx = 0;
      // looking for closest piece up from the current location
      bool found = false;
      for(xx = x-1; ; --xx)
      {
         if(board[xx][y] != 0)
         {
            found = true;
            break;
         }
         if(xx == 0)
            break;
      }
      if(found)
         ++xx; // to compensate for checking
      return x - xx;
   }
   else if(dir == right)
   {
      //size_t width = b.length();
      // check right
      if(x == width - 1)
         return 0;
      size_t xx = width - 1;
      // looking for closest piece down from the current location
      bool found = false;
      for(xx = x+1; ; ++xx)
      {
         if(board[xx][y] != 0)
         {
            found = true;
            break;
         }
         if(xx == width - 1)
            break;
      }
      if(found)
         --xx; // to compensate for checking, see comments in 'check up'
      return xx - x;
   }
   throw std::runtime_error("distance in given direction cannot be measured");
}

void domino_problem::remove_at(size_t x, size_t y)
{
   half_elem*& he = board[x][y];
   domino_elem_located* e = he->owner;
   possible.find(e).remove();
   checked.find(e).remove();
   on_board.find(e).remove();
   board[e->x][e->y] = 0;
   if(e->is_vertical)
      board[e->x][e->y+1] = 0;
   else
      board[e->x+1][e->y] = 0;
}

//simple_list<domino_problem::elements_t,size_t> domino_problem::all_best_soluions()
//{
//   simple_list<domino_problem::elem_list,size_t> solutions;
//   //simple_tree<whole_board,size_t> partial(board);

//   size_t problem_size = width * height;

//   for(size_t i=0; i < problem_size; ++i)
//   {
//      elem_loc_ptr_list moves = find_all_possible_moves(board);
//      //solutions

//      for(size_t i=0; i < problem_size; ++i)
//      {

//      }
//   }

//   return solutions;
//}

#ifdef DEBUG

//void domino_problem::demo_solution()
//{
//   std::cout << find_all_possible_moves(board) << std::endl;
//   //std::cout << str() << std::endl;

//   removeAt(2, 0, board, elements);

//   std::cout << find_all_possible_moves(board) << std::endl;
//   //std::cout << str() << std::endl;

//   removeAt(1, 1, board, elements);

//   std::cout << find_all_possible_moves(board) << std::endl;
//   //std::cout << str() << std::endl;
//}

#endif // DEBUG
