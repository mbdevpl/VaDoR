#include "summary_window.h"
#include "ui_summary_window.h"

summary_window::summary_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::summary_window)
{
    ui->setupUi(this);
}

summary_window::~summary_window()
{
    delete ui;
}
