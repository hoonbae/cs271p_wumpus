#include "MyAI.hpp"
#include <cassert>
#include <iostream>

using std::cout;
using std::endl;

bool operator==(const Location &lhs, const Location &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void test_init(MyAI &ai) {
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 0));
    cout << "test_init ... OK" << endl;
}

void test_rotate(MyAI &ai) {
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::left);
    assert(ai.get_cur_location() == Location(0, 0));

    // boundary test
    assert(ai.move_forward(false) == Agent::Action::FORWARD);
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
    cout << "test_rotate ... OK" << endl;
}

void test_move(MyAI &ai) {
    assert(ai.move_forward(false) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 1));
    // assert(ai.location_safe(Location(0, 1)) == false);
    assert(ai.location_visited(Location(0, 1)) == false);
    // assert(ai.cur_location_safe() == false);
    assert(ai.cur_location_visited() == false);
    // assert(ai.location_safe(Location(0, 0)) == true);
    assert(ai.location_visited(Location(0, 0)) == true);
    cout << "test_move ... OK" << endl;
}

void test_bump(MyAI &ai) {
    assert(ai.move_forward(true) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 1));
    // assert(ai.location_safe(Location(0, 1)) == false);
    assert(ai.location_visited(Location(0, 1)) == false);
    // assert(ai.cur_location_safe() == false);
    assert(ai.cur_location_visited() == false);
    // assert(ai.location_safe(Location(0, 0)) == true);
    assert(ai.location_visited(Location(0, 0)) == true);

    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(0, 1));

    assert(ai.move_forward(false) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(1, 1));
    // assert(ai.location_safe(Location(1, 1)) == false);
    assert(ai.location_visited(Location(1, 1)) == false);
    // assert(ai.cur_location_safe() == false);
    assert(ai.cur_location_visited() == false);
    // assert(ai.location_safe(Location(0, 1)) == true);
    assert(ai.location_visited(Location(0, 1)) == true);
    cout << "test_bump ... OK" << endl;
}

void test_setters(MyAI &ai) {
    ai.set_breeze(true);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].breeze == true);
    ai.set_stench(true);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].stench == true);
    ai.set_breeze(false);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].breeze == false);
    ai.set_stench(false);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].stench == false);
    cout << "test_setters ... OK" << endl;
}

void test_boundary() {
    MyAI ai;
    assert(ai.get_board().size() == 10 && ai.get_board().front().size() == 10);

    for (int i = 0; i < 3; ++i) ai.move_forward(false);
    assert(ai.get_cur_location() == Location(0, 3));

    ai.move_forward(true);
    assert(ai.get_cur_location() == Location(0, 3));
    assert(ai.get_board().front().size() == 4);
    assert(ai.get_board().size() == 10);

    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::up);

    for (int i = 0; i < 4; ++i) ai.move_forward(false);
    assert(ai.get_cur_location() == Location(4, 3));

    ai.move_forward(true);
    assert(ai.get_cur_location() == Location(4, 3));
    assert(ai.get_board().front().size() == 4);
    assert(ai.get_board().size() == 5);

    ai.move_forward(true);
    assert(ai.get_cur_location() == Location(4, 3));
    assert(ai.get_board().front().size() == 4);
    assert(ai.get_board().size() == 5);

    cout << "test_boundary ... OK" << endl;
}

int main() {
    MyAI ai;
    test_init(ai);
    test_rotate(ai);
    test_move(ai);
    test_bump(ai);
    test_boundary();
};