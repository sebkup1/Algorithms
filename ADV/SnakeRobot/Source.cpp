#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

enum class Dir{
  toLeft,
  toRight,
  toUp,
  toDown
};
struct Line
{
  int p1x;
  int p1y;
  int p2x;
  int p2y;
  bool vert;
};

struct Point
{
  int x;
  int y;
};

bool compare(Line prev, Line cur)
{
  //równoleg³e
  if (prev.vert == cur.vert)
  {
    if (prev.vert == true)
    {
      if (prev.p1x == cur.p1x) //pokrywaj¹ siê
      {
        if (((prev.p1y >= cur.p1y && prev.p1y >= cur.p2y) && (prev.p2y >= cur.p1y && prev.p2y >= cur.p2y)) ||
          ((prev.p1y <= cur.p1y && prev.p1y <= cur.p2y) && (prev.p2y <= cur.p1y && prev.p2y <= cur.p2y)))
        {
          return true;
        }
        else
        {
          return false;
        }

      }
    }
    else
    {
      if (prev.p1y == cur.p1y)
      {
        if (((prev.p1x >= cur.p1x && prev.p1x >= cur.p2x) && (prev.p2x >= cur.p1x && prev.p2x >= cur.p2x)) ||
          ((prev.p1x <= cur.p1x && prev.p1x <= cur.p2x) && (prev.p2x <= cur.p1x && prev.p2x <= cur.p2x)))
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }
  }

  // prost
  else
  {
    if (prev.vert == false)
    {
      // y
      if ((prev.p1y < cur.p1y && prev.p1y > cur.p2y) ||
        (prev.p1y > cur.p1y && prev.p1y < cur.p2y))
      {
        // x
        if ((cur.p1x < prev.p1x && cur.p1x > prev.p2x) ||
          (cur.p1x > prev.p1x && cur.p1x < prev.p2x))
        {
          return false;
        }
      }
    }
    else
    {
      // y
      if ((cur.p1y < prev.p1y && cur.p1y > prev.p2y) ||
        (cur.p1y > prev.p1y && cur.p1y < prev.p2y))
      {
        // x
        if ((prev.p1x < cur.p1x && prev.p1x > cur.p2x) ||
          (prev.p1x > cur.p1x && prev.p1x < cur.p2x))
        {
          return false;
        }
      }
    }

  }

  return true;
}


int main(int argc, char** argv){

  int test_case;
  int T;
  int moves, direction, value;
  Point curPoint;

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2013\\Projects\\snake_robot\\Debug\\snake_sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    curPoint.x = 0;
    curPoint.y = 0;
    int answer = -1;

    cin >> moves;

    Line* lines = new Line[moves];

    for (int i = 0; i < moves; ++i)
    {
      lines[i].p1x = curPoint.x;
      lines[i].p1y = curPoint.y;

      lines[i].p2x = curPoint.x;
      lines[i].p2y = curPoint.y;

      cin >> direction;
      cin >> value;

      switch (direction)
      {
      case 1:
        lines[i].p2y = curPoint.y + value;
        curPoint.y = curPoint.y + value;
        lines[i].vert = true;
        break;
      case 2:
        lines[i].p2y = curPoint.y - value;
        curPoint.y = curPoint.y - value;
        lines[i].vert = true;
        break;
      case 3:
        lines[i].p2x = curPoint.x + value;
        curPoint.x = curPoint.x + value;
        lines[i].vert = false;
        break;
      case 4:
        lines[i].p2x = curPoint.x - value;
        curPoint.x = curPoint.x - value;
        lines[i].vert = false;
        break;
      default:
        break;
      }
    }

    for (int i = 0; i < moves; ++i)
    {
      // compare prev lines
      bool hardBreak = false;
      for (int j = 0; j < i; j++)
      {
        if (compare(lines[j], lines[i]) == false)
        {
          answer = i + 1;
          hardBreak = true;
          break;
        }
      }

      if (hardBreak)
        break;
    }


    // Print the answer to standard output(screen).
    cout << "#" << test_case << " " << answer << endl;

    delete[] lines;
  }

  return 0; // Your program should return 0 on normal termination.
}