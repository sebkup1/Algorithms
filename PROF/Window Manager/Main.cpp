#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define MAX_WIN 500
#define WIDTH   160
#define HEIGHT  120

typedef enum
{
  CMD_INIT,
  CMD_CREATE,
  CMD_DESTROY,
  CMD_FILLRECT,
  CMD_ACTIVE,
  CMD_CLICK
} COMMAND;

static int Score, ScoreIdx;
static int UpdataCheck;
static int CheckIdx, CheckSum;
static int mHandle[MAX_WIN];

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern void InitWinManager(char bgcolor);
extern int  CreateWindow(int parent, int x, int y, int width, int height, char bgcolor);
extern void DestroyWindow(int handle);
extern void FillRect(int handle, int x, int y, int width, int height, char color);
extern void ActiveWindow(int handle);
extern void Click(int x, int y);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void DisplayUpdate(char dispbuf[HEIGHT][WIDTH])
{
  int sum = 0;

  for (int i = 0; i < HEIGHT; i++)
    for (int k = 0; k < WIDTH; k++)
      sum += dispbuf[i][k];

  int check = sum % CheckIdx;
  if (check != CheckSum)
    Score -= ScoreIdx;

  UpdataCheck = 1;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static void init()
{
  Score = 1000;

  for (int i = 0; i < MAX_WIN; i++)
    mHandle[i] = -1;
}

static void cmd_init()
{
  int bgcolor;
  scanf("%d", &bgcolor);

  InitWinManager((char)bgcolor);
}

static void cmd_create()
{
  int parent, h, x, y, width, height, bgcolor;
  scanf("%d %d %d %d %d %d %d", &parent, &h, &x, &y, &width, &height, &bgcolor);

  if (parent != -1)
    parent = mHandle[parent];

  mHandle[h] = CreateWindow(parent, x, y, width, height, (char)bgcolor);
}

static void cmd_destroy()
{
  int h;
  scanf("%d", &h);

  DestroyWindow(mHandle[h]);
  mHandle[h] = -1;
}

static void cmd_fillrect()
{
  int h, x, y, width, height, color;
  scanf("%d %d %d %d %d %d", &h, &x, &y, &width, &height, &color);

  FillRect(mHandle[h], x, y, width, height, (char)color);
}

static void cmd_active()
{
  int h;
  scanf("%d", &h);

  ActiveWindow(mHandle[h]);
}

static void cmd_click()
{
  int x, y;
  scanf("%d %d", &x, &y);

  Click(x, y);
}

static void run()
{
  int N;
  scanf("%d", &N);
  ScoreIdx = Score / N;
  if (ScoreIdx * N < Score) ScoreIdx++;

  for (int i = 0; i < N; i++)
  {
    int cmd;
    scanf("%d %d %d", &cmd, &CheckIdx, &CheckSum);

    UpdataCheck = 0;
    switch (cmd)
    {
    case CMD_INIT:     cmd_init(); break;
    case CMD_CREATE:   cmd_create(); break;
    case CMD_DESTROY:  cmd_destroy(); break;
    case CMD_FILLRECT: cmd_fillrect(); break;
    case CMD_ACTIVE:   cmd_active(); break;
    case CMD_CLICK:    cmd_click(); break;
    default: break;
    }

    if (UpdataCheck == 0)
      Score -= (ScoreIdx * 10);
  }
}

int main()
{
  setbuf(stdout, NULL);
  freopen("sample_input.txt", "r", stdin);

  int T;
  scanf("%d", &T);

  int TotalScore = 0;
  for (int tc = 1; tc <= T; tc++)
  {
    init();

    run();

    if (Score < 0)
      Score = 0;

    TotalScore += Score;
    printf("#%d %d\n", tc, Score);
  }
  printf("TotalScore = %d\n", TotalScore);

  return 0;
}
