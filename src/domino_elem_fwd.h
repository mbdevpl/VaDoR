#ifndef DOMINO_ELEM_FWD_H
#define DOMINO_ELEM_FWD_H

class domino_elem;

typedef unsigned int domino_elem_value_t;

enum half_direction {
   up,down,left,right,none
};

enum current_algorithm {
    accurate,mateusz,radek,stanislaw
};

#endif // DOMINO_ELEM_FWD_H
