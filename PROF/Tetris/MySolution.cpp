#include <iostream>
using namespace std;
#define BLOCK_MAX_SIZE      3
#define MAX_BOARD_HEIGHT    200

void putZerosInblock(int* block);
void copyBlock(int* old, int* newb);
void rotate90();
int findHighest();
void printBoard();
void printBlock();

int* board = nullptr;
int boardWidth;
int curBlock[9];
int curBlockPos;
int curBlockWidth;
int curBlockHeight;
int highest;

void init(int w)
{
  if (board != nullptr)
    delete board;

  board = new int[w * MAX_BOARD_HEIGHT];

  for (int i = 0; i < MAX_BOARD_HEIGHT; i++)
  {
    for (int j = 0; j < w; j++)
      board[i*w + j] = 0;
  }

  boardWidth = w;
  highest = MAX_BOARD_HEIGHT;
}

void newBlock(int block[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE], int width, int height)
{
  curBlockPos = 0;
  curBlockWidth = width;
  curBlockHeight = height;

  putZerosInblock(curBlock);

  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      curBlock[i * 3 + j] = block[i][j];
    }
  }
}

void move(int distance)
{
  curBlockPos += distance;
}

void rotate(int angle)
{
  switch (angle)
  {
  case 3: // 270 
    rotate90();
  case 2: // 180 
    rotate90();
  case 1: // 90 
    rotate90();
    break;
  }
}

void rotate90()
{
  int* tempBlock = new int[9];
  copyBlock(curBlock, tempBlock);

  curBlock[0] = tempBlock[6];
  curBlock[1] = tempBlock[3];
  curBlock[2] = tempBlock[0];

  curBlock[3] = tempBlock[7];
  //curBlock[4] is the same
  curBlock[5] = tempBlock[1];

  curBlock[6] = tempBlock[8];
  curBlock[7] = tempBlock[5];
  curBlock[8] = tempBlock[2];


  while (curBlock[0] == 0 && curBlock[3] == 0 && curBlock[6] == 0)
  {
    curBlock[0] = curBlock[1];
    curBlock[3] = curBlock[4];
    curBlock[6] = curBlock[7];

    curBlock[1] = curBlock[2];
    curBlock[4] = curBlock[5];
    curBlock[7] = curBlock[8];

    curBlock[2] = curBlock[5] = curBlock[8] = 0;
  }


  int temp = curBlockWidth;
  curBlockWidth = curBlockHeight;
  curBlockHeight = temp;

  delete[] tempBlock;
}

int land()
{
  //cout << "======= land() ==============" << endl;

  // Rotation of block on the x axis
  int *tempBlock = new int[9];
  copyBlock(curBlock, tempBlock);

  curBlock[0] = tempBlock[6];
  curBlock[1] = tempBlock[7];
  curBlock[2] = tempBlock[8];

  curBlock[6] = tempBlock[0];
  curBlock[7] = tempBlock[1];
  curBlock[8] = tempBlock[2];

  // move block if it's out of board
  while (curBlockPos + curBlockWidth > boardWidth)
    curBlockPos--;

  // Calculate min dist
  int minDist = MAX_BOARD_HEIGHT;
  int properRow;

  for (int boi = curBlockPos, bli = 0;
    bli < curBlockWidth;
    boi++, bli++)
  {
    bool toConsider = false;
    int curDist = MAX_BOARD_HEIGHT - 3;
    int addDist = 0;

    if (curBlock[6 + bli] == 1)
    {
      addDist = 2;
      toConsider = true;
    }

    if (curBlock[3 + bli] == 1)
    {
      addDist = 1;
      toConsider = true;
    }

    if (curBlock[bli] == 1)
    {
      addDist = 0;
      toConsider = true;
    }

    if (!toConsider) continue;

    curDist += addDist;

    int row = 0;
    while (board[boardWidth*row + boi] == 1)
    {
      row++;
    }

    row -= addDist;
    curDist -= row;

    if (curDist < minDist)
    {
      minDist = curDist;
      properRow = row;
    }

  }


  // Put the Block
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (curBlock[i * 3 + j] == 1)
      {
        board[boardWidth*(properRow + i) + curBlockPos + j] = curBlock[i * 3 + j];
      }
    }

  }

  // find fullfilled lines
  for (int i = 0; i < MAX_BOARD_HEIGHT; i++)
  {
    bool good = true;
    for (int j = 0; j < boardWidth; j++)
    {
      if (board[i*boardWidth + j] == 0)
      {
        good = false;
        break;
      }
    }

    if (good)
    {
      //move all above board down
      for (int k = i; k < MAX_BOARD_HEIGHT - 1; k++)
      {
        for (int l = 0; l < boardWidth; l++)
        {
          board[boardWidth*k + l] = board[boardWidth*(k + 1) + l];
        }
      }
      i--;
    }
  }

  //printBoard();
  //printBlock();
  highest = findHighest();
  return highest;
}




// helpers

int findHighest()
{
  int highest = 0;
  for (int i = 0; i < MAX_BOARD_HEIGHT; i++)
  {
    for (int j = 0; j < boardWidth; j++)
    {
      if (board[i*boardWidth + j] == 1)
        highest = i;
    }
  }

  if (highest == 0)
    return 0;
  else
    return highest + 1;
}


void putZerosInblock(int* block)
{
  for (int i = 0; i < BLOCK_MAX_SIZE*BLOCK_MAX_SIZE; i++)
  {
    block[i] = 0;
  }
}

void copyBlock(int* old, int* newb)
{
  for (int i = 0; i < BLOCK_MAX_SIZE*BLOCK_MAX_SIZE; i++)
  {
    newb[i] = old[i];
  }
}


// debug

void printBoard()
{
  cout << endl << "Board: " << endl;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < boardWidth; j++)
    {
      cout << board[i * boardWidth + j];
    }
    cout << endl;
  }

  cout << endl;
}

void printBlock()
{
  cout << "Current Block: " << endl;
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      cout << curBlock[i * 3 + j];
    }
    cout << endl;
  }

  cout << "curBlockPos: " << curBlockPos << endl;
}