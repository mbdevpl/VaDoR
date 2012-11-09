#include "approximate_r.h"

/*
  Approximate algorithm by Radoslaw Lojek
  Computer Science - Group AR
  */
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// computational functions
///////////////////////////////////////////////////////////////////////////////

/*
  Constructor for the problem solving class.
  Puts the elements in a 2-dimensional array - matrix form
  */
approximate_r::approximate_r(domino_problem_r &problem_r)
{
    present = new QVector<domino_elem_located*>(problem_r.elem_list);
    this->board_height = problem_r.board_height;
    this->board_width = problem_r.board_width;
    board = new half_elem**[this->board_width];
    for (int i = 0; i < this->board_width; i++)
        board[i] = new half_elem*[this->board_height];
    for (int i = 0; i < this->board_width; i++)
        for (int j = 0; j < this->board_height; j++)
            board[i][j] = NULL;
    foreach (domino_elem_located* el, problem_r.elem_list){
        board[el->h1.loc_x()][el->h1.loc_y()] = &el->h1;
        board[el->h2.loc_x()][el->h2.loc_y()] = &el->h2;
    }
    setPieceByPiece(false);
    isDelay=false;
}

/*
  Chief part of an algorithm
  goes through all the elements in the matrix, and removes everything that is possible to remove
  efficiency of the algorithm is not very high, however speed is mind-blowing :)
  */
void approximate_r::run()
{
    int timeElapsed = 0;
    int iterPiecesRemoved = 0;
    myTimer.start();

    do {
        iterPiecesRemoved = 0;
        for (int y = 0; y <this->board_height ; y++)
            for (int x = 0; x < this->board_width; x++)
                if (board[x][y]!=NULL && isRemovable(x,y))
                    {
                        remove_piece(x,y);
                        iterPiecesRemoved += 1;
                    }
    } while (iterPiecesRemoved > 0);

    timeElapsed=myTimer.elapsed();
    sendResultsToGui(timeElapsed);
}

/*
  checks if element value is equal to distance to the other piece in given direction
  */
bool approximate_r::checkSide(int x, int y, half_direction direction)
{
    int cnt=0,xt=x,yt=y;
    switch (direction)
    {
        case left:
        if (xt > 0)
            while(board[--xt][yt]==NULL){ cnt++; if (xt==0)break;}
        if (board[x][y]->value == cnt)
            return true;
        break;
        case right:
        if (xt < this->board_width-1)
            while(board[++xt][yt]==NULL){ cnt++; if (xt==this->board_width-1)break;}
        if (board[x][y]->value == cnt) return true;
        break;
        case up:
        if (yt > 0)
            while(board[xt][--yt]==NULL){ cnt++; if (yt==0)break;}
        if (board[x][y]->value == cnt) return true;
        break;
        case down:
        if (yt < this->board_height-1)
            while(board[xt][++yt]==NULL){ cnt++; if (yt==this->board_height-1)break;}
        if (board[x][y]->value == cnt) return true;
        break;
    }
    return false;
}

/*
  Checks if an element on location X:Y is removable
  */
bool approximate_r::isRemovable(int x, int y)
{
    bool isVertical = board[x][y]->owner->is_vertical;
    if (!isVertical)
    {
           if (checkSide( board[x][y]->loc_x(), board[x][y]->loc_y(), left))
                return true;
           else if (checkSide( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), right))
                return true;
           else if (checkSide( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), down) &&
                checkSide( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), down) )
            return true;
            else if (checkSide( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), up) &&
                checkSide( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), up) )
            return true;
   } else
    {
        if (checkSide( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), up))
            return true;
        else if (checkSide( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), down))
            return true;
        else if (checkSide( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), left) &&
            checkSide( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), left) )
        return true;
        else if (checkSide( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), right) &&
            checkSide( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), right) )
        return true;
    }
    return false;
}


/*
  Removes element from a board
  */
void approximate_r::remove_piece(int x, int y)
{
        removed.push_back(board[x][y]->owner);
        present->remove(present->indexOf(board[x][y]->owner));
        board[board[x][y]->other_half->loc_x()][board[x][y]->other_half->loc_y()] = NULL;
        board[board[x][y]->loc_x()][board[x][y]->loc_y()] = NULL;
}

bool approximate_r::toBoolean(int value)
{
    return (value>0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// auxiliary functions - not influencing the computation
///////////////////////////////////////////////////////////////////////////////

void approximate_r::sendResultsToGui(int timeElapsed)
{
    mutex.lock();
    foreach(domino_elem_located* elem,removed)
    {
            if (delay>0) this->msleep(this->delay);
            else if (isPieceByPiece) mutex.lock();
            emit threadRemovePiece(elem->y,elem->x);
    }
    emit threadComputationOver(timeElapsed,present,removed);
}

void approximate_r::setPieceByPiece(bool on)
{
    this->isPieceByPiece = on;
}

void approximate_r::setDelay(int delay)
{
    this->isDelay = true;
    this->delay = delay;
}

void approximate_r::increaseMutex(){
    mutex.unlock();
}

void approximate_r::stopExecution(){
    delay=0;
    isPieceByPiece=false;
    increaseMutex();
}

void approximate_r::print_out() //prints out the board matrix on standard output
{
    for (int y = 0; y < this->board_height ; y++) {
        for (int x = 0; x < this->board_width; x++) {
            if (board[x][y] != NULL)
                std::cout <<  board[x][y]->value << " ";
            else
                std::cout << "N ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n\n";
}

///////////////////////////////////////////////////////////////////////////////
