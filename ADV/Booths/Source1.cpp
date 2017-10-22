#include <iostream>

using namespace std;

int fielsdCount[6] = {0,0,0,0,0,0,};
int N;

struct Index
{
  int row;
  int col;
  Index(int row, int col) : row(row), col(col) {}
  Index() {}
};

template <typename T>
class MySet
{
public:
  int size;
  T* uniqueObj;

  MySet(int maxSize) 
  {
    size = 0;
    uniqueObj = new T[maxSize];
  }

  void insert(T item)
  {
    for(int i = 0; i < size; i++)
    {
      if (uniqueObj[i] == item) return;
    }

    uniqueObj[size] = item;
    size++;
  }
};

struct Area
{
public:
  int model;
  int size;
  Area(int model) : model(model), size(0){}
};

struct Area2Replace 
{
public:
  Area* area;
  int model;
  int size;
  MySet<Index*> indexex;
  Area2Replace(int model) : model(model), size(0), indexex(N*N) {}
};


void searchArea(int r, int c, int N, int* array, Area** arrOfPtrs, Area* previous, MySet<Area2Replace*>* areas2replace)
{
  if (arrOfPtrs[r*N +c] != nullptr ||
    r >= N || c >= N || r < 0 || c < 0) return;

  if (previous == nullptr)
  {
    arrOfPtrs[r*N +c] = new Area(array[r*N +c]);
    arrOfPtrs[r*N +c]->size++;
    fielsdCount[array[r*N +c]]++;

    if (array[r*N +c] == 0)
    {
      Area2Replace * area2repl = new Area2Replace(0);
      area2repl->indexex.insert(new Index(r, c));
      area2repl->area = arrOfPtrs[r*N +c];
      areas2replace->insert(area2repl);
    }
  }
  else
  {
    if (previous->model != array[r*N+c]) return;
    else
    {
      arrOfPtrs[r*N +c] = previous;
      arrOfPtrs[r*N +c]->size++;

      if (array[r*N +c] == 0)
      {
        for (int k = 0; k < areas2replace->size; k++)
        {
          if (previous == areas2replace->uniqueObj[k]->area)
            areas2replace->uniqueObj[k]->indexex.insert(new Index(r,c));
        }
      }
    }
  }


  // recursion for searching
  // up
  searchArea(r-1, c, N, array, arrOfPtrs, arrOfPtrs[r*N +c], areas2replace);
  //down
  searchArea(r+1, c, N, array, arrOfPtrs, arrOfPtrs[r*N +c], areas2replace);
  //left
  searchArea(r, c-1, N, array, arrOfPtrs, arrOfPtrs[r*N +c], areas2replace);
  //right
  searchArea(r, c+1, N, array, arrOfPtrs, arrOfPtrs[r*N +c], areas2replace);

}

int main(int argc, char** argv)
{
  int test_case;
  int T;

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2012\\Projects\\booths_test\\Debug\\sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;

  for (test_case = 1; test_case <= T; test_case++)
  {
    cin >> N;
    int* array = new int[N*N];
    Area** arrOfPtrs = new Area*[N*N];
    MySet<Area2Replace*> areas2replace(N*N);

    // read test case data
    for (int row = 0; row < N; ++row)
    {
      for (int col = 0; col < N; ++col)
      {
        cin >> array[row*N + col];
        //cout << array[row*N + col] << " " ;
        arrOfPtrs[row*N + col] = nullptr;
      }
      //cout << endl;
    }


    // recursion for finding common areas
    for (int row = 0; row < N; ++row)
    {
      for (int col = 0; col < N; ++col)
      {
        searchArea(row, col, N, array, arrOfPtrs, nullptr, &areas2replace);
      }
    }


    // replace zeros
    for (int i = 0; i < areas2replace.size; i++)
    {
      MySet<Area*> adajcentsAreas(N*N);
      for (int j = 0; j < areas2replace.uniqueObj[i]->indexex.size; j++)
      {
        Index curInd = *areas2replace.uniqueObj[i]->indexex.uniqueObj[j];

        // up
        if (curInd.row > 0 && array[(curInd.row - 1)*N + curInd.col] != 0)
          adajcentsAreas.insert(arrOfPtrs[(curInd.row - 1)*N + curInd.col]);

        // down
        if (curInd.row < N - 1 && array[(curInd.row + 1)*N + curInd.col] != 0)
          adajcentsAreas.insert(arrOfPtrs[(curInd.row + 1)*N + curInd.col]);

        // left
        if (curInd.col > 0 && array[(curInd.row)*N + curInd.col - 1] != 0)
          adajcentsAreas.insert(arrOfPtrs[(curInd.row)*N + curInd.col - 1]);

        // right
        if (curInd.col < N - 1 &&  array[(curInd.row)*N + curInd.col + 1] != 0)
          adajcentsAreas.insert(arrOfPtrs[(curInd.row)*N + curInd.col + 1]);
      }

      int counts[6] = {0,0,0,0,0,0};

      for (int k = 0; k < adajcentsAreas.size; k++)
      {
        counts[adajcentsAreas.uniqueObj[k]->model] += adajcentsAreas.uniqueObj[k]->size;
      }

      int replacingModel = 0;
      int maxCount = 0;

      for (int k = 1; k < 6; k++)
      {
        if (maxCount <= counts[k])
        {
          maxCount = counts[k];
          replacingModel = k;
        }
      }


      for (int j = 0; j < areas2replace.uniqueObj[i]->indexex.size; j++)
      {
        Index curInd = *areas2replace.uniqueObj[i]->indexex.uniqueObj[j];

        array[curInd.row*N + curInd.col] = replacingModel;
      }

    }


    for (int i = 0; i < 6; i++)
      fielsdCount[i] = 0;


    for (int row = 0; row < N; ++row)
    {
      for (int col = 0; col < N; ++col)
      {
        //cout << array[row*N + col] << " " ;
        arrOfPtrs[row*N + col] = nullptr;
      }
      //cout << endl;
    }



    MySet<Area2Replace*> areas2replacenull(N*N);

    // 2 recursion for finding common areas
    for (int row = 0; row < N; ++row)
    {
      for (int col = 0; col < N; ++col)
      {
        searchArea(row, col, N, array, arrOfPtrs, nullptr, &areas2replacenull);
      }
    }

    int sum = 0;
    for (int i = 1; i < 6; i++)
    {
      sum += fielsdCount[i];
    }

    // Print the answer to standard output(screen).
    cout << "#" << test_case << ": " << sum << endl;

  }

  return 0; // Your program should return 0 on normal termination.
}

