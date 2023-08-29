# Pacman-Game-in-C-Programming
The classical single player pacman game.

The classical single player pacman game. The player will move pacman using WAXD keys on the keyboard. The board will be drawn as a fixed maze with pILLS in it and the prison at the middle which keeps the ghosts. The game will have 5 ghosts and the pacman. At the game start, the ghosts will initially be trapped in the prison for 10 seconds after which they will exit the prison. The pacman will start from the middle of the game board. There will be 4 power-ups at the four corners of the game board. If the pacman eats those power-ups, it will be invincible for 10 seconds. Any ghost it eats while it is invincible will be teleported back to the prison in the middle of the maze.

The game will have two difficulty levels. In the easy level, ghosts will simply move randomly. In the hard level, they will chase pacman according to the shortest path algorithm (they will try to minimize their distance to the pacman). A score will be displayed at the BOTTOM of the game board in the CORNER. It will increment by 1 for each pellet eaten and by 5 for each ghost eaten. Pacman will have 3 lives in total. If pacman is eaten by a ghost, it will lose one of its lives and all ghosts will be teleported to the prison. The game will continue from its current state. If the pacman loses all of its lives, a "Game over!" message will be written to the screen. If pacman eats all the pILLs, "You win!" message will be displayed.

# Algorithm Design

DEFINITIONS:
Structures used:
str_pacman (Pacman):
It contains pacman's current x and y positions, pacman's initial x and y positions and a flag whether or not the pacman has active power.

str_ghost (Ghost):
Contains the current x and y positions of a ghost, the initial x and y positions of a ghost, a flag whether it is alive or dead, and the last action taken by the same.

Functions used:
int Find_Ghost_Qtd
Receives: Pointer to the map matrix.

The purpose of this function is to count how many ghosts are in the maze obtained from the file. This is done with two chained forts, through which the entire matrix is ​​traversed looking for the character "W" and incrementing the qnt counter every time one is found. This function returns the final value of qnt after traversing the matrix.

int Get_Map
Receives: Pointer to the map matrix.

The purpose of this function is to obtain the maze used as a map. The matrix is ​​filled, character by character obtained from the file that was opened. If there is an error opening the file, it returns 0. If the file is opened and filled without problems, it returns 1.

void Find_Pacman
Receives: Pointer to the map matrix, Pointers to the x and y positions of the pacman.

The function traverses the matrix through two for, behind the character "C", which designates the pacman's initial position, and assigns the coordinates of that character to the pacman's initial positions.

void Find_Ghost
Receives: Pointer to the map matrix, Pointers to the x and y positions of a ghost, Index of the current ghost.

The function traverses the matrix through two fors, after the character "W", which denominates the starting position of a ghost. The variable qnt is used to verify that the ghost found is the correct index ghost, and if so, it assigns the character's x and y coordinates to the ghost's initial positions with that index.

void Print_Map
Receives: Pointer to the map matrix.

This function prints the matrix obtained from the file in the terminal.

void Update_Score
Receives: Pointer to the player_score variable, value to add to the score.

The function updates the player_score variable, used to store the player's score, adding the integer received to the variable. The function also updates the score display in the lower left corner of the screen.

void Update_Time
Receives: Pointer to the gameplay_time variable.

The function updates the time display in the lower left corner of the screen every second, using the gameplay_time variable, which stores the game time.

void Update_Lives
Receives: Pointer to the variable lives.

The function updates the time display in the lower left corner of the screen when the pacman dies, using the lives variable, which stores the player's number of lives.

int Verify_Win
Receives: Pointer to the map matrix.

This function uses two fors to cycle through the matrix behind the "o" character, which represents a tablet. If a chip is found, the function returns 0, symbolizing that the game is not over yet. If no chip is found, it returns 1, indicating that the player has won.

void Restart_Game
Receives: Pointers to the pacman's initial and current positions, Pointer to the last key pressed, Pointer to the lives variable.

The purpose of this function is to restart the game when the pacman dies. The current positions of the pacman are reset, given the value of the initial positions of the same. The last key pressed is also reset, receiving ''. The function shows a 5-second counter before starting the game again.

void Print_Ghost
Receives: Pointer to the super flag.

This function prints the ghost on the screen. If the super flag is 1, this means that the pacman has the power, so the function prints a blue "w". If the flag is 0, then the pacman lacks the power, and the function prints a red "W".

void Move_Pacman
Receives: Pointers to the current x and y positions of the pacman, Last key pressed by the player.

The function moves the pacman visually according to the directional key pressed, and updates the x and y positions of the same.

void Move_Ghost
Receives: Pointers to the current x and y positions of the ghost and pacman, superpower flag, Last movement of the ghost.

The function visually moves the 5 ghosts according to the position the pacman is in and changes their X and Y coordinates. To do this, a calculation is made of the distance module in X and Y of each ghost for the pacman. From there, the ghost chooses the coordinate where the pacman is most distant to follow. In function, a 90% chance of the ghost following the pacman and a 1% chance that it chooses a random path is calculated. Still, a ghost is always prevented from “walking backwards” using the last_move variable, this behavior is only interrupted if the ghost is in a “dead end” on the map.

int module
Receives: Integer.

The function returns the module of a received X value.

void Pause_Game
Receives: Pointer to the is_paused flag.

The is_paused flag signals whether the game is paused or not. If it is 1, the function prints "PAUSED" at the bottom of the screen. If it is 0, print spaces to delete what was written when it was paused.

void End_Game
Receives: Pointer to the player_score variable, Pointer to the lives variable.

This function prints the final menu at the end of the game. If the variable lives is 0, it means that the game is over for lack of lives, then print "GAME OVER" and the player's score. If it is not 0, then the game is over for lack of tiles on the map, then print «you wın!" and the player's score.

int main
All variables used are defined.
Read the map, find the pacman and the ghosts, print the map, start the score, the timer and the lives, and then start the game replay.
The movement character is read, the character is validated, so that the pacman does not stop if the movement is towards a wall and then the pacman moves.
Then the first test is done to check if the pacman touched a ghost.
After that, all ghosts are moved and a second test of the pacman collision with the ghosts is done.
After the crash tests, it is checked if the pacman has taken any tablets and the score values ​​and the superpower flag are changed. In addition, it is checked if the tablets are out of order for the pacman to win the game.
At the end, a millisecond timer is used to move the pacman and ghosts. For this, flags can_move_pacman and can_move_ghost are used, which become true whenever the timer reaches the values ​​of pacman_speed and ghosts_speed, respectively. The ghosts have the same speed as the pacman in normal play and 70% of his speed when the pacman is overpowered. A timer is also used to control the super power time, set to 5 seconds.
