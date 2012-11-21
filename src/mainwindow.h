#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QThread>
#include <QObject>
#include <QWidget>
#include <rapidxml.hpp>
#include <math.h>
#include "domino_problem_r.h"
#include "domino_problem_s.h"
#include "summary_window.h"
#include "approximate_r.h"
#include "approximate_s.h"
#include "domino_problem_input.h"
#include "domino_problem_solver.h"
#include "accurate_thread.h"

#define HALF_PIECE_DIM 65

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    summary_window summary_win;
    QList<QPushButton*> buttonPieces;
    QGridLayout *scrollLayout;
    domino_problem_r problem_r;
    domino_problem_s problem_s;
    domino_problem_input *input;
    QWidget *viewport;
    approximate_r *apprThread_r;
    approximate_s *apprThread_s;
    accurate_thread *accThread;
    QMessageBox msgBox;
    current_algorithm selectedAlgorithm;

    // current list of members of scrollLayout
    QList<QWidget*> scrollLayoutMembers;

    int elemCurr;
    void setBoardSize(int,int);
    void addPiece(int loc_x, int loc_y, bool isVertical, int val1, int val2);
    void setGuiEnabledWhileComputing(bool value, bool isPieceByPiece);
    void setGuiForAccurate(bool value);
    QString bolden(QString text);
    QString bolden2(QString text);
    QString spc();
    QString OrToStr(bool val);
    QString getPiecesInfo(QVector<domino_elem_located*> pieces);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initAndRunAlgorithmByMateuszBysiek(bool approximateVersion);
public slots:
    void computationOver(int time, QVector<domino_elem_located*>* present, QVector<domino_elem_located*> removed);
    void removePiece(int loc_x, int loc_y);
    void exitApplicationClicked();
    void openFileClicked();
    void goToEndClicked();
    void algorithmChanged();
    void runClicked();
    void stopClicked();
    void removeNextPieceClicked();
    void resetGUI();
    void clearBoard();
};

#endif // MAINWINDOW_H
