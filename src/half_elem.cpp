#include "half_elem.h"

half_elem::half_elem()
   : value(0), other_half(nullptr), direction(none), owner(nullptr) { }

half_elem::half_elem(domino_elem_value_t value)
   : value(value), other_half(nullptr), direction(none), owner(nullptr) { }

half_elem::half_elem(const half_elem& half)
   : value(half.value), other_half(nullptr), direction(half.direction), owner(nullptr) { }

half_elem& half_elem::operator=(const half_elem& half)
{
   value = half.value;
   other_half = nullptr;
   direction = half.direction;
   owner = nullptr;
   return *this;
}

std::ostream& operator<<(std::ostream& os, const half_elem& half)
{
   os << '[' << +(half.value) << ' ' << half.direction << ']';
   return os;
}

std::ostream& operator<<(std::ostream& os, half_elem* half)
{
   if(half)
      os << *half;
   else
      os << "nullptr";
   return os;
}
