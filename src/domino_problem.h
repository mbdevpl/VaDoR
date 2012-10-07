#ifndef DOMINO_PROBLEM_H
#define DOMINO_PROBLEM_H

#include <sstream>
#include <string>
#include "simple_list.h"
#include "half_elem.h"
#include "domino_elem.h"
#include "domino_elem_located.h"
//#include "simple_tree.h"

// temporary!!!
#include <iostream>

#ifdef DEBUG
//#define DEBUG_DOMINO
#endif

class raw_domino_problem
{
public:
   simple_list<domino_elem_located,size_t> elements;
   size_t width;
   size_t height;
public:
   friend std::ostream& operator<<(std::ostream& os, const raw_domino_problem& pr)
   {
      os << '[' << pr.width << 'x' << pr.height << "]: " << pr.elements;
      return os;
   }
};

class domino_problem
{
public:
   // list of elements
   typedef simple_list<domino_elem,size_t> elem_list;
   // list of located elements
   typedef simple_list<domino_elem_located,size_t> elem_loc_list;
private:
   // list of pointers to elements
   typedef simple_list<domino_elem*,size_t> elem_ptr_list;
   // list of pointers to located elements
   typedef simple_list<domino_elem_located*,size_t> elem_loc_ptr_list;
private:
   typedef simple_list<half_elem*,size_t> board_column;
   typedef simple_list<board_column,size_t> whole_board;
private:
   // constant collection of domino_elem*
   elem_loc_ptr_list elements;
   size_t width;
   size_t height;
   // constant collection of half_elem* that come from 'elements' field
   whole_board initial_board;
   // collection of half_elem* that come from 'elements' field
   whole_board board;
   // collection of domino_elem*
   elem_loc_ptr_list on_board;
public:
   domino_problem(size_t width, size_t height,
                  const elem_loc_list& elements);
#ifdef DEBUG
   void demo_solution();
#endif // DEBUG
   simple_list<elem_list,size_t> all_best_soluions();
   std::string str();
private:
   static void removeAt(size_t x, size_t y, whole_board& b, elem_loc_ptr_list& backing_list);
   void removePiece(domino_elem_located* piece, whole_board& b, elem_loc_ptr_list& backing_list);
   static size_t distance(size_t x, size_t y, whole_board& b, half_direction dir)
   {
      if(dir == up)
      {
         // check up
         if(y == 0)
            return 0;
         size_t yy = 0;
         // looking for closest piece up from the current location
         bool found = false;
         for(yy = y-1; ; --yy)
         {
            if(b[x][yy] != 0)
            {
               found = true;
               break;
            }
            if(yy == 0)
               break;
         }
         if(found)
            ++yy; // to compensate for checking
         // if y == 3, and item was found in yy == 2, then the distance
         //  is not 1, but 0
         return y - yy;
      }
      else if(dir == down)
      {
         size_t height = b.first().value_ref_const().length();
         // check down
         if(y == height - 1)
            return 0;
         size_t yy = height - 1;
         // looking for closest piece down from the current location
         bool found = false;
         for(yy = y+1; ; ++yy)
         {
            if(b[x][yy] != 0)
            {
               found = true;
               break;
            }
            if(yy == height - 1)
               break;
         }
         if(found)
            --yy; // to compensate for checking, see comments in 'check up'
         return yy - y;
      }
      else if(dir == left)
      {
         // check left
         if(x == 0)
            return 0;
         size_t xx = 0;
         // looking for closest piece up from the current location
         bool found = false;
         for(xx = x-1; ; --xx)
         {
            if(b[xx][y] != 0)
            {
               found = true;
               break;
            }
            if(xx == 0)
               break;
         }
         if(found)
            ++xx; // to compensate for checking
         return x - xx;
      }
      else if(dir == right)
      {
         size_t width = b.length();
         // check right
         if(x == width - 1)
            return 0;
         size_t xx = width - 1;
         // looking for closest piece down from the current location
         bool found = false;
         for(xx = x+1; ; ++xx)
         {
            if(b[xx][y] != 0)
            {
               found = true;
               break;
            }
            if(xx == width - 1)
               break;
         }
         if(found)
            --xx; // to compensate for checking, see comments in 'check up'
         return xx - x;
      }
      throw std::runtime_error("distance in given direction cannot be measured");
   }
   static elem_loc_ptr_list find_all_possible_moves(whole_board& b/*, elem_ptr_list& elems*/)
   {
      elem_loc_ptr_list found;
      size_t width = b.length();
      size_t x = 0;
      for(whole_board::elem col = b.first(); col; ++col, ++x)
      {
         size_t height = col.value_ref().length();
         // checking short sides
         size_t y = 0;
         for(board_column::elem cell = col.value_ref().first(); cell; ++cell, ++y)
         {
            half_elem*& h = cell.value_ref();
            if(!h)
               continue;
            domino_elem_located*& e = h->owner;
            if(e->is_vertical)
            {
               // checking short sides in vertical pieces
               if(h->direction == down)
               {
                  // check up
                  if(h->value > y)
                     continue; // if value greater then distance from border...
                  size_t yy = 0;
                  if(y > 0)
                  {
                     // looking for closest piece up from the current location
                     bool found = false;
                     for(yy = y-1; ; --yy)
                     {
                        if(b[x][yy] != 0)
                        {
                           found = true;
                           break;
                        }
                        if(yy == 0)
                           break;
                     }
                     if(found)
                        ++yy; // to compensate for checking
                     // if y == 3, and item was found in yy == 2, then the distance
                     //  is not 1, but 0
                  }
                  if(h->value != y - yy)
                     continue;
                  found.append(e);

               }
               else if(h->direction == up)
               {
                  // check down
                  if(h->value > (height - 1 - y))
                     continue; // if value greater then distance from border...
                  size_t yy = height - 1;
                  if(y < height - 1)
                  {
                     // looking for closest piece down from the current location
                     bool found = false;
                     for(yy = y+1; ; ++yy)
                     {
                        if(b[x][yy] != 0)
                        {
                           found = true;
                           break;
                        }
                        if(yy == height - 1)
                           break;
                     }
                     if(found)
                        --yy; // to compensate for checking, see comments in 'check up'
                  }
                  if(h->value != yy - y)
                     continue;
                  found.append(e);
               }
               else
                  throw std::runtime_error("piece is vertical but another half is on the left/right");
            }
            else
            {
               // checking short sides in horizontal pieces
               if(h->direction == right)
               {
                  // check left
                  if(h->value > x)
                     continue; // if value greater then distance from border...
                  size_t xx = 0;
                  if(x > 0)
                  {
                     // looking for closest piece up from the current location
                     bool found = false;
                     for(xx = x-1; ; --xx)
                     {
                        if(b[xx][y] != 0)
                        {
                           found = true;
                           break;
                        }
                        if(xx == 0)
                           break;
                     }
                     if(found)
                        ++xx; // to compensate for checking
                  }
                  if(h->value != x - xx)
                     continue;
                  found.append(e);
               }
               else if(h->direction == left)
               {
                  // check right
                  if(h->value > (width - 1 - x))
                     continue; // if value greater then distance from border...
                  size_t xx = width - 1;
                  if(x < width - 1)
                  {
                     // looking for closest piece down from the current location
                     bool found = false;
                     for(xx = x+1; ; ++xx)
                     {
                        if(b[xx][y] != 0)
                        {
                           found = true;
                           break;
                        }
                        if(xx == width - 1)
                           break;
                     }
                     if(found)
                        --xx; // to compensate for checking, see comments in 'check up'
                  }
                  if(h->value != xx - x)
                     continue;
                  found.append(e);
               }
               else
                  throw std::runtime_error("piece is horizontal but another half is up/down");
            }
         }

         //checking long sides
         y = 0;
         for(board_column::elem cell = col.value_ref().first(); cell; ++cell, ++y)
         {
            half_elem*& h = cell.value_ref();
            if(!h)
               continue;
            domino_elem_located*& e = h->owner;
            // this loop checks all first halves, otherwise the elements
            //  would be checked two times
            if(h != &(e->h1))
               continue; // if it's not the first half...
            if(e->is_vertical)
            {
               // checking long sides in vertical pieces
               if(h->direction == down || h->direction == up)
               {
                  // check left
                  bool dst1_correct = false, dst2_correct = false;
                  if(distance(x, y, b, left) == e->h1.value)
                     dst1_correct = true;
                  if(h->direction == down)
                  {
                     if(distance(x, y+1, b, left) == e->h2.value)
                        dst2_correct = true;
                  }
                  else if(h->direction == up)
                  {
                     if(distance(x, y-1, b, left) == e->h2.value)
                        dst2_correct = true;
                  }
                  if(dst1_correct && dst2_correct)
                     found.append(e);
                  else
                  {
                     // checking left failed, so check right
                     if(distance(x, y, b, right) != e->h1.value)
                        continue;
                     if(h->direction == down)
                     {
                        if(distance(x, y+1, b, right) != e->h2.value)
                           continue;
                     }
                     else if(h->direction == up)
                     {
                        if(distance(x, y-1, b, right) != e->h2.value)
                           continue;
                     }
                     found.append(e);
                  }
               }
               else
                  throw std::runtime_error("piece is vertical but another half is on the left/right");
            }
            else
            {
               // checking short sides in horizontal pieces
               if(h->direction == right || h->direction == left)
               {
                  // check up
                  bool dst1_correct = false, dst2_correct = false;
                  if(distance(x, y, b, up) == e->h1.value)
                     dst1_correct = true;
                  if(h->direction == right)
                  {
                     if(distance(x+1, y, b, up) == e->h2.value)
                        dst2_correct = true;
                  }
                  else if(h->direction == left)
                  {
                     if(distance(x-1, y, b, up) == e->h2.value)
                        dst2_correct = true;
                  }
                  if(dst1_correct && dst2_correct)
                     found.append(e);
                  else
                  {
                     // checking up failed, so check down
                     if(distance(x, y, b, down) != e->h1.value)
                        continue;
                     if(h->direction == right)
                     {
                        if(distance(x+1, y, b, down) != e->h2.value)
                           continue;
                     }
                     else if(h->direction == left)
                     {
                        if(distance(x-1, y, b, down) != e->h2.value)
                           continue;
                     }
                     found.append(e);
                  }
               }
               else
                  throw std::runtime_error("piece is horizontal but another half is up/down");
            }

         }
      }
      return found;
   }
};

#endif // DOMINO_PROBLEM_H
