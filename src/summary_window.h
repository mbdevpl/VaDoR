#ifndef SUMMARY_WINDOW_H
#define SUMMARY_WINDOW_H

#include <QDialog>

namespace Ui {
class summary_window;
}

class summary_window : public QDialog
{
    Q_OBJECT
    
public:
    explicit summary_window(QWidget *parent = 0);
    ~summary_window();
    
private:
    Ui::summary_window *ui;
};

#endif // SUMMARY_WINDOW_H
