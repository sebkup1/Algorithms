#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

int N = 0;
int sortedInd[3] = { 0, 0, 0 };
int counts[3] = { 0, 0, 0 }, pos[3] = { 0, 0, 0 };
int length = 0;
bool outOfBound = 0;
int distMeasure = 0;
int maxDistMeasure = 0;

void distribute(int* places)
{
  distMeasure = 0;

  for (int k = 0; k < 3; k++)
  {
    length = 0;

    outOfBound = false;

    for (int f = 0; f < counts[sortedInd[k]]; f++)
    {

      int toBreak = false;

      while (places[pos[sortedInd[k]] - 1 + length] != 0)
      {
        if (outOfBound)
        {
          if (length > 0)
          {
            length++;
          }
          else
          {
            length--;
          }

          if (places[pos[sortedInd[k]] - 1 + length] > N || places[pos[sortedInd[k]] - 1 + length] < 0)
            toBreak = true;

          continue;

        }

        // find closest
        else if (length <= 0)
        {
          length = (-length) + 1;

          // if last && has two choices
          if (f + 1 == counts[sortedInd[k]] && k < 2 && (places[pos[sortedInd[k]] - 1 - length] == 0))
          {
            int leftSideFishermans = 0, rightSideFishermans = 0;
            int leftSidePlaces = 0, rightSidePlaces = 0;

            //pending fishermans
            for (int j = k + 1; j < 3; j++)
            {
              if (pos[sortedInd[j]] > pos[sortedInd[k]])
                rightSideFishermans += counts[sortedInd[j]];

              else
                leftSideFishermans += counts[sortedInd[j]];

            }

            //free places
            for (int j = 0; j < N; j++)
            {

              if (places[j] == 0)
              {
                if (j < pos[sortedInd[k]])
                {
                  leftSidePlaces++;
                }
                else
                {
                  rightSidePlaces++;
                }

              }

            }

            if ((double)leftSideFishermans / leftSidePlaces < (double)rightSideFishermans / rightSidePlaces)
            {
              length = (-length);
            }

          }

        }

        else
          length = (-length);

        if ((pos[sortedInd[k]] - 1 + length) > N - 1 || (pos[sortedInd[k]] - 1 + length) < 0)
        {
          outOfBound = true;
          length = (-length);
        }

      }

      if (toBreak) break;

      int index = pos[sortedInd[k]] - 1 + length;

      if (index < 0 || index >= N)
      {
        int r;
        r = 6;
      }

      places[index] = sortedInd[k] + 1;

      if (length > 0)
      {
        distMeasure += length + 1;
      }
      else
      {
        distMeasure += -length + 1;
      }

    }

  }

  

  if (distMeasure < maxDistMeasure) maxDistMeasure = distMeasure;

  //for (int i = 0; i < N; i++)
  //{
  //  cout << places[i] << "\t";
  //}
  //cout << endl;

  //cout << "\t  " << distMeasure << endl; 
  //cout << endl;
  //  cout << endl;

} 

int main(int argc, char** argv)
{
  int test_case;
  int T;
  int* places = nullptr;
  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2012\\Projects\\fisherman\\Debug\\sample_input.txt", "r", stdin);
  std::ios::sync_with_stdio(false);

  cin >> T;

  for (test_case = 1; test_case <= T; test_case++)
  {
    int g1Pos, g1Count, g2Pos, g2Count, g3Pos, g3Count;

    maxDistMeasure = 1000;

    cin >> N;
    cin >> g1Pos;
    cin >> g1Count;
    cin >> g2Pos;
    cin >> g2Count;
    cin >> g3Pos;
    cin >> g3Count;
    counts[0] = g1Count;
    counts[1] = g2Count;
    counts[2] = g3Count;
    pos[0] = g1Pos;
    pos[1] = g2Pos;
    pos[2] = g3Pos;

    places = new int[N];

    //for (int i = 0; i < N; i++)
    //{
    //  if (pos[0] == i + 1)
    //    cout << 1 << "(" << counts[0] << ")";

    //  else if (pos[1] == i + 1)
    //    cout << 2 << "(" << counts[1] << ")";

    //  else if (pos[2] == i + 1)
    //    cout << 3 << "(" << counts[2] << ")";

    //  else
    //    cout << 0;

    //  cout << "\t";

    //}

    cout << endl; 

    for (int i = 0; i < N; i++) places[i] = 0;
    //cout << "comb 1 2 3" << endl;
    sortedInd[0] = 0;
    sortedInd[1] = 1;
    sortedInd[2] = 2;
    distribute(places);

    for (int i = 0; i < N; i++) places[i] = 0;
    //cout << "comb 1 3 2" << endl;
    sortedInd[0] = 0;
    sortedInd[1] = 2;
    sortedInd[2] = 1;
    distribute(places);

    for (int i = 0; i < N; i++) places[i] = 0;
    //cout << "comb 2 1 3" << endl;
    sortedInd[0] = 1;
    sortedInd[1] = 0;
    sortedInd[2] = 2;
    distribute(places);

    for (int i = 0; i < N; i++) places[i] = 0;
    //cout << "comb 2 3 1" << endl;
    sortedInd[0] = 1;
    sortedInd[1] = 2;
    sortedInd[2] = 0;
    distribute(places);

    for (int i = 0; i < N; i++) places[i] = 0;
    //cout << "comb 3 1 2" << endl;
    sortedInd[0] = 2;
    sortedInd[1] = 0;
    sortedInd[2] = 1;
    distribute(places);

    for (int i = 0; i < N; i++) places[i] = 0;
    //cout << "comb 3 2 1" << endl;
    sortedInd[0] = 2;
    sortedInd[1] = 1;
    sortedInd[2] = 0;
    distribute(places); 




    // Print the answer to standard output(screen).
    cout << "#" << test_case << ": " << maxDistMeasure << endl;

    delete[] places;

  } 

  return 0; // Your program should return 0 on normal termination.

}