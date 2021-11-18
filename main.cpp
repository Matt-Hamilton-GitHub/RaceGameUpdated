

#include <iomanip>
#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include <fstream>
#include "player.h"

using namespace std;

int states(Player players[] ,int player_index, int N, int M, int die1, int die2);
int pointsAndStates(Player players[], int player_index,  int M, int die1, int die2);
int getCollisions(Player players[],int player_index, int N, int die1, int die2);
void solveCollisions(Player players[], int player_index, int collision_index, int die1, int die2);


int main()
{

	
  string name;

cout << "Please enter the data file name: ";
	string file_name;
	cin >> file_name;
	ifstream inps(file_name);

	if (!inps.good())
	{
		cout << "cannot open " << file_name << endl;
		system("pause");
		exit (EXIT_FAILURE);
	}
	
	int M;
	int N;
	inps >> M;
	if (M < 3)
	{
		cout << M << " is too small for winner points" << endl;
		cout << "The winner points must be at least 3" << endl;
		system("pause");
		exit(1);
	}
	inps >> N;
	
	if (N < 1)
	{
		cout << N << " is too small for number of players" << endl;
		cout << "number of players must be at least 1" << endl;
		system("pause");
		exit(2);
	}

	Player *players = new Player[N];


   for(int index = 0; index < N; index++)
   {
	   //set all the data for each player
	   inps >> name;
       players[index].setPoints(0);
       players[index].setTurns(0);
       players[index].setName(name);
   }
  
  
    default_random_engine e((unsigned)time(nullptr));
	uniform_int_distribution<int> pu(0, N - 1);
	uniform_int_distribution<int> du(1, 6);
	
	
   int index = pu(e);
   bool gameOver = false;
   int count = 0;
   
   do
   {
	   
		int die1 = du(e);
		int die2 = du(e);
	   
       if(players[index].getTurns() < 0)
       {
           players[index].setTurns(players[index].getTurns() + 1 );
           cout <<"***" << players[index].getName() << " -> loses turn [points < 0] " << endl;
           index = (index + 1) % N;
       }
       else
       {
          
           int sum_dice = die1 + die2;
           int state = states(players,index, N, M, die1, die2);
           int index2 = -1;
           switch (state)
           {
           case 0:
               players[index].setPoints(players[index].getPoints() + sum_dice );
               cout << "\t [ " << ++count << " ] " << players[index].getName() << "/" << sum_dice << "/" << players[index].getPoints() << endl;
               break;
           case 1: 
		   case 7:
               players[index].setPoints(players[index].getPoints() + sum_dice);
               cout << "\t [ " << ++count << " ] "<< players[index].getName() << '/' << sum_dice << '/' << players[index].getPoints() << endl;
               cout <<"*** CONGARTS! " << players[index].getName() << " -> wins the game!" << endl;
               gameOver = true;
               break;
           case 2:
               players[index].setPoints(players[index].getPoints()  / 2);
               players[index].setTurns( players[index].getTurns() - 1);
               cout << "\t [ " << ++count << " ] "<< players[index].getName() << '/' << sum_dice << '/' << players[index].getPoints() << endl;
               cout << "***" << players[index].getName() << " -> loses 1/2 points and next turn" << endl;
               index2 = getCollisions(players, index, N, die1, die2);
               if (index2 != -1)
               {
                   solveCollisions(players, index, index2, die1, die2);
                   cout <<"***" <<  players[index].getName() << " -> recovers next turn [collision]" << endl;
                   cout << players[index2].getName() << " loses points and next turn [collision]" << endl;
               }
               break;
           case 3:
               cout << "\t [ " << ++count << " ] "<< players[index].getName()<< '/' << sum_dice << '/' << players[index].getPoints() << endl;
               cout <<"" << players[index].getName() << "  did not roll doubles and does not move" << endl;
               break;
           case 4:
               cout  << "\t [ " << ++count << " ] "<< players[index].getName() << '/' << sum_dice << '/' << players[index].getPoints() << endl;
               players[index].setPoints(  players[index].getPoints() + sum_dice);
               index2 = getCollisions(players, index, N, die1, die2);
               solveCollisions(players,  index, index2, die1, die2);
               cout <<"***" <<  players[index].getName() << " -> gets extra turn [collision]" << endl;
               cout << players[index2].getName() << " loses points and must roll doubles to move again [collision]" << endl;
               break;
           case 8:
               cout  << "\t [ " << ++count << " ] "<< players[index].getName() << '/' << sum_dice << '/' << players[index].getPoints() << endl;
               players[index].setPoints(players[index].getPoints() + sum_dice );
               players[index].setTurns(players[index].getTurns() + 1);
               cout <<"***" << players[index].getName() << " -> gets extra turn [doubles]" << endl;
               break;
           case 10:
               players[index].setPoints(players[index].getPoints()  / 2);
               cout << "***" << players[index].getName() << " -> loses 1/2 points" << endl;
               index2 = getCollisions(players, index,  N, die1, die2);
               if (index2 != -1)
               {
                   solveCollisions(players, index, index2, die1, die2);
                   cout << "***" << players[index].getName() << " -> recovers extra turn  [collision]" << endl;
                   cout << players[index2].getName() << " loses points and next turn [collision]" << endl;
               }
               break;
           case 9:
               players[index].setTurns(players[index].getTurns() + 1);
               index2 = getCollisions(players, index, N, die1, die2);
               solveCollisions(players, index, index2, die1, die2);
               cout <<"***" << players[index].getName() << "-> gets extra turn [doubles]" << endl;
               cout << "***" << players[index].getName() << "-> gets extra turn [collision]" << endl;
               cout << players[index2].getName() << " loses points and next turn  [collision]" << endl;
               break;
           }
           if (players[index].getTurns() <= 0)
               index = (index + 1) % N;
           else
               players[index].setTurns(players[index].getTurns() - 1);
       }
   }
   while(!gameOver);
   std::system("pause");
   return 0;
}



int getCollisions(Player players[],int player_index, int N, int die1, int die2){

   
   if (players[player_index].getPoints() == -1)//if user's points = -1
       players[player_index].setPoints(die1 + die2);//set new points
   else
       players[player_index].setPoints(players[player_index].getPoints() + die1 + die2);//if not, add current points to the sum of the dies
   int index = 0;

   while (index < N && (players[index].getPoints() != players[player_index].getPoints() || index == player_index))
       index++;
   players[player_index].setPoints( players[player_index].getPoints());
   if (index < N)
       return index;
   else
       return -1;
}


void solveCollisions(Player players[], int player_index, int collision_index, int die1, int die2)
{
   players[collision_index].setPoints(0);
   players[collision_index].setTurns(players[collision_index].getTurns() -1);
   players[player_index].setTurns(players[collision_index].getTurns() + 1);;
}


int pointsAndStates(Player players[], int player_index,  int M, int die1, int die2)
{
   
   if (players[player_index].getPoints() == -1) //if user's points = -1
       players[player_index].setPoints(die1 + die2);//set new points equl to the sum of the two dies
   else
	   //if not, add current points to the sum of the dies
       players[player_index].setPoints(players[player_index].getPoints() + die1 + die2);
   int pts_state;
   if (players[player_index].getPoints() < -1)
       pts_state = 3;
   else if (0 < players[player_index].getPoints() && players[player_index].getPoints() < M)
       pts_state = 0;
   else if (players[player_index].getPoints() == M)
       pts_state = 1;
   else
       pts_state = 2;
   players[player_index].setPoints(players[player_index].getPoints());
   return pts_state;
}

int states(Player players[] ,int player_index, int N, int M, int die1, int die2)
{
	//dice sate is based on whether die1 equls die2
   int dice_state = die1 == die2 ? 1 : 0;
   
   int collision_state = getCollisions(players, player_index, N, die1, die2) > -1 ? 1 : 0;
   int points_state = pointsAndStates(players, player_index, M, die1, die2);
   //state calculation
   int state = 8 * dice_state + 4 * collision_state + points_state;
   return state;
}

