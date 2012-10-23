#ifndef DOMINO_PROBLEM_H
#define DOMINO_PROBLEM_H

#include <sstream>
#include <string>
#include "simple_list.h"
#include "half_elem.h"
#include "domino_elem.h"
#include "domino_elem_located.h"
#include "domino_problem_input.h"
//#include "simple_tree.h"

#ifdef DEBUG
#include <iostream>
//#define DEBUG_DOMINO
#endif

class domino_problem : public domino_problem_input
{
protected:
   // pieces currently on the board
   elements_t on_board;
   // possible to remove in the next turn
   elements_t possible;
   // not longer on board, removed in the previous turns
   elements_t removed;
   // algorithm does not know anything about these pieces
   elements_t unresolved;
   // possible to remove if other pieces are placed right
   elements_t checked;
   // impossible to remove due to size of the board
   elements_t invalid;
public:
   domino_problem();
   domino_problem(const domino_problem& problem);
   domino_problem(const domino_problem_input& input);
private:
   // checks all unresolved pieces to know which cannot be removed at all
   void resolve_elements();
   // scans board to find removable pieces
   void scan_board();
public:
   void add_possible_outcomes(simple_list<domino_problem,size_t>& outcomes);
   simple_list<domino_problem,size_t> get_possible_outcomes();
   bool state_equals(const domino_problem& problem);
private:
   bool can_be_removed(size_t x, size_t y);
   size_t distance(size_t x, size_t y, half_direction dir);
   void remove_at(size_t x, size_t y);
public:
   std::string on_board_str() const { return on_board.str(); }
   std::string possible_str() const { return possible.str(); }
   std::string removed_str() const { return removed.str(); }
#ifdef DEBUG
   void demo_solution();
#endif // DEBUG
};

#endif // DOMINO_PROBLEM_H
