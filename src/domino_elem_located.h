#ifndef DOMINO_ELEM_LOCATED_H
#define DOMINO_ELEM_LOCATED_H

#include <ostream>
#include "domino_elem.h"

class domino_elem_located : public domino_elem {
public:
   size_t x;
   size_t y;
public:
   domino_elem_located()
      : domino_elem(), x(0), y(0) { }
   domino_elem_located(domino_elem_value_t val1, domino_elem_value_t val2,
                bool is_vertical, size_t x, size_t y);
public:
   friend std::ostream& operator<<(std::ostream& os, const domino_elem_located& domino);
};

#endif // DOMINO_ELEM_LOCATED_H
