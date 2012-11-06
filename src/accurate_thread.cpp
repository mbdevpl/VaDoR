#include "accurate_thread.h"

accurate_thread::accurate_thread(domino_problem_input &input) : input(nullptr)
{
    this->input = &input;
}

void accurate_thread::run()
{
    domino_problem prob(*input);
    domino_problem_solver solver(prob);

    solver.execute(false, 4000, false, DEPTH_SORT, true);

    domino_problem::solution_t frst = solver.find_first_best_solution();
  //  domino_problem best_state = frst.last().value();

    for(domino_problem::solution_t::elem i = frst.first(); i; ++i)
        std::cout << (*i).board_str() << std::endl << std::endl;

    domino_problem::solution_t::elem i = frst.last();
     std::cout << (*i).removed_str() << std::endl
           << (*i).board_str() << std::endl << std::endl;

}

