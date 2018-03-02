// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

#define MAX_SZ 10

MyAI::MyAI() : Agent(), m_cur_location(0, 0)
{
    m_cur_direction = Direction::right;
    m_board.assign(MAX_SZ, vector<State>(MAX_SZ));
	m_turn_dir_l = 0;
	m_shot_status = false;
	m_gold_status = false;
	m_go_fwd_status = true;
	m_go_back_status = false;
	m_go_up_status = false;
	m_go_back_count = 0;
    m_go_home_mode = false;
}

Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
    // =====================================
    // DO NOT EDIT
    set_stench(stench);
    set_breeze(breeze);
    // =====================================

	// agent target dir - right
	// agent keep going forward until stench, glitter, breeze, or bump
	bool gofwd = get_fwd_status();
	while(gofwd)
	{
		if (stench)
		{
			bool is_shot = get_shot_status();
			if (is_shot)
			{
				set_go_fwd(false);
				set_go_back(true);
				break;
			} else {
				set_shot_to_true();
				return SHOOT;
			}
		} else if (glitter) {									// if glitter - there is gold - grab ...
			set_go_fwd(false);
			set_go_back(true);
			set_gold_to_true();
			return GRAB;
		} else if (breeze) {
			set_go_fwd(false);
			set_go_back(true);
			break;
		} else if (bump) {
			set_go_fwd(false);
			set_go_back(true);
			break;
		} else {
			return FORWARD;
		}
	}

	bool goback = get_go_back_status();
	int  goback_count = get_go_back_count();
	while(goback && goback_count == 0)
	{
		int turn_count = get_turn_dir_l();
		if (turn_count < 2)
		{
			increment_turn_dir_l();
			return TURN_LEFT;
		} else {
			bool is_gold = get_gold_status();
			if (bump && is_gold)
			{
				return CLIMB;
			}
			if(bump)
			{
				set_go_up(true);
				set_go_back(false);
				increment_go_back_count();
				return TURN_RIGHT;
			}
			return FORWARD;
		}
	}


	while(goback && goback_count == 1)
	{
		int turn_count = get_turn_dir_l();
		bool is_gold = get_gold_status();
		if (turn_count < 4)
		{
			if (is_gold)
			{
				increment_turn_dir_l();
				return TURN_RIGHT;
			} else {
				increment_turn_dir_l();
				increment_turn_dir_l();
				return TURN_RIGHT;
			}
		} else {
			if(bump)
			{
				return CLIMB;
			}
			return FORWARD;
		}
	}


	bool goup = get_go_up_status();
	while (goup)
	{
		if (stench)
		{
			bool is_shot = get_shot_status();
			if (is_shot)
			{
				set_go_up(false);
				set_go_back(true);
				return TURN_RIGHT;
			} else {
				set_shot_to_true();
				return SHOOT;
			}
		} else if (glitter) {
			set_go_up(false);
			set_go_back(true);
			set_gold_to_true();
			return GRAB;
		} else if (breeze) {
			set_go_up(false);
			set_go_back(true);
			return TURN_RIGHT;
		} else if (bump) {
			set_go_up(false);
			set_go_back(true);
			return TURN_RIGHT;
		} else {
			return FORWARD;
		}
	}
}



void MyAI::increment_turn_dir_l()
{
	m_turn_dir_l++;
}
void MyAI::increment_go_back_count()
{
	m_go_back_count++;
}
void MyAI::set_shot_to_true()
{
	m_shot_status = true;
}
void MyAI::set_gold_to_true()
{
	m_gold_status = true;
}
void MyAI::set_go_fwd(bool gofwd)
{
	m_go_fwd_status = gofwd;
}
void MyAI::set_go_back(bool goback)
{
	m_go_back_status = goback;
}

void MyAI::set_go_up(bool goup)
{
	m_go_up_status = goup;
}

int MyAI::get_turn_dir_l()
{
	return m_turn_dir_l;
}
int MyAI::get_go_back_count()
{
	return m_go_back_count;
}
bool MyAI::get_shot_status()
{
	return m_shot_status;
}
bool MyAI::get_gold_status()
{
	return m_gold_status;
}
bool MyAI::get_fwd_status()
{
	return m_go_fwd_status;
}
bool MyAI::get_go_back_status()
{
	return m_go_back_status;
}
bool MyAI::get_go_up_status()
{
	return m_go_up_status;
}

const Board& MyAI::get_board() {
	return m_board;
}

Location MyAI::get_cur_location() {
	return m_cur_location;
}

Direction MyAI::get_cur_direction() {
	return m_cur_direction;
}

bool MyAI::location_safe(const Location &location) {
	return m_board[location.x][location.y].safe;
}

bool MyAI::location_visited(const Location &location) {
	return m_board[location.x][location.y].visited;
}

bool MyAI::cur_location_safe() {
    return m_board[m_cur_location.x][m_cur_location.y].safe;
}

bool MyAI::cur_location_visited() {
    return m_board[m_cur_location.x][m_cur_location.y].visited;
}

void MyAI::set_breeze(bool breeze) {
    m_board[m_cur_location.x][m_cur_location.y].breeze = breeze;
}

void MyAI::set_stench(bool stench) {
    m_board[m_cur_location.x][m_cur_location.y].stench = stench;
}

Agent::Action MyAI::move_forward(bool bump) {
    if (!bump) {
        m_board[m_cur_location.x][m_cur_location.y].visited = true;
        m_board[m_cur_location.x][m_cur_location.y].safe = true;

        switch (m_cur_direction) {
            case Direction::up:
                m_cur_location.x++;
                break;

            case Direction::down:
                m_cur_location.x--;
                break;

            case Direction::left:
                m_cur_location.y--;
                break;

            case Direction::right:
                m_cur_location.y++;
                break;
        }
    }
	if (!m_go_home_mode) m_actions_taken.push_back(Action::FORWARD);
    return Action::FORWARD;
}

Agent::Action MyAI::turn_left() {
    switch (m_cur_direction) {
        case Direction::up:
            m_cur_direction = Direction::left;
            break;

        case Direction::right:
            m_cur_direction = Direction::up;
            break;

        case Direction::down:
            m_cur_direction = Direction::right;
            break;

        case Direction::left:
            m_cur_direction = Direction::down;
            break;
    }
    if (!m_go_home_mode) m_actions_taken.push_back(Action::TURN_LEFT);
    return Action::TURN_LEFT;
}

Agent::Action MyAI::turn_right() {
    switch (m_cur_direction) {
        case Direction::up:
            m_cur_direction = Direction::right;
            break;

        case Direction::left:
            m_cur_direction = Direction::up;
            break;

        case Direction::down:
            m_cur_direction = Direction::left;
            break;

        case Direction::right:
            m_cur_direction = Direction::down;
            break;
    }
    if (!m_go_home_mode) m_actions_taken.push_back(Action::TURN_RIGHT);
    return Action::TURN_RIGHT;
}