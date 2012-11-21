#include "approximate_s.h"

/*
  Approximate algorithm by Stanislaw Peryt
  Computer Science - Group AR
  */
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// computational functions
///////////////////////////////////////////////////////////////////////////////

//
//Constructor for the problem solving class.
//Puts the elements in a 2-dimensional array - matrix form
//
approximate_s::approximate_s(domino_problem_s &problem_s)
{
    neighbors = new QVector<domino_elem_located*>();
    removedPreviously = new QVector<domino_elem_located*>();
    present = new QVector<domino_elem_located*>(problem_s.elem_list);
    this->board_height = problem_s.board_height;
    this->board_width = problem_s.board_width;
    board = new half_elem**[this->board_width];
    for (int i = 0; i < this->board_width; i++)
        board[i] = new half_elem*[this->board_height];
    for (int i = 0; i < this->board_width; i++)
        for (int j = 0; j < this->board_height; j++)
            board[i][j] = NULL;
    foreach (domino_elem_located* el, problem_s.elem_list){
        board[el->h1.loc_x()][el->h1.loc_y()] = &el->h1;
        board[el->h2.loc_x()][el->h2.loc_y()] = &el->h2;
    }
    setPieceByPiece(false);
    isDelay=false; delay=0;
}

void approximate_s::run()
{
    solve();
}

void approximate_s::test(QVector<domino_elem_located *> pieces)
{
    int i = 0;
    std::cout<<std::endl;
    foreach (domino_elem_located* piece, pieces)
    {
        //std::cout<<"index: "<<i<<",value x: "<<(piece->h1.value)<<",value y: "<<(piece->h2.value)<<std::endl;
        if(piece->value1()==0||piece->value2()==0)
        {
            std::cout<<"\n possible root at: ";
            this->neighbors->push_back(piece);
        }
        std::cout<<"index: "<<i<<piece<<std::endl;
        i++;
    }
                    //spc()+ bolden2("Y: ")+QString::number(piece->y)+
                    //spc()+ bolden2("Orientation: ")+OrToStr(piece->is_vertical)+"<br>"
}

void approximate_s::findRoots()
{
    foreach (domino_elem_located* piece, *present)
    {
        if(piece->value1()==0||piece->value2()==0)
        {
            this->neighbors->push_back(piece);
        }
    }
}

