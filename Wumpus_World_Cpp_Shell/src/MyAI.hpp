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

enum class Safety {
    safe,
    danger,
    maybe
};

struct State {
    State() : visited(false), breeze(false), stench(false), safety(Safety::maybe) {}
    bool visited = false;
    bool breeze = false;
    bool stench = false;
    Safety safety = Safety::maybe;
};

struct Location {
    Location() : x(0), y(0) {}
    Location(int x_, int y_) : x(x_), y(y_) {}
    int x;
    int y;

    bool operator==(const Location &other);
    bool operator!=(const Location &other);
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

    ~MyAI();

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
    Location            left(const Location &location);
    Location            right(const Location &location);
    Location            up(const Location &location);
    Location            down(const Location &location);
    Location            get_cur_location();
    Direction           get_cur_direction();
    const Board&        get_board();
    const State&        get_state(const Location &location);

    Safety              location_safe(const Location &location);
    bool                location_visited(const Location &location);
    Safety              cur_location_safe();
    bool                cur_location_visited();
    bool                has_visited_left_neighbor(const Location &location);
    bool                has_visited_right_neighbor(const Location &location);
    bool                has_visited_up_neighbor(const Location &location);
    bool                has_visited_down_neighbor(const Location &location);

    void                set_breeze(bool breeze);
    void                set_stench(bool stench);
	void 				set_visited();
	void				set_safety(const Location &location, Safety safety);

    void                process_bump();
	bool				all_neighbors_visited(const Location &location);
	bool				has_visited_and_not_breezy_neighbors(const Location &location);
	bool				has_visited_and_not_stenchy_neighbors(const Location &location);

    Action              move_forward(bool bump);
    Action              turn_left();
    Action              turn_right();

	Action				check_curloc(bool stench, bool breeze, bool glitter, bool bump, bool scream);
	Action              go_home();
    void                go_to(Location destination);
    void                expand_neighbors(Location current_location);

    Location            up(Direction face_direction, Location current_location);
    Location            down(Direction face_direction, Location current_location);
    Location            left(Direction face_direction, Location current_location);
    Location            right(Direction face_direction, Location current_location);
    Location            NE(Direction face_direction, Location current_location);
    Location            SE(Direction face_direction, Location current_location);
    Location            NW(Direction face_direction, Location current_location);
    Location            SW(Direction face_direction, Location current_location);


private:

	// Agent Variables
    bool    m_first_run;
	bool 	m_shot_status;
	bool	m_gold_status;
	bool    m_go_home_mode;

	Location    m_cur_location;
	Direction   m_cur_direction;
	Board       m_board;
	vector<Action> m_actions_taken;

	vector<Action> m_goto_path;
    vector<Action> m_turn_to_home;
	vector<Location> m_stack;

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif