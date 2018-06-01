typedef struct
{
  int y, x;
}AXIS;

struct Memo {
  unsigned short int x, y, width, height, textSize, stackPos;
  char text[2510];
};

Memo memosList[10001];

struct stackElem {
  int memoInd = -1;
  bool valid = false;
};

stackElem memoStack[30000];
unsigned int begin, end;
bool donePixels[5][5];
int screenSize;

void moveMemoOfTop(int mId) {
  memoStack[memosList[mId].stackPos].valid = false;
  memoStack[end].valid = true;
  memoStack[end].memoInd = mId;
  end++;
}

char getMemoChar(Memo* memo, int x, int y) {
  int diffX = x - memo->x;
  int diffY = y - memo->y;
  return memo->text[diffY*memo->width + diffX];
}

void init(int mScreenSize) {
  screenSize = mScreenSize;
  begin = 0;
  end = 0;
}

void create_memo(int mId, int mY, int mX, int mHeight, int mWidth, char str[]) {
  memosList[mId].y = mY;
  memosList[mId].x = mX;
  memosList[mId].height = mHeight;
  memosList[mId].width = mWidth;
  memosList[mId].stackPos = end;
  int i = 0;
  while (str[i] != 0) {
    memosList[mId].text[i] = str[i];
    i++;
  }
  if (memosList[mId].textSize > 0 && memosList[mId].textSize <= 2510)
    while (i <= memosList[mId].textSize) {
      memosList[mId].text[i] = '\0';
      i++;
    }
  memosList[mId].textSize = i;
  memoStack[end].memoInd = mId;
  memoStack[end].valid = true;

  end++;
}

AXIS select_memo(int mId){
  AXIS axis;
  axis.x = memosList[mId].x;
  axis.y = memosList[mId].y;
  moveMemoOfTop(mId);
  return axis;
}

void move_memo(int mId, int mY, int mX) {
  memosList[mId].x = mX;
  memosList[mId].y = mY;
  moveMemoOfTop(mId);
}

void modify_memo(int mId, int mHeight, int mWidth, char str[]) {
  memosList[mId].height = mHeight;
  memosList[mId].width = mWidth;
  int i = 0;
  while (str[i] != 0) {
    memosList[mId].text[i] = str[i];
    i++;
  }
  while (i <= memosList[mId].textSize) {
    memosList[mId].text[i] = '\0';
    i++;
  }
  memosList[mId].textSize = i;

  moveMemoOfTop(mId);
}

void get_screen_context(int mY, int mX, char res[5][5]){
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      res[i][j] = '\0';
      donePixels[i][j] = false;
    }
  }

  int remainingPixCount = 25;
  int stackIt = end - 1;
  while (remainingPixCount > 0 && stackIt != -1) {
    if (memoStack[stackIt].valid == false) {
      stackIt--;
      continue;
    }

    int memoId = memoStack[stackIt].memoInd;
    Memo* memo = &memosList[memoId];

    for (int x = memo->x; x < (memo->x + memo->width); x++){  // bezwzglêdne indeksy
      for (int y = memo->y; y < (memo->y + memo->height); y++) {

        if (x >= mX && y >= mY && x < mX + 5 && y < mY + 5 && !donePixels[y - mY][x - mX]) {
          // rzutowanie wspórzêdnych
          res[y - mY][x - mX] = getMemoChar(memo, x, y);
          donePixels[y - mY][x - mX] = true;
          remainingPixCount--;
        }
      }
    }
    stackIt--;
  }
}