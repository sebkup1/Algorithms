#define _CRT_SECURE_NO_WARNINGS	// Ignore build errors when using unsafe functions in Visual Studio.

#include <iostream>

using namespace std;

struct OneTimeRepDist
{
  int start;
  OneTimeRepDist* prev;
  OneTimeRepDist* next;
  OneTimeRepDist(){ };
  OneTimeRepDist(int s, OneTimeRepDist* pr, OneTimeRepDist* nxt) : start(s), prev(pr), next(nxt){};

  OneTimeRepDist(OneTimeRepDist& pr)
  {
    OneTimeRepDist dist;
    dist.next = pr.next;
    dist.prev = pr.prev;
    dist.start = pr.start;
  };

};

bool damagedRoadDists[10001];
bool coveredRoad[10001];

int main(int argc, char** argv)
{
  int test_case;
  int T;
  int N, K;
  int coveredDistances;
  freopen("C:\\Users\\sebastian\\Documents\\Visual Studio 2013\\Projects\\ExpresswayRepair\\ExpresswayRepair\\sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;

  for (test_case = 1; test_case <= T; test_case++)
  {
    // clearing the road
    for (int d = 0; d < 10001; d++)
    {
      damagedRoadDists[d] = false;
      coveredRoad[d] = false;
    }

    cin >> N;
    cin >> K;
    coveredDistances = 0;

    int elem;
    int firstDamage = 10001;
    int lastDamage = -1;

    // get road damaging
    for (int damage = 0; damage < N; damage++)
    {
      cin >> elem;

      if (elem > lastDamage)
        lastDamage = elem;

      if (elem < firstDamage)
        firstDamage = elem;

      damagedRoadDists[elem] = true;

    }


    // I. cover the road
    // cover first damage
    OneTimeRepDist first(firstDamage, nullptr, nullptr);

    int i = firstDamage;

    OneTimeRepDist* last = &first;

    // cover next
    while (true)
    {
      if (++i > lastDamage + K + 1) break;

      if (damagedRoadDists[i])
      {
        if (last->start + K - 1 < i)
        {
          // create new reparing dist
          OneTimeRepDist* current = new OneTimeRepDist(i, last, nullptr);

          last->next = current;

          // first 
          if (current->prev == nullptr)
          {
            last = current;
            continue;
          }


          // II. move back prev

          // prev Daamge
          int prevDamage = last->start + K - 1;

          // find next damage
          int nextDamage = i + 1;

          int tempIt = current->start + K - 1;
          while (!damagedRoadDists[nextDamage] && nextDamage < lastDamage) nextDamage++;


          if (nextDamage < lastDamage && i - prevDamage - 1 < K && nextDamage - i > i - prevDamage)
          {
            current->start = i - K + 1;
          }

          last = current;

        }
      }

    }

    // try move last
    int tempIt = last->start + K - 1;

    while (!damagedRoadDists[tempIt])
    {
      last->start--;
      tempIt--;
    }

    OneTimeRepDist* dist = &first;

    // count coveredRoad regions
    while (dist != nullptr)
    {
      int tempIt = dist->start;

      while (tempIt < dist->start + K)
      {
        if (!coveredRoad[tempIt])
        {
          coveredRoad[tempIt] = true;
          coveredDistances++;
        }

        tempIt++;
      }
      //cout << dist->start << " ";
      dist = dist->next;
    }

    // Print the answer to standard output(screen).
    first;

    cout << "#" << test_case << " " << coveredDistances << endl;
  }


  return 0; // Your program should return 0 on normal termination.

}