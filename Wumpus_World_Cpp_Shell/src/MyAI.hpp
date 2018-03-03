// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <vector>
#include <utility>

using std::vector;
using std::pair;

struct State {
    bool visited = false;
    bool safe = false;
    bool breeze = false;
    bool stench = false;
};

struct Location {
    Location(int x_, int y_) :x(x_), y(y_) {}
    int x = 0;
    int y = 0;
};

enum class Direction {
    left,
    right,
    up,
    down
};

typedef vector<vector<State>> Board;

class MyAI : public Agent
{
public:
	MyAI ( void );

	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);

	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
    const Board&        get_board(); // figure out current size of world (minimum: 4x4, maximum: 7x7)

    Location            get_cur_location();
    Direction           get_cur_direction();

    bool                location_safe(const Location &location);
    bool                location_visited(const Location &location);
    bool                cur_location_safe();
    bool                cur_location_visited();

    void                set_breeze(bool breeze);
    void                set_stench(bool stench);

    Action              move_forward(bool bump);
    Action              turn_left();
    Action              turn_right();
    Action              move_cw(bool stench, bool breeze, bool glitter, bool bump, bool scream);
    Action              move_countercw(bool stench, bool breeze, bool glitter, bool bump, bool scream);
    Action              go_home();
    Action              move_turnaround(bool bump);
    Action              move_turntohome();

	// Setters

	// Getters
  bool  get_pass_home();

private:

	// Agent Variables
	bool 	m_shot_status;
	bool	m_gold_status;
  bool  m_dir_cw;
  bool  m_pass_home;
  int   m_bump_turn_count;
  int   m_switch_dir;
  bool  m_first_action;
  bool  m_change_dir;

  bool    m_go_home_mode;

  Location    m_cur_location;
  Direction   m_cur_direction;
  Board       m_board;
  vector<Action> m_actions_taken;

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif