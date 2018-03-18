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
#include <exception>
#include <algorithm>
#include <iostream>

#define MAX_SZ 10

using std::find;
using std::cout;
using std::endl;

bool Location::operator==(const Location &other) {
    return this->x == other.x && this->y == other.y;
}

bool Location::operator!=(const Location &other) {
    return !(*this == other);
}

MyAI::MyAI() : Agent()
{
    m_cur_location = Location(0, 0);
    m_cur_direction = Direction::right;
    m_board.assign(MAX_SZ, vector<State>(MAX_SZ));
    m_go_home_mode = false;
    m_first_run = true;
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
    set_stench(stench);
    set_breeze(breeze);
    // =====================================
    if (get_cur_location() == Location(0, 0) && m_first_run) {
        m_stack.push_back(m_cur_location);
        m_first_run = false;
    }
    return check_curloc(stench, breeze, glitter, bump, scream);
  // =====================================
  // =====================================
}

Agent::Action MyAI::check_curloc(bool stench, bool breeze, bool glitter, bool bump, bool scream) {
    if (!m_goto_path.empty()) {
        Action last_move = m_goto_path.back();
        m_goto_path.pop_back();
        return last_move;
    }


    if (m_go_home_mode) {
        if (!m_turn_to_home.empty()) {
            Action last_move = m_turn_to_home.back();
            m_turn_to_home.pop_back();
            return last_move;
        }
        if (get_cur_location() == Location(0, 0)) {
            return CLIMB;
        } else {
            return go_home();
        }
    }
    if (bump) {
        Location last_location = m_stack.back();
        m_stack.pop_back();
        go_to(last_location);
    } else if (get_cur_location() == m_stack.back() && cur_location_visited()) {
        if (m_stack.back() == Location(0, 0)) {
            return CLIMB;
        }
        Location last_location = m_stack.back();
        m_stack.pop_back();
        go_to(last_location);
    } else if (glitter) {
        m_go_home_mode = true;
        m_turn_to_home.push_back(TURN_RIGHT);
        m_turn_to_home.push_back(TURN_RIGHT);
        return GRAB;
    } else if (breeze || stench) {
        if (get_cur_location() == Location (0,0)) {
            return CLIMB;
        }
        m_stack.pop_back();
        m_goto_path.push_back(FORWARD);     // TO DO: Refactor
        m_goto_path.push_back(TURN_LEFT);   // TO DO: Refactor
        m_goto_path.push_back(TURN_LEFT);   // TO DO: Refactor
    } else if (get_cur_location() != m_stack.back()) {
        Location last_location = m_stack.back();
        go_to(last_location);
    } else {
        expand_neighbors(get_cur_location());
        if (m_stack.back() == get_cur_location()) {
            m_stack.pop_back();
        }
        go_to(m_stack.back());
    }
    Action last_move = m_goto_path.back();
    m_goto_path.pop_back();
    if (last_move == FORWARD) {
        return move_forward(bump);
    } else if (last_move == TURN_RIGHT) {
        return turn_right();
    } else if (last_move == TURN_LEFT) {
        return turn_left();
    }
}
// TODO: refactor
void MyAI::expand_neighbors(Location current_location) {
    if (m_cur_direction == Direction::right) {
        if (m_cur_location.y < m_board.size() - 1 &&
            !location_visited(Location(m_cur_location.x, m_cur_location.y + 1)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x, m_cur_location.y + 1)) == m_stack.end()) {
            m_stack.push_back(Location(m_cur_location.x, m_cur_location.y + 1));
        }
        if (m_cur_location.y > 0 &&
            !location_visited(Location(m_cur_location.x, m_cur_location.y - 1)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x, m_cur_location.y - 1)) == m_stack.end()) {
            m_stack.push_back(Location(m_cur_location.x, m_cur_location.y - 1));
        }
        if (m_cur_location.x < m_board.front().size() - 1 &&
            !location_visited(Location(m_cur_location.x + 1, m_cur_location.y)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x + 1, m_cur_location.y)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x + 1, m_cur_location.y));
    } else if (m_cur_direction == Direction::up) {
        if (m_cur_location.x > 0 &&
            !location_visited(Location(m_cur_location.x - 1, m_cur_location.y)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x - 1, m_cur_location.y)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x - 1, m_cur_location.y));
        if (m_cur_location.x < m_board.front().size() - 1 &&
            !location_visited(Location(m_cur_location.x + 1, m_cur_location.y)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x + 1, m_cur_location.y)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x + 1, m_cur_location.y));
        if (m_cur_location.y < m_board.size() - 1 &&
            !location_visited(Location(m_cur_location.x, m_cur_location.y + 1)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x, m_cur_location.y + 1)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x, m_cur_location.y + 1));
    } else if (m_cur_direction == Direction::left) {
        if (m_cur_location.y > 0 &&
            !location_visited(Location(m_cur_location.x, m_cur_location.y - 1)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x, m_cur_location.y - 1)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x, m_cur_location.y - 1));
        if (m_cur_location.y < m_board.size() - 1 &&
            !location_visited(Location(m_cur_location.x, m_cur_location.y + 1)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x, m_cur_location.y + 1)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x, m_cur_location.y + 1));
        if (m_cur_location.x > 0 &&
            !location_visited(Location(m_cur_location.x - 1, m_cur_location.y)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x - 1, m_cur_location.y)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x - 1, m_cur_location.y));
    } else if (m_cur_direction == Direction::down) {
        if (m_cur_location.x < m_board.front().size() - 1 &&
            !location_visited(Location(m_cur_location.x + 1, m_cur_location.y)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x + 1, m_cur_location.y)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x + 1, m_cur_location.y));
        if (m_cur_location.x > 0 &&
            !location_visited(Location(m_cur_location.x - 1, m_cur_location.y)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x - 1, m_cur_location.y)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x - 1, m_cur_location.y));
        if (m_cur_location.y > 0 &&
            !location_visited(Location(m_cur_location.x, m_cur_location.y - 1)) &&
            find(m_stack.begin(), m_stack.end(), Location(m_cur_location.x, m_cur_location.y - 1)) == m_stack.end())
            m_stack.push_back(Location(m_cur_location.x, m_cur_location.y - 1));
    }
}

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

