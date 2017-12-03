#define MAX_STICKS 50
#define MAX_LENGTH 1000
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

struct Stick
{
  int length;
  bool used;
  bool considered;
};

int sticksN;
Stick Sticks[MAX_STICKS];
int countOfLength[MAX_LENGTH+1];

bool findSum(int);

int main(int argc, char** argv)
{
  int test_case;
  int T;
  int answer;
  int sumOfBlocks;

  freopen("sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  
  for (test_case = 1; test_case <= T; test_case++)
  {
    cin >> sticksN;
    sumOfBlocks = 0;
    answer = 0;

    for (int i = 0; i < MAX_STICKS; i++)
    {
      Sticks[i].used = false;
      Sticks[i].considered = false;
    }

    for (int i = 0; i < sticksN; i++)
    {
      int length;
      cin >> length;
      countOfLength[length]++;
      sumOfBlocks += length;
    }

    // sort array
    int element = 0;

    for (int i = MAX_LENGTH; i > 0; i--)
    {
      while (countOfLength[i] > 0)
      {
        Sticks[element++].length = i;
        countOfLength[i]--;
      }
    }

    int halfSum = sumOfBlocks / 2;

    while (halfSum > 0)
    {
      for (int i = 0; i < sticksN; i++)
      {
        Sticks[i].used = false;
        Sticks[i].considered = false;
      }

      bool firstSumFound = false;

      firstSumFound = findSum(halfSum);

      if (firstSumFound)
      {
        for (int i = 0; i < sticksN; i++)
        {
          if (!Sticks[i].used)
            Sticks[i].considered = false;
        }

        if (findSum(halfSum))
        {
          answer = halfSum;
          break;
        }
      }

      halfSum--;
    }


    // Print the answer to standard output(screen).
    cout << "#" << test_case << " " << answer << endl;

  }

  return 0; // Your program should return 0 on normal termination.
}

bool findSum(int sum)
{
  for (int i = 0; i < sticksN; i++)
  {
    if (!Sticks[i].used && !Sticks[i].considered)
    {
      if (sum == Sticks[i].length)
      {
        Sticks[i].used = true;
        return true;
      }
      else if (sum - Sticks[i].length > 0)
      {
        sum -= Sticks[i].length;
        Sticks[i].considered = true;
        Sticks[i].used = true;

        if (findSum(sum))
          return true;

        sum += Sticks[i].length;
        Sticks[i].used = false;

      }
    }
  }
  return false;
}