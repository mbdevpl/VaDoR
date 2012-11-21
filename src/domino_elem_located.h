#ifndef DOMINO_ELEM_LOCATED_H
#define DOMINO_ELEM_LOCATED_H

#include <ostream>
#include <string>
#include <sstream>
#include "domino_elem.h"
#include "domino_elem_located_fwd.h"

class domino_elem_located : public domino_elem {
public:
   size_t x;
   size_t y;
public:
   domino_elem_located();
   domino_elem_located(domino_elem_value_t val1, domino_elem_value_t val2,
                bool is_vertical, size_t x, size_t y);
   domino_elem_located(const domino_elem_located& domino);
   domino_elem_located& operator=(const domino_elem_located& domino);
   inline domino_elem_value_t value1() const { return h1.value; }
   inline domino_elem_value_t value2() const { return h2.value; }
public:
   std::string str(bool compact = false) const;
   friend std::ostream& operator<<(std::ostream& os, const domino_elem_located& domino);
   friend std::ostream& operator<<(std::ostream& os, domino_elem_located* domino);
};

#endif // DOMINO_ELEM_LOCATED_H