void MyAI::go_to(Location destination) {
    if (up(get_cur_direction(), get_cur_location()) == destination) {
        m_goto_path.push_back(FORWARD);
    } else if (down(get_cur_direction(), get_cur_location()) == destination) {
        m_goto_path.push_back(FORWARD);
        m_goto_path.push_back(TURN_RIGHT);
        m_goto_path.push_back(TURN_RIGHT);
    } else if (left(get_cur_direction(), get_cur_location()) == destination) {
        m_goto_path.push_back(FORWARD);
        m_goto_path.push_back(TURN_LEFT);
    } else if (right(get_cur_direction(), get_cur_location()) == destination) {
        m_goto_path.push_back(FORWARD);
        m_goto_path.push_back(TURN_RIGHT);
    } else if (NE(get_cur_direction(), get_cur_location()) == destination) {
        if (location_visited(right(get_cur_direction(), get_cur_location()))) {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
        } else {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
            m_goto_path.push_back(FORWARD);
        }
    } else if (SE(get_cur_direction(), get_cur_location()) == destination) {
        if (location_visited(right(get_cur_direction(), get_cur_location()))) {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
        } else {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
            m_goto_path.push_back(TURN_RIGHT);
        }
    } else if (NW(get_cur_direction(), get_cur_location()) == destination) {
        if (location_visited(left(get_cur_direction(), get_cur_location()))) {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
        } else {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
            m_goto_path.push_back(FORWARD);
        }
    } else if (SW(get_cur_direction(), get_cur_location()) == destination) {
        if (location_visited(left(get_cur_direction(), get_cur_location()))) {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
        } else {
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_RIGHT);
            m_goto_path.push_back(FORWARD);
            m_goto_path.push_back(TURN_LEFT);
            m_goto_path.push_back(TURN_LEFT);
        }
    }
}

Location MyAI::up(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x, current_location.y + 1);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x, current_location.y - 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x - 1, current_location.y);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x + 1, current_location.y);
    }
}
Location MyAI::down(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x, current_location.y - 1);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x, current_location.y + 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x + 1, current_location.y);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x - 1, current_location.y);
    }
}
Location MyAI::left(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x - 1, current_location.y);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x + 1, current_location.y - 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x, current_location.y - 1);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x, current_location.y + 1);
    }
}
Location MyAI::right(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x + 1, current_location.y);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x - 1, current_location.y);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x, current_location.y + 1);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x, current_location.y - 1);
    }
}
Location MyAI::NE(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x + 1, current_location.y + 1);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x - 1, current_location.y - 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x - 1, current_location.y + 1);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x + 1, current_location.y - 1);
    }
}
Location MyAI::NW(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x - 1, current_location.y + 1);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x + 1, current_location.y - 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x - 1, current_location.y - 1);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x + 1, current_location.y + 1);
    }
}
Location MyAI::SE(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x + 1, current_location.y - 1);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x - 1, current_location.y + 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x + 1, current_location.y + 1);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x - 1, current_location.y - 1);
    }
}
Location MyAI::SW(Direction face_direction, Location current_location) {
    if (face_direction == Direction::up) {
        return Location(current_location.x - 1, current_location.y - 1);
    } else if (face_direction == Direction::down) {
        return Location(current_location.x + 1, current_location.y + 1);
    } else if (face_direction == Direction::left) {
        return Location(current_location.x + 1, current_location.y - 1);
    } else if (face_direction == Direction::right) {
        return Location(current_location.x - 1, current_location.y + 1);
    }
}

const Board& MyAI::get_board() {
	return m_board;
}

const State& MyAI::get_state(const Location &location) {
    if (location.x < 0 || location.x >= m_board.front().size() ||
        location.y < 0 || location.y >= m_board.size()) {
        throw std::runtime_error("bad location");
    }
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
        m_board[m_cur_location.x][m_cur_location.y].visited = true;
        switch (m_cur_direction) {
            case Direction::up:
                if (m_cur_location.y < m_board.size() - 1) m_cur_location.y++;
                break;

            case Direction::down:
                if (m_cur_location.y > 0) m_cur_location.y--;
                break;

            case Direction::left:
                if (m_cur_location.x > 0)m_cur_location.x--;
                break;

            case Direction::right:
                if (m_cur_location.x < m_board.size() - 1) m_cur_location.x++;
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
    if (m_cur_direction == Direction::right) { // restrict cols
        for (auto &col : m_board) col.resize(m_cur_location.x + 1);
    } else if (m_cur_direction == Direction::up) { // restrict rows
        m_board.resize(m_cur_location.y + 1);
    }
}

void MyAI::set_visited() {
    m_board[m_cur_location.x][m_cur_location.y].visited = true;
}

void MyAI::set_safety(const Location &location, Safety safety) {
    m_board[location.x][location.y].safety = safety;
}

Location MyAI::up(const Location &location) {
    return Location(location.x, location.y + 1);
}

Location MyAI::down(const Location &location) {
    return Location(location.x, location.y - 1);
}

Location MyAI::left(const Location &location) {
    return Location(location.x - 1, location.y);
}

Location MyAI::right(const Location &location) {
    return Location(location.x + 1, location.y);
}

bool MyAI::has_visited_up_neighbor(const Location &location) {
    return location.y < m_board.size() - 1 &&
           (up(location) == m_cur_location || get_state(up(location)).visited);
}

bool MyAI::has_visited_down_neighbor(const Location &location) {
    return location.y > 0 &&
           (down(location) == m_cur_location || get_state(down(location)).visited);
}

bool MyAI::has_visited_left_neighbor(const Location &location) {
    return location.x > 0 &&
           (left(location) == m_cur_location || get_state(left(location)).visited);
}

bool MyAI::has_visited_right_neighbor(const Location &location) {
    return location.x < m_board.front().size() - 1 &&
           (right(location) == m_cur_location || get_state(right(location)).visited);
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