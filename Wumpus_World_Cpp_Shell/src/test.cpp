#include "MyAI.hpp"
#include <cassert>
#include <iostream>

using std::cout;
using std::endl;

void test_init() {
    MyAI ai;
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(0, 0));
    cout << "test_init ... OK" << endl;
}

void test_rotate() {
    MyAI ai;
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::up);
    assert(ai.get_cur_location() == Location(0, 0));
    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_direction() == Direction::left);
    assert(ai.get_cur_location() == Location(0, 0));

    // boundary test
    assert(ai.move_forward(true) == Agent::Action::FORWARD);
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

void test_move() {
    MyAI ai;
    assert(ai.move_forward(false) == Agent::Action::FORWARD);
    assert(ai.get_cur_direction() == Direction::right);
    assert(ai.get_cur_location() == Location(1, 0));
    assert(ai.location_safe(Location(1, 0)) == Safety::safe);
    assert(ai.location_visited(Location(1, 0)) == false);
    assert(ai.cur_location_safe() == Safety::safe);
    assert(ai.cur_location_visited() == false);
    assert(ai.location_safe(Location(0, 0)) == Safety::safe);
    assert(ai.location_visited(Location(0, 0)) == true);
    cout << "test_move ... OK" << endl;
}

void test_setters(MyAI &ai) {
    ai.set_breeze(true);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].breeze == true);
    ai.set_stench(true);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].stench == true);
    ai.set_visited();
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].visited == true);
    ai.set_breeze(false);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].breeze == false);
    ai.set_stench(false);
    assert(ai.get_board()[ai.get_cur_location().x][ai.get_cur_location().y].stench == false);
    cout << "test_setters ... OK" << endl;
}

void test_udlw() {
    Location location(1,1);
    MyAI ai;
    assert(ai.left(location) == Location(0, 1));
    assert(ai.right(location) == Location(2, 1));
    assert(ai.up(location) == Location(1, 2));
    assert(ai.down(location) == Location(1, 0));
    cout << "test_up_down_left_right ... OK" << endl;
}

void test_boundary() {
    vector<vector<int>> matrix(3, vector<int>(5));
    assert(matrix.size() == 3 ); // rows
    assert(matrix.front().size() == 5); // cols

    MyAI ai;
    assert(ai.get_board().size() == 10 && ai.get_board().front().size() == 10);

    for (int i = 0; i < 3; ++i) ai.move_forward(false);
    assert(ai.get_cur_location() == Location(3, 0));

    ai.move_forward(true);
    assert(ai.get_cur_location() == Location(3, 0));
    assert(ai.get_board().size() == 10); // rows
    assert(ai.get_board().front().size() == 4); // cols

    assert(ai.turn_left() == Agent::Action::TURN_LEFT);
    assert(ai.get_cur_location() == Location(3, 0));
    assert(ai.get_cur_direction() == Direction::up);

    for (int i = 0; i < 4; ++i) {
        ai.move_forward(false);
    }
    assert(ai.get_cur_location() == Location(3, 4));

    ai.move_forward(true);
    assert(ai.get_cur_location() == Location(3, 4));
    assert(ai.get_board().size() == 5); // row
    assert(ai.get_board().front().size() == 4); // cols

    ai.move_forward(true);
    assert(ai.get_cur_location() == Location(3, 4));
    assert(ai.get_board().front().size() == 4);
    assert(ai.get_board().size() == 5);

    for (int i = 0; i < 4; ++i) {
        assert(ai.location_safe(Location(i, 0)) == Safety::safe);
        assert(ai.location_visited(Location(i, 0)) == true);
    }
    for (int j = 0; j < 4; ++j) {
        assert(ai.location_safe(Location(3, j)) == Safety::safe);
        assert(ai.location_visited(Location(3, j)) == true);
    }
    assert(ai.location_safe(Location(3, 4)) == Safety::safe);
    assert(ai.location_visited(Location(3, 4)) == false);
    assert(ai.has_visited_left_neighbor(Location(3, 0)) == true);

    ai.turn_left();
    ai.move_forward(false);
    assert(ai.location_visited(Location(3, 4)) == true);
    assert(ai.has_visited_up_neighbor(Location(3, 0)) == true);
    assert(ai.has_visited_left_neighbor(Location(3, 0)) == true);
    assert(ai.has_visited_right_neighbor(Location(3, 0)) == false);
    assert(ai.has_visited_down_neighbor(Location(3, 0)) == false);
    cout << "test_boundary ... OK" << endl;
}

int main() {
    test_init();
    test_rotate();
    test_move();
    test_udlw();
    test_boundary();
};