#ifndef HALF_ELEM_H
#define HALF_ELEM_H

#include <ostream>
#include "domino_elem_fwd.h"
#include "domino_elem_located_fwd.h"

class half_elem {
public:
   domino_elem_value_t value;
   half_elem* other_half;
   half_direction direction;
   domino_elem_located* owner;
   int loc_x;
   int loc_y;
public:
   half_elem()
      : value(0), other_half(0), direction(none), owner(0) { }
   half_elem(domino_elem_value_t value)
      : value(value), other_half(0), direction(none), owner(0) { }
public:
   friend std::ostream& operator<<(std::ostream& os, const half_elem& domino);
   friend std::ostream& operator<<(std::ostream& os, half_elem* domino);
};

#endif // HALF_ELEM_H
