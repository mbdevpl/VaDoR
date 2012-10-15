#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <rapidxml.hpp>
#include <math.h>

#define HALF_PIECE_DIM 65

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
  Ui::MainWindow *ui;
  QList<QPushButton*> buttonPieces;
  QGridLayout *scrollLayout;
  QWidget *viewport;
  int elemCurr;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addPiece(int loc_x, int loc_y, bool isVertical, int val1, int val2);
    void removePiece(int loc_x, int loc_y);
    void setBoardSize(int,int);

public slots:
    void exitApplicationClicked();
    void openFileClicked();
};

#endif // MAINWINDOW_H
