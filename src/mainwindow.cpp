#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->elemCurr = 0;
    connect( ui->ExitMenu, SIGNAL(triggered()), this, SLOT( exitApplicationClicked() ) );
    connect( ui->OpenMenu, SIGNAL(triggered()), this, SLOT( openFileClicked() ) );

    scrollLayout = new QGridLayout;
    viewport = new QWidget;
    viewport->setLayout(scrollLayout);
    ui->boardScrollArea->setWidget(viewport);

    setBoardSize(10,10);
    this->addPiece(0,0,true,30000,50000);
    this->addPiece(1,1,false,30,50000);
    this->addPiece(2,2,true,3,5);
    this->addPiece(0,1,false,3,5);
    this->addPiece(2,0,false,3,5);
    this->addPiece(3,0,false,3,5);
    this->addPiece(0,3,true,3,5);
    this->addPiece(2,3,true,3,5);
    this->addPiece(5,5,true,3,5);
    this->addPiece(8,8,true,3,5);
    this->addPiece(7,7,true,3,5);
    setBoardSize(10,10);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFileClicked()
{
    ui->progressBar->setValue(100);
    this->removePiece(5,5);
    QString s = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.xml)"));
}


void MainWindow::exitApplicationClicked()
{
    this->close();
}

void MainWindow::removePiece(int loc_x, int loc_y)
{
    QLayoutItem *item = scrollLayout->itemAtPosition(loc_x, loc_y);
    if (item!=NULL) item->widget()->close();
    this->elemCurr-=1;
    ui->piecesCurrLcdNumber->display(this->elemCurr);
    double val = (this->elemCurr/ui->piecesInitLcdNumber->value())*100;
    ui->progressBar->setValue(val+((int)((val-floor(val))>0.5)?1:0));
}

void MainWindow::setBoardSize(int width, int height)
{
   viewport->setFixedSize(HALF_PIECE_DIM*width+(width*5),HALF_PIECE_DIM*height+(height*5));
   scrollLayout->setSpacing(0);
   for (int i=0;i<height;i++)
   {
       scrollLayout->setColumnStretch(i,1);
       scrollLayout->setColumnMinimumWidth(i,1);
   }
   for (int i=0;i<width;i++)
   {
       scrollLayout->setRowStretch(i,1);
       scrollLayout->setRowMinimumHeight(i,1);
   }
   for (int i=0;i<height;i++)
   {
       scrollLayout->setColumnStretch(i,1);
       scrollLayout->setColumnMinimumWidth(i,1);
   }
}



void MainWindow::addPiece(int loc_x, int loc_y, bool isVertical, int val1, int val2)
{
    QString str1= QString::number(val1);
    QString str2= QString::number(val2);
    QPushButton *piece = new QPushButton;
    piece->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    piece->setFlat(true);
    if (isVertical){
    scrollLayout->addWidget(piece,loc_x,loc_y,2,1,0);
    piece->setFixedSize(HALF_PIECE_DIM,HALF_PIECE_DIM*2);
    piece->setStyleSheet("QPushButton{ background:url(pictures/blankpiecevert.png) no-repeat center; }");
    piece->setText(str1+"\n\n\n\n\n"+str2);
    }
    else{
    scrollLayout->addWidget(piece,loc_x,loc_y,1,2,0);
    piece->setFixedSize(2*HALF_PIECE_DIM,HALF_PIECE_DIM);
    piece->setStyleSheet("QPushButton{ background:url(pictures/blankpiecehor.png) no-repeat center; }");
    piece->setText(str1+"             "+str2);
    }
    this->elemCurr+=1;
    ui->piecesInitLcdNumber->display(this->elemCurr);
    ui->piecesCurrLcdNumber->display(this->elemCurr);
}
