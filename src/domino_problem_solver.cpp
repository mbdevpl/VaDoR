#include "domino_problem_solver.h"

domino_problem_solver::domino_problem_solver()
   :  domino_problem(), states(), in_tree(), best_path(), best_state() { }

domino_problem_solver::domino_problem_solver(const domino_problem& problem)
   : domino_problem(problem), states(problem), in_tree(problem.checked_length()),
     best_path(), best_state() { }

domino_problem_solver::~domino_problem_solver()
{
   states.clear();
}

void domino_problem_solver::execute(bool output, long long delay, bool approximate, size_t mode,
                                    bool purgeUseless)
{
   ull* filler = nullptr;
   // memory needed in case of bad alloc
   filler = new ull[262144]; // 262144 * 64 bits = 2MB
   try
   {
      if(approximate)
         construct_path(output, delay, false, size()*size());
      else
         construct_full_tree(output, delay, (mode == DEPTH_LAST || mode == DEPTH_FIRST || mode == DEPTH_SORT),
                             purgeUseless, mode == DEPTH_LAST, mode == DEPTH_SORT, false, -1);
   }
   catch(std::runtime_error& ex)
   {
      std::cout << "Exception: runtime error caused by inconsistent input "
                << "or bugs within the algorithm, \"" << ex.what() << "\"." << std::endl;
   }
   catch(std::bad_alloc& ex)
   {
      delete filler;
      filler = nullptr;
      std::cout << "Exception: no more memory available to allocate the tree, \""
                << ex.what() << "\"." << std::endl;
   }
   if(filler)
   {
      delete filler;
      filler = nullptr;
   }
}

void domino_problem_solver::construct_full_tree(bool output, long long delay, bool depthFirst,
                                                bool purgeUseless, bool startFromRight,
                                                bool doOutcomeSort, bool stopOnFirstDeadEnd,
                                                long long maxStatesChecked)
{
   // current number of scanned states (i.e. states for which the children are known)
   unsigned long long state_count = 0;
   // counter used for output, is not and shold not be used otherwise
   long long temp_state_count = delay;
   unsigned long long not_put_in_tree = 0;
   // maximum number of states that can be reached theoretically
   ull max_states = ((ull)1) << checked.length();
   --max_states;
   // currently known best state
   best_state = states.root();
   // current minimum number of pieces on board
   ull best_on_board = best_state.value_ref().on_board_length();
   // temporary variable storing new outcomes that were not yet checked
   //  if they already are in a tree or not
   solution_t possibilities_not_checked;
   // list of new states that are in a tree but they were not yet explored
   simple_list<states_t::elem,size_t> new_states_in_tree;
   // exploration starts from root i.e. initial state
   new_states_in_tree.append(states.root());

   toolkit::program_timer global_timer;
   toolkit::program_timer timer;
   global_timer.start();
   timer.start();

   if(output)
   {
      std::cout << "Started constructing tree..." << std::endl;
      std::cout << "Total number of theoretically possible states: 2^" << checked.length();
      if(checked.length() >= 64)
         std::cout << " > 2^64 , i.e. cannot display the number";
      else
         std::cout << " = " << std::to_string(max_states);
      std::cout << std::endl << std::endl;
      std::cout << "Construction in progress, current number of states:" << std::endl;
      cout_status(0, new_states_in_tree.length(), global_timer, !depthFirst, on_board_length(), states_t::elem());
   }

   if(new_states_in_tree.length() > 0)
      new_states_in_tree.first().value_ref().value_ref().scan_board();

#ifdef DEBUG
   const long long memory_checking_step = 5000;
   const unsigned long long memory_checking_value = 1800000000;
#else
   const long long memory_checking_step = 500000;
   const unsigned long long memory_checking_value = 1950000000;
#endif // DEBUG
   long long memory_checking = memory_checking_step;

   const long long hash_optimization_step = 2000000;
   long long hash_optimization = hash_optimization_step;
   const long long tree_optimization_step = 30000;
   long long tree_optimization = tree_optimization_step;

   while(new_states_in_tree.length() > 0)
   {
      simple_list<states_t::elem,size_t>::elem state_elem
            = depthFirst ? new_states_in_tree.last() : new_states_in_tree.first();
      states_t::elem& state = *state_elem;
      domino_problem& problem = *state;
      possibilities_not_checked.clear();
      problem.add_possible_outcomes(possibilities_not_checked);

      bool new_possibilities = false;
      if(possibilities_not_checked.length() > 0)
      {
         new_possibilities = true;
         if(doOutcomeSort)
            possibilities_not_checked.sort_bubblesort();
         // iterate over all new possible states
         solution_t::elem i = startFromRight ? possibilities_not_checked.first() : possibilities_not_checked.last();
         while(i)
         {
            // if the state is already in the tree, do not add it,
            // but simply connect the current state to the existing one
            //(*i).calculate_keys();
            ull key = 0;
            for(elements_t::elem_const el = (*i).elements_first(), on = (*i).on_board_first(); el; ++el)
            {
               if(invalid->find(*el))
               {
                  ++on;
                  continue;
               }
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
               ++not_put_in_tree;
            }
            else
            {
               state.appendSub(*i); // add new substate to the tree of states
               new_states_in_tree.append(state.copy().lastSub());
               --tree_optimization;
               if(!hash_optimization)
               {
                  in_tree.optimize();
                  hash_optimization = hash_optimization_step;
               }
               else
                  --hash_optimization;
            }
            if(startFromRight)
               ++i;
            else
               --i;
         }
      }
      ++state_count;
      if(output)
      {
         --temp_state_count;
         if(!temp_state_count) {
            timer.stop();
            temp_state_count = delay;
            cout_status(state_count, new_states_in_tree.length(), timer, !depthFirst, problem.on_board_length(), state);
            timer.start();
         }
      }
      if(problem.on_board_length() < best_on_board)
      {
         best_state = state.copy();
         best_on_board = problem.on_board_length();
         if(output && !new_possibilities)
         {
            std::cout << " new best solution, (" << best_on_board << " pieces left) removed "
                      << problem.removed_length() << ":\n" << problem.removed_str(true)
                      << std::endl;
         }
         rebuild_best_path();
         if(purgeUseless && depthFirst)
         {
            purge_right_side(*(best_path.last()), false);
            tree_optimization = tree_optimization_step;
         }
      }
      else
      {
         if(purgeUseless && depthFirst)
         {
            if(tree_optimization < 0)
            {
               purge_right_side(state, true);
               tree_optimization = tree_optimization_step;
            }
         }
      }
      state_elem.remove();
      problem.pack();
      if(best_on_board == invalid->length())
         break;
      if(maxStatesChecked != -1 && state_count >= (ull)maxStatesChecked)
         break;
      if(stopOnFirstDeadEnd && !new_possibilities)
         break;

      // memory cleanup
      if(!memory_checking)
      {
         memory_checking = memory_checking_step;
         unsigned long long mem = toolkit::program_info::physical_memory_used();
         if(mem > memory_checking_value)
         {
            purge_right_side(state, true);
            if(output)
               std::cout << "  Purged the tree";
            mem = toolkit::program_info::physical_memory_used();
            if(mem > memory_checking_value)
            {
               unsigned long long cleared = in_tree.nodes();
               in_tree.clear();
               if(output)
               {
                  std::cout
                        << ", and removed " << cleared << " nodes from hash";
               }
            }
            if(output)
               std::cout << ".";
            if(output)
            {
               std::cout << " Memory: " << mem / MEGABYTE_DIVISOR << "MB -> "
                         << toolkit::program_info::physical_memory_used()/ MEGABYTE_DIVISOR
                         << "MB" << std::endl;
            }
         }

      }
      else
         --memory_checking;
   }
   if(output)
   {
      timer.stop();
      global_timer.stop();
      cout_status(state_count, new_states_in_tree.length(), global_timer, true, best_on_board, states.root());
   }
}

void domino_problem_solver::construct_path(bool output, long long delay, bool stopOnFirstDeadEnd,
                                           long long maxStatesChecked)
{
   construct_full_tree(output, delay, true, false, false, true, stopOnFirstDeadEnd, maxStatesChecked);

}

long double domino_problem_solver::estimate_progress(const states_t::elem& state)
{
   if(state.empty())
      return 0;
   typedef simple_list<states_t::elem,ull> state_list;
   simple_list<states_t::elem,ull> path;
   for(states_t::elem e(state); e; e.root())
      path.insertFirst(e);

   if(path.length() < 2)
      return 0;

   long double done = 0;
   simple_list<long double,size_t> worths;
   for(state_list::elem s = path.first(); s; ++s)
   {
      simple_list<states_t::elem,ull> subs = (*s).subList();
      ull subs_len = subs.length();
      if(subs_len == 0)
         worths.append(1);
      else
         worths.append(1 / ((long double)subs_len));
      // calculating number of explored nodes
      ull nodes_done = 0;
      if(subs_len == 0)
         nodes_done = 1;
      else
      {
         state_list::elem sn = s;
         sn.forward();
         if(sn.valid())
         {
            for(state_list::elem sub = subs.last(); sub; --sub, ++nodes_done)
               if(*sub == *sn)
                  break;
         }
      }
      // calculating worth of current node
      long double curr_worth = 1;
      for(simple_list<long double,size_t>::elem w = worths.first(); w; ++w)
         curr_worth *= *w;
      done += curr_worth * nodes_done;
   }

   return done;
}

void domino_problem_solver::cout_status(unsigned long long scanned_states, unsigned long long not_scanned_states,
                                        toolkit::program_timer& timer, bool show_pieces,
                                        unsigned long long pieces_left, const states_t::elem& state)
{
   //static const size_t output_indent = 10;
   static std::string timer_format("h:m.s");

   if(state == states.root())
      std::cout << std::setw(13) << std::setfill(' ') << std::fixed << std::setprecision(9)
                << (double)100
                << '%';
   else
      std::cout << std::setw(13) << std::setfill(' ') << std::fixed << std::setprecision(9)
                << (estimate_progress(state)*100)
                   //<< ((long double)((long long)((estimate_progress(state) * 100)*10000)))/ 10000
                << '%';
   bool/* t = false, m = false,*/ k = false;
   /*if(scanned_states > 1000000000)
   {
      t = true;
      scanned_states /= 1000000000;
   }
   else if(scanned_states > 1000000)
   {
      m = true;
      scanned_states /= 1000000;
   }
   else */if(scanned_states >= 1000)
   {
      k = true;
      scanned_states /= 1000;
   }
   std::cout << "; scanned: " << std::setw(6) << std::setfill(' ') << scanned_states;
   /*if(t) std::cout << 'T';
   if(m) std::cout << 'M';
   else */if(k) std::cout << 'K';
   else std::cout << ' ';
   std::cout
         << "; new: " << std::setw(3) << std::setfill(' ') << not_scanned_states
         << "; hash: " << std::setw(8) << std::setfill(' ') << in_tree.nodes()
         << "; tree: " << std::setw(5) << std::setfill(' ') << states.count();
   if(show_pieces)
      std::cout << "; pieces left: " << std::setw(3) << std::setfill(' ') << pieces_left;
   std::cout << "; " << timer.str(timer_format);
   //std::cout << "; " << toolkit::program_info::physical_memory_used();
   std::cout << std::endl;
}

void domino_problem_solver::purge_right_side(states_t::elem startElem, bool stopAtBestPath)
{
   bool stoppedAtBestPath = false;
   simple_list<states_t::elem,ull>::elem el;
   states_t::elem e(startElem);
   //purging is currently only supported for depth-first search
   for(; e; e.root())
   {
      while(true)
      {
         states_t::elem copy(e);
         copy.right();
         if(copy.empty())
            break;

         if(stopAtBestPath)
         {
            el = best_path.find(copy);
            if(el.valid())
            {
               stoppedAtBestPath = true;
               break;
            }
         }
         copy.removeRecursive();
      }
      if(stoppedAtBestPath)
         break;
   }
   if(stoppedAtBestPath)
   {
      e = *el;
      //      if(e.subCount() > 1)
      //      {
      //         e.hasRoot();
      //      }
      e.lastSub();
      for(; e; e.lastSub())
      {
         while(true)
         {
            states_t::elem copy(e);
            copy.left();
            if(copy.empty())
               break;
            copy.removeRecursive();
         }
      }
   }
}

void domino_problem_solver::rebuild_best_path()
{
   best_path.clear();
   for(states_t::elem e(best_state); e; e.root())
      best_path.insertFirst(e.copy());
}

domino_problem::solution_t domino_problem_solver::find_first_best_solution()
{
   solution_t sol;
   for(states_t::elem e(best_state); e; e.root())
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
