#include "summary_window.h"
#include "ui_summary_window.h"

summary_window::summary_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::summary_window)
{
    ui->setupUi(this);
      connect( ui->CloseButton, SIGNAL(clicked()), this, SLOT( closeClicked() ) );
}

summary_window::~summary_window()
{
    delete ui;
}

void summary_window::closeClicked()
{
    this->hide();
    emit reloadGUI();
}

void summary_window::publishResults(int time, QVector<domino_elem_located *> present, QVector<domino_elem_located *> removed)
{
    QString piecesLeft = getPiecesInfo(present);
    QString piecesRemoved = getPiecesInfo(removed);
    QString text = "ELAPSED TIME:\n"+QString::number(time)+" ms \n\n"+
            "PIECES REMOVED:\n"+piecesRemoved+"\n"+
            "PIECES LEFT: \n"+piecesLeft;
    ui->infoTextEdit->setText(text);
}

QString summary_window::bolden(QString text)
{
    return "<FONT color=\"red\" style=\"font-weight: bold;\" >"+ text +"</FONT> ";
}

QString summary_window::getPiecesInfo(QVector<domino_elem_located *> pieces)
{
    QString info;
    foreach (domino_elem_located* piece, pieces)
        info.append("Value 1: "+QString::number(piece->h1.value)+
                         "   Value 2: "+QString::number(piece->h2.value)+
                         "   X location: "+QString::number( piece->x)+
                         "   Y location: "+QString::number(piece->y)+
                    "   Vertical: "+QString::number(piece->is_vertical)+ '\n');
    return info;
}
