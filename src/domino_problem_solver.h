#ifndef DOMINO_PROBLEM_SOLVER_H
#define DOMINO_PROBLEM_SOLVER_H

#include "domino_problem.h"
#include <iostream>
#include <iomanip>
#include <new> // for bad_alloc
#include "simple_tree.h"
#include "binary_hash.h"
#include "program_timer.h"

#ifdef DEBUG
#endif // DEBUG

#define DEPTH_LAST 1
#define DEPTH_FIRST 2
#define DEPTH_SORT 4
#define BREADTH 8

#define STATE_COUNT_DELAY 100000

class domino_problem_solver : public domino_problem
{
   typedef simple_tree<domino_problem,ull> states_t;
protected:
   // stores tree of all possible states of the board (it grows during
   //  execution of the algorithm)
   states_t states;
   // hash table backed by binary tree, used to check very very quickly if a
   // given state is already in the tree
   binary_hash in_tree;
   // Currently known best path, i.e. partial solution
   simple_list<states_t::elem,ull> best_path;
   // stores the currently known best state (first reached state
   //  with as few domino pieces on board as possible)
   states_t::elem best_state;
public:
   domino_problem_solver();
   //domino_problem_solver(const std::string& path);
   domino_problem_solver(const domino_problem& problem);
   ~domino_problem_solver();
private:
   domino_problem_solver(const domino_problem_solver& solver);
   domino_problem_solver& operator=(const domino_problem_solver& solver);
public:
   void execute(bool output = true, long long delay = STATE_COUNT_DELAY, bool approximate = false,
                size_t mode = BREADTH, bool purgeUseless = false);
private:
   // constructs a full tree of states, until either:
   //  1) all possible ways of removing pieces are served, or
   //  2) complete solution is found
   void construct_full_tree(bool output = true, long long delay = STATE_COUNT_DELAY, bool depthFirst = false,
                            bool purgeUseless = false, bool startFromRight = false, bool doOutcomeSort = false,
                            bool stopOnFirstDeadEnd = false, long long maxStatesChecked = -1);
   // constructs a path until a full solution is found or
   //  a dead-end is reached
   void construct_path(bool output = true, long long delay = STATE_COUNT_DELAY, bool stopOnFirstDeadEnd = false,
                       long long maxStatesChecked = -1);
   void cout_status(unsigned long long scanned_states, unsigned long long not_scanned_states,
                    program_timer& timer, bool show_pieces,
                    unsigned long long pieces_left = 0);
   void purge_right_side(states_t::elem startElem, bool stopAtBestPath);
   void rebuild_best_path();
public:
   solution_t find_first_best_solution();
   simple_list<solution_t,ull> find_all_best_solutions();
};

#endif // DOMINO_PROBLEM_SOLVER_H
