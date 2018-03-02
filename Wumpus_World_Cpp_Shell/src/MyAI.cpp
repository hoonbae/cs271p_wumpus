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
    m_go_home_mode = false;
    m_turn_count = 0;
    m_gold_status = false;
    m_shot_status = false;
    m_dir_cw = true;
    m_pass_home = false;

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

  bool go_home_mode = get_go_home_mode();
  if (!go_home_mode) {
    if (get_dir_cw_status) {
      // move clockwise
    } else {
      // move counter clockwise
    }
  } else {

  }

}



Agent::Action MyAI::move_cw(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
  if (stench || breeze) {
    is_dir_cw(false);
  }
  if (bump) {
    int bump_turn_count = get_bump_turn_count();
    if(bump_turn_count == 3) {
      return CLIMB;
    }
    increment_bump_turn_count();
    return turn_right();
  }
  if (glitter) {
    set_gold_to_true();
    is_go_home_mode(true);
    return GRAB;
  }
  return move_forward(bump);
}

Agent::Action MyAI::move_countercw(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
  // just changed direcetion?

  if (stench || breeze) {
    is_go_home_mode(true);
  }
  if (bump) {
    int bump_turn_count = get_bump_turn_count();
    bool is_pass_home = get_pass_home();

    // moving counter clockwise but haven't passed by home yet
    if (bump_turn_count > 0 && !is_pass_home) {
      decrement_bump_turn_count();
      return turn_left();
    }

    // moving counter clockwise - landed home, first time passing
    if (bump_turn_count == 0 && !is_pass_home) {
      increment_bump_turn_count();
      is_pass_home(true);
      return turn_left();
    }

    if (bump_turn_count > 3 && is_pass_home) { // ?
      is_go_home_mode(true);
      // ?? return something here?
    }

    // moving counter clockwise - have passed home
    if (bump_turn_count > 0 && is_pass_home) {
      increment_bump_turn_count();
      return turn_left();
    }
  }
  if (glitter) {
    set_gold_to_true();
    is_go_home_mode(true);
    return GRAB;
  }
  return move_forward(bump);

}



// Setters
void MyAI::set_shot_to_true()
{
	m_shot_status = true;
}
void MyAI::set_gold_to_true()
{
	m_gold_status = true;
}
void  MyAI::increment_bump_turn_count() {
  m_turn_count++;
}
void  MyAI::decrement_bump_turn_count() {
  m_turn_count--;
}
bool  MyAI::is_dir_cw(bool is_clockwise) {
  m_dir_cw = is_clockwise;
}
bool  MyAI::is_go_home_mode(bool is_home) {
  m_go_home_mode = is_home;
}
bool MyAI::is_pass_home(bool pass_home) {
  m_pass_home = pass_home;
}


// Getters
bool MyAI::get_shot_status()
{
	return m_shot_status;
}
bool MyAI::get_gold_status()
{
	return m_gold_status;
}
int   MyAI::get_bump_turn_count() {
  return m_bump_turn_count;
}
bool  MyAI::get_dir_cw_status() {
  return m_dir_cw;
}
bool  MyAI::get_go_home_mode() {
  return m_go_home_mode;
}
bool MyAI::get_pass_home() {
  return m_pass_home;
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