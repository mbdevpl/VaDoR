#ifndef DOMINO_ELEM_R_H
#define DOMINO_ELEM_R_H
#include<domino_field_r.h>

class domino_elem_r
{
public:
    int loc_x;
    int loc_y;
    domino_field_r sucessor;
    domino_field_r predecessor;
    bool isVertical;
};

#endif // DOMINO_ELEM_R_H
