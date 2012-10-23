#include "domino_problem_solver.h"

domino_problem_solver::domino_problem_solver(const domino_problem& problem)
   : domino_problem(problem), states(problem)
{
   //
}

void domino_problem_solver::execute(bool optimized)
{
   if(optimized)
      construct_tree();
   else
      construct_full_tree();
}

void domino_problem_solver::construct_full_tree()
{
   typedef simple_list<domino_problem,size_t> outcomes_t;
   outcomes_t outcomes;
   states_t::elem state = states.root();
   domino_problem& prob = state.value_ref();
   prob.add_possible_outcomes(outcomes);
   for(outcomes_t::elem i = outcomes.first(); i; ++i)
   {
      state.insertLastSub(i.value_ref());
   }
#ifdef DEBUG
   std::cout << outcomes;
#endif // DEBUG

   //while(true)
   {
      for(states_t::elem i = state.sub(); i.valid(); i.right())
      {
   #ifdef DEBUG
         std::cout << "\nMoving on with this shit:\n";
   #endif // DEBUG
         outcomes.clear();
         domino_problem& curr_problem = i.value_ref();
         curr_problem.add_possible_outcomes(outcomes);
   #ifdef DEBUG
         std::cout << outcomes;
   #endif // DEBUG
         for(outcomes_t::elem out = outcomes.first(); out; ++out)
         {
            i.insertLastSub(out.value_ref());
         }
      }
   }
}

void domino_problem_solver::construct_tree()
{

}
