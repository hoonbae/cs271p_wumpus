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
#include "exception"

#define MAX_SZ 10

MyAI::MyAI() : Agent()
{
    m_cur_location = Location(0, 0);
    m_cur_direction = Direction::right;
    m_board.assign(MAX_SZ, vector<State>(MAX_SZ));
    m_go_home_mode = false;
    m_bump_turn_count = 0;
    m_gold_status = false;
    m_shot_status = false;
    m_dir_cw = true;
    m_pass_home = false;
    m_switch_dir = 0;
    m_first_action = true;
    m_change_dir = false;

}

MyAI::~MyAI() {
    m_board.clear();
    m_actions_taken.clear();
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
    set_visited();
    set_stench(stench);
    set_breeze(breeze);
    // =====================================

  if (m_first_action) {
    m_first_action = false;

    if (stench || breeze) {
      return Agent::CLIMB;
    }

    if (glitter) {
      m_gold_status = true;
      m_actions_taken.push_back(Agent::CLIMB);
      return Agent::GRAB;
    }
  }

  if (!m_go_home_mode) {
    if (m_dir_cw) {
      return move_cw(stench, breeze, glitter, bump, scream);
    } else {
      if (m_change_dir) {
        return move_turnaround(bump);
      }
      return move_countercw(stench, breeze, glitter, bump, scream);
    }
  } else {
    if (m_change_dir) {
      return move_turntohome();
    }
    return go_home();
  }

  // =====================================
  // =====================================
}


// Go Home Actions
Agent::Action MyAI::go_home() {
    if (!m_actions_taken.empty()) {
      auto last_action_taken = m_actions_taken.back();
      m_actions_taken.pop_back();
      if (last_action_taken == Action::TURN_LEFT) return Action::TURN_RIGHT;
      else if(last_action_taken == Action::TURN_RIGHT) return Action::TURN_LEFT;
      return last_action_taken;
    } else {
      return Action::CLIMB;
    }
}

Agent::Action MyAI::move_turnaround(bool bump) {
  if (m_switch_dir < 2) {
    m_switch_dir++;
    return turn_left();
  } else {
    m_change_dir = false;
    m_switch_dir = 0;
    return move_forward(bump);
  }
}

Agent::Action MyAI::move_turntohome() {
  if (m_switch_dir < 1) {
    m_switch_dir++;
    return Action::TURN_RIGHT;
  } else {
    m_change_dir = false;
    m_switch_dir = 0;
    return Action::TURN_RIGHT;
  }
}

// Exploring Perimeter Functions
Agent::Action MyAI::move_cw(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
  if (glitter) {
    m_gold_status = true;
    m_change_dir = true;
    m_go_home_mode = true;
    return Action::GRAB;
  }
  if (stench || breeze) {
    m_dir_cw = false;
    m_change_dir = true;
    return Action::SHOOT;
  }
  if (bump) {
    if(m_bump_turn_count == 3) {
      return Action::CLIMB;
    }
    m_bump_turn_count++;
    return turn_left();
  }
  return move_forward(bump);
}

Agent::Action MyAI::move_countercw(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
    if (glitter) {
      m_gold_status = true;
      m_change_dir = true;
      m_go_home_mode = true;
      return Action::GRAB;
    }
    if (stench || breeze) {
      m_change_dir = true;
      m_go_home_mode = true;
      return Action::SHOOT;
    }
    if (bump) {
      // moving counter clockwise but haven't passed by home yet
      if (m_bump_turn_count > 0 && !m_pass_home) {
        m_bump_turn_count--;
        return turn_right();
      }
      // moving counter clockwise - landed home, first time passing
      if (m_bump_turn_count == 0 && !m_pass_home) {
        m_bump_turn_count++;
        m_pass_home = true;
        m_actions_taken.push_back(Action::CLIMB);
        return turn_right();
      }
      // moving counter clockwise - have passed home
      if (m_bump_turn_count > 0 && m_pass_home) {
        m_bump_turn_count++;
        return turn_right();
      }
    }
    return move_forward(bump);
}


const Board& MyAI::get_board() {
	return m_board;
}

const State& MyAI::get_state(const Location &location) {
    if (location.x < 0 || location.x >= m_board.size() ||
        location.y < 0 || location.y >= m_board.front().size())
        throw std::runtime_error("bad location");
    return m_board[location.x][location.y];
}

