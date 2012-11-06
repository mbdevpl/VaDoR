#ifndef SUMMARY_WINDOW_H
#define SUMMARY_WINDOW_H

#include <QDialog>
#include <QVector>

#include "domino_elem_located.h"

namespace Ui {
class summary_window;
}

class summary_window : public QDialog
{
    Q_OBJECT

signals:
    void reloadGUI();

public:
    explicit summary_window(QWidget *parent = 0);
    ~summary_window();
    void publishResults(int time,QVector<domino_elem_located*> present,QVector<domino_elem_located*> removed);
    
public slots:
    void closeClicked();

private:
    QString bolden(QString text);
    QString getPiecesInfo(QVector<domino_elem_located*> pieces);
    Ui::summary_window *ui;
};

#endif // SUMMARY_WINDOW_H
