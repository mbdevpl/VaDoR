#include "domino_problem.h"

domino_problem::domino_problem()
   : domino_problem_input(), on_board(), possible(), removed() { }

domino_problem::domino_problem(const domino_problem& problem)
   : domino_problem_input(problem), on_board(problem.on_board), possible(problem.possible),
     removed(problem.removed) { }

domino_problem::domino_problem(const domino_problem& problem, bool copy_possible)
   : domino_problem_input(problem), on_board(problem.on_board),
     possible(), removed(problem.removed)
{
   if(copy_possible)
      possible.initFrom(problem.possible);
   if(problem.is_compact)
      throw std::runtime_error("cannot copy packed object, use expand() first");
}

domino_problem::domino_problem(const domino_problem_input& input)
   : domino_problem_input(input), on_board(*elements), possible(), removed()
{
   //resolve_elements();
   //scan_board();
}

domino_problem::~domino_problem()
{
   //   if(!is_compact)
   //   {
   //      on_board.clear();
   //      possible.clear();
   //      removed.clear();
   //   }
}

void domino_problem::scan_board()
{
   for(elements_t::elem i = checked.first(); i; ++i)
   {
      domino_elem_located* e = i.value_ref();
      size_t& x = e->x;
      size_t& y = e->y;
      if(can_be_removed(x, y))
         possible.append(e);
   }
}

void domino_problem::add_possible_outcomes(domino_problem::solution_t& outcomes)
{
   if(is_compact)
      throw std::runtime_error("use expand() first");
   if(possible.length() == 0)
      return;
   for (elements_t::elem i = possible.first(); i; ++i)
   {
      outcomes.append(domino_problem(*this, false));
      domino_problem& pr = outcomes.last().value_ref();
      domino_elem_located* e = i.value_ref();
      pr.remove_at(e->x, e->y);
      //pr.possible.clear();
      pr.scan_board();
   }
}

domino_problem::solution_t domino_problem::get_possible_outcomes()
{
   //typedef simple_list<domino_problem,size_t> results_t;
   solution_t outcomes;
   add_possible_outcomes(outcomes);
   return outcomes;
}

bool domino_problem::state_equals(const domino_problem& problem)
{
   if(is_compact)
      throw std::runtime_error("use expand() first");
   if(problem.on_board == on_board)
      return true;
   return false;
}

bool domino_problem::can_be_removed(size_t x, size_t y)
{
   if(is_compact)
      throw std::runtime_error("use expand() first");
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
   if(is_compact)
      throw std::runtime_error("use expand() first");
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
   if(is_compact)
      throw std::runtime_error("use expand() first");
   half_elem*& he = board[x][y];
   domino_elem_located* e = he->owner;
   removed.append(e);
   possible.find(e).remove();
   checked.find(e).remove();
   on_board.find(e).remove();
   board[e->x][e->y] = 0;
   if(e->is_vertical)
      board[e->x][e->y+1] = 0;
   else
      board[e->x+1][e->y] = 0;
}

void domino_problem::pack()
{
   if(is_compact)
      return;

   on_board_key = 0;
   possible_key = 0;
   //removed_key = 0;  // this is in fact equal to bitwise !(on_board_key)
   //  but order also has to be stored, therefore I can't see how to encode this, yet
   for(elements_t::elem_const el = elements->first(), on = on_board.first(),
       po = possible.first()/*, re = removed.first()*/; el; ++el)
   {
      on_board_key <<= 1;
      if(on && *el == *on)
      {
         on_board_key += 1;
         ++on;
      }
      possible_key <<= 1;
      if(po && *el == *po)
      {
         possible_key += 1;
         ++po;
      }
      //      re = removed.find(*el);
      //      removed_key <<= 1;
      //      if(re && *el == *re)
      //      {
      //         removed_key += 1;
      //         //re;
      //      }
   }
   on_board.clear();
   possible.clear();
   //removed.clear();

   domino_problem_input::pack();

   is_compact = true;
}

void domino_problem::expand()
{
   if(!is_compact)
      return;

   if(on_board.length() > 0 || possible.length() > 0)
      throw std::runtime_error("error: problem was modified while packed");

   domino_problem_input::expand();

   for(elements_t::elem_const el = elements->last()/*, on = on_board.first(),
           po = possible.first(), re = removed.first()*/; el; --el)
   {
      if(on_board_key & 1)
      {
         on_board.insertFirst(*el);
      }
      on_board_key >>= 1;
      if(possible_key & 1)
      {
         possible.insertFirst(*el);
      }
      possible_key >>= 1;
   }
   is_compact = false;
}

#ifdef DEBUG

void domino_problem::demo_solution()
{
   //std::cout << find_all_possible_moves(board) << std::endl;
   std::cout << board_str() << std::endl;
   remove_at(2, 0);

   //std::cout << find_all_possible_moves(board) << std::endl;
   std::cout << board_str() << std::endl;
   remove_at(1, 1);

   //std::cout << find_all_possible_moves(board) << std::endl;
   std::cout << board_str() << std::endl;
}

#endif // DEBUG
