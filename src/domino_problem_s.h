#ifndef DOMINO_PROBLEM_S_H
#define DOMINO_PROBLEM_S_H

#include "domino_problem_r.h"

class domino_problem_s : public domino_problem_r
{
public:
    domino_problem_s();
    domino_problem_s(domino_problem_r &problem);
    QVector<domino_elem_located*> neighbors_list;
};

#endif // DOMINO_PROBLEM_S_H
