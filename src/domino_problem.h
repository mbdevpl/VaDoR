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
public:
   typedef unsigned long long ull;
   typedef simple_list<domino_problem,ull> solution_t;
protected:
   // pieces currently on the board
   elements_t on_board;
   // possible to remove in the next turn
   elements_t possible;
   // not longer on board, removed in the previous turns
   elements_t removed;
   // sequence of zeros and ones, with ones when a given element from 'elements' is on board
   ull on_board_key;
   // sequence of zeros and ones, with ones when a given element from 'elements'
   //  can be removed in current situation
   ull possible_key;
   // sequence of zeros and ones, with ones when a given element from 'elements' is removed
   ull removed_key;
public:
   // Default constructor.
   domino_problem();
   // Copy constructor.
   domino_problem(const domino_problem& problem);
   // Creates a copy with or without copying 'possible' field.
   domino_problem(const domino_problem& problem, bool copy_possible);
   // Creates a problem from a problem input.
   domino_problem(const domino_problem_input& input);
   // Destructor.
   // ~domino_problem();
public:
   // Scans board to find removable pieces.
   void scan_board();
public:
   void add_possible_outcomes(domino_problem::solution_t& outcomes);
   domino_problem::solution_t get_possible_outcomes();
   bool state_equals(const domino_problem& problem);
   bool operator<(const domino_problem& problem);
   bool operator>(const domino_problem& problem);
private:
   bool can_be_removed(size_t x, size_t y);
   size_t distance(size_t x, size_t y, half_direction dir);
   void remove_at(size_t x, size_t y);
public:
   // Packs the object, decreasing memory usage.
   void pack();
   // Unpacks the object, increasing memory usage.
   void expand();
   const elements_t::elem_const on_board_first() const { return on_board.first(); }
   const elements_t::elem_const removed_first() const { return removed.first(); }
   size_t on_board_length() const { return on_board.length(); }
   size_t possible_length() const { return possible.length(); }
   size_t removed_length() const { return removed.length(); }
   std::string on_board_str() const { return on_board.str(); }
   std::string possible_str() const { return possible.str(); }
   std::string removed_str(bool compact = false) const
   {
      if(!compact)
         return removed.str();
      std::stringstream s;
      s << "{ ";
      for(elements_t::elem_const e = removed.first(); e; ++e)
      {
         s << e.value_ref_const()->str(true) << ' ';
      }
      s << '}';
      return s.str();
   }
   std::string board_line(
         size_t line, char before_line, bool fill_middle, bool value_mode,
         half_direction dir, bool three_chars_after_if_dir, const std::string& after_otherwise,
         const std::string& middle_if_empty, const std::string& after, bool no_default_spaces = false) const;
   std::string board_str() const;
};

#endif // DOMINO_PROBLEM_H
