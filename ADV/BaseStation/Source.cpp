# define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

struct Ind
{
  int col;
  int row;
  Ind::Ind(int r, int c) : row(r), col(c) {};
};

int maxSum=0;
int baseStations[4]={0,0,0,0};
int height;
int width;

bool checkPrevInds(Ind prev[4], Ind current, int currentSize)
{
  for (int i = 0; i < currentSize; i++)
  {
    if (current.col == prev[i].col && current.row == prev[i].row)
      return false;
  }
  return true;
}


void countFunc(int row, int col, int size, int* array, int sum, int users[4], Ind prevInds[4])
{
  if (!checkPrevInds(prevInds, Ind(row, col), size)) 
    return;

  prevInds[size] = Ind(row, col);
  users[size] = array[row*width + col];
  sum += array[row*width + col];
  size++;

  if (size == 4)
  {
    if (sum > maxSum)
    {
      maxSum = sum;
      baseStations[0] = users[0];
      baseStations[1] = users[1];
      baseStations[2] = users[2];
      baseStations[3] = users[3];
      int fi = 3;
    }
    return;
  }

  if (row == 0 && col == 5)
  {
    int fi = 3;
  }

  // parity
  if (col%2 == 0)
  {
    //right-up
    if (col < width-1 && row > 0 && col > 0)
      countFunc(row-1, col+1, size, array, sum, users, prevInds);

    //up-left
    if (row > 0 && col > 0)
      countFunc(row-1, col-1, size, array, sum, users, prevInds);

  }
  //non-parity
  else
  {
    //right-down
    if (row < height-1 && col < width-1)
      countFunc(row+1, col+1, size, array, sum, users, prevInds);

    //down-left
    if (row < height-1 && col > 0)
      countFunc(row+1, col-1, size, array, sum, users, prevInds);
  }

  //right
  if (col < width-1)
    countFunc(row, col+1, size, array, sum, users, prevInds);

  //down
  if (row < height-1)
    countFunc(row+1, col, size, array, sum, users, prevInds);


  //left
  if (col > 0)
    countFunc(row, col-1, size, array, sum, users, prevInds);


  // stars
  if (size == 1 && col > 0 && row > 0 && 
    col < width - 1 && row < height -1)
  {
    // parity
    if (col%2 == 0)
    {
      //single up
      int sumStars = sum + array[(row-1)*width + col] + array[row*width + col - 1] + array[row*width + col + 1];
      if (sumStars > maxSum)
      {
        baseStations[0] = sum;
        baseStations[1] = array[(row-1)*width + col]; 
        baseStations[2] = array[row*width + col - 1];
        baseStations[1] = array[row*width + col + 1];
        maxSum = sumStars;
      }

      //single down
      int sumStars2 = sum + array[(row+1)*width + col] + array[(row-1)*width + col - 1] + array[(row-1)*width + col + 1];
      if (sumStars2 > maxSum)
      {
        baseStations[0] = sum;
        baseStations[1] = array[(row+1)*width + col];
        baseStations[2] = array[(row-1)*width + col - 1];
        baseStations[1] = array[(row-1)*width + col + 1];
        maxSum = sumStars2;
      }
    }
    //non-parity
    else
    {
      //single up
      int sumStars = sum + array[(row-1)*width + col] + array[(row+1)*width + col - 1] + array[(row+1)*width + col + 1];
      if (sumStars > maxSum)
      {
        baseStations[0] = sum;
        baseStations[1] = array[(row-1)*width + col]; 
        baseStations[2] = array[(row+1)*width + col - 1];
        baseStations[1] = array[(row+1)*width + col + 1];
        maxSum = sumStars;
      }

      //single down
      int sumStars2 = sum + array[(row+1)*width + col] + array[row*width + col - 1] + array[row*width + col + 1];
      if (sumStars2 > maxSum)
      {
        baseStations[0] = sum;
        baseStations[1] = array[(row+1)*width + col];
        baseStations[2] = array[row*width + col - 1];
        baseStations[1] = array[row*width + col + 1];
        maxSum = sumStars2;
      }
    }
  }

}

int main(int argc, char** argv)
{
  int test_case;
  int T;

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2012\\Projects\\base_station\\Debug\\sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    // Read data
    cin >> width;
    cin >> height;

    int* cels = new int[width*height];

    for(int row = 0; row < height; row++)
    {
      for (int col = 0; col < width; col++)
      {
        cin >> cels[row*width + col];
      }
    }

    //display
    //for(int row = 0; row < height; row++)
    //{
    //  //parity
    //  for (int col = 0; col < width; col=col+2)
    //  {
    //    cout << cels[row*width + col] << "\t\t";
    //  }
    //  cout << endl;

    //  cout << "\t";
    //  //non-parity
    //  for (int col = 1; col < width; col=col+2)
    //  {
    //    cout << cels[row*width + col] << "\t\t";
    //  }
    //  cout << endl;
    //}


    // examin cells recursive
    for(int row = 0; row < height; row++)
    {
      for (int col = 0; col < width; col++)
      {
        Ind prevInds[4]={Ind(-1,-1),Ind(-1,-1),Ind(-1,-1),Ind(-1,-1)};
        int users[4]={0,0,0,0};
        countFunc(row, col, 0, cels, 0, users, prevInds);
      }
    }




    // Print the answer to standard output(screen).
    cout << "#" << test_case << " " << maxSum*maxSum;
    for (int i = 0; i < 4; i++)
    {
      cout << " " << baseStations[i] << " ";
    }
    cout << endl;

  }

  return 0; // Your program should return 0 on normal termination.
}

