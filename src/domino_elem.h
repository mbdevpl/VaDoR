#ifndef DOMINO_ELEM_H
#define DOMINO_ELEM_H

#include <ostream>
#include "half_elem.h"
#include "domino_elem_fwd.h"

class domino_elem {
public:
   half_elem h1;
   half_elem h2;
   bool is_vertical;
public:
   domino_elem();
   domino_elem(domino_elem_value_t val1, domino_elem_value_t val2, bool is_vertical);
   domino_elem(const domino_elem& domino);
   domino_elem& operator=(const domino_elem& domino);
public:
   friend std::ostream& operator<<(std::ostream& os, const domino_elem& domino);
   friend std::ostream& operator<<(std::ostream& os, domino_elem* domino);
};

#endif // DOMINO_ELEM_H