bool approximate_s::classify(domino_elem_located* piece){
/*
    if(piece->is_vertical)
    {//original Piece vertical
        //h1
        //up case
        //y>0 determine that we are still on the board
        //question: should we really start with y=piece.y?
        for(int y=piece->h1.loc_y();y>0;--y)
        {
            domino_elem_located* otherPiece = board[piece->h1.loc_x()][y]->owner;
            if(otherPiece!=NULL)
            {
                //other Piece vertical
                if(otherPiece->is_vertical)
                {
                    if(piece->h1.loc_y()-otherPiece->h2.loc_y()==otherPiece->value2()+1)
                    {
                        //it needs you, but maybe it has also other possibilities to be taken
                        //anyway, wait.
                        //wait()
                        return false;
                    }
                }
                else//other Piece horizontal
                {
                    //determine which half of the other piece is in your line
                    //half_elem otherPieceHE;
                    if(otherPiece->h1.loc_x()==piece->h1.loc_x())
                    {
                        //otherPieceHE=otherPiece->h1;
                        //check if it needs you, i.e. if you are in the scope
                        if(piece->h1.loc_y()-otherPiece->h1.loc_y()==otherPiece->value1()+1)
                        {
                            //check if the second piece, which is needed for the otherPiece
                            //is not empty

                            //first check if you are in the board
                            if((otherPiece->h2.loc_y()+otherPiece->h2.value+1)<(this->board_height))
                            {
                                //there is another piece, which is needed, or there is a border in good place
                                if((board[otherPiece->h2.loc_x()][otherPiece->h2.loc_y()+otherPiece->h2.value+1]!=NULL)||(otherPiece->h2.loc_y()+otherPiece->h2.value+1==this->board_height))
                                {
                                    //you should wait
                                    return false;
                                }
                                else
                                {
                                    //second piece was not waiting, so you can remove yourself
                                    return true;
                                }
                            }
                        }
                    }else //h2
                    {
                        //otherPieceHE=otherPiece->h2;
                        //check if it needs you, i.e. if you are in the scope
                        if(piece->h1.loc_y()-otherPiece->h2.loc_y()==otherPiece->value2()+1)
                        {
                            //check if the second piece, which is needed for the otherPiece
                            //is not empty

                            //first check if you are in the board
                            if(otherPiece->h1.loc_y()+otherPiece->h1.value+1<this->board_height)
                            {
                                //there is another piece, which is needed, or there is a border in good place
                                if((board[otherPiece->h1.loc_x()][otherPiece->h1.loc_y()+otherPiece->h1.value+1]!=NULL)||(otherPiece->h1.loc_y()+otherPiece->h1.value+1==this->board_height))
                                {
                                    //you should wait
                                    return false;
                                }
                                else
                                {
                                    //second piece was not waiting, so you can remove yourself
                                    return true;
                                }
                            }
                        }
                    }
                }

            }
        }
        //left case
        for(int x=piece->h1.loc_x();x>0;--x)
        {
            int y = piece->h1.loc_y();
            if( board[x][y]==NULL)
            {
                domino_elem_located* otherPiece = NULL;
            }
            else
            {
                domino_elem_located* otherPiece = board[x][piece->h1.loc_y()]->owner;
            }
            if(otherPiece!=NULL)
            {
                //other Piece horizontal
                if(!otherPiece->is_vertical)
                {
                    if(piece->h1.loc_x()-otherPiece->h2.loc_x()==otherPiece->value2()+1)
                    {
                        //it needs you, but maybe it has also other possibilities to be taken
                        //anyway, wait.
                        //wait()
                        return false;
                    }
                }
                else//other Piece vertical
                {
                    //determine which half of the other piece is in your line
                    //half_elem otherPieceHE;
                    if(otherPiece->h1.loc_y()==piece->h1.loc_y())
                    {
                        //you are at the same height
                        //check if it needs you, i.e. if you are in the scope
                        if(piece->h1.loc_x()-otherPiece->h1.loc_x()==otherPiece->value1()+1)
                        {
                            //since you are at the same heigth, the only possibility
                            //when you are in the scope is when the second value on the
                            //otherPiece is equal to it's first value
                            if(otherPiece->value1()==otherPiece->value2())
                            {
                                //you should wait..
                                return false;
                            }
                        }
                    }
                    else
                        //h2 at your h1 y-location
                    {
                        //otherPieceHE=otherPiece->h2;
                        //check if it needs you, i.e. if you are in the scope
                        if(piece->h1.loc_x()-otherPiece->h2.loc_x()==otherPiece->value2()+1)
                        {
                            //check if the second piece, which is needed for the otherPiece
                            //is not empty

                            //first check if you are in the board
                            if(otherPiece->h1.loc_x()+otherPiece->h1.value+1<this->board_width)
                            {
                                //then check if there is another piece, which is needed, or there is a border in good place
                                if((board[otherPiece->h1.loc_x()+otherPiece->h1.value+1][otherPiece->h1.loc_y()]!=NULL)||(otherPiece->h1.loc_x()+otherPiece->h1.value+1==this->board_height))
                                {
                                    //you should wait
                                    return false;
                                }
                                else
                                {
                                    //second piece was not waiting, so you can remove yourself
                                    return true;
                                }
                            }
                        }
                    }
                }

            }
        }
        //right case
        //maybe board_width-1?
        for(int x=piece->h1.loc_x();x<this->board_width;++x)
        {
            domino_elem_located* otherPiece = board[x][piece->h1.loc_y()]->owner;
            if(otherPiece!=NULL)
            {
                //other Piece horizontal
                if(!otherPiece->is_vertical)
                {
                    if(otherPiece->h1.loc_x()-piece->h1.loc_x()==otherPiece->value1()+1)
                    {
                        //it needs you, but maybe it has also other possibilities to be taken
                        //anyway, wait.
                        //wait()
                        return false;
                    }
                }
                else//other Piece vertical
                {
                    //determine which half of the other piece is in your line
                    //half_elem otherPieceHE;
                    if(otherPiece->h1.loc_y()==piece->h1.loc_y())
                    {
                        //you are at the same height
                        //check if it needs you, i.e. if you are in the scope
                        if(otherPiece->h1.loc_x()-piece->h1.loc_x()==otherPiece->value1()+1)
                        {
                            //since you are at the same heigth, the only possibility
                            //when you are in the scope is when the second value on the
                            //otherPiece is equal to it's first value
                            if(otherPiece->value1()==otherPiece->value2())
                            {
                                //you should wait..
                                return false;
                            }
                        }
                    }
                    else
                        //h2 at your h1 y-location
                    {
                        //otherPieceHE=otherPiece->h2;
                        //check if it needs you, i.e. if you are in the scope
                        if(otherPiece->h2.loc_x()-piece->h1.loc_x()==otherPiece->value2()+1)
                        {
                            //check if the second piece, which is needed for the otherPiece
                            //is not empty

                            //first check if you are in the board
                            if(otherPiece->h1.loc_x()-otherPiece->h1.value>0)
                            {
                                //then check if there is another piece, which is needed, or there is a border in good place
                                if((board[otherPiece->h1.loc_x()-otherPiece->h1.value][otherPiece->h1.loc_y()]!=NULL)||(otherPiece->h1.loc_x()-otherPiece->h1.value==0))
                                {
                                    //you should wait
                                    return false;
                                }
                                else
                                {
                                    //second piece was not waiting, so you can remove yourself
                                    return true;
                                }
                            }
                        }
                    }
                }

            }
        }

        //h2
        //down case
        //left
        //right
    }
    else
        //original Piece horizontal
    {
        //h1:
        //left,up,down
        //h2:
        //up,down,right
        return true;
    }

        return true;
*/
    return true;
}
//
//Check ih half_element has equal value to it's distance from the boarders
//
bool approximate_s::checkHalfBoardIndependence(int x, int y, half_direction direction)
{
    switch (direction)
    {
        case left:
        if (board[x][y]->value == x)
           return true;
        break;
        case right:
        if (board[x][y]->value == this->board_width-x-1) return true;
        break;
        case up:
        if (board[x][y]->value == y) return true;
        break;
        case down:
        if (board[x][y]->value == this->board_height-y-1) return true;
        break;
    }
    return false;
}



