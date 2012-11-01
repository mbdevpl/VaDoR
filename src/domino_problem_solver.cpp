#include "domino_problem_solver.h"

domino_problem_solver::domino_problem_solver()
   :  domino_problem(), states(), in_tree(), best_path(), best_state() { }

domino_problem_solver::domino_problem_solver(const domino_problem& problem)
   : domino_problem(problem), states(problem), in_tree(problem.size()),
     best_path(), best_state() { }

domino_problem_solver::~domino_problem_solver()
{
   states.clear();
}

void domino_problem_solver::execute(bool output, bool approximate)
{
   ull* filler = nullptr;
   try
   {
      // memory needed in case of bad alloc
      filler = new ull[262144]; // 262144 * 64 bits = 2MB
      if(approximate)
         construct_path();
      else
         construct_full_tree(output, true, true);
      if(filler)
      {
         delete filler;
         filler = nullptr;
      }
   }
   catch(std::bad_alloc& ba)
   {
      delete filler;
      filler = nullptr;
      std::cout << "Exception: no more memory available to allocate the tree."
                << std::endl << ba.what() << std::endl;
   }
}

void domino_problem_solver::construct_full_tree(bool output, bool depthFirst, bool purgeUseless)
{
   solution_t outcomes;
   simple_list<states_t::elem,size_t> new_states;
   new_states.append(states.root());

   // current number of scanned states (i.e. states for which the children are known)
   ull state_count = 0;
   long long temp_state_count = STATE_COUNT_DELAY;
   // maximum number of states that can be reached theoretically
   ull max_states = ((ull)1) << elements->length();
   // currently known best state
   best_state = states.root();
   ull best_on_board = best_state.value_ref().on_board_length();

   //#ifdef DEBUG
   if(output)
   {
      std::cout << "Started constructing tree...\n";
      std::cout << "Total number of theoretically possible states:"
                << " 2^" << elements->length() << " = "
                << (elements->length() >= 64 ? "... a lot" : std::to_string(max_states))
                << "\n\n";
      std::cout << "Construction in progress, current number of states:\n";
      std::cout << " scanned: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << 0
                << "; in tree: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << states.count()
                << "; new states: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << new_states.length()
                << "; pieces left: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << on_board_length();
      std::cout << std::endl;
   }
   //#endif // DEBUG

   new_states.first().value_ref().value_ref().scan_board();

   static std::string timer_format("h:m.s");
   program_timer timer;
   timer.start();

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

      bool new_outcomes = false;
      if(outcomes.length() > 0)
      {
         new_outcomes = true;
         // iterate over all new possible states
         for(solution_t::elem i = outcomes.first(); i; ++i)
         {
            //(*i).pack();
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
         }
      }
      if(output)
      {
         ++state_count;
         --temp_state_count;
         if(!temp_state_count) {
            timer.stop();
            temp_state_count = STATE_COUNT_DELAY;
            std::cout << " scanned: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << state_count
                      << "; in tree: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << states.count()
                      << "; new states: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << new_states.length()
                      << "; " << timer.str(timer_format);
            if(!depthFirst)
               std::cout << "; pieces left: "
                         << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << problem.on_board_length();
            std::cout << std::endl;
            timer.start();
         }
      }
      if(problem.on_board_length() < best_on_board)
      {
         best_state = state.copy();
         best_on_board = problem.on_board_length();
         if(output)
         {
            std::cout << " new best solution, (" << best_on_board << " pieces left) removed " << problem.removed_length() << ":\n"
                      << problem.removed_str(true);
            std::cout << std::endl;
         }

         rebuild_best_path();
         if(purgeUseless)
         {
            if(depthFirst)
            {
               //purging is currently only supported for depth-first search
               for(states_t::elem e = *(best_path.last()); e; e.root())
               {
                  while(true)
                  {
                     states_t::elem copy(e);
                     copy.right();
                     if(copy.valid())
                        copy.removeRecursive();
                     else
                        break;
                  }
               }
            }
         }
      }
      else
      {
         if(purgeUseless)
         {
            if(depthFirst)
            {
               //purging is currently only supported for depth-first search
               for(states_t::elem e(state); e; e.root())
               {
                  while(true)
                  {
                     states_t::elem copy(e);
                     copy.right();
                     if(copy.empty())
                        break;
                     if(best_path.find(copy))
                        break;
                     if(copy.valid())
                        copy.removeRecursive();
                     else
                        break;
                  }
               }
            }
         }
      }
      //      states_t::elem right = state_elem.copy().value().right();
      //      if(right.valid())
      //      {
      //         rebuild_best_path();
      //         simple_list<states_t::elem,ull>::elem on_best_path_elem = best_path.find(right);
      //         if(on_best_path_elem.valid())
      //         {
      //            //states_t::elem,ull>::elem on_best_path_elem = best_path.find(right);
      //            for(states_t::elem e = *(best_path.last()); e; e.root())
      //            {
      //               e.copy().right().removeRecursive();
      //            }
      //         }
      //         else
      //         {
      //            best_path.first();
      //         }
      //         // future optimization to check:
      //         // state_elem.copy().right().removeRecursive(); // except longest found path
      //      }
      state_elem.remove();
      problem.pack();
      if(best_on_board == 0)
         break;
   }
   if(output)
   {
      timer.stop();
      std::cout << " scanned: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << state_count
                << "; in tree: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << states.count()
                << "; " << timer.str(timer_format);
      //if(!depthFirst)
      std::cout << "; pieces left: " << std::setw(OUTPUT_NUMBER_LEN) << std::setfill(' ') << best_on_board;
      std::cout << std::endl;
   }
}

void domino_problem_solver::construct_path()
{
   // nothing yet
}

void domino_problem_solver::rebuild_best_path()
{
   best_path.clear();
   for(states_t::elem e = best_state.copy(); e; e.root())
      best_path.insertFirst(e.copy());
}

domino_problem::solution_t domino_problem_solver::find_first_best_solution()
{
   solution_t sol;
   for(states_t::elem e = best_state.copy(); e; e.root())
   {
      (*e).expand();
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
