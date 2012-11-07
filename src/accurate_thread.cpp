#include "accurate_thread.h"

accurate_thread::accurate_thread(domino_problem_input &input, bool make_it_approximate) : input(nullptr)
{
   this->is_in_fact_approximate = make_it_approximate;
   this->input = &input;
   present = new QVector<domino_elem_located*>();
}

accurate_thread::~accurate_thread()
{
   if(present != nullptr)
      delete present;
}

void accurate_thread::run()
{
   if(input == nullptr || present == nullptr)
      throw std::runtime_error("cannot run algorithm for undefined input and/or without allocated list");
   domino_problem prob(*input);
   domino_problem_solver solver(prob);
   myTimer.start();
   solver.execute(false, 100000, is_in_fact_approximate, DEPTH_SORT, true);
   int timeElapsed = myTimer.elapsed();
   domino_problem::solution_t frst = solver.find_first_best_solution();
   domino_problem& best_state = frst.last().value_ref();
   for(domino_problem::elements_t::elem_const e = best_state.removed_first(); e; ++e)
   {
      size_t x = (*e)->x;
      size_t y = (*e)->y;
      emit threadRemovePiece((int)y,(int)x);
      removed.append(*e);
   }
   for(domino_problem::elements_t::elem_const e = best_state.on_board_first(); e; ++e)
      present->append(*e);

   emit threadComputationOver(timeElapsed,present,removed);
}

