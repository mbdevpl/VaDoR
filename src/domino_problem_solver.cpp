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
   simple_list<domino_problem,size_t> outcomes;
   simple_list<states_t::elem,size_t> new_states;
   new_states.append(states.root());

   //states_t::elem initial_state = states.root();
   while(new_states.length() > 0)
   {
      simple_list<states_t::elem,size_t>::elem state_elem = new_states.first();
      states_t::elem& state = *state_elem;
      domino_problem& problem = *state;
      size_t len_before = outcomes.length(); // remember length
      problem.add_possible_outcomes(outcomes);
      size_t len_after = outcomes.length(); // check new length
#ifdef DEBUG
      //std::cout << "  From:";
      //std::cout << "\non_board=" << problem.on_board_str();
      //std::cout << std::endl;
      //std::cout << "removed=" << problem.removed_str();
      //std::cout << std::endl;
      if(len_after > len_before)
      {
         //std::cout << "  New possible states detected:\n";
         for(outcomes_t::elem i = outcomes.element(len_before); i; ++i)
         {
            //std::cout << "on_board=" << (*i).on_board_str() << std::endl;
            //std::cout << "removed=" << (*i).removed_str() << std::endl;
            //std::cout << (*i).board_str();
         }
      }
      //else
      //   std::cout << "  No new possible states...";
      //std::cout << std::endl;
#endif // DEBUG
      // iterate over all new possible states
      for(outcomes_t::elem i = outcomes.element(len_before); i; ++i)
      {
         // if the state is already in the tree, do not add it,
         // but simply connect the current state to the existing one
         bool already_in_tree = false;
         domino_problem& pr1 = *i;
         size_t count = 0;
         for(outcomes_t::elem it = outcomes.first(); it && count < len_before; ++it, ++count)
         {
            domino_problem& pr2 = *it;
            if(pr1.state_equals(pr2))
            {
               already_in_tree = true;
               break;
            }
         }
         if(already_in_tree)
         {
#ifdef DEBUG
            //std::cout << "Found a state that already exists in the tree.\n";
#endif // DEBUG
            // find where exactly in the tree the state is
            //            states_t::elem found = states.find(pr1);
            // connect the current state to the existing one, forming an edge
            //  between two already existing states.
            //  this step is crucial to achieve complexity 2^n, which is better than n!
            //            state.appendSub(found);
            continue;
         }
         state.appendSub(i.value_ref()); // add new substate to the tree of states
         new_states.append(state.copy().lastSub());
         ++len_before;
#ifdef DEBUG
         //std::cout << "Added a new state!\n";
#endif // DEBUG
      }
      state_elem.remove();
   }
}

void domino_problem_solver::construct_tree()
{
   // nothing yet
}

simple_list<domino_problem,size_t> domino_problem_solver::find_first_best_solution()
{
   simple_list<domino_problem,size_t> sol;

   // +1 because path contains vertices (i.e. states), not the edges (i.e. operations).
   //  one piece removal takes 2 states, two removals take 3 states, and so on...
   simple_list<states_t::elem,size_t> path = states.find_first_longest_path(elements.length()+1);
   for(simple_list<states_t::elem,size_t>::elem e = path.first(); e; ++e)
      sol.append(**e);

   return sol;
}

simple_list<simple_list<domino_problem,size_t>,size_t> domino_problem_solver::find_all_best_solutions()
{
   simple_list<simple_list<domino_problem,size_t>,size_t> all;

   simple_list<simple_list<states_t::elem,size_t>,size_t> paths
         = states.find_all_longest_paths(elements.length()+1);
   for(simple_list<simple_list<states_t::elem,size_t>,size_t>::elem ee = paths.first(); ee; ++ee)
   {
      simple_list<states_t::elem,size_t>& path = *ee;
      all.append(simple_list<domino_problem,size_t>());
      simple_list<domino_problem,size_t>& sol = all.last().value_ref();
      for(simple_list<states_t::elem,size_t>::elem e = path.first(); e; ++e)
         sol.append(**e);
   }
   return all;
}
