#ifndef HALF_ELEM_H
#define HALF_ELEM_H

#include <ostream>
#include "domino_elem_fwd.h"
#include "domino_elem_located_fwd.h"

class half_elem {
public:
   // Value.
   domino_elem_value_t value;
   // Pointer to the 2nd half of the piece.
   half_elem* other_half;
   // Direction to the 2nd half of the piece.
   half_direction direction;
   // Pointer to the owner of this half.
   domino_elem_located* owner;

public:
   half_elem();
   half_elem(domino_elem_value_t value);
   half_elem(const half_elem& half);
   half_elem& operator=(const half_elem& half);

public:
   size_t loc_x();
   size_t loc_y();
public:
   friend std::ostream& operator<<(std::ostream& os, const half_elem& half);
   friend std::ostream& operator<<(std::ostream& os, half_elem* half);
};

#endif // HALF_ELEM_H
