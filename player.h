#pragma once
#include <string>
using namespace std;

class Player
{
public:
	Player(const string& new_name = "No Name");
	string getName() const;
	int getPoints() const;
  int getTurns() const;
	void setName(const string& new_name);
	void setPoints(int new_points);
  void setTurns(int new_turns);
private:
	string name;
	int turns, points;
};