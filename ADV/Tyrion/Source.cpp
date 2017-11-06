#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

int maxCoins;
bool abelToWin;

void processRow(int* gameField, int curPos, int curRow, int N, bool bombAvailable, int coins)
{
  if (curRow == N)
  {
    if (coins >= maxCoins)
    {
      maxCoins = coins;
      abelToWin = true;
    }
    return;
  }

  if (bombAvailable)
  {
    int tempCoins = coins;

    // no dropping bomb

    // update status
    if (curRow >= 0)
    {
      if (gameField[curRow * 5 + curPos] == 1)
        tempCoins++;
      else if (gameField[curRow * 5 + curPos] == 2)
      {
        tempCoins--;
        if (tempCoins == -1) return;
      }

    }


    // go left
    if (curPos > 0)
    {
      processRow(gameField, curPos - 1, curRow + 1, N, bombAvailable, tempCoins);
    }

     //stay at position
    processRow(gameField, curPos, curRow + 1, N, bombAvailable, tempCoins);

    // go right
    if (curPos < 4)
    {
      processRow(gameField, curPos + 1, curRow + 1, N, bombAvailable, tempCoins);
    }


    // dropping  bomb

    // prepare claeredGameField
    int* claeredGameField = new int[5 * N];

    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < 5; ++j)
      {
        if (i < N && i >= curRow + 1 && i < curRow + 1 + 5 && gameField[5 * i + j] == 2)
        {
          claeredGameField[5 * i + j] = 0;
        }
        else
          claeredGameField[5 * i + j] = gameField[5 * i + j];
      }
    }

    processRow(claeredGameField, curPos, curRow, N, false, coins);

    delete[] claeredGameField;
  }
  else
  {
    // update status
    if (curRow >= 0)
    {
      if (gameField[curRow * 5 + curPos] == 1)
        coins++;
      else if (gameField[curRow * 5 + curPos] == 2)
      {
        coins--;
        if (coins == -1) return;
      }

    }


    // go left
    if (curPos > 0)
    {
      processRow(gameField, curPos - 1, curRow + 1, N, bombAvailable, coins);
    }

    // stay at position
    processRow(gameField, curPos, curRow + 1, N, bombAvailable, coins);

    // go right
    if (curPos < 4)
    {
      processRow(gameField, curPos + 1, curRow + 1, N, bombAvailable, coins);
    }

  }
}

int main(int argc, char** argv)
{
  int test_case;
  int T;
  int N;


  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2012\\Projects\\tyrion\\Debug\\sample_input_TC.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    maxCoins = 0;
    abelToWin = false;

    cin >> N;
    int* gameField = new int[5 * N];

    for (int i = N - 1; i >= 0; --i)
    {
      for (int j = 0; j < 5; ++j)
      {
        cin >> gameField[5 * i + j];
      }
    }


    processRow(gameField, 2, -1, N, true, 0);

    if (!abelToWin)
      maxCoins = -1;

    // Print the answer to standard output(screen).
    cout << "#" << test_case << " " << maxCoins << endl;

    delete[] gameField;
  }

  return 0; // Your program should return 0 on normal termination.
}

