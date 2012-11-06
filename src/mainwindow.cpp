#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), scrollLayoutMembers(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    selectedAlgorithm = accurate;
    setGuiForAccurate(true);
    ui->progressBar->setValue(0);
    this->elemCurr = 0;
    scrollLayout = new QGridLayout;
    viewport = new QWidget;
    ui->boardScrollArea->setWidget(viewport);
    viewport->setLayout(scrollLayout);
    qRegisterMetaType< QVector<domino_elem_located*> >("QVector<domino_elem_located*>");

    connect( ui->ExitMenu, SIGNAL(triggered()), this, SLOT( exitApplicationClicked() ) );
    connect( ui->OpenMenu, SIGNAL(triggered()), this, SLOT( openFileClicked() ) );
    connect( ui->runDirectButton, SIGNAL(clicked()), this, SLOT( runClicked() ) );
    connect( ui->runPieceByPieceButton, SIGNAL(clicked()), this, SLOT( runClicked() ) );
    connect( ui->stopRunDirectButton, SIGNAL(clicked()), this, SLOT( stopClicked() ) );
    connect( ui->stopRunStepByStepButton, SIGNAL(clicked()), this, SLOT( stopClicked() ) );
    connect( ui->removeNextPieceButton, SIGNAL(clicked()), this, SLOT( removeNextPieceClicked() ) );
    connect( ui->goToEndButton, SIGNAL(clicked()), this, SLOT( goToEndClicked() ) );
    connect( ui->accurateAlgRadioBox, SIGNAL(clicked()), this, SLOT( algorithmChanged()) );
    connect( ui->matiAlgRadioBox, SIGNAL(clicked()), this, SLOT( algorithmChanged()) );
    connect( ui->radekAlgRadioBox, SIGNAL(clicked()), this, SLOT( algorithmChanged()) );
    connect( ui->stanislawAlgRadioBox, SIGNAL(clicked()), this, SLOT( algorithmChanged()) );
    QObject::connect(&summary_win, SIGNAL(reloadGUI()), this, SLOT(resetGUI()), Qt::QueuedConnection);
    QObject::connect(ui->actionClearBoard, SIGNAL(triggered()), this, SLOT(clearBoard()));
}


void MainWindow::resetGUI()
{

}

void MainWindow::algorithmChanged()
{
    if (ui->accurateAlgRadioBox->isChecked())
    {
        selectedAlgorithm = accurate;
        setGuiForAccurate(true);
    }
    else if (ui->matiAlgRadioBox->isChecked())
    {
        selectedAlgorithm = mateusz;
        setGuiForAccurate(false);
    }
    else if (ui->radekAlgRadioBox->isChecked())
    {
        selectedAlgorithm = radek;
        setGuiForAccurate(false);
    }
    else if (ui->stanislawAlgRadioBox->isChecked())
    {
        selectedAlgorithm = stanislaw;
        setGuiForAccurate(false);
    }
}

void MainWindow::setGuiForAccurate(bool value)
{
    ui->groupBox_3->setEnabled(!value);
    ui->tabWidget->setTabEnabled(1,!value);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runClicked()
{
    if (problem_r.elem_list.size() > 0)
    {
        QPushButton *currButton = (QPushButton*)sender();
        QString but_name = currButton->objectName();
        if (but_name == "runDirectButton")
        {
            switch (selectedAlgorithm){
            case accurate:
                accThread = new accurate_thread(*input);
                QObject::connect(accThread, SIGNAL(threadRemovePiece(int,int)), this, SLOT(removePiece(int,int)), Qt::QueuedConnection);
                QObject::connect(accThread, SIGNAL(threadComputationOver(int, QVector<domino_elem_located*>*, QVector<domino_elem_located*>)), this, SLOT(computationOver(int, QVector<domino_elem_located*>*, QVector<domino_elem_located*>)), Qt::QueuedConnection);
                accThread->start();

                setGuiEnabledWhileComputing(false,false);
                break;
            case mateusz:
                break;
            case radek:
                apprThread_r = new approximate_r(problem_r);
                if (ui->isDelayCheckBox->isChecked() && ui->spinBox->value()>0) apprThread_r->setDelay(ui->spinBox->value());
                QObject::connect(apprThread_r, SIGNAL(threadRemovePiece(int,int)), this, SLOT(removePiece(int,int)), Qt::QueuedConnection);
                QObject::connect(apprThread_r, SIGNAL(threadComputationOver(int, QVector<domino_elem_located*>*, QVector<domino_elem_located*>)), this, SLOT(computationOver(int, QVector<domino_elem_located*>*, QVector<domino_elem_located*>)), Qt::QueuedConnection);
                apprThread_r->start();
                setGuiEnabledWhileComputing(false,false);
                break;
            case stanislaw:
                break;
            }

        }
        else if (but_name == "runPieceByPieceButton")
        {
            switch (selectedAlgorithm){
            case accurate:
                break;
            case mateusz:
                break;
            case radek:
                apprThread_r = new approximate_r(problem_r);
                apprThread_r->setPieceByPiece(true);
                QObject::connect(apprThread_r, SIGNAL(threadRemovePiece(int,int)), this, SLOT(removePiece(int,int)), Qt::QueuedConnection);
                QObject::connect(apprThread_r, SIGNAL(threadComputationOver(int, QVector<domino_elem_located*>*, QVector<domino_elem_located*>)), this, SLOT(computationOver(int, QVector<domino_elem_located*>*, QVector<domino_elem_located*>)), Qt::QueuedConnection);
                apprThread_r->start();
                setGuiEnabledWhileComputing(false,true);
                break;
            case stanislaw:
                break;
        }
        }
    } else {
        msgBox.setWindowTitle("Message");
        msgBox.setInformativeText("Cannot start computation - the board is empty!");
        msgBox.exec();
    }
}

void MainWindow::openFileClicked()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Vador files (*.xml *.txt)"));

    if (!s.isEmpty())
    {
        problem_r = domino_problem_r();
        problem_r.load(s);
        input = new domino_problem_input(s.toStdString());
        clearBoard(); // here is the place, where clearing the board should appear, but i am not shure if the trouble lies in not removed pieces ;/
        setBoardSize(problem_r.board_width,problem_r.board_height);
        foreach (domino_elem* el, problem_r.elem_list)
            addPiece(el->h1.loc_x(), el->h1.loc_y(), el->is_vertical, el->h1.value, el->h2.value);
        setBoardSize(problem_r.board_width,problem_r.board_height);
        ui->progressBar->setValue(100);
    }
}

