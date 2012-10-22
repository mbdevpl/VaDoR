#ifndef DOMINO_PROBLEM_R_H
#define DOMINO_PROBLEM_R_H

#include<iostream>
#include<QVector>
#include<QDomDocument>
#include<QFile>
#include<QString>
#include "domino_elem.h"
#include "half_elem.h"


class domino_problem_r
{   
private:
    QDomDocument open_file(QString in_path);

public:
    int board_width;
    int board_height;
    QVector<domino_elem*> elem_list;

    domino_problem_r(){}
    void load(QString in_path);
};

#endif // DOMINO_PROBLEM_R_H
