#include "accurate_thread.h"

accurate_thread::accurate_thread(domino_problem_input &input) : input(nullptr)
{
   this->input = &input;
}

void accurate_thread::run()
{
    //QVector<domino_elem_located>
   domino_problem prob(*input);
   domino_problem_solver solver(prob);
   myTimer.start();
   solver.execute(false, 4000, false, DEPTH_SORT, true);
   int timeElapsed = myTimer.elapsed();
   domino_problem::solution_t frst = solver.find_first_best_solution();
   domino_problem& best_state = frst.last().value_ref();
   for(domino_problem::elements_t::elem_const e = best_state.removed_first(); e; ++e)
   {
      size_t x = (*e)->x;
      size_t y = (*e)->y;
      emit threadRemovePiece((int)y,(int)x);
   }

  // emit threadComputationOver(timeElapsed,NULL,NULL);
}

