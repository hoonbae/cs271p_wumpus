#include "MyAI.hpp"
#include <cassert>

bool operator==(const Location &lhs, const Location &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

int main() {
    MyAI ai;
    // initialization checks
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 0));

    // rotation checks
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::left);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::down);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 0));

    assert(ai.turn_right() == Agent::Action::TURN_RIGHT);
    assert(ai.get_cur_direction() == Direction::down);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_right() == Agent::Action::TURN_RIGHT);
    assert(ai.get_cur_direction() == Direction::left);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_right() == Agent::Action::TURN_RIGHT);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_right() == Agent::Action::TURN_RIGHT);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 0));

    // movement checks
    assert(ai.move_forward(false) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 1));
    assert(ai.location_safe(Location(0, 1)) == false);
    assert(ai.location_visited(Location(0, 1)) == false);
    assert(ai.cur_location_safe() == false);
    assert(ai.cur_location_visited() == false);
    assert(ai.location_safe(Location(0, 0)) == true);
    assert(ai.location_visited(Location(0, 0)) == true);

    // if there's a bump and agent tries to move forward, nothing should change!
    assert(ai.move_forward(true) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 1));
    assert(ai.location_safe(Location(0, 1)) == false);
    assert(ai.location_visited(Location(0, 1)) == false);
    assert(ai.cur_location_safe() == false);
    assert(ai.cur_location_visited() == false);
    assert(ai.location_safe(Location(0, 0)) == true);
    assert(ai.location_visited(Location(0, 0)) == true);

    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(0, 1));

    assert(ai.move_forward(false) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(1, 1));
    assert(ai.location_safe(Location(1, 1)) == false);
    assert(ai.location_visited(Location(1, 1)) == false);
    assert(ai.cur_location_safe() == false);
    assert(ai.cur_location_visited() == false);
    assert(ai.location_safe(Location(0, 1)) == true);
    assert(ai.location_visited(Location(0, 1)) == true);

    // checking setters
    ai.set_breeze(true);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].breeze == true);
    ai.set_stench(true);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].stench == true);
};