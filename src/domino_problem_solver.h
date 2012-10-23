#ifndef DOMINO_PROBLEM_SOLVER_H
#define DOMINO_PROBLEM_SOLVER_H

#include "domino_problem.h"
#include "simple_tree.h"

class domino_problem_solver : public domino_problem
{
   typedef simple_tree<domino_problem,size_t> states_t;
protected:
   // stores
   states_t states;
public:
   domino_problem_solver(const domino_problem& problem);
private:
   domino_problem_solver(const domino_problem_solver& solver);
   domino_problem_solver& operator=(const domino_problem_solver& solver);
public:
   void execute(bool optimized = false);
private:
   // constructs a full tree of states, until all possible ways
   //  of removing pieces are served
   void construct_full_tree();
   // constructs a tree until a full solution is found
   void construct_tree();
public:
    simple_list<domino_problem,size_t> find_first_best_solution();
    simple_list<simple_list<domino_problem,size_t>,size_t> find_all_best_solutions();
};

#endif // DOMINO_PROBLEM_SOLVER_H
