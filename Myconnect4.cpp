#include <iostream.h>
#include <stdlib.h> //needed for rand
#include <windows.h>

// this makes sense
//Height and Columns are global variables... not stored in a variable but in #define
#define Rows 6
#define Columns 7

// display board is a function that returns nothing (void) it draws a board but we don't need to gather any info from it
void display_board();
void player_movement(int player);
// bool must have a true or false win (or lose) condition
bool check_for_winner(int x, int y, int player);
bool check_diagonal_combo_SW_NE(int x, int y, int player);
bool check_diagonal_combo_NW_SE(int x, int y, int player);
bool check_vertical_combo(int x, int y, int player);
bool check_horizontal_combo(int x, int y, int player);

// Specify 2D array for playing board
int board[Rows][Columns] = {{0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0}};

// declaring to separate variables here, but using one 'int' to save repetition
// also declaring them here, outside the main() function scope makes them globals

int LastMoveX, LastMoveY;
 
int main()
{
// the first local variables
   int counter = 0;
   bool winner = false;
   
   srand(GetTickCount());   //sets randomizer as time *unique*
   // get TickCount returns a semi random number for feeding into the srand function

   cout << "Please select a number from 1-7" << endl;
   cout << "| 1| 2| 3| 4| 5| 6| 7" << endl;
   cout << "_____________________";
   
   // even through function hasn't been described in the code, the compiler knows to look further down here (possibly looking for angle brackets) 
   display_board();

   // MAIN GAME LOOP
   // 42 spaces (6 by 7 array), divded by 2 players == maximumm no. of rounds
   for (int i = 0; i < 21; i++)
   {
      player_movement(1);
      display_board();
      winner = check_for_winner(LastMoveX, LastMoveY, 1);
      if (winner)
      {
         cout << "\nYou Win" << endl;
         break;
      }
   
      player_movement(2);
      display_board();
      winner = check_for_winner(LastMoveX, LastMoveY, 2);
      if (winner)
      {
         cout << "\nYou Win" << endl;
         break;
      }   
   }
   
   system("PAUSE");
   return 0;
}

void display_board()
{
   cout << endl;
   
   for (int i = 0; i < Rows; i++)
   {
      for (int j = 0; j < Columns; j++)
      {
         // this line concatenates, so you have an output of | | |, or |X|X| or |O|O)
         cout << "| ";
         // this function reinitialises the board, deleting the zero and adding a space
         if (board[i][j] == 0) cout << " ";
         
         else if (board[i][j] == 1) cout << "X";
         else if (board[i][j] == 2) cout << "O";
      }
      cout << "\n_____________________" << endl;
   }
}

void player_movement(int player)
{

   int choice;
   cout << "\nPlayer" << player << ", please select a number from 1 - 7: ";
   cin >> choice;

   //Error Checking
   
   if (cin.fail())
   // look here for details about cin.fail() - http://www.cs.utk.edu/~cs102/lectures/cinLecture.html
   {
      cout << "Error!";
      exit(1);
   }
   // simple error checking to see if number will fit in array Columns
   while (choice > Columns || choice <=0)
   {
      cout << "\nPlease select again: ";
      cin >> choice;
   }
   // the (-1) is because arrays count on zeroth basis. 
   // This function checks that there is a space available to claim for the player
   // Iterates through the rows, looking for a free space
   // 
   int number = 0;
   // this simulates the gravity
   // the !0 evaluation is checking that the space isn't already taken
   // this tunnels down the Rows' rows, to find a free one space in that column

   while (board[(Rows-1)-number][(choice-1)] != 0)
   {
      number++;
      // is there space left in the column for more entries?
      if (number > (Rows-1))
      {
         cout << "\nPlease select again: ";
         cin >> choice;
         number = 0;  //You must reset the number else it will start at the beginning of the array
      }
   };
   // this assigns the choice as the current player's, by filling in with the correct number
   // and records the last move, by stashing these in globals
   board[(Rows-1)-number][choice-1] = player;
   // adjusting for zero based counting, and recording in global
   LastMoveY = (Rows-1)-number;
   LastMoveX = choice-1;
   // C: cripes! C++ can only return a single value - you need pointers for the rest!
}
// this where the victory condition checking starts coming in:
bool check_for_winner(int x, int y, int player)
{
   bool winner;
    // checks if you can count up to four in any of the possible directions
   if (check_diagonal_combo_SW_NE(x,y,player)) return true;
   else if (check_diagonal_combo_NW_SE(x,y,player)) return true;
   else if (check_vertical_combo(x,y,player)) return true;
   else if (check_horizontal_combo(x,y,player)) return true;
   else return false;
}

bool check_diagonal_combo_SW_NE(int x, int y, int player)
{
   int score = 1;
   int count = 1;
   // check the length of arm towards NE  
   // check there are rows to check, and we haven't gone through the floor 
   while((y-count >= 0) && (x+count < Columns))
   {
      if (board[y-count][x+count] == player)  //Check SW to NE
      {
         score++;
         count++;
      }
      else break;  //If no combo is detected break from the loop
   }

   count = 1;
      // check the length of arm towards SE
   while((y+count < Rows) && (x-count >=0))
   {
      if (board[y+count][x-count] == player)  //Check NE to SW
      {
         score++;
         count++;
      }   
      else break;  //If no combo is detected break from the loop
   }
   
   // if length of both arms is 4, then you win!
   if (score == 4) return true;
   else return false;
}

bool check_diagonal_combo_NW_SE(int x, int y, int player)
{
   int score = 1;
   int count = 1;
   
   while((y+count >= 0) && (x+count < Columns))
   {
      if (board[y+count][x+count] == player)  //Check NW to SE
      {
         score++;
         count++;
      }
      else break;  //If no combo is detected break from the loop
   }

   count = 1;
   while((y-count < Rows) && (x-count >=0))
   {
      if (board[y-count][x-count] == player)  //Check SE to NW
      {
         score++;
         count++;
      }   
      else break;  //If no combo is detected break from the loop
   }
   
   if (score == 4) return true;
   else return false;
}

bool check_vertical_combo(int x, int y, int player)
{
   int score = 1;
   int count = 1;
   
   while(y+count >= 0 && y+count < Rows)
   {
      if (board[y+count][x] == player)  //Check Down
      {
         score++;
         count++;
      }
      else break;  //If no combo is detected break from the loop
   }
  
   if (score == 4) return true;
   else return false;
}

bool check_horizontal_combo(int x, int y, int player)
{
   int score = 1;
   int count = 1;
   
   while((x+count >= 0) && (x+count < Columns))
   {
      if (board[y][x+count] == player)  //Check Left
      {
         score++;
         count++;
      }
      else break;  //If no combo is detected break from the loop
   }

   count = 1;
   while((x-count < Columns) && (x-count >=0))
   {
      if (board[y][x-count] == player)  //Check Right
      {
         score++;
         count++;
      }   
      else break;  //If no combo is detected break from the loop
   }
   
   if (score == 4) return true;
   else return false;
}

//This is my simple 'Connect4 Game'
//Two player game , no computer player

