#include "domino_problem_s.h"

domino_problem_s::domino_problem_s()
{
}

domino_problem_s::domino_problem_s(domino_problem_r& problem)
{
    this->board_width = problem.board_width;
    this->board_height = problem.board_height;
    this->elem_list = problem.elem_list;

}