//
//Checks if an element is removable from empty board
//
bool approximate_s::isIndependent(int x, int y)
{
    bool isVertical = board[x][y]->owner->is_vertical;
    if (!isVertical)
    {
           if (checkHalfBoardIndependence(board[x][y]->loc_x(), board[x][y]->loc_y(), left))
                return true;
           else if (checkHalfBoardIndependence( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), right))
                return true;
           else if (checkHalfBoardIndependence( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), down) &&
                checkHalfBoardIndependence( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), down) )
            return true;
            else if (checkHalfBoardIndependence( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), up) &&
                checkHalfBoardIndependence( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), up) )
            return true;
   } else
    {
        if (checkHalfBoardIndependence( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), up))
            return true;
        else if (checkHalfBoardIndependence( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), down))
            return true;
        else if (checkHalfBoardIndependence( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), left) &&
            checkHalfBoardIndependence( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), left) )
        return true;
        else if (checkHalfBoardIndependence( board[x][y]->owner->h1.loc_x(), board[x][y]->owner->h1.loc_y(), right) &&
            checkHalfBoardIndependence( board[x][y]->owner->h2.loc_x(), board[x][y]->owner->h2.loc_y(), right) )
        return true;
    }
    return false;
}
//
//determine new neighbors into neighbor list
//
void approximate_s::determineNewNeighbors()
{
    if(removedPreviously!=NULL){
        foreach(domino_elem_located* piece,*removedPreviously)
        {
            domino_elem_located* newPiece;
            int X=piece->x;
            int Y=piece->y;

            //horizontal
            if((X-1)>=0&&board[X-1][Y]!=NULL)
                newPiece = board[X-1][Y]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((Y-1)>=0&&board[X][Y-1]!=NULL)
                newPiece = board[X][Y-1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((Y+1)<this->board_height&&(board[X][Y+1])!=NULL)
                newPiece = board[X][Y+1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X+1)<this->board_width&&(Y-1)>=0&&board[X+1][Y-1]!=NULL)
                newPiece = board[X+1][Y-1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X+1)<this->board_width&&((Y+1)<this->board_height)&&board[X+1][Y+1]!=NULL)
                newPiece = board[X+1][Y+1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X+2)<this->board_width&&board[X+2][Y]!=NULL)
                newPiece = board[X+2][Y]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            //vertical
            if((Y-1)>=0&& board[X][Y-1]!=NULL)
                newPiece = board[X][Y-1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X-1)>=0 && board[X-1][Y]!=NULL)
                newPiece = board[X-1][Y]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X+1)<this->board_width&&board[X+1][Y]!=NULL)
                newPiece = board[X+1][Y]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X-1)>=0&&(Y+1)<this->board_height && board[X-1][Y+1]!=NULL)
                newPiece = board[X-1][Y+1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((X+1)<this->board_width&&(Y+1)<this->board_height && board[X+1][Y+1]!=NULL)
                newPiece = board[X+1][Y+1]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
            if((Y+2)<this->board_height&&board[X][Y+2]!=NULL)
                newPiece = board[X][Y+2]->owner;
            if(!neighbors->contains(newPiece))
                neighbors->push_back(newPiece);
        }
    }
}
//
//solving domino problem
//
void approximate_s::solve()
{
    int timeElapsed = 0;
    bool stop = false;
    myTimer.start();
    findRoots();
    //std::cout<<"\n roots found"<<std::endl;
    while(!stop){
        //removedPreviously->clear();
        foreach (domino_elem_located* piece, *neighbors)
        {
            //std::cout<<piece<<std::endl;
            //std::cout<<"\n before"<<std::endl;
            if(piece!=NULL){
            if(isRemovable(piece->x,piece->y))
                {
                    //std::cout<<"\n is removable?"<<std::endl;
                    if(isIndependent(piece->x,piece->y))
                    {
                        //not working properly i.e. 00 is removed..
                        //std::cout<<"\n is independent?"<<std::endl;
                        //std::cout<<"\n board width: "<<this->board_width<<", board height: "<<this->board_height<<std::endl;
                    }
                    else
                    {
                        //if(classify(piece))
                        //{
                            remove_piece(piece->x,piece->y);
                            //std::cout<<"\n 1. removed: "<<piece<<std::endl;
                        //}
                    }
                }
            }
            //std::cout<<"\n after"<<std::endl;
        }
        //determine new neighbors into temp
        QVector<domino_elem_located*>* temp = new QVector<domino_elem_located*>();
        foreach(domino_elem_located* piece,*neighbors){
            temp->push_back(piece);
        }
        std::cout<<"";
        //std::cout<<"\n try determine new neighbors"<<std::endl;
        determineNewNeighbors();
        //std::cout<<"\n succed determining"<<std::endl;
        //clear removedPreviously
        removedPreviously->clear();
        if(temp->operator ==(*neighbors))
        {
            bool changed = false;
            foreach(domino_elem_located* piece,*neighbors){
                if(piece!=NULL){
                    if(isRemovable(piece->x,piece->y)){
                        //if(classify(piece))
                        //{
                            remove_piece(piece->x,piece->y);
                            //std::cout<<"\n 2. temp==neighbors, removed: "<<piece<<std::endl;
                            changed=true;
                            break;
                        //}
                    }
                }
            }
            if(!changed)
            {
                foreach(domino_elem_located* piece,*present){
                if(isRemovable(piece->x,piece->y)){
                    //if(classify(piece))
                    //{
                        remove_piece(piece->x,piece->y);
                        //std::cout<<"\n 3. removed: "<<piece<<std::endl;
                        changed=true;
                        break;
                    //}
                    }
                }
            }
            if(!changed){
                stop=true;
            }
        }
        else
        {
            //std::cout<<"\n NEW NEIGHBORS DIFFRENT: "<<std::endl;
            //neighbors=temp;
        }
    }
    timeElapsed=myTimer.elapsed();
    sendResultsToGui(timeElapsed);

}
//
//checks if element value is equal to distance to the other piece in given direction
//
bool approximate_s::checkSide(int x, int y, half_direction direction)
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

