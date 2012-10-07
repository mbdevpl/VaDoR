#include "domino_elem_located.h"

domino_elem_located::domino_elem_located(domino_elem_value_t val1, domino_elem_value_t val2,
                           bool is_vertical, size_t x, size_t y)
   : domino_elem(val1, val2, is_vertical), x(x), y(y)
{
   h1.owner = this;
   h2.owner = this;
}

std::ostream& operator<<(std::ostream& os, const domino_elem_located& domino)
{
   os << '[' << domino.h1.value << ' ' << domino.h2.value << ' '
      << (domino.is_vertical ? "ver" : "hor") << '@' << domino.x << 'x' << domino.y << ']';
   return os;
}
