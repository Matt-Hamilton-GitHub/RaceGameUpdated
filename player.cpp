#include <cassert>
#include "player.h"

Player::Player(const string& new_name)
{
	setName(new_name);
	points = 0;
}
string Player::getName() const
{
	return name;
}
int Player::getPoints() const
{
	return points;
}
int Player::getTurns() const{
  return turns;
}

void Player::setName(const string& new_name)
{
	name = new_name;
}
void Player::setTurns(int new_turns)
{
	turns = new_turns;
}
void Player::setPoints(int new_points)
{
	points = new_points;
}