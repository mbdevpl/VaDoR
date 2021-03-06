#ifndef APPROXIMATE_R_H
#define APPROXIMATE_R_H

#include<domino_problem_r.h>
#include<QVector>
#include<QThread>
#include<QMutex>
#include<QTime>
#include "domino_elem_fwd.h"

class approximate_r : public QThread
{
    Q_OBJECT
signals:
    void threadRemovePiece(int x,int y);
    void threadComputationOver(int time,QVector<domino_elem_located*>* present,QVector<domino_elem_located*> removed);

private:
    half_elem*** board;
    QVector<domino_elem_located*>* present;
    QVector<domino_elem_located*> removed;
    int board_width;
    int board_height;

    bool isPieceByPiece;
    QMutex mutex;

    bool isDelay;
    int delay;

    QTime myTimer;

    void print_out();
    bool isRemovable(int x, int y);
    bool checkSide(int x, int y, half_direction direction);
    bool toBoolean(int value);
    void remove_piece(int x, int y);
    int countFieldValue(int x, int y);
    void run();
    void sendResultsToGui(int timeElapsed);

public:
    approximate_r();
    approximate_r(domino_problem_r &problem);
    void setPieceByPiece(bool on);
    void setIsDelay(bool on);
    void setDelay(int delay);
    void increaseMutex();
    void stopExecution();
};

#endif // APPROXIMATE_R_H