//
//Checks if an element on location X:Y is removable
//
bool approximate_s::isRemovable(int x, int y)
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


//
//Removes element from a board
//
void approximate_s::remove_piece(int x, int y)
{
        removed.push_back(board[x][y]->owner);
        removedPreviously->push_back(board[x][y]->owner);
        neighbors->remove(neighbors->indexOf(board[x][y]->owner));
        present->remove(present->indexOf(board[x][y]->owner));
        board[board[x][y]->other_half->loc_x()][board[x][y]->other_half->loc_y()] = NULL;
        board[board[x][y]->loc_x()][board[x][y]->loc_y()] = NULL;
}


bool approximate_s::toBoolean(int value)
{
    return (value>0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// auxiliary functions - not influencing the computation
///////////////////////////////////////////////////////////////////////////////

void approximate_s::sendResultsToGui(int timeElapsed)
{
    mutex.lock();
    foreach(domino_elem_located* elem,removed)
    {
            if (this->isDelay) this->msleep(this->delay);
            else if (isPieceByPiece) mutex.lock();
            emit threadRemovePiece(elem->y,elem->x);
    }
    emit threadComputationOver(timeElapsed,present,removed);
}

void approximate_s::setPieceByPiece(bool on)
{
    this->isPieceByPiece = on;
}

void approximate_s::setDelay(int delay)
{
    this->isDelay = true;
    this->delay = delay;
}

void approximate_s::increaseMutex(){
    mutex.unlock();
}

void approximate_s::stopExecution(){
    delay=0;
    isPieceByPiece=false;
    increaseMutex();
}

void approximate_s::print_out() //prints out the board matrix on standard output
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
