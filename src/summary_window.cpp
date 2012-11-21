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
    QString text = bolden("ELAPSED TIME: <br>")+QString::number(time)+" ms <br><br>"+
            bolden("PIECES REMOVED: <br>")+piecesRemoved+"<br>"+
            bolden("PIECES LEFT: <br>")+piecesLeft;
    ui->infoTextEdit->setText(text);
}

QString summary_window::bolden(QString text)
{
    return "<FONT color=\"red\" style=\"font-weight: bold;\" >"+ text +"</FONT> ";
}

QString summary_window::bolden2(QString text)
{
    return "<FONT color=\"blue\" >"+ text +"</FONT> ";
}

QString summary_window::spc()
{
    return "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
}

QString summary_window::OrToStr(bool val)
{
    if (val==true) return "Vertical";
    return "Horizontal";
}

QString summary_window::getPiecesInfo(QVector<domino_elem_located *> pieces)
{
    QString info;
    foreach (domino_elem_located* piece, pieces)
        info.append(bolden2("Value 1: ")+QString::number(piece->h1.value)+
                    spc()+ bolden2("Value 2: ")+QString::number(piece->h2.value)+
                    spc()+ bolden2("X: ")+QString::number( piece->x)+
                    spc()+ bolden2("Y: ")+QString::number(piece->y)+
                    spc()+ bolden2("Orientation: ")+OrToStr(piece->is_vertical)+"<br>"
                    );
    return info;
}
