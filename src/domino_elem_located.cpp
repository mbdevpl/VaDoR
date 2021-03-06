#include "domino_elem_located.h"

domino_elem_located::domino_elem_located()
   : domino_elem(), x(0), y(0) { }

domino_elem_located::domino_elem_located(domino_elem_value_t val1, domino_elem_value_t val2,
                                         bool is_vertical, size_t x, size_t y)
   : domino_elem(val1, val2, is_vertical), x(x), y(y)
{
   h1.owner = this;
   h2.owner = this;
}

domino_elem_located::domino_elem_located(const domino_elem_located& domino)
   : domino_elem(domino), x(domino.x), y(domino.y)
{
   h1.owner = this;
   h2.owner = this;
}

domino_elem_located& domino_elem_located::operator=(const domino_elem_located& domino)
{
   domino_elem::operator=(domino);
   x = domino.x;
   y = domino.y;
   h1.owner = this;
   h2.owner = this;
   return *this;
}

std::string domino_elem_located::str(bool compact) const
{
   std::stringstream s;
   if(compact)
      s << +value1() << '-' << +value2()
        << '@' << x << ',' << y;
   else
      s << '[' << +value1() << ' ' << +value2() << ' '
        << (is_vertical ? "ver" : "hor") << '@' << x << 'x' << y << ']';
   return s.str();
}

std::ostream& operator<<(std::ostream& os, const domino_elem_located& domino)
{
   os << domino.str();
   return os;
}

std::ostream& operator<<(std::ostream& os, domino_elem_located* domino)
{
   if(domino)
      os << *domino;
   else
      os << "nullptr";
   return os;
}
