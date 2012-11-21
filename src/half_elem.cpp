#include "half_elem.h"
#include "domino_elem_located.h"

half_elem::half_elem()
   : value(0), other_half(nullptr), direction(none), owner(NULL) { }

half_elem::half_elem(domino_elem_value_t value)
   : value(value), other_half(nullptr), direction(none), owner(NULL) { }

half_elem::half_elem(const half_elem& half)
   : value(half.value), other_half(nullptr), direction(half.direction), owner(NULL) { }

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

size_t half_elem::loc_x()
{
    if ((&owner->h1 == this) || owner->is_vertical)
        return this->owner->x;
    else
        return this->owner->x+1;
    return 0;
}


size_t half_elem::loc_y()
{
    if (&owner->h1 == this || !this->owner->is_vertical)
        return this->owner->y;
    else
        return this->owner->y+1;
    return 0;
}