Location MyAI::get_cur_location() {
	return m_cur_location;
}

Direction MyAI::get_cur_direction() {
	return m_cur_direction;
}

Safety MyAI::location_safe(const Location &location) {
    if (m_board[location.x][location.y].safety == Safety::safe ||
        m_board[location.x][location.y].safety == Safety::danger)
        return m_board[location.x][location.y].safety;

    if (has_visited_and_not_breezy_neighbors(location) && has_visited_and_not_stenchy_neighbors(location)) {
        set_safety(location, Safety::safe);
        return Safety::safe;
    }

    return Safety::maybe;
}

bool MyAI::location_visited(const Location &location) {
	return m_board[location.x][location.y].visited;
}

Safety MyAI::cur_location_safe() {
    return m_board[m_cur_location.x][m_cur_location.y].safety;
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
        m_board[m_cur_location.x][m_cur_location.y].safety = Safety::safe;

        switch (m_cur_direction) {
            case Direction::up:
                if (m_cur_location.x < m_board.size()) m_cur_location.x++;
                break;

            case Direction::down:
                if (m_cur_location.x > 0) m_cur_location.x--;
                break;

            case Direction::left:
                if (m_cur_location.x > 0)m_cur_location.y--;
                break;

            case Direction::right:
                if (m_cur_location.y < m_board.front().size()) m_cur_location.y++;
                break;
        }
    } else {
        process_bump();
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

void MyAI::process_bump() {
    if (m_cur_direction == Direction::right) {
        for (auto &col : m_board) col.resize(m_cur_location.y + 1);
    } else if (m_cur_direction == Direction::up) {
        m_board.resize(m_cur_location.x + 1);
    }
}

void MyAI::set_visited() {
    m_board[m_cur_location.x][m_cur_location.y].visited = true;
}

void MyAI::set_safety(const Location &location, Safety safety) {
    m_board[location.x][location.y].safety = safety;
}

Location MyAI::up(const Location &location) {
    return Location(location.x, location.y - 1);
}

Location MyAI::down(const Location &location) {
    return Location(location.x, location.y + 1);
}

Location MyAI::left(const Location &location) {
    return Location(location.x - 1, location.y);
}

Location MyAI::right(const Location &location) {
    return Location(location.x + 1, location.y);
}

bool MyAI::has_visited_up_neighbor(const Location &location) {
    return location.y > 0 && get_state(up(location)).visited;
}

bool MyAI::has_visited_down_neighbor(const Location &location) {
    return location.y < m_board.front().size() - 1 && get_state(down(location)).visited;
}

bool MyAI::has_visited_left_neighbor(const Location &location) {
    return location.x > 0 && get_state(left(location)).visited;
}

bool MyAI::has_visited_right_neighbor(const Location &location) {
    return location.x < m_board.size() - 1 && get_state(right(location)).visited;
}

bool MyAI::all_neighbors_visited(const Location &location) {
    bool all_visited = true;

    if (location.x > 0)
        all_visited &= has_visited_left_neighbor(location);
    if (location.x < m_board.size() - 1)
        all_visited &= has_visited_right_neighbor(location);
    if (location.y > 0)
        all_visited &= has_visited_up_neighbor(location);
    if (location.y < m_board.front().size() - 1)
        all_visited &= has_visited_down_neighbor(location);

    return all_visited;
}

bool MyAI::has_visited_and_not_breezy_neighbors(const Location &location) {
    return (has_visited_up_neighbor(location) && !get_state(up(location)).breeze) ||
           (has_visited_down_neighbor(location) && !get_state(down(location)).breeze) ||
           (has_visited_left_neighbor(location) && !get_state(left(location)).breeze) ||
           (has_visited_right_neighbor(location) && !get_state(right(location)).breeze);
}

bool MyAI::has_visited_and_not_stenchy_neighbors(const Location &location) {
    return (has_visited_up_neighbor(location) && !get_state(up(location)).stench) ||
           (has_visited_down_neighbor(location) && !get_state(down(location)).stench) ||
           (has_visited_left_neighbor(location) && !get_state(left(location)).stench) ||
           (has_visited_right_neighbor(location) && !get_state(right(location)).stench);
}