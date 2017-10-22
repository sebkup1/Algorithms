#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

int add(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result);
int sub(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result, bool neg);
int mul(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result, bool* neg);
int div(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result);
bool bigger(int* firstDigit, int* secDigit, int firstDigitCount);
bool equal(int* firstDigit, int* secDigit, int firstDigitCount);

int main(int argc, char** argv){

  int test_case;
  int T;

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2013\\Projects\\calc\\Debug\\calc_sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    int operation;
    cin >> operation;

    int firstDigitCount;
    cin >> firstDigitCount;

    int* firstDigit = new int[firstDigitCount];

    for (int i = firstDigitCount - 1; i >= 0; i--)
    {
      cin >> firstDigit[i];
    }

    int secDigitCount;
    cin >> secDigitCount;

    int* secDigit = new int[secDigitCount];

    for (int i = secDigitCount - 1; i >= 0; i--)
    {
      cin >> secDigit[i];
    }


    // calculating result
    int* result = new int[firstDigitCount + secDigitCount];
    bool* neg = nullptr;
    int resSize = 0;

    for (int i = 0; i < (firstDigitCount + secDigitCount); i++)
    {
      result[i] = 0;
    }

    switch (operation)
    {
    case 1:
      resSize = add(firstDigit, firstDigitCount, secDigit, secDigitCount, result);
      break;
    case 2:
      resSize = sub(firstDigit, firstDigitCount, secDigit, secDigitCount, result, neg);
      break;
    case 3:
      resSize = mul(firstDigit, firstDigitCount, secDigit, secDigitCount, result, neg);
      break;
    case 4:
      resSize = div(firstDigit, firstDigitCount, secDigit, secDigitCount, result);
      break;
    default:
      break;
    }

    bool minus = false;
    if (resSize < 0)
    {
      minus = true;
      resSize = -resSize;
    }

    //clean up
    int toCut = 0;
    for (int i = resSize - 1; i >= 0; i--){
      if (result[i] == 0 || result[i] == -1)
        toCut++;
      else
        break;
    }

    resSize -= toCut;

    // Print the answer to standard output(screen).
    cout << "#" << test_case << " ";
    if (minus) cout << "-";
    for (int i = resSize - 1; i >= 0; i--){
      cout << result[i];
    }
    if (resSize == 0) cout << 0;
    cout << endl;

    delete[] secDigit, firstDigit, result;
  }

  return 0; // Your program should return 0 on normal termination.
}

int add(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result){
  int offset = 0;
  for (int i = 0; i < firstDigitCount; i++)
  {
    result[i] += firstDigit[i];
  }
  for (int i = 0; i < secDigitCount; i++) {
    result[i] += secDigit[i];
  }

  int j = 0;
  while (true)
  {
    if (result[j] >= 10){
      result[j] = result[j] - 10;

      if (j + 1 == firstDigitCount + secDigitCount)
        offset = 1;


      result[j + 1]++;
    }
    j++;
    if (j >= (firstDigitCount + secDigitCount + offset)) break;
  }

  //clean up
  int toCut = 0;
  for (int k = j - 1; k >= 0; k--){
    if (result[k] == 0 || result[k] == -1)
      toCut++;
    else
      break;
  }

  return (j - toCut);
}

int div(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result)
{
  int* temp = nullptr;
  int size = secDigitCount;
  int times = 0;
  //result[0] = -1;

  while (true)
  {
    if (temp == nullptr)
    {
      temp = new int[firstDigitCount + secDigitCount + 1];
      for (int i = 0; i < firstDigitCount + secDigitCount + 1; i++) temp[i] = 0;
    }

    size = add(new int(0), 1, secDigit, secDigitCount, temp);

    if (size > firstDigitCount || bigger(temp, firstDigit, firstDigitCount))
    {
      if (equal(temp, firstDigit, firstDigitCount)) add(new int(1), 1, new int(0), 1, result);
      break;
    }
    add(new int(1), 1, new int(0), 1, result);
  }
  return firstDigitCount;
}

bool equal(int* firstDigit, int* secDigit, int digitCount)
{
  int pos = digitCount - 1;
  while (pos >= 0)
  {
    if (firstDigit[pos] != secDigit[pos])
    {
      return false;
    }
    pos--;
  }
  return true;
}

bool bigger(int* firstDigit, int* secDigit, int digitCount)
{
  int pos = digitCount - 1;
  while (pos >= 0)
  {
    if (firstDigit[pos] > secDigit[pos])
    {
      return true;
    }
    else if (firstDigit[pos] < secDigit[pos])
    {
      return false;
    }
    else
    {
      if (pos == 0) return false;
      pos--;
    }

  }
  return false;
}

int mul(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result, bool* neg){

  if (secDigitCount > firstDigitCount)
  {
    return mul(secDigit, secDigitCount, firstDigit, firstDigitCount, result, neg);
  }

  int pos = 0;
  int secPos = 0;
  while (pos < firstDigitCount)
  {
    while (secPos < secDigitCount)
    {
      int res = (firstDigit[pos] * secDigit[secPos]) + result[pos + secPos];

      if (res >= 10)
      {
        result[pos + secPos] = res % 10;
        result[pos + secPos + 1] = res / 10;
      }
      else
      {
        result[pos + secPos] = res;
      }
      secPos++;
    }
    pos++;
    secPos = 0;
  }

  //if (result[pos + secDigitCount] != 0)
  //{
  //  pos++;
  //}

  return firstDigitCount + secDigitCount;
}

int sub(int* firstDigit, int firstDigitCount, int* secDigit, int secDigitCount, int* result, bool neg){
  if (firstDigitCount == secDigitCount && equal(firstDigit, secDigit, firstDigitCount))
  {
    result[0] == 0;
    return 1;
  }
  if ((secDigitCount > firstDigitCount) || ((secDigitCount == firstDigitCount) && (firstDigit[firstDigitCount - 1] < secDigit[secDigitCount - 1])))
    return sub(secDigit, secDigitCount, firstDigit, firstDigitCount, result, true);

  if (secDigitCount == firstDigitCount){
    int pos = firstDigitCount - 1;
    while (firstDigit[pos] == secDigit[pos])
    {
      pos--;
      if (firstDigit[pos] < secDigit[pos])
        return sub(secDigit, secDigitCount, firstDigit, firstDigitCount, result, true);
    }
  }

  int pos = 0;
  while (pos < firstDigitCount && pos < secDigitCount)
  {
    result[pos] = firstDigit[pos] - secDigit[pos] + result[pos];
    if (result[pos] < 0) {
      result[pos] = 10 + result[pos];
      result[pos + 1]--;
    }
    pos++;
  }

  if (firstDigitCount > secDigitCount)
  {
    while (pos < firstDigitCount)
    {
      result[pos] += firstDigit[pos];
      pos++;
    }
  }

  if (neg)
    return -pos;
  else
    return pos;
}


