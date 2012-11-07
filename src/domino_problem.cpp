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

//domino_problem::~domino_problem()
//{
//   if(!is_compact)
//   {
//      on_board.clear();
//      possible.clear();
//      removed.clear();
//   }
//}

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
      //#ifdef DEBUG
      //      std::cout << pr.board_str();
      //#endif
      pr.scan_board();
   }
   //#ifdef DEBUG
   //      std::cout << possible.length();
   //#endif
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
   if(is_compact || problem.is_compact)
      throw std::runtime_error("cannot compare packed objects");

   if(problem.on_board == on_board)
      return true;
   return false;
}

bool domino_problem::operator<(const domino_problem& problem)
{
   if(is_compact || problem.is_compact)
      throw std::runtime_error("cannot compare packed objects");

   if(on_board.length() < problem.on_board.length())
      return true;
   if(on_board.length() > problem.on_board.length())
      return false;

   if(possible.length() > problem.possible.length())
      return true;
   return false;
}

bool domino_problem::operator>(const domino_problem& problem)
{
   if(is_compact || problem.is_compact)
      throw std::runtime_error("cannot compare packed objects");

   if(on_board.length() > problem.on_board.length())
      return true;
   if(on_board.length() < problem.on_board.length())
      return false;

   if(possible.length() < problem.possible.length())
      return true;
   return false;

}

bool domino_problem::can_be_removed(size_t x, size_t y)
{
   if(is_compact)
      throw std::runtime_error("use expand() before checking if a piece can be removed");
   half_elem* h = board[x][y];
#ifdef DEBUG
   if(h == nullptr)
   {
      //std::cout << board_str();
      return false;
   }
#endif // DEBUG
   domino_elem_located* e = h->owner;
   if(e == nullptr)
      throw std::runtime_error("owner of half_elem must be set");
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
   half_elem* he = board[x][y];
   if(he == nullptr)
      return;
   domino_elem_located* e = he->owner;
   if(e == nullptr)
      throw std::runtime_error("owner of half_elem must be set");
   removed.append(e);
   possible.find(e).remove();
   checked.find(e).remove();
   on_board.find(e).remove();
   if(e->is_vertical)
   {
#ifdef DEBUG
      if(board[e->x][(e->y)+1]->owner != board[e->x][e->y]->owner)
         throw std::runtime_error("owner mismatch");
#endif // DEBUG
      board[e->x][(e->y)+1] = 0;
   }
   else
   {
#ifdef DEBUG
      if(board[(e->x)+1][e->y]->owner != board[e->x][e->y]->owner)
         throw std::runtime_error("owner mismatch");
#endif // DEBUG
      board[(e->x)+1][e->y] = 0;
   }
   board[e->x][e->y] = 0;
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
      if(invalid->find(*el))
      {
         ++on;
         continue;
      }
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
      if(invalid->find(*el))
      {
         on_board.insertFirst(*el);
         continue;
      }
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

std::string domino_problem::board_line(
      size_t line, char before_line, bool fill_middle, bool value_mode,
      half_direction dir, bool three_chars_after_if_dir, const std::string& after_otherwise,
      const std::string& middle_if_empty, const std::string& after, bool no_default_spaces) const
{
   static char sep_ver = -77;
   static char sep_hor = -60;
   static char sep_cross = -59;

   static char on_board_ch = -79;
   static char possible_ch = -80;
   static char invalid_ch = -98;
   static char fill = on_board_ch;

   std::stringstream s;
   s << before_line;
   for(size_t x = 0; x < width; ++x)
   {
      half_elem* h_elem = board[x][line];
      if(h_elem)
      {
         elements_t* pos = (elements_t*)&possible;
         typedef domino_elem_located* del_p;
         const del_p own = h_elem->owner;
         if(invalid->find(own).valid())
            fill = invalid_ch;
         else if(pos->find(own).valid())
            fill = possible_ch;
         else
            fill = on_board_ch;

         if(value_mode)
         {
            if(fill_middle)
               s << fill << +(h_elem->value) << fill;
            else
               s << ' ' << +(h_elem->value) << ' ';
         }
         else
         {
            if(fill_middle)
               s << fill << fill << fill;
            else if(!no_default_spaces)
               s << "   ";
         }
         if(h_elem->direction == dir)
         {
            if(three_chars_after_if_dir)
               s << fill << fill << fill;
            else
               s << fill;
         }
         else
            s << after_otherwise;
      }
      else
         s << middle_if_empty;
      s << after;
   }
   s << '\n';
   return s.str();
}

std::string domino_problem::board_str() const
{
   std::stringstream s;

   static char sep_ver = -77;
   static char sep_hor = -60;
   static char sep_cross = -59;

   static char on_board_ch = -79;
   static char possible_ch = -80;
   static char invalid_ch = -98;
   static char fill = on_board_ch;

   std::stringstream ss_hor;
   ss_hor << sep_hor << sep_hor << sep_hor;

   static std::string s_hor = ss_hor.str();
   static std::string s_ver = std::string(&sep_ver,1);
   static std::string s_cross = std::string(&sep_cross,1);

   s << on_board_ch << " - on board, "
     << possible_ch << " - possible, "
     << invalid_ch << " - invalid, " << "\n";

   //above board
   s << domino_problem_input::board_line(0, sep_cross, s_hor, false, right, "", "", s_hor, s_cross);

   for(size_t y = 0; y < height; ++y)
   {
      // above domino number
      std::stringstream ss;
      ss << fill << fill << fill;
      s << board_line(y, sep_ver, true, false, right, false, s_ver, "    ", "");

      // domino number
      std::stringstream ss2;
      ss2 << fill;
      s << board_line(y, sep_ver, true, true, right, false, s_ver, "    ", "");

      // below domino number
      s << board_line(y, sep_ver, true, false, right, false, s_ver, "    ", "");

      // line between dominos
      s << board_line(y, sep_cross, false, false, down, true, s_hor, "   ", s_cross, true);
      //s << domino_problem_input::board_line(y, sep_cross, "", false, down, ss.str(), s_hor, "   ", s_cross);
   }
   return s.str();
}