void MainWindow::removeNextPieceClicked()
{
    switch (selectedAlgorithm){
    case accurate:
        break;
    case mateusz:
        break;
    case radek:
        if (apprThread_r->workFlag){
            apprThread_r->increaseMutex();
        }
        break;
    case stanislaw:
        break;
    }
}

void MainWindow::goToEndClicked()
{
    switch (selectedAlgorithm){
    case accurate:
        break;
    case mateusz:
        break;
    case radek:
        if (apprThread_r->workFlag){
            apprThread_r->setPieceByPiece(false);
            apprThread_r->increaseMutex();
        }
        break;
    case stanislaw:
        break;
    }
}

void MainWindow::exitApplicationClicked()
{
    this->close();
}

void MainWindow::stopClicked()
{
    switch (selectedAlgorithm){
    case accurate:
        break;
    case mateusz:
        break;
    case radek:
        if (apprThread_r->workFlag)
            apprThread_r->stopExecution();
        break;
    case stanislaw:
        break;
    }
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
   viewport->setFixedSize(HALF_PIECE_DIM*width+(width*2),HALF_PIECE_DIM*height+(height*4));
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
   scrollLayout->invalidate();

}


void MainWindow::clearBoard()
{
//   QLayoutItem *child;
//   while ((child = scrollLayout->takeAt(0)) != 0) {
//      child->hide()
//     delete child;
//   }
   if(elemCurr > 0)
   {
      elemCurr = 0;
      ui->piecesInitLcdNumber->display(this->elemCurr);
      ui->piecesCurrLcdNumber->display(this->elemCurr);
   }

   if(scrollLayoutMembers.isEmpty())
      return;

   QList<QWidget*>::Iterator it = scrollLayoutMembers.begin();
   for(; it != scrollLayoutMembers.end(); ++it)
   {
      QWidget* widg = *it;
      widg->hide();
      scrollLayout->removeWidget(widg);
      //delete widg;
   }
   scrollLayoutMembers.clear();
   scrollLayout->invalidate();

   QList<QPushButton*>::Iterator i = buttonPieces.begin();
   for(; i != buttonPieces.end(); ++i)
   {
      (*i)->hide();
      delete *i;
   }
   buttonPieces.clear();

   //scrollLayout->children().clear();
}

void MainWindow::addPiece(int loc_x, int loc_y, bool isVertical, int val1, int val2)
{
    QString str1= QString::number(val1);
    QString str2= QString::number(val2);
    QPushButton *piece = new QPushButton;
    piece->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    piece->setFlat(true);
    if (isVertical)
    {
        scrollLayout->addWidget(piece,loc_y,loc_x,2,1,0);
        piece->setFixedSize(HALF_PIECE_DIM,HALF_PIECE_DIM*2);
        piece->setStyleSheet("QPushButton{ background:url(pictures/blankpiecevert.png) no-repeat center; }");
        piece->setText(str1+"\n\n\n\n\n"+str2);
        //buttonPieces.push_back(piece);
    }
    else
    {
        scrollLayout->addWidget(piece,loc_y,loc_x,1,2,0);
        piece->setFixedSize(2*HALF_PIECE_DIM,HALF_PIECE_DIM);
        piece->setStyleSheet("QPushButton{ background:url(pictures/blankpiecehor.png) no-repeat center; }");
        piece->setText(str1+"             "+str2);
    }
    buttonPieces.push_back(piece);
    scrollLayoutMembers.append(piece);

    this->elemCurr+=1;
    ui->piecesInitLcdNumber->display(this->elemCurr);
    ui->piecesCurrLcdNumber->display(this->elemCurr);
}

void MainWindow::computationOver(int time, QVector<domino_elem_located*>* present, QVector<domino_elem_located*> removed)
{
    summary_win.show();
    summary_win.publishResults(time,*present,removed);
}

void MainWindow::setGuiEnabledWhileComputing(bool value, bool isPieceByPiece)
{
    ui->frame->setEnabled(value);
    if (isPieceByPiece){
    ui->runPieceByPieceButton->setEnabled(value);
    ui->tabWidget->setTabEnabled(0,value);
    } else if (!isPieceByPiece){
        ui->runDirectButton->setEnabled(value);
        ui->tabWidget->setTabEnabled(1,value);
    }
}
