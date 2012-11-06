#ifndef ACCURATE_THREAD_H
#define ACCURATE_THREAD_H

#include<domino_problem_r.h>
#include<QVector>
#include<QThread>
#include<QMutex>
#include<QTime>
#include "domino_elem_fwd.h"
#include "domino_problem.h"
#include "domino_elem_located.h"
#include "domino_problem_solver.h"
#include "simple_list.h"

class accurate_thread : public QThread
{
    Q_OBJECT
signals:
    void threadRemovePiece(int x,int y);
    void threadComputationOver(int time,QVector<domino_elem_located*>* present,QVector<domino_elem_located*> removed);

public:
    accurate_thread()
       : present(nullptr), input(nullptr) { }
    accurate_thread(domino_problem_input& input);
    ~accurate_thread();
private:
    half_elem*** board;
    QVector<domino_elem_located*>* present;
    QVector<domino_elem_located*> removed;
    domino_problem_input *input;
    QTime myTimer;
    void run();

};

#endif // ACCURATE_THREAD_H

