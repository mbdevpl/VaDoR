#include "half_elem.h"

std::ostream& operator<<(std::ostream& os, const half_elem& domino)
{
   os << '[' << domino.value << ' ' << domino.direction << ']';
   return os;
}

std::ostream& operator<<(std::ostream& os, half_elem* domino)
{
   if(domino)
      os << *domino;
   else
      os << "nullptr";
   return os;
}
