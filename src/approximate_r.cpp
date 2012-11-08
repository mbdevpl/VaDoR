#include "approximate_r.h"

/*
  Approximate algorithm by Radoslaw Lojek
  Computer Science - Group AR
  */
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// computational functions - influencing the computation
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
        for (int y = 0; y < this->board_height ; y++)
            for (int x = 0; x < this->board_width; x++)
                    if (present->size() > 0 && isRemovable(x,y))
                    {
                        remove_piece(x,y);
                        iterPiecesRemoved += 1;
                    }
    } while (iterPiecesRemoved > 0);

    timeElapsed=myTimer.elapsed();
    sendResultsToGui(timeElapsed);
}

/*
  Assigns values for each domino half piece, depending on element removability.
  Adds 2, if an element can be removed with regard to the shorter side.
  Adds 1, id a half element satisfies condition of removability, with regard to the longer side.
*/
int approximate_r::countFieldValue(int x, int y)
{
    half_direction direction = board[x][y]->direction;
    int result = 0;
    int cnt=0,xt=x,yt=y;
    if (direction == right || direction == up || direction == down){
        if (xt > 0)
            while(board[--xt][yt]==NULL){ cnt++; if (xt==0)break;}
        if (board[x][y]->value == cnt && direction == right) result += 2;
        else if (board[x][y]->value == cnt) result += 1;
    }
    cnt=0;xt=x;yt=y;
    if (direction == left || direction == up || direction == down){
        if (xt < this->board_width-1)
            while(board[++xt][yt]==NULL){ cnt++; if (xt==this->board_width-1)break;}
        if (board[x][y]->value == cnt && direction == left) result += 2;
        else if (board[x][y]->value == cnt) result += 1;
    }
    cnt=0;xt=x;yt=y;
    if (direction == down || direction == right || direction == left){
        if (yt > 0)
            while(board[xt][--yt]==NULL){ cnt++; if (yt==0)break;}
        if (board[x][y]->value == cnt && direction == down) result += 2;
        else if (board[x][y]->value == cnt) result += 1;
    }
    cnt=0;xt=x;yt=y;
    if (direction == up || direction == right || direction == left){
        if (yt < this->board_height-1)
            while(board[xt][++yt]==NULL){ cnt++; if (yt==this->board_height-1)break;}
        if (board[x][y]->value == cnt && direction == up) result += 2;
        else if (board[x][y]->value == cnt) result += 1;
    }
    return result;
}

/*
  Checks if an element on location X:Y is removable
  */
bool approximate_r::isRemovable(int x, int y)
{
    int me,sibling=0;
    if (board[x][y] != NULL)
    {
        if (board[x][y]->value == 0) return true;
        else
        {
            me = countFieldValue(x,y);
            switch(board[x][y]->direction){
            case up:
                sibling = countFieldValue(x,y-1);
                break;
            case down:
                sibling = countFieldValue(x,y+1);
                break;
            case left:
                sibling = countFieldValue(x-1,y);
                break;
            case right:
                sibling = countFieldValue(x+1,y);
                break;
            default:
                break;
            }
            if ((me+sibling)>=2) return true;
        }
    } else
        return false;
    return false;
}

/*
  Removes element from a board
  */
void approximate_r::remove_piece(int x, int y)
{
    for(int i =0; i < present->size();i++)
        if ((present->at(i)->h1.loc_x()==x && present->at(i)->h1.loc_y()==y)||
                (present->at(i)->h2.loc_x()==x && present->at(i)->h2.loc_y()==y))
        {
            board[present->at(i)->h1.loc_x()][present->at(i)->h1.loc_y()] = NULL;
            board[present->at(i)->h2.loc_x()][present->at(i)->h2.loc_y()] = NULL;
            removed.push_back(present->at(i));
            present->remove(i);
            break;
        }
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
