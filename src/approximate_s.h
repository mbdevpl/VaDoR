#ifndef APPROXIMATE_S_H
#define APPROXIMATE_S_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include <QTime>
#include "domino_elem_fwd.h"
#include "domino_problem_s.h"

class approximate_s : public QThread
{
    Q_OBJECT
signals:
    void threadRemovePiece(int x,int y);
    void threadComputationOver(int time,QVector<domino_elem_located*>* present,QVector<domino_elem_located*> removed);

private:
    half_elem*** board;
    QVector<domino_elem_located*>* present;
    QVector<domino_elem_located*>* neighbors;
    QVector<domino_elem_located*>* removedPreviously;
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
    void solve();
    void findRoots();
    bool classify(domino_elem_located* piece);
    bool isIndependent(int x, int y);
    void determineNewNeighbors();
    bool checkHalfBoardIndependence(int x, int y, half_direction direction);
    void test(QVector<domino_elem_located *> pieces);
    void sendResultsToGui(int timeElapsed);

public:
    approximate_s(void);
    approximate_s(domino_problem_s &problem);
    void setPieceByPiece(bool on);
    void setIsDelay(bool on);
    void setDelay(int delay);
    void increaseMutex();
    void stopExecution();
};

#endif // APPROXIMATE_S_H
