#ifndef APPROXIMATE_R_H
#define APPROXIMATE_R_H

#include<domino_problem_r.h>
#include<QVector>
#include<QThread>
#include<QMutex>
#include "domino_elem_fwd.h"

class approximate_r : public QThread
{
    Q_OBJECT
signals:
    void threadRemovePiece(int x,int y);
    void threadComputationOver(int time,int objectId);

private:
    half_elem*** board;
    QVector<domino_elem*>* present;
    QVector<domino_elem*> removed;
    int board_width;
    int board_height;

    bool isPieceByPiece;
    QMutex mutex;

    bool isDelay;
    int delay;

    void print_out();
    bool isRemovable(int x, int y);
    bool isBoardEmpty();
    void remove_piece(int x, int y);
    int countFieldValue(int x, int y);
    void run();

public:
    bool workFlag;
    approximate_r();
    approximate_r(domino_problem_r &problem);
    void setPieceByPiece(bool on);
    void setIsDelay(bool on);
    void setDelay(int delay);
    void increaseMutex();
    void stopExecution();
};

#endif // APPROXIMATE_R_H
