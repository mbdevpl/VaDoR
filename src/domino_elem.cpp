#include "domino_elem.h"

domino_elem::domino_elem(domino_elem_value_t val1, domino_elem_value_t val2,
                         bool is_vertical)
   : h1(half_elem(val1)), h2(half_elem(val2)), is_vertical(is_vertical) {
   // 1st half
   h1.other_half = &h2;
   h1.direction = (is_vertical ? down : right);
   //h1.owner = this;

   // 2nd half
   h2.other_half = &h1;
   h2.direction = (is_vertical ? up : left);
   //h2.owner = this;
}

std::ostream& operator<<(std::ostream& os, const domino_elem& domino)
{
   os << '[' << domino.h1.value << ' ' << domino.h2.value << ' '
      << (domino.is_vertical ? "ver" : "hor") << ']';
   return os;
}

std::ostream& operator<<(std::ostream& os, domino_elem* domino)
{
   if(domino)
      os << *domino;
   else
      os << "nullptr";
   return os;
}
