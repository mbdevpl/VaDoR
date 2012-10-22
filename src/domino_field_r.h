#ifndef DOMINO_FIELD_R_H
#define DOMINO_FIELD_R_H
#include "domino_elem_r.h"

class domino_field_r
{
public:
  int value;
  int loc_x;
  int loc_y;
  domino_field_r *twin;
  bool isSucessor;

 // domino_field_r(): value(0),loc_x(0),loc_y(0),twin(NULL),isSucessor(false) { }
 // domino_field_r(): value(0), loc_x(0), loc_y(0), twin(), isSucessor(true) {}
  //domino_field_r(int _value, int _loc_x, int _loc_y, domino_field_r *_twin, bool _isSucessor):
  //    value(0), loc_x(0), loc_y(0), twin(), isSucessor(0){}
};

#endif // DOMINO_FIELD_R_H
