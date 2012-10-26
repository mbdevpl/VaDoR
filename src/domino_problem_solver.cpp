#include "domino_problem_solver.h"

domino_problem_solver::domino_problem_solver(const domino_problem& problem)
   : domino_problem(problem), states(problem), in_tree(problem.size())
{
   //
}

void domino_problem_solver::execute(bool optimized)
{
   if(optimized)
      construct_tree();
   else
      construct_full_tree(true);
}

void domino_problem_solver::construct_full_tree(bool depthFirst)
{
   solution_t outcomes;
   simple_list<states_t::elem,size_t> new_states;
   new_states.append(states.root());

   // current number of scanned states (i.e. states for which the children are known)
   ull state_count = 0;
   // maximum number of states that can be reached theoretically
   ull max_states = ((ull)1) << elements->length();
   // currently known best state
   /*states_t::elem*/ best_state = states.root();
   ull best_on_board = best_state.value_ref().on_board_length();

   //#ifdef DEBUG
   std::cout << "Started constructing tree...\n";
   std::cout << "Total number of theoretically possible states:"
             << " 2^" << elements->length() << " = "
             << (elements->length() >= 64 ? "... a lot" : std::to_string(max_states))
             << "\n\n";
   std::cout << "Construction in progress, current number of states:\n";
   std::cout << " scanned: " << std::setw(6) << std::setfill(' ') << 0
             << "; in tree: " << std::setw(6) << std::setfill(' ') << states.count()
             << "; pieces left: " << std::setw(3) << std::setfill(' ') << on_board_length();
   std::cout << std::endl;
   //#endif // DEBUG

   new_states.first().value_ref().value_ref().scan_board();

   while(new_states.length() > 0)
   {
      simple_list<states_t::elem,size_t>::elem state_elem
            = depthFirst ? new_states.last() : new_states.first();
      states_t::elem& state = *state_elem;
      domino_problem& problem = *state;
      //problem.scan_board();
      outcomes.clear();
      //ull len_before = outcomes.length(); // remember length
      problem.add_possible_outcomes(outcomes);
      //ull len_after = outcomes.length(); // check new length

      //#ifdef DEBUG
      //      //std::cout << "  From:";
      //      //std::cout << "\non_board=" << problem.on_board_str();
      //      //std::cout << std::endl;
      //      //std::cout << "removed=" << problem.removed_str();
      //      //std::cout << std::endl;
      //      //if(len_after > len_before)
      //      {
      //         //std::cout << "  New possible states detected:\n";
      //         //for(solution_t::elem i = outcomes.element(len_before); i; ++i)
      //         {
      //            //std::cout << "on_board=" << (*i).on_board_str() << std::endl;
      //            //std::cout << "removed=" << (*i).removed_str() << std::endl;
      //            //std::cout << (*i).board_str();
      //         }
      //      }
      //      //else
      //      //   std::cout << "  No new possible states...";
      //      //std::cout << std::endl;
      //#endif // DEBUG
      // iterate over all new possible states
      for(solution_t::elem i = outcomes.first()/*element(len_before)*/; i; ++i)
      {
         // if the state is already in the tree, do not add it,
         // but simply connect the current state to the existing one
         ull key = 0;
         for(elements_t::elem_const el = (*i).elements_first(), on = (*i).on_board_first(); el; ++el)
         {
            key <<= 1;
            if(on && *el == *on)
            {
               key += 1;
               ++on;
            }
         }
         bool already_in_tree = !in_tree.put(key);

         if(already_in_tree)
         {
            //#ifdef DEBUG
            //std::cout << "Found a state that already exists in the tree.\n";
            //#endif // DEBUG
            // find where exactly in the tree the state is
            //            states_t::elem found = states.find(pr1);
            // connect the current state to the existing one, forming an edge
            //  between two already existing states.
            //  this step is crucial to achieve complexity 2^n, which is better than n!
            //            state.appendSub(found);
            continue;
         }
         state.appendSub(*i); // add new substate to the tree of states
         new_states.append(state.copy().lastSub());

         //++len_before;
         //#ifdef DEBUG
         //std::cout << "Added a new state!\n";
         //#endif // DEBUG
      }

      ++state_count;
      if(state_count % 500 == 0) {
         std::cout << " scanned: " << std::setw(6) << std::setfill(' ') << state_count
                   << "; in tree: " << std::setw(6) << std::setfill(' ') << states.count()
                   << "; pieces left: " << std::setw(3) << std::setfill(' ') << problem.on_board_length();
         std::cout << std::endl;
      }
      if(problem.on_board_length() < best_on_board)
      {
         best_state = state.copy();
         best_on_board = problem.on_board_length();
         std::cout << " new best solution, removed " << problem.removed_length() << ":\n"
                   << problem.removed_str(true);
         std::cout << std::endl;
      }
      state_elem.remove();
      if(best_on_board == 0)
         break;
   }
   std::cout << " scanned: " << std::setw(6) << std::setfill(' ') << state_count
             << "; in tree: " << std::setw(6) << std::setfill(' ') << states.count();
   std::cout << std::endl;
}

void domino_problem_solver::construct_tree()
{
   // nothing yet
}

domino_problem::solution_t domino_problem_solver::find_first_best_solution()
{
   solution_t sol;

   simple_list<states_t::elem,ull> path;
   for(states_t::elem e = best_state.copy(); e; e.root())
   {
      path.insertFirst(e.copy());
      sol.insertFirst(*e);
   }
   return sol;

   //   solution_t sol;
   //
   //   // +1 because path contains vertices (i.e. states), not the edges (i.e. operations).
   //   //  one piece removal takes 2 states, two removals take 3 states, and so on...
   //   simple_list<states_t::elem,ull> path = states.find_first_longest_path(elements->length()+1);
   //   for(simple_list<states_t::elem,ull>::elem e = path.first(); e; ++e)
   //      sol.append(**e);
   //
   //   return sol;
}

simple_list<domino_problem::solution_t,domino_problem::ull> domino_problem_solver::find_all_best_solutions()
{
   simple_list<solution_t,ull> all;

   simple_list<simple_list<states_t::elem,ull>,ull> paths
         = states.find_all_longest_paths(elements->length()+1);
   for(simple_list<simple_list<states_t::elem,ull>,ull>::elem ee = paths.first(); ee; ++ee)
   {
      simple_list<states_t::elem,ull>& path = *ee;
      all.append(simple_list<domino_problem,ull>());
      solution_t& sol = all.last().value_ref();
      for(simple_list<states_t::elem,ull>::elem e = path.first(); e; ++e)
         sol.append(**e);
   }
   return all;
}
