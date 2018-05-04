#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CMD_MOVECURSOR (0)
#define CMD_REVERT (1)
#define CMD_INPUT (2)
#define CMD_GETSUBSTRING (9)

int N;
static int score, curScore;

extern void init();
extern void cmd_input(char ch[]);
extern void cmd_revert();
extern void cmd_movecursor(int mCursor);
extern void get_substring(int mPosition, int mLength, char res[]);


void make_input()
{
  char ch[30];

  scanf("%s", ch);

  cmd_input(ch);
}

static void run() {

  char ori[35], res[35];
  scanf("%d", &N);

  init();

  for (int i = 0; i < N; i++) {
    int cmd;

    scanf("%d", &cmd);

    if (cmd == CMD_INPUT)
    {
      make_input();
    }
    else if (cmd == CMD_REVERT)
    {
      cmd_revert();
    }
    else if (cmd == CMD_MOVECURSOR)
    {
      int pos;

      scanf("%d", &pos);

      cmd_movecursor(pos);
    }
    else if (cmd == CMD_GETSUBSTRING)
    {
      int pos, len;

      scanf("%d%d%s", &pos, &len, ori);

      get_substring(pos, len, res);
      if (strcmp(ori, res) != 0)
      {
        curScore = 0;
      }
    }
  }
}

int main() {
  setbuf(stdout, NULL);
  freopen("C:\\Users\\Sebastian\\source\\repos\\TextEditor\\TextEditor\\sample_input.txt", "r", stdin);

  int T;
  scanf("%d", &T);
  score = 0;

  for (int test_case = 1; test_case <= T; test_case++) {
    curScore = 100;
    run();

    printf("#%d %d\n", test_case, curScore);
    score += curScore;

  }

  printf("Total Score = %d\n", score / T);
  return 0;
}
