#ifndef DOMINO_PROBLEM_SOLVER_H
#define DOMINO_PROBLEM_SOLVER_H

#include "domino_problem.h"
#include <iostream>
#include <iomanip>
#include "simple_tree.h"
#include "binary_hash.h"

#ifdef DEBUG
#endif // DEBUG

class domino_problem_solver : public domino_problem
{
   typedef simple_tree<domino_problem,ull> states_t;
protected:
   // stores tree of all possible states of the board (it grows during
   //  execution of the algorithm)
   states_t states;
   // stores the currently known best state (first reached state
   //  with as few domino pieces on board as possible)
   states_t::elem best_state;
   // hash table backed by binary tree, used to check very very quickly if a
   // given state is already in the tree
   binary_hash in_tree;
public:
   domino_problem_solver();
   //domino_problem_solver(const std::string& path);
   domino_problem_solver(const domino_problem& problem);
   ~domino_problem_solver();
private:
   domino_problem_solver(const domino_problem_solver& solver);
   domino_problem_solver& operator=(const domino_problem_solver& solver);
public:
   void execute(bool approximate = false);
private:
   // constructs a full tree of states, until either:
   //  1) all possible ways of removing pieces are served, or
   //  2) complete solution is found
   void construct_full_tree(bool depthFirst = false);
   // constructs a path until a full solution is found or
   //  a dead-end is reached
   void construct_path();
public:
    solution_t find_first_best_solution();
    simple_list<solution_t,ull> find_all_best_solutions();
};

#endif // DOMINO_PROBLEM_SOLVER_H
