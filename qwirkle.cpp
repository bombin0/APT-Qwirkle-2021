#include "LinkedList.h"
#include "TileCodes.h"
#include "Player.h"
#include "Tile.h"
#include "TileBag.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define MAX_TILEBAG_SIZE 108
#define MAX_HAND_SIZE 6

#define MAX_SIZE_BOARD 26
#define EMPTY_TILE "|  |"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

using namespace std;

//2D tile vector
vector<vector<Tile>> board(27);

void playGame(std::string TileBagFile, TileBag *tilebag, Player *playerOne, Player *playerTwo);
//void playTheGame(Board *board, Player *playerOne);
//void cleanupGame(Board *board, Player *playerOne);
//void playerAction();
void printBoard();
void placeInitialTile(TileBag *tilebag);
void placeTile(TileBag *tilebag);
void refreshHand(TileBag *tilebag, TileBag *hand);
bool validLocation(Tile *playedTile, std::string boardLocation);
void replaceTile(TileBag *hand, TileBag *tilebag);
void checkHand(TileBag *hand, TileBag *tilebag);
bool readOneTile(std::ifstream &file, Colour *colour, Shape *shape);
void saveGame(TileBag *handp1, TileBag *handp2, TileBag *tilebag);

std::string player1name;
std::string player2name;
string tileToPlace;
string tileToReplace;
string boardPos;

// Function loads a game
void loadGame()
{
  std::fstream fin;
  std::string filename;
  std::cout << "Enter the filename you want to load " << std::endl;
  std::cout << "> "; cin >> filename;
  fin.open(filename, std::ios::in);
  if (fin.fail())
    std::cout << "Could not open file \n";
  else
  {
    std::string value;
    getline(fin, value);
    std::cout << "Value read from file " << value;
  }
  fin.close();
}

// Function starts new game
void newGame()
{
  bool isNameOneSet = false;
  bool isNameTwoSet = false;

  std::cout << "Enter player 1 name (uppercase only)" << std::endl;
  while (isNameOneSet == false)
  {
    bool isInvalid = true;
    std::cout << "> "; cin >> player1name;
    std::cout << std::endl;
    for (size_t i = 0; i < player1name.length(); i++)
    {
      if (!isupper(static_cast<unsigned char>(player1name[i])))
      {
        isInvalid = false;
      }
    }
    if (isInvalid == false)
    {
      std::cout << "isInvalid -- Please enter your name in UPPERCASE" << std::endl;
    }
    isNameOneSet = isInvalid;
  }

  std::cout << "Enter player 2 name (uppercase only)" << std::endl;
  while (isNameTwoSet == false)
  {
    bool isInvalid = true;
    std::cout << "> "; cin >> player2name;
    std::cout << std::endl;
    for (size_t i = 0; i < player2name.length(); i++)
    {
      if (!isupper(static_cast<unsigned char>(player2name[i])))
      {
        isInvalid = false;
      }
    }
    if (isInvalid == false)
    {
      std::cout << "isInvalid -- Please enter your name in UPPERCASE" << std::endl;
    }
    isNameTwoSet = isInvalid;
  }
}

int selectMenuOption(void)
{
  int option;
  std::cout << "Menu" << std::endl;
  std::cout << "-----------" << std::endl;
  std::cout << "1. New Game" << std::endl;
  std::cout << "2. Load game" << std::endl;
  std::cout << "3. Credits (Show student information)" << std::endl;
  std::cout << "4. Quit" << std::endl;
  std::cout << "> "; cin >> option;
  fflush(stdin);
  return option;
}
void showCredits()
{
  std::cout << "-----------------------------------" << std::endl;
  std::cout << "Name: Jamaine George" << std::endl;
  std::cout << "Student ID: s3660962" << std::endl;
  std::cout << "Email: s3660962@student.rmit.edu.au" << std::endl;
  std::cout << "-----------------------------------" << std::endl;

  std::cout << "Name: William Bossen" << std::endl;
  std::cout << "Student ID: s3658961" << std::endl;
  std::cout << "Email: s3658961@student.rmit.edu.au" << std::endl;
  std::cout << "-----------------------------------" << std::endl;

  std::cout << "Name: Ellery Alexander Setiadi" << std::endl;
  std::cout << "Student ID: s3886701" << std::endl;
  std::cout << "Email: s3886701@student.rmit.edu.au" << std::endl;
  std::cout << "-----------------------------------" << std::endl;

  std::cout << "Name: Arjun Sharma" << std::endl;
  std::cout << "Student ID: s3604973" << std::endl;
  std::cout << "Email: s3604973@student.rmit.edu.au" << std::endl;
  std::cout << "-----------------------------------" << std::endl;
}

//storing placeholder value
void storePlaceholder()
{
  for (auto &row : board)
  {
    row.resize(26);
    for (auto &val : row)
    {
      val.shape = 80;
      val.colour = 'X';
    }
  }
}

void printBoard()
{
  for (unsigned int i = 0; i < board.size(); i++)
  {

    //Numbered top index of board
    if (i == 0)
    {
      cout << "    0"
           << "   1"
           << "   2"
           << "   3"
           << "   4"
           << "   5"
           << "   6"
           << "   7"
           << "   8"
           << "   9"
           << "  10"
           << "  11"
           << "  12"
           << "  13"
           << "  14"
           << "  15"
           << "  16"
           << "  17"
           << "  18"
           << "  19"
           << "  20"
           << "  21"
           << "  22"
           << "  23"
           << "  24"
           << "  25\n";
      cout << "   ---------------------------------------------------"
           << "----------------------------------------------------";
    }

    //Lettered side index of board
    if (i == 1)
    {
      cout << "A ";
    }
    if (i == 2)
    {
      cout << "B ";
    }
    if (i == 3)
    {
      cout << "C ";
    }
    if (i == 4)
    {
      cout << "D ";
    }
    if (i == 5)
    {
      cout << "E ";
    }
    if (i == 6)
    {
      cout << "F ";
    }
    if (i == 7)
    {
      cout << "G ";
    }
    if (i == 8)
    {
      cout << "H ";
    }
    if (i == 9)
    {
      cout << "I ";
    }
    if (i == 10)
    {
      cout << "J ";
    }
    if (i == 11)
    {
      cout << "K ";
    }
    if (i == 12)
    {
      cout << "L ";
    }
    if (i == 13)
    {
      cout << "M ";
    }
    if (i == 14)
    {
      cout << "N ";
    }
    if (i == 15)
    {
      cout << "O ";
    }
    if (i == 16)
    {
      cout << "P ";
    }
    if (i == 17)
    {
      cout << "Q ";
    }
    if (i == 18)
    {
      cout << "R ";
    }
    if (i == 19)
    {
      cout << "S ";
    }
    if (i == 20)
    {
      cout << "T ";
    }
    if (i == 21)
    {
      cout << "U ";
    }
    if (i == 22)
    {
      cout << "V ";
    }
    if (i == 23)
    {
      cout << "W ";
    }
    if (i == 24)
    {
      cout << "X ";
    }
    if (i == 25)
    {
      cout << "Y ";
    }
    if (i == 26)
    {
      cout << "Z ";
    }

    for (unsigned int j = 0; j < board[i].size(); j++)
    {
      //doesnt print top row of vector where the index is
      if (i == 0)
      {
        //do nothing
      }
      else
      {
        //replace placeholder with empty board space
        if (board[i][j].shape == 80 && board[i][j].colour == 'X')
        {
          cout << "|  |";
        }
        else
        {
          //if board space is not placeholder then print it on the board
          cout << "|" << board[i][j].colour << board[i][j].shape << "|";
        }
      }
    }
    cout << endl;
  }
}

void placeInitialTile(TileBag *tilebag){
  cout << "Which tile would you like to place?" << endl;

  bool tileExists = false;
  char placedColour = 'X';

  //input tile
  do
  {
    cout << "> "; cin >> tileToPlace;

    //copy tile string
    std::string temp = "temp string";
    temp = tileToPlace;

    //take int value of the string for the shape
    size_t i = 0;
    for (; i < tileToPlace.length(); i++)
    {
      if (isdigit(tileToPlace[i]))
        break;
    }

    tileToPlace = tileToPlace.substr(i, tileToPlace.length() - i);

    int placedShape = atoi(tileToPlace.c_str());

    //set colour to string index 0
    placedColour = temp[0];

    //check if tile is in the hand
    int k = 0;

    while (k < MAX_HAND_SIZE)
    {
      //move from tilebag to player
      Tile *tile = new Tile(*tilebag->get(k));
      //cout << tile->colour << endl;

      if (tile->colour == placedColour && tile->shape == placedShape)
      {
        tileExists = true;
        tilebag->remove(k);
        break;
      }
      else
      {
        tileExists = false;
      }

      ++k;
    }
    if (tileExists == false)
    {
      cout << "Invalid Tile! Try again" << endl;
    }
  } while (tileExists == false);
  int placedShape = atoi(tileToPlace.c_str());
  //printHand(tilebag);

  cout << "Where would you like to place this tile?" << endl;

  //input board position
  cout << "> "; cin >> boardPos;
  std::string temp2 = "temp string";
  temp2 = boardPos;

  char row = 'Z';

  //take int value of the string for the col position
  size_t j = 0;
  for (; j < boardPos.length(); j++)
  {
    if (isdigit(boardPos[j]))
      break;
  }

  boardPos = boardPos.substr(j, boardPos.length() - j);

  int col = atoi(boardPos.c_str());

  //set row to be index 0 of input
  row = temp2[0];

  int rowIndex = 444;

  //convert row letter to a number
  if (row == 'A')
  {
    rowIndex = 1;
  }
  if (row == 'B')
  {
    rowIndex = 2;
  }
  if (row == 'C')
  {
    rowIndex = 3;
  }
  if (row == 'D')
  {
    rowIndex = 4;
  }
  if (row == 'E')
  {
    rowIndex = 5;
  }
  if (row == 'F')
  {
    rowIndex = 6;
  }
  if (row == 'G')
  {
    rowIndex = 7;
  }
  if (row == 'H')
  {
    rowIndex = 8;
  }
  if (row == 'I')
  {
    rowIndex = 9;
  }
  if (row == 'J')
  {
    rowIndex = 10;
  }
  if (row == 'K')
  {
    rowIndex = 11;
  }
  if (row == 'L')
  {
    rowIndex = 12;
  }
  if (row == 'M')
  {
    rowIndex = 13;
  }
  if (row == 'N')
  {
    rowIndex = 14;
  }
  if (row == 'O')
  {
    rowIndex = 15;
  }
  if (row == 'P')
  {
    rowIndex = 16;
  }
  if (row == 'Q')
  {
    rowIndex = 17;
  }
  if (row == 'R')
  {
    rowIndex = 18;
  }
  if (row == 'S')
  {
    rowIndex = 19;
  }
  if (row == 'T')
  {
    rowIndex = 20;
  }
  if (row == 'U')
  {
    rowIndex = 21;
  }
  if (row == 'V')
  {
    rowIndex = 22;
  }
  if (row == 'W')
  {
    rowIndex = 23;
  }
  if (row == 'X')
  {
    rowIndex = 24;
  }
  if (row == 'Y')
  {
    rowIndex = 25;
  }
  if (row == 'Z')
  {
    rowIndex = 26;
  }

      board[rowIndex][col].shape = placedShape;
      board[rowIndex][col].colour = placedColour;
}

void placeTile(TileBag *tilebag){

  cout << "Which tile would you like to place?" << endl;

  bool tileExists = false;
  char placedColour = 'X';

  //input tile
  do
  {
    cout << "> "; cin >> tileToPlace;

    //copy tile string
    std::string temp = "temp string";
    temp = tileToPlace;

    //take int value of the string for the shape
    size_t i = 0;
    for (; i < tileToPlace.length(); i++)
    {
      if (isdigit(tileToPlace[i]))
        break;
    }

    tileToPlace = tileToPlace.substr(i, tileToPlace.length() - i);

    int placedShape = atoi(tileToPlace.c_str());

    //set colour to string index 0
    placedColour = temp[0];

    //check if tile is in the hand
    int k = 0;

    while (k < MAX_HAND_SIZE)
    {
      //move from tilebag to player
      Tile *tile = new Tile(*tilebag->get(k));
      //cout << tile->colour << endl;

      if (tile->colour == placedColour && tile->shape == placedShape)
      {
        tileExists = true;
        tilebag->remove(k);
        break;
      }
      else
      {
        tileExists = false;
      }

      ++k;
    }
    if (tileExists == false)
    {
      cout << "Invalid Tile! Try again" << endl;
    }
  } while (tileExists == false);
  int placedShape = atoi(tileToPlace.c_str());
  //printHand(tilebag);

  cout << "Where would you like to place this tile?" << endl;

  //input board position
  cout << "> "; cin >> boardPos;
  std::string temp2 = "temp string";
  temp2 = boardPos;

  char row = 'Z';

  //take int value of the string for the col position
  size_t j = 0;
  for (; j < boardPos.length(); j++)
  {
    if (isdigit(boardPos[j]))
      break;
  }

  boardPos = boardPos.substr(j, boardPos.length() - j);

  int col = atoi(boardPos.c_str());

  //set row to be index 0 of input
  row = temp2[0];

  int rowIndex = 444;

  //convert row letter to a number
  if (row == 'A')
  {
    rowIndex = 1;
  }
  if (row == 'B')
  {
    rowIndex = 2;
  }
  if (row == 'C')
  {
    rowIndex = 3;
  }
  if (row == 'D')
  {
    rowIndex = 4;
  }
  if (row == 'E')
  {
    rowIndex = 5;
  }
  if (row == 'F')
  {
    rowIndex = 6;
  }
  if (row == 'G')
  {
    rowIndex = 7;
  }
  if (row == 'H')
  {
    rowIndex = 8;
  }
  if (row == 'I')
  {
    rowIndex = 9;
  }
  if (row == 'J')
  {
    rowIndex = 10;
  }
  if (row == 'K')
  {
    rowIndex = 11;
  }
  if (row == 'L')
  {
    rowIndex = 12;
  }
  if (row == 'M')
  {
    rowIndex = 13;
  }
  if (row == 'N')
  {
    rowIndex = 14;
  }
  if (row == 'O')
  {
    rowIndex = 15;
  }
  if (row == 'P')
  {
    rowIndex = 16;
  }
  if (row == 'Q')
  {
    rowIndex = 17;
  }
  if (row == 'R')
  {
    rowIndex = 18;
  }
  if (row == 'S')
  {
    rowIndex = 19;
  }
  if (row == 'T')
  {
    rowIndex = 20;
  }
  if (row == 'U')
  {
    rowIndex = 21;
  }
  if (row == 'V')
  {
    rowIndex = 22;
  }
  if (row == 'W')
  {
    rowIndex = 23;
  }
  if (row == 'X')
  {
    rowIndex = 24;
  }
  if (row == 'Y')
  {
    rowIndex = 25;
  }
  if (row == 'Z')
  {
    rowIndex = 26;
  }


  //check if location is valid

  //place tile on the board

  //refresh the players hand

    Tile *tileCheck = new Tile(placedColour, placedShape);
    if (validLocation(tileCheck, temp2) == true){
      board[rowIndex][col].shape = placedShape;
      board[rowIndex][col].colour = placedColour;
    }
}

bool validLocation(Tile *playedTile, std::string boardLocation)
{
  int row, column;
  bool result = false;

  row = boardLocation[0] - 64;
  column = stoi(boardLocation.substr(1));

  //TESTING -->
  // cout << "col:" << column << " row:" << row << endl;
  // cout << "col -1: " << column-1 << " row -1: " << row-1 << endl;
  // cout << "playedTile->shape:" << playedTile->shape << " playedTile->colour:" << playedTile->colour << endl;
  // cout << "board[row][column].shape:" << board[row][column].shape << " board[row][column].colour:" << board[row][column].colour << endl;
  // cout  << "Shape at <-:" << board[column-1][row].shape
  //       << " Shape at ->:" << board[column+1][row].shape
  //       << " Colour at <-" << board[column-1][row].colour
  //       << " Colour at ->" << board[column+1][row].colour << endl;

  // cout << "Tile at B,0 " << board[2][0].shape << ", " << board[2][0].colour << endl;

  //To check if the inputing location is within bounds
  if (row > MAX_SIZE_BOARD || column > MAX_SIZE_BOARD)
    std::cout << "Input is out of bounds" << endl;

  //Check if there is already a tile in the location
  else if (board[row][column].shape != 80 && board[row][column].colour != 'X')
  {
    std::cout << "There is a tile already in that location" << std::endl;
  }

  //Check if tile is adjacent to valid tile
  else if (board[row-1][column].shape == playedTile->shape){
    cout << "Location Valid (Shape on row matches)" << board[row-1][column].shape << ", " << playedTile->shape << endl;
    //loop is to search through line to see if tile already exists
    for(int i=0;i<6;i++){
      if((row-i) < 0){
        //do noithing
      }
      //continue for each direction...
      else if(board[row-i][column].shape == playedTile->shape && board[row-i][column].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row-1][column].colour == playedTile->colour){
    cout << "Location Valid (Colour on row matches)" << board[row-1][column].colour <<  ", "<< playedTile->colour << endl;
    for(int i=0;i<6;i++){
      if((row-i)< 0){
        //do nothing
      }
      else if(board[row-i][column].shape == playedTile->shape && board[row-i][column].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row+1][column].shape == playedTile->shape){
    cout << "Location Valid (Shape on row matches)" << board[row+1][column].shape << ", " << playedTile->shape << endl;
    for(int i=0;i<6;i++){
      if((row+i)>(MAX_SIZE_BOARD)){
        //do nothing
      }
      else if(board[row+i][column].shape == playedTile->shape && board[row+i][column].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row+1][column].colour == playedTile->colour){
    cout << "Location Valid (Colour on row matches)" << board[row+1][column].colour <<  ", "<< playedTile->colour << endl;
    for(int i=0;i<6;i++){
      if((row+i)>(MAX_SIZE_BOARD)){
        //do nothing
      }
      else if(board[row+i][column].shape == playedTile->shape && board[row+i][column].colour == playedTile->colour){
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row][column-1].shape == playedTile->shape){
    cout << "Location Valid (Shape on col matches)" << board[row][column-1].shape << ", " << playedTile->shape << endl;
    for(int i=0;i<6;i++){
      if((column-i)<0){
        //do nothing
      }
      else if(board[row][column-i].shape == playedTile->shape && board[row][column-i].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row][column-1].colour == playedTile->colour){
    cout << "Location Valid (Colour on col matches)" << board[row][column-1].colour <<  ", "<< playedTile->colour << endl;
    for(int i=0;i<6;i++){
      if((column-i)<0){
        //do nothing
      }
      else if(board[row][column-i].shape == playedTile->shape && board[row][column-i].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row][column+1].shape == playedTile->shape){
    cout << "Location Valid (Shape on col matches)" << board[row][column+1].shape << ", " << playedTile->shape << endl;
    for(int i=0;i<6;i++){
      if((column+i)>MAX_SIZE_BOARD){
        //do nothing
      }
      else if(board[row][column+i].shape == playedTile->shape && board[row][column+i].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else if (board[row][column+1].colour == playedTile->colour){
    cout << "Location Valid (Colour on col matches)" << board[row][column+1].colour <<  ", "<< playedTile->colour << endl;
    for(int i=0;i<6;i++){
      if((column+i)>MAX_SIZE_BOARD){
        //do nothing
      }
      else if(board[row][column+i].shape == playedTile->shape && board[row][column+i].colour == playedTile->colour){
        cout << "Tile already in the line" << endl;
        result = false;
        break;
      }else{
        result = true;
      }
    }
  }
  else{
    cout << "Tile must be inline with with a matching tile" << endl;
    result = false;
  }
  return result;
}

int main(int argc, char **argv)
{
  //LinkedList* list = new LinkedList();
  //delete list;

  //std::cout << "TODO: Implement Qwirkle!" << std::endl;

  std::cout << "Welcome to Qwirkle!" << std::endl;
  std::cout << "-------------------" << std::endl;

  int option = 0;

  while (option != 4)
  {
    option = selectMenuOption();
    if (option == 1)
    {
      newGame();
      std::string TileBagFile = "tileBag.txt";
      if (argc >= 2)
      {
        TileBagFile = argv[1];
      }

      TileBag *tilebag = new TileBag();
      Player *playerOne = new Player(1);
      Player *playerTwo = new Player(2);
      storePlaceholder();
      playGame(TileBagFile, tilebag, playerOne, playerTwo);
    }
    else if (option == 2)
      loadGame();
    else if (option == 3)
      showCredits();
    else if (option == 4)
      std::cout << "Goodbye!" << std::endl;
  }

  return EXIT_SUCCESS;
}

void playGame(std::string TileBagFile, TileBag *tilebag, Player *playerOne, Player *playerTwo)
{
  std::ifstream file(TileBagFile);

  int numRead = 0;
  while (!file.eof() && numRead < MAX_TILEBAG_SIZE)
  {
    Colour colour = 'R';
    Shape shape = 0;
    bool readSuccess = false;
    readSuccess = readOneTile(file, &colour, &shape);

    if (!file.eof() && readSuccess)
    {
      // Put the tile in the linked list
      Tile *tile = new Tile(colour, shape);
      // TODO - USE Deck
      tilebag->add(tile);

      // Increment tiles read
      ++numRead;
    }
  }

  // Close the file
  file.close();

  //print the tilebag

  //std::cout << "Tilebag: " << std::endl;
  //printTilebag(tilebag);

  // Create a new temporary tileBag
  TileBag *player1hand = new TileBag();
  TileBag *player2hand = new TileBag();

  // Randomise the players hand
  std::random_device randomSeed;
  std::uniform_int_distribution<int> uniform_dist(0, numRead - 1);
  int i = 0;
  int j = 0;
  while (i < MAX_HAND_SIZE)
  {
    // Pick a random tile from the tilebag
    int randIndex = uniform_dist(randomSeed);

    if (tilebag->get(randIndex) != nullptr)
    {
      //move from tilebag to player
      Tile *tile = new Tile(*tilebag->get(randIndex));
      player1hand->add(tile);
      tilebag->remove(randIndex);

      ++i;
    }
  }
  while (j < MAX_HAND_SIZE)
  {
    // Pick a random tile from the tilebag
    int randIndex1 = uniform_dist(randomSeed);

    if (tilebag->get(randIndex1) != nullptr)
    {
      //move from tilebag to player
      Tile *tile1 = new Tile(*tilebag->get(randIndex1));
      player2hand->add(tile1);
      tilebag->remove(randIndex1);

      ++j;
    }
  }

  bool optionCheck = false;
  string option = "temp temp";

  printBoard();
  cout << player1name << "'s hand" << endl;

  checkHand(player1hand, tilebag);
  refreshHand(tilebag, player1hand);
  checkHand(player1hand, tilebag);
  refreshHand(tilebag, player1hand);
  printHand(player1hand);

  cout << endl;
 
  while(optionCheck == false){
  
  cout << "Press 1 to place a tile from your hand\nPress 2 to replace a tile in your hand\nPress 3 to save game" << endl;
  
  cout << "> "; cin >> option;
  
  
  if(option == "1"){
    placeInitialTile(player1hand);
    optionCheck = true;
  }
  else if(option == "2"){
    replaceTile(player1hand, tilebag);
    refreshHand(tilebag, player1hand);
    cout << player1name << "'s new hand:"<< endl;
    
    printHand(player1hand);
    cout << endl;
    optionCheck = true;
  }
  else if(option == "3"){
    saveGame(player1hand, player2hand, tilebag);
    optionCheck = true;
    
  }
  else{
    cout << "Invalid option" << endl;
  }
  }
  

  cout << endl;

  for(int d = 0; d<10; d++){
  bool loopCheck = false;
  bool loopCheck1 = false;

  printBoard();
  cout << player2name << "'s hand" << endl;

  refreshHand(tilebag, player2hand);
  checkHand(player2hand, tilebag);
  refreshHand(tilebag, player2hand);
  checkHand(player2hand, tilebag);
  refreshHand(tilebag, player2hand);
  printHand(player2hand);

  cout << endl;
  
  while(loopCheck == false){
  cout << "Press 1 to place a tile from your hand\nPress 2 to replace a tile in your hand\nPress 3 to save game" << endl;

  cout << "> "; cin >> option;
  
  
  if(option == "1"){
    placeTile(player2hand);
    loopCheck = true;
  }
  else if(option == "2"){
    replaceTile(player2hand, tilebag);
    cout << player2name << "'s new hand:"<< endl;
    //refreshHand(tilebag, player2hand);
    printHand(player2hand);
    cout << endl;
    loopCheck = true;
  }
  else if(option == "3"){
    saveGame(player1hand, player2hand, tilebag);
    loopCheck = true;
  }
  else{
    cout << "Invalid option" << endl;
  }
  }

  cout << endl;

  printBoard();
  cout << player1name << "'s hand" << endl;


  refreshHand(tilebag, player1hand);
  checkHand(player1hand, tilebag);
  refreshHand(tilebag, player1hand);
  checkHand(player1hand, tilebag);
  refreshHand(tilebag, player1hand);
  printHand(player1hand);

  cout << endl;
  
  
  while(loopCheck1 == false){
  cout << "Press 1 to place a tile from your hand\nPress 2 to replace a tile in your hand\nPress 3 to save game" << endl;

  cout << "> "; cin >> option;
  
  
  if(option == "1"){
    placeTile(player1hand);
    loopCheck1 = true;
  }
  else if(option == "2"){
    replaceTile(player1hand, tilebag);
    cout << player1name << "'s new hand:"<< endl;
    refreshHand(tilebag, player1hand);
    printHand(player1hand);
    cout << endl;
    loopCheck1 = true;
  }
  else if(option == "3"){
    saveGame(player1hand, player2hand, tilebag);
    loopCheck1 = true;
  }
  else{
    cout << "Invalid option" << endl;
  }
  }

  }
  
}
void replaceTile(TileBag *hand, TileBag *tilebag){
  
  cout << "Which tile would you like to replace?" << endl;

  bool tileExists = false;
  char replacedColour = 'X';

  //input tile
  do
  {
    cout << "> "; cin >> tileToReplace;

    //copy tile string
    std::string temp = "temp string";
    temp = tileToReplace;

    //take int value of the string for the shape
    size_t i = 0;
    for (; i < tileToReplace.length(); i++)
    {
      if (isdigit(tileToReplace[i]))
        break;
    }

    tileToReplace = tileToReplace.substr(i, tileToReplace.length() - i);

    int replacedShape = atoi(tileToReplace.c_str());

    //set colour to string index 0
    replacedColour = temp[0];

    //check if tile is in the hand
    int k = 0;

    while (k < MAX_HAND_SIZE)
    {
      //move from tilebag to player
      Tile *tile = new Tile(*hand->get(k));
      //cout << tile->colour << endl;

      if (tile->colour == replacedColour && tile->shape == replacedShape)
      {
        tileExists = true;
        hand->remove(k);
        break;
      }
      else
      {
        tileExists = false;
      }

      ++k;
    }
    if (tileExists == false)
    {
      cout << "Invalid Tile! Try again" << endl;
    }
  } while (tileExists == false);
  int replacedShape = atoi(tileToPlace.c_str());

  Tile *replacedTile = new Tile(replacedColour, replacedShape);

  //add replaced tile to the Tilebag
  tilebag->add(replacedTile);
}

void refreshHand(TileBag *tilebag, TileBag *hand)
{
  int numRead = 107;
  std::random_device randomSeed;
  std::uniform_int_distribution<int> uniform_dist(0, numRead - 1);
  // Pick a random tile from the tilebag
  int randIndex1 = uniform_dist(randomSeed);

while(hand->size() < 6){  
  if (tilebag->get(randIndex1) != nullptr)
  {
    //move from tilebag to player
    Tile *tile1 = new Tile(*tilebag->get(randIndex1));
    hand->add(tile1);
    tilebag->remove(randIndex1);
  }
  }
}

void checkHand(TileBag *hand, TileBag *tilebag){

  //check if two tiles in the hand are the same
  bool tilesExist = false;
  int handIndex = 222;

  for(int i=0;i<MAX_HAND_SIZE;i++){
    Tile *tileI = new Tile(*hand->get(i));
    for(int j=0;j<MAX_HAND_SIZE;j++){
      Tile *tileJ = new Tile(*hand->get(j));
      if(i != j && tileI->colour == tileJ->colour && tileI->shape == tileJ->shape){
        tilesExist = true;
        handIndex = j;

        break;
      }
      else{
        tilesExist = false;
      }
    }
  }
 if(tilesExist == true){
   //same tile exists
        Tile *sameTile = new Tile(*hand->get(handIndex));
        hand->remove(handIndex);
        tilebag->add(sameTile);
 }
 else{
   //all tiles in hand are unique
 }
}

bool readOneTile(std::ifstream &file, Colour *colour, Shape *shape)
{

  //read colour
  Colour readColour = 'R';
  bool readSuccess = true;
  file >> readColour;

  if (readColour == 'R')
  {
    *colour = 'R';
  }
  else if (readColour == 'O')
  {
    *colour = 'O';
  }
  else if (readColour == 'Y')
  {
    *colour = 'Y';
  }
  else if (readColour == 'G')
  {
    *colour = 'G';
  }
  else if (readColour == 'B')
  {
    *colour = 'B';
  }
  else if (readColour == 'P')
  {
    *colour = 'P';
  }
  else
  {
    readSuccess = false;
    std::cout << "Incorrect color try again!" << std::endl;
  }

  //read shame
  file >> *shape;

  return readSuccess;
}

void saveGame(TileBag *handp1, TileBag *handp2, TileBag *tilebag){
  ofstream outfile;
  cout << "What is the name of the saved game?" << endl;
  string savedGame = "temp";
  cin >> savedGame;
  outfile.open(savedGame + ".txt");



  if(outfile.is_open()){
    outfile << player1name << endl;
    outfile << "no score" << endl;
  for (int i = 0; i < handp1->size(); ++i)
   {
     Tile *handtile = new Tile(*handp1->get(i));
      if (handtile != nullptr)
      {
        outfile << handtile->getColour()
                << handtile ->getShape() << ", ";
      }
   }
   outfile << endl;
   outfile << player2name << endl;
    outfile << "no score" << endl;
  for (int i = 0; i < handp2->size(); ++i)
   {
     Tile *handtile1 = new Tile(*handp2->get(i));
      if (handtile1 != nullptr)
      {
        outfile << handtile1->getColour()
                << handtile1 ->getShape() << ", ";
      }
   }

  outfile << endl;

  char letterIndex = '@';
    for (unsigned int i = 0; i < board.size(); i++)
  {


    for (unsigned int j = 0; j < board[i].size(); j++)
    {
      //doesnt print top row of vector where the index is
      if (i == 0)
      {
        //do nothing
      }
      else
      {

    if (i == 1)
    {
      letterIndex = 'A';
    }
    if (i == 2)
    {
      letterIndex = 'B';
    }
    if (i == 3)
    {
      letterIndex = 'C';
    }
    if (i == 4)
    {
      letterIndex = 'D';
    }
    if (i == 5)
    {
      letterIndex = 'E';
    }
    if (i == 6)
    {
      letterIndex = 'F';
    }
    if (i == 7)
    {
      letterIndex = 'G';
    }
    if (i == 8)
    {
      letterIndex = 'H';
    }
    if (i == 9)
    {
      letterIndex = 'I';
    }
    if (i == 10)
    {
      letterIndex = 'J';
    }
    if (i == 11)
    {
      letterIndex = 'K';
    }
    if (i == 12)
    {
      letterIndex = 'L';
    }
    if (i == 13)
    {
      letterIndex = 'M';
    }
    if (i == 14)
    {
      letterIndex = 'N';
    }
    if (i == 15)
    {
      letterIndex = 'O';
    }
    if (i == 16)
    {
      letterIndex = 'P';
    }
    if (i == 17)
    {
      letterIndex = 'Q';
    }
    if (i == 18)
    {
      letterIndex = 'R';
    }
    if (i == 19)
    {
      letterIndex = 'S';
    }
    if (i == 20)
    {
      letterIndex = 'T';
    }
    if (i == 21)
    {
      letterIndex = 'U';
    }
    if (i == 22)
    {
      letterIndex = 'V';
    }
    if (i == 23)
    {
      letterIndex = 'W';
    }
    if (i == 24)
    {
      letterIndex = 'X';
    }
    if (i == 25)
    {
      letterIndex = 'Y';
    }
    if (i == 26)
    {
      letterIndex = 'Z';
    }
        //replace placeholder with empty board space
        if (board[i][j].shape == 80 && board[i][j].colour == 'X')
        {
          //do nothing
        }
        else
        {
          //if board space is not placeholder then print it on the board
          outfile << board[i][j].colour << board[i][j].shape << "@" << letterIndex << j << " ";
        }
      }
    }
  }
  outfile << endl;
  for (int i = 0; i < tilebag->size(); ++i)
   {
     Tile *tilebagTile = new Tile(*tilebag->get(i));
      if (tilebagTile != nullptr)
      {
        outfile << tilebagTile->getColour()
                << tilebagTile ->getShape() << ", ";
      }
   }
  }

  else{
    cout << "error saving file" << endl;
  }

  selectMenuOption();

}
