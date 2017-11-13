#define WIDTH	26
#define HEIGHT	99
#define LENGTH	200
const int THRESHOLD = 100000007;
char Table[HEIGHT][WIDTH][LENGTH];
bool ValidCell[HEIGHT][WIDTH];
bool CalculatedTable[HEIGHT][WIDTH];

int highestRow, highestCol;
int noOfUndetermined;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
struct GraphNode
{
  int row;
  int col;
  GraphNode* prev;
  GraphNode(int r, int c, GraphNode* p) : row(r), col(c), prev(p) {};
};

bool processCell(int row, int col, int value[HEIGHT][WIDTH], GraphNode* prev);
int getIntVal(char*, int& i);

void initTable()
{
  highestRow = 0;
  highestCol = 0;

  for (int idx = 0; idx < HEIGHT; idx++)
  {
    for (int jdx = 0; jdx < WIDTH; jdx++)
    {
      ValidCell[idx][jdx] = true;
      CalculatedTable[idx][jdx] = false;
      for (int kdx = 0; kdx < LENGTH; kdx++)
      {
        Table[idx][jdx][kdx] = '\0';
      }
    }
  }
}

bool updateCell(int row, int col, char equation[LENGTH], int value[HEIGHT][WIDTH])
{
  bool ret = true;
  noOfUndetermined = 0;

  if (row > highestRow)
    highestRow = row;
  if (col > highestCol)
    highestCol = col;

  int i = 0;
  while (equation[i] != 0)
  {
    Table[row][col][i] = equation[i];
    i++;
  }
  Table[row][col][i] = 0;

  for (int i = 0; i <= highestRow; i++)
  {
    for (int j = 0; j <= highestCol; j++)
    {
      CalculatedTable[i][j] = false;
    }
  }

  if (ValidCell[row][col] == false)
  {
    for (int i = 0; i <= highestRow; i++)
    {
      for (int j = 0; j <= highestCol; j++)
      {
        ValidCell[i][j] = true;
        value[i][j] = 0;
      }
    }
  }

  // process cells
  for (int i = 0; i <= highestRow; i++)
  {
    for (int j = 0; j <= highestCol; j++)
    {
      if (!CalculatedTable[i][j])
      {
        if (!processCell(i, j, value, &GraphNode(i, j, nullptr)))
        {
          ret = false;
          noOfUndetermined++;
          ValidCell[i][j] = false;
        }
      }
    }
  }

  noOfUndetermined = 0;
  for (int i = 0; i <= highestRow; i++)
  {
    for (int j = 0; j <= highestCol; j++)
    {
      if (!ValidCell[i][j])
      {
        noOfUndetermined++;
      }
    }
  }

  for (int i = 0; i <= highestRow; i++)
  {
    for (int j = 0; j <= highestCol; j++)
    {
      if (!ValidCell[i][j])
      {
        value[i][j] = noOfUndetermined;
      }
    }
  }

  return ret; // Need to be changed
}

bool processCell(int row, int col, int value[HEIGHT][WIDTH], GraphNode* prevNode)
{
  //chack for circulars
  GraphNode* itNode = prevNode->prev;
  while (itNode != nullptr)
  {
    if (itNode->col == col && itNode->row == row)
    {
      ValidCell[row][col] = false;
      return false;
    }
    itNode = itNode->prev;
  }

  value[row][col] = 0;

  bool ret = true;
  int i = 0;
  char sign = '+';
  while (Table[row][col][i] != '\0')
  {
    if (Table[row][col][i] == '-')
    {
      sign = '-';
    }
    else if (Table[row][col][i] == '+')
    {
      sign = '+';
    }
    else if (Table[row][col][i] >= 'A' && Table[row][col][i] <= 'Z')
    {
      // recursion
      int innerCol = Table[row][col][i] - 'A';
      i++;
      int innerRow = getIntVal(&Table[row][col][i], i) - 1;

      if (innerCol == col && innerRow == row) // same 
      {
        ValidCell[row][col] = false;
        return false;
      }
      else if (ValidCell[innerRow][innerCol] == false)
      {
        ValidCell[row][col] = false;
        return false;
      }
      else
      {
        if (CalculatedTable[row][col] == false)
        {
          bool retFromRec = processCell(innerRow, innerCol, value, &GraphNode(innerRow, innerCol, prevNode));
          if (!retFromRec)
          {
            ValidCell[row][col] = false;
            return false;
          }
          else
          {
            if (sign == '-')
              value[row][col] -= value[innerRow][innerCol];
            else if (sign == '+')
              value[row][col] += value[innerRow][innerCol];
          }
        }
        else
        {
          if (ValidCell[innerRow][innerCol])
          {
            if (sign == '-')
              value[row][col] -= value[innerRow][innerCol];
            else if (sign == '+')
              value[row][col] += value[innerRow][innerCol];
          }
          else
          {
            ValidCell[row][col] = false;
            return false;
          }
        }
      }
    }
    else if (Table[row][col][i] >= '0' && Table[row][col][i] <= '9')
    {
      if (sign == '+')
      {
        value[row][col] += getIntVal(&Table[row][col][i], i);
      }
      else if (sign == '-')
      {
        value[row][col] -= getIntVal(&Table[row][col][i], i);
      }
    }

    i++;
  }

  value[row][col] %= THRESHOLD;
  CalculatedTable[row][col] = true;
  return ret;
}

int getIntVal(char* val, int& i)
{
  int localIt = 0;
  int ret = 0;

  while (val[localIt] >= '0' &&  val[localIt] <= '9')
  {
    if (ret == 0)
    {
      ret = val[localIt] - '0';
    }
    else
    {
      ret *= 10;
      ret += (val[localIt] - '0');
    }
    i++;
    localIt++;
  }

  i--;

  return ret;
}