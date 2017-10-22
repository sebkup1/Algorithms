#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

struct Block {

  int startIndex;
  char type;
  Block* prev;
  bool couldBeChanged;

  Block(Block* prev, int startIndex, char type, bool couldBeChanged) : startIndex(startIndex), prev(prev), /*next(next),*/ type(type), couldBeChanged(couldBeChanged)
  {};
};

int calculateCost(char* data, int length, int costOfMan, int costOfUse);
bool tryMovePrevRecursively(Block* block, int blockSize, char* data, int length);
void deleteBlocks(Block* block);

int main(int argc, char** argv)
{
  int test_case;
  int T;
  int length;
  int costOfMan;
  int costOfUse;
  char* data = nullptr;

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2013\\Projects\\Bloki\\Bloki\\input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++) {

    cin >> length;

    if (data != nullptr) {

      delete[] data;
      data = nullptr;
    }

    data = new char[length];

    cin >> costOfMan;
    cin >> costOfUse;

    for (int i = 0; i < length; ++i)
    {
      cin >> data[i];
    }

    cout << "#" << test_case << " " << calculateCost(data, length, costOfMan, costOfUse) << endl;
  }

  return 0; // Your program should return 0 on normal termination.
}


int calculateCost(char* data, int length, int costOfMan, int costOfUse){

  int minCost = -1;
  int cost;
  int blocksCount;
  bool valid = true;

  for (int blockSize = 1; valid; blockSize++){

    blocksCount = 0;

    //put first 
    int i = 0;
    while (data[i] == '*') {

      i++;
    }

    char curChar = data[i];

    i = 0;
    // first block
    Block* first = new Block(nullptr, 0, curChar, false);
    Block* cur = first;
    blocksCount++;
    Block* prev = nullptr;

    //try put next
    while (i < length && valid){

      if (i != 0) {

        if (data[i] == '*') {

          cur = new Block(prev, i, curChar, true);
        }
        else {

          cur = new Block(prev, i, data[i], false);
          curChar = data[i];
        }
        blocksCount++;
      }

      // validate new block
      int curStart = i;
      while (true){

        // is overlength
        if (curStart + blockSize - 1 >= length) {

          if (cur->couldBeChanged){

            int j = i;
            while (data[j] == '*' && j < length) {

              j++;
            }

            if (j < length){

              cur->type = data[j];
              cur->couldBeChanged = false;
              continue;
            }

          }

          if (tryMovePrevRecursively(cur, blockSize, data, length)) {   // move by one

            curStart = i = cur->startIndex;
            curChar = cur->type;
            continue;
          }
          else {

            valid = false;
            break;
          }

        }

        // is data ok
        if ((data[i] == '*' || data[i] == curChar)){

          i++;

          if (i == (curStart + blockSize) || i == length)
            break;
        }
        else {

          if (cur->couldBeChanged){

            curChar = data[i];
            cur->type = data[i];
          }
          else {

            if (tryMovePrevRecursively(cur, blockSize, data, length)) {   // move by one

              curStart = i = cur->startIndex;
              curChar = cur->type;
              continue;
            }
            else {

              valid = false;
              break;
            }

          }

        }

      }

      prev = cur;
    }

    if (valid){

      cost = costOfMan*blockSize + costOfUse*blocksCount;

      if (cost < minCost || minCost < 0)
        minCost = cost;
    }

    deleteBlocks(cur);
  }

  return minCost;
}


bool tryMovePrevRecursively(Block* block, int blockSize, char* data, int length){

  if (block->prev == nullptr){

    return false;
  }

  Block* cur = block;
  Block* prev = cur->prev;

  while (true){

    if (cur->type == prev->type) {

      if (prev->startIndex < cur->startIndex - 1){

        goto checkIftillCovers;
      }
      else{

        if (tryMovePrevRecursively(prev, blockSize, data, length)){

          goto checkIftillCovers;
        }
        else {

          return false;
        }
      }
    }
    else{

      if (tryMovePrevRecursively(prev, blockSize, data, length)){

        goto checkIftillCovers;
      }
      else {

        return false;
      }
    }
  }

checkIftillCovers:

  if (cur->couldBeChanged){

    cur->startIndex--;

    if (cur->prev->startIndex + blockSize >= cur->startIndex)
      cur->couldBeChanged = false;

    return true;
  }

  if (data[cur->startIndex - 1 + blockSize] != cur->type || cur->startIndex - 1 + blockSize >= length){

    cur->startIndex--;
    return true;
  }
  else{

    return false;
  }

}

void deleteBlocks(Block* block) {

  if (block->prev != nullptr)
    deleteBlocks(block->prev);

  delete block;
}