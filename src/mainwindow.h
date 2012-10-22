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
#include "summary_window.h"
#include "approximate_r.h"

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
    QWidget *viewport;
    approximate_r *apprThread_r;
    QMessageBox msgBox;
    current_algorithm selectedAlgorithm;
    int elemCurr;
    void setBoardSize(int,int);
    void addPiece(int loc_x, int loc_y, bool isVertical, int val1, int val2);
    void setGuiEnabledWhileComputing(bool value, bool isPieceByPiece);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void computationOver(int time, int threadId);
    void removePiece(int loc_x, int loc_y);
    void exitApplicationClicked();
    void openFileClicked();
    void goToEndClicked();
    void algorithmChanged();
    void runClicked();
    void stopClicked();
    void removeNextPieceClicked();
};

#endif // MAINWINDOW_H
