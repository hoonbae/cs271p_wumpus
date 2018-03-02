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
    const Board&        get_board();

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

	// Setters
	void 	increment_turn_dir_l();
	void	increment_go_back_count();
	void 	set_shot_to_true();
	void 	set_gold_to_true();
	void 	set_go_fwd(bool gofwd);
	void 	set_go_back(bool goback);
	void	set_go_up(bool goup);

	// Getters
	int 	get_turn_dir_l();
	int     get_go_back_count();
	bool	get_shot_status();
	bool	get_gold_status();
	bool	get_fwd_status();
	bool	get_go_back_status();
	bool	get_go_up_status();

private:

	// Agent Variables
	int 	m_turn_dir_l;
	int     m_go_back_count;
	bool 	m_shot_status;
	bool	m_gold_status;
	bool	m_go_fwd_status;
	bool	m_go_back_status;
	bool	m_go_up_status;


    Location    m_cur_location;
    Direction   m_cur_direction;
    Board       m_board;

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};


/*
	Minimal AI - Design Outline

		Agent moves in a single direction. If agent senses...
			- stench: shoot arrow
					if scream: keep going
					else: turn around, go back to start, climb out
			- breeze: turn around, go back to start, climb out
			- glitter: grab gold, turn around, go back to start, climb out
			- bump: turn around, go back to start, climb out
*/

#endif