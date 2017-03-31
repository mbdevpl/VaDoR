#ifndef DOMINO_PROBLEM_R_H
#define DOMINO_PROBLEM_R_H

#include<iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <string>
#include <streambuf>
#include<QVector>
#include<QDomDocument>
#include<QFile>
#include<QString>
#include "domino_elem_located.h"
#include "half_elem.h"


class domino_problem_r
{   
private:
    QDomDocument open_file(QString in_path);

public:
    int board_width;
    int board_height;
    QVector<domino_elem_located*> elem_list;

    domino_problem_r(){}
    void load(QString in_path);
    void load_xml(QString in_path);
    void load_txt(const std::string &path);
};

#endif // DOMINO_PROBLEM_R_H
