#include <iomanip>
#include <iostream>
#include <string>
#include <ctime>
#include <random>

using namespace std;

const int N = 5;
const int M = 64;

struct Player
{
   string name;
   int turns;
   int points;
};
struct Dice
{
   int die1;
   int die2;
};

default_random_engine e;
uniform_int_distribution<int> uDice(1,6);
uniform_int_distribution<int> uPlayers(0, N - 1);
Player players[N];
Dice dice;

void init(string names[])
{
   for(int index = 0; index < N; index++)
   {
       players[index].points = 0;
       players[index].turns = 0;
       players[index].name = names[index];
   }
}
void rollDice()
{
   dice.die1 = uDice(e);
   dice.die2 = uDice(e);
}

int findCollision(int player_index)
{
   int old_points = players[player_index].points;
   if (old_points == -1)
       players[player_index].points = dice.die1 + dice.die2;
   else
       players[player_index].points += dice.die1 + dice.die2;
   int points = players[player_index].points;
   int index = 0;

   while (index < N && (players[index].points != points || index == player_index))
       index++;
   players[player_index].points = old_points;
   if (index < N)
       return index;
   else
       return -1;
}
void manageCollision(int player_index, int collision_index)
{
   players[collision_index].points = 0;
   players[collision_index].turns--;
   players[player_index].turns++;
}
int computePointsState(int player_index)
{
   int old_points = players[player_index].points;
   if (old_points == -1)
       players[player_index].points = dice.die1 + dice.die2;
   else
       players[player_index].points += dice.die1 + dice.die2;
   int points = players[player_index].points;
   int points_state;
   if (points < -1)
       points_state = 3;
   else if (0 < points && points < M)
       points_state = 0;
   else if (points == M)
       points_state = 1;
   else
       points_state = 2;
   players[player_index].points = old_points;
   return points_state;
}
int computeState(int player_index)
{
   int dice_state = dice.die1 == dice.die2 ? 1 : 0;
   int collision_state = findCollision(player_index) > -1 ? 1 : 0;

   int points_state = computePointsState(player_index);
   int state = 8 * dice_state + 4 * collision_state + points_state;
   return state;
}
int main()
{
   string names[] = { "Ned", "Zed", "Jed", "Ted", "Red" };
   init( names);  
  
   e.seed((unsigned)time(0));
   int index = uPlayers(e);
   bool game_continues = true;
   int throw_count = 0;
   do
   {
       if(players[index].turns < 0)
       {
           players[index].turns++;
           cout << players[index].name << " gives up dice" << endl;
           index = (index + 1) % N;
       }
       else
       {
           rollDice();
           int sum_dice = dice.die1 + dice.die2;
           //players[index].points += sum_dice;
           //cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
           int state = computeState(index);
           int index2 = -1;
           switch (state)
           {
           case 0:
               players[index].points += sum_dice;
               cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
               break;
           case 1: case 7:
               players[index].points += sum_dice;
               cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
               cout << players[index].name << " wins" << endl;
               game_continues = false;
               break;
           case 2:
               players[index].points /= 2;
               players[index].turns--;
               cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
               cout << players[index].name << " loses 1/2 points and next turn" << endl;
               index2 = findCollision(index);
               if (index2 != -1)
               {
                   manageCollision(index, index2);
                   cout << players[index].name << " recovers next turn - collision" << endl;
                   cout << players[index2].name << " loses points and next turn - collision" << endl;
               }
               break;
           case 3:
               cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
               cout << players[index].name << " did not roll doubles and does not move" << endl;
               break;
           case 4:
               cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
               players[index].points += sum_dice;
               index2 = findCollision(index);
               manageCollision(index, index2);
               cout << players[index].name << " gets extra turn - collision" << endl;
               cout << players[index2].name << " loses points and must roll doubles to move again - collision" << endl;
               break;
           case 8:
               cout << setw(4) << ++throw_count << '/' << players[index].name << '/' << sum_dice << '/' << players[index].points << endl;
               players[index].points += sum_dice;
               players[index].turns++;
               cout << players[index].name << " gets extra turn - doubles" << endl;
               break;
           case 10:
               players[index].points /= 2;
               cout << players[index].name << " loses 1/2 points" << endl;
               index2 = findCollision(index);
               if (index2 != -1)
               {
                   manageCollision(index, index2);
                   cout << players[index].name << " recovers extra turn - collision" << endl;
                   cout << players[index2].name << " loses points and next turn - collision" << endl;
               }
               break;
           case 9:
               players[index].turns++;
               index2 = findCollision(index);
               manageCollision(index, index2);
               cout << players[index].name << " gets extra turn - doubles" << endl;
               cout << players[index].name << " gets extra turn - collision" << endl;
               cout << players[index2].name << " loses points and next turn - collision" << endl;
               break;
           }
           if (players[index].turns <= 0)
               index = (index + 1) % N;
           else
               players[index].turns--;
       }
   }
   while(game_continues);
   std::system("pause");
   return 0;
}