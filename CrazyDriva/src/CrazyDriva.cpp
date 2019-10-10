//============================================================================
// Name        : CrazyDriva.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctime>
using namespace std;

//Vroom-Vroom! You are Minion Dave! You and your buddy Stuart are a crazy driving team! On a
//15 x 15 plaza in a shopping mall, take a ride on the wild side! Collect as many bananas as
//possible, but watch out! Don’t get caught by the bad-guy El Macho or his Crazy Chicken! The
//race last 10-20 seconds! There are severe penalties for hitting obstacles like trash cans and little
//old ladies. Collect more bananas than El Macho and the Chicken without getting caught by
//them and you will be first across the Finish Line!
//Make up the rules yourself! Make it simple & fun. Place bananas and obstacles for maximum
//excitement. Likewise, move El Macho and his Crazy Chicken for maximum engagement. Use the
//class Competitors for Dave&Stewart, El Macho, and his CrazyChicken, with appropriate
//methods such as move(). Use the class Obstacles for TrashCans and LittleOldLadies,
//including the penalties for hitting them. When the game is over, announce how Dave&Stewart
//did, and list the number of bananas collected and all the obstacles hit.

const int MAX_NAME_LEN = 32;

int dist(int x1, int y1, int x2, int y2){
	int dx = x1 - x2;
    int dy = y1 - y2;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}
bool full (char field[][15]){
	int filled = 0;
	for (int i=0; i<15; i++){
		for (int j=0; j<15; j++){
			if (field[i][j] != ' ')
				filled++;
		}
	}
	if (filled == 225)
		return true;
	else return false;
}

class obstacle {
	public :
		string name;
		char initial;
		int x;
		int y;
		char* next;
		char obstacleInitial (string name){
			if (name == "TrashCans"){
				return 'T';
			}
			else if (name == "OldLadies"){
				return 'L';
			}
			else return 0;
		}
		void createObstacle (string name, obstacle OBSTACLE[15][15], int x, int y, char field[][15]){
			OBSTACLE[y][x].name = name;
			OBSTACLE[y][x].next = &field[y][x];
			OBSTACLE[y][x].x = x;
			OBSTACLE[y][x].y = y;
		}
		void spawnTrashCans(obstacle OBSTACLE[15][15], char field[15][15], int obstacleX, int obstacleY){
			field[obstacleY][obstacleX] = obstacleInitial("TrashCans");
			createObstacle ("TrashCans", OBSTACLE, obstacleX, obstacleY, field);
		}
		void spawnOldLadies(obstacle OBSTACLE[15][15], char field[15][15], int obstacleX, int obstacleY){
			field[obstacleY][obstacleX] = obstacleInitial("OldLadies");
			createObstacle ("OldLadies", OBSTACLE, obstacleX, obstacleY, field);
		}
		void penaltyTrashCan(char field[15][15], obstacle OBSTACLE[15][15], int x, int y, int& numOfBananas) {
			numOfBananas = numOfBananas/2;
			OBSTACLE[y][x].next = nullptr;
		}
		void penaltyOldLadies(char field[15][15], obstacle OBSTACLE[15][15], int x, int y, int& numOfBananas) {
			numOfBananas = 0;
			OBSTACLE[y][x].next = nullptr;
		}
};
class competitor {
	public :
		char initial;
		int x;
		int y;
		int numOfBananas;
		// chase player
		void moveElMacho (competitor& ELMACHO, char field[15][15], competitor& PLAYER, bool game, obstacle OBSTACLE[15][15]){
			int pastX = ELMACHO.x, pastY = ELMACHO.y;
			int newX = pastX, newY = pastY;
			// decide kevin movement
			if (ELMACHO.x>PLAYER.x ){
				//left
				newX = pastX - 1;
				if (field[newY][newX] == 'C' || field[newY][newX] == 'T' || field[newY][newX] == 'L'){
					if (ELMACHO.y<PLAYER.y && field[pastY+1][pastX] != 'C'){
						//down
						newY = pastY + 1;
						newX = pastX;
					}else if (ELMACHO.y>PLAYER.y && field[pastY-1][pastX] != 'C'){
						//up
						newY = pastY - 1;
						newX = pastX;
					}
					// else condition it wont move
					else {
						newX = pastY;
						newY = pastX;
					}
				}
			}
			else if (ELMACHO.x<PLAYER.x){
				//right
				newX = pastX + 1;
				newY = pastY;
				if (field[newY][newX] == 'C' || field[newY][newX] == 'T' || field[newY][newX] == 'L'){
					if (ELMACHO.y<PLAYER.y && field[pastY+1][pastX] != 'C'){
						//down
						newY = pastY + 1;
						newX = pastX;
					}else if (ELMACHO.y>PLAYER.y && field[pastY-1][pastX] != 'C'){
						//up
						newY = pastY - 1;
						newX = pastX;
					}
					// else condition it wont move
					else{
						newX = pastX;
						newY = pastY;
					}
				}
			}
			else{
				if (ELMACHO.y<PLAYER.y){
					//down
					newY = pastY + 1;
					newX = pastX;
					if (field[newY][newX] == 'C'){
						//right
						if (field[pastY][pastX+1] != 'C'){
							newX = pastX + 1;
							newY = pastY;
						}
						//left
						else {
							newX = pastX - 1;
							newY = pastY;
						}
					}
				}
				else if (ELMACHO.y>PLAYER.y){
					//up
					newY = pastY - 1;
					newX = pastX;
					if (field[newY][newX] == 'C'){
						if (field[pastY][pastX+1] != 'C'){
							//right
							newX = pastX + 1;
							newY = pastY;
						}
						//left
						else {
							newX = pastX - 1;
							newY = pastY;
						}
					}
				}
			}
			if (field[newY][newX] == ' '){
				field[pastY][pastX] = ' ';
				ELMACHO.x = newX; ELMACHO.y = newY;
			}
			else if (field[newY][newX] == 'J'){
				field[pastY][pastX]= ' ';
				ELMACHO.x = newX; ELMACHO.y = newY;
				ELMACHO.numOfBananas++;
				}
			else if (field[newY][newX] == PLAYER.initial){
				field[pastY][pastX] = ' ';
				ELMACHO.x = newX; ELMACHO.y = newY;
			}
			else if (field[newY][newX] == 'T'){
				field[pastY][pastX] = ' ';
				ELMACHO.x = newX; ELMACHO.y = newY;

				// hit by penalty
				OBSTACLE[newY][newX].penaltyTrashCan(field, OBSTACLE, ELMACHO.x, ELMACHO.y, ELMACHO.numOfBananas);
			}
			else if (field[newY][newX] == 'L'){
				field[pastY][pastX] = ' ';
				ELMACHO.x = newX; ELMACHO.y = newY;

				// hit by penalty
				OBSTACLE[newY][newX].penaltyOldLadies(field, OBSTACLE, ELMACHO.x, ELMACHO.y, ELMACHO.numOfBananas);
			}
			field[newY][newX] = ELMACHO.initial;
		}
		// get bananas
		void moveChicken (competitor& CHICKEN, char field[15][15], competitor& PLAYER, bool game, obstacle OBSTACLE[15][15]){
			int pastX = CHICKEN.x, pastY = CHICKEN.y;
			int newX = pastX, newY = pastY;
			int bananaX = 0 , bananaY = 0;
			for (int i=0; i<15; i++){
				for (int j=0; j<15; j++){
					if (field[i][j] == 'J'){
						if (dist(j,y,pastX,pastY)<dist(bananaX,bananaY,pastX,pastY)){
							bananaX = j;
							bananaY = i;
						}
					}
				}
			}
			if (CHICKEN.x>bananaX ){
				//left
				newX = pastX - 1;
				if (field[newY][newX] == 'M'){
					if (CHICKEN.y<bananaY && field[pastY+1][pastX] != 'M'){
						//down
						newY = pastY + 1;
						newX = pastX;
					}else if (CHICKEN.y>bananaY && field[pastY-1][pastX] != 'M'){
						//up
						newY = pastY - 1;
						newX = pastX;
					}
					// else condition it wont move
					else {
						newX = pastY;
						newY = pastX;
					}
				}
			}
			else if (CHICKEN.x<bananaX){
				//right
				newX = pastX + 1;
				newY = pastY;
				if (field[newY][newX] == 'M' ){
					if (CHICKEN.y<bananaY && field[pastY+1][pastX] != 'M'){
						//down
						newY = pastY + 1;
						newX = pastX;
					}else if (CHICKEN.y>bananaY && field[pastY-1][pastX] != 'M'){
						//up
						newY = pastY - 1;
						newX = pastX;
					}
					// else condition it wont move
					else{
						newX = pastX;
						newY = pastY;
					}
				}
			}
			else{
				if (CHICKEN.y<bananaY){
					//down
					newY = pastY + 1;
					newX = pastX;
					if (field[newY][newX] == 'M' ){
						//right
						if (field[pastY][pastX+1] != 'M'){
							newX = pastX + 1;
							newY = pastY;
						}
						//left
						else {
							newX = pastX - 1;
							newY = pastY;
						}
					}
				}
				else if (CHICKEN.y>bananaY){
					//up
					newY = pastY - 1;
					newX = pastX;
					if (field[newY][newX] == 'M' ){
						if (field[pastY][pastX+1] != 'M'){
							//right
							newX = pastX + 1;
							newY = pastY;
						}
						//left
						else {
							newX = pastX - 1;
							newY = pastY;
						}
					}
				}
			}
			if (field[newY][newX] == ' '){
				CHICKEN.x = newX; CHICKEN.y = newY;
				field[pastY][pastX] = ' ';
			}
			else if (field[newY][newX] == 'J'){
				field[pastY][pastX]= ' ';
				CHICKEN.x = newX; CHICKEN.y = newY;
				CHICKEN.numOfBananas++;
			}
			else if (field[newY][newX] == PLAYER.initial){
				field[pastY][pastX] = ' ';
				CHICKEN.x = newX; CHICKEN.y = newY;
				game = true;
			}
			else if (field[newY][newX] == 'T'){
				CHICKEN.x = newX; CHICKEN.y = newY;
				field[pastY][pastX] = ' ';

				// hit by penalty
				OBSTACLE[newY][newX].penaltyTrashCan(field, OBSTACLE, CHICKEN.x, CHICKEN.y, CHICKEN.numOfBananas);
			}
			else if (field[newY][newX] == 'L'){
				CHICKEN.x = newX; CHICKEN.y = newY;
				field[pastY][pastX] = ' ';

				// hit by penalty
				OBSTACLE[newY][newX].penaltyOldLadies(field, OBSTACLE, CHICKEN.x, CHICKEN.y, CHICKEN.numOfBananas);
			}
			field[newY][newX] = CHICKEN.initial;
		}
		void movePlayer (competitor& PLAYER, char field[15][15], char move, obstacle OBSTACLE[15][15]){
			int pastX = PLAYER.x , pastY = PLAYER.y;
			int newX = pastX , newY = pastY;
			switch (move){
			//
				case 'a':
					if (pastX != 0)
						newX = pastX-1;
					else if (pastX == 0)
						newX = pastX;
					break;
				case 'd':
					if (pastX != 14)
						newX = pastX+1;
					else if (pastX == 14)
						newX = pastX;
					break;
				case 'w':
					if (pastY != 0)
						newY = pastY-1;
					else if (pastY == 0)
						newY = pastY;
					break;
				case 's':
					if (pastY != 14)
						newY = pastY+1;
					else if (pastY == 14)
						newY = pastY;
					break;
			}
			if (field[newY][newX] == ' '){
				field[PLAYER.y][PLAYER.x] = ' ';
				PLAYER.x = newX;
				PLAYER.y = newY;
			}
			else if (field[newY][newX] == 'J'){
				field[PLAYER.y][PLAYER.x] = ' ';
				PLAYER.x = newX;
				PLAYER.y = newY;
				PLAYER.numOfBananas++;
			}
			else if (field[newY][newX] == 'T'){
				PLAYER.x = newX; PLAYER.y = newY;
				field[pastY][pastX] = ' ';

				// hit by penalty
				OBSTACLE[newY][newX].penaltyTrashCan(field, OBSTACLE, PLAYER.x, PLAYER.y, PLAYER.numOfBananas);
			}
			else if (field[newY][newX] == 'L'){
				PLAYER.x = newX; PLAYER.y = newY;
				field[pastY][pastX] = ' ';

				// hit by penalty
				OBSTACLE[newY][newX].penaltyOldLadies(field, OBSTACLE, PLAYER.x, PLAYER.y, PLAYER.numOfBananas);
			}
			field[newY][newX] = PLAYER.initial;
		}
};

void spawnBananas (char field[15][15], char banana){
	int x = 0, y = 0;
	int filled = 0;
	for (int i=0; i<15; i++){
		for (int j=0; j<15; j++){
			if (field[i][j] != ' ')
				filled++;
		}
	}
	if (filled<225){
		do{
			x = rand()% 15;
			y = rand()% 15;
		}
		while (field[y][x] != ' ');
	}
	field[y][x] = banana;
}
void board (const char field[15][15]){
	for (int i=0; i<15; i++){
		for (int j=0; j<15; j++){
			cout<<" "<<field[i][j]<<" ";
		}
		cout<<endl;
	}
}
void calculateWinner (competitor& PLAYER, competitor& ELMACHO, competitor& CHICKEN){
	int scorePlayer = PLAYER.numOfBananas;
	int scoreEnemy = ELMACHO.numOfBananas + CHICKEN.numOfBananas;
	if (scorePlayer > scoreEnemy){
		cout<<"Congrats you have won the game"<<endl;
	}
	else if (scorePlayer <= scoreEnemy){
		cout<<"You Lost"<<endl;
	}
}
bool check(competitor& PLAYER, competitor& ELMACHO, competitor& CHICKEN){
	if ((PLAYER.x == ELMACHO.x && PLAYER.y == ELMACHO.y) || (PLAYER.x == CHICKEN.x && PLAYER.y == CHICKEN.y)){
		return true;
	}
	else return false;
}

int main() {
	srand (time(NULL));

	// variable definition & declaration
	char banana = 'J';
    int seconds;
    char move;
    int chance;
    bool game;

    // Assigning the field
	char field[15][15];
	for (int i=0; i<15; i++){
		for (int j=0; j<15; j++){
			field[i][j]=' ';
		}
	}

	// create object
	competitor PLAYER, ELMACHO, CHICKEN;
	PLAYER.x = 7;	PLAYER.y = 7;	PLAYER.initial = 'P';	PLAYER.numOfBananas = 0;
		field[PLAYER.y][PLAYER.x] = PLAYER.initial;
	ELMACHO.x = 0;	ELMACHO.y = 0;	ELMACHO.initial = 'M';	ELMACHO.numOfBananas = 0;
		field[ELMACHO.y][ELMACHO.x] = ELMACHO.initial;
	CHICKEN.x = 14;	CHICKEN.y = 14;	CHICKEN.initial = 'C';	CHICKEN.numOfBananas = 0;
		field[CHICKEN.y][CHICKEN.x] = CHICKEN.initial;

	obstacle OBSTACLE[15][15];

    cout << "Enter the number of seconds: ";
    cin >> seconds;
    cout << "Start ... " << endl << flush;

    time_t start_time = time(nullptr);
    // start the loop
    while ( (time(nullptr) - start_time) < seconds){
    	game = false;
    	chance = rand()% 5;
    	board(field);
    	cout<<endl<<"Enter your desired move ";
    	cin>>move; cout<<endl;

    	PLAYER.movePlayer (PLAYER, field, move, OBSTACLE);
    	ELMACHO.moveElMacho (ELMACHO, field, PLAYER, game, OBSTACLE);
    	CHICKEN.moveChicken (CHICKEN, field, PLAYER, game, OBSTACLE);
    	if (full(field) == false){
			if (chance == 0){
				int obstacleX, obstacleY;
				do{
					obstacleX = rand()% 15;
					obstacleY = rand()% 15;
				}
				while (field[obstacleY][obstacleX] != ' ');
				OBSTACLE[obstacleY][obstacleX].spawnTrashCans(OBSTACLE, field, obstacleX, obstacleY);
			}
			else if (chance == 1){
				int obstacleX, obstacleY;
				do{
					obstacleX = rand()% 15;
					obstacleY = rand()% 15;
				}
				while (field[obstacleY][obstacleX] != ' ');
				OBSTACLE[obstacleY][obstacleX].spawnOldLadies(OBSTACLE, field, obstacleX, obstacleY);
			}
			else spawnBananas (field, banana);
    	}

    	cout<<"PLAYER "<<PLAYER.numOfBananas<<endl;
    	cout<<"ELMACHO "<<ELMACHO.numOfBananas<<endl;
    	cout<<"CHICKEN "<<CHICKEN.numOfBananas<<endl;
    	check (PLAYER, ELMACHO, CHICKEN);
    	if (check(PLAYER, ELMACHO, CHICKEN) == true)
    		break;
    }
	board(field);
    if (check(PLAYER, ELMACHO, CHICKEN) == true)
    	cout<<"U have been eaten lol"<<endl;
    else calculateWinner (PLAYER, ELMACHO, CHICKEN);


	return 0;
}

