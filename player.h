#pragma once
#include <string>
using namespace std;

class Player
{
public:
	Player(const string& new_name = "No Name");
	string getName() const;
	int getPoints() const;
	void setName(const string& new_name);
	void setPoints(int new_points);
private:
	string name;
	int points;
};