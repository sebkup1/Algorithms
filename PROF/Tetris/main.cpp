#define _CRT_SECURE_NO_WARNINGS	// Ignore build errors when using unsafe functions in Visual Studio.
#include <stdio.h>

#define BOARD_MAX_WIDTH     10
#define BLOCK_MAX_SIZE      3
#define CMD_ROTATE          100
#define CMD_MOVE            200

extern void init(int width);
extern void newBlock(int block[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE], int width, int height);
extern void rotate(int angle);
extern void move(int distance);
extern int land();

int main(void)
{
  setbuf(stdout, NULL);

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2013\\Projects\\Tetris\\Tetris\\sample_input.txt", "r", stdin);

  int testCnt;
  double totalScore = 0.0;
  int block[BLOCK_MAX_SIZE][BLOCK_MAX_SIZE];

  scanf("%d", &testCnt);
  for (int tc = 1; tc <= testCnt; ++tc)
  {
    int boardWidth, blockCnt;
    scanf("%d %d", &boardWidth, &blockCnt);

    int score = blockCnt;

    init(boardWidth);

    for (int i = 0; i < blockCnt; ++i) {
      for (int h = 0; h < BLOCK_MAX_SIZE; ++h) {
        for (int w = 0; w < BLOCK_MAX_SIZE; ++w) {
          block[h][w] = 0;
        }
      }

      int width, height;
      scanf("%d %d", &width, &height);

      for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
          scanf("%d", &block[h][w]);
        }
      }

      newBlock(block, width, height);

      int cmdCnt, cmd, option;
      scanf("%d", &cmdCnt);
      for (int j = 0; j < cmdCnt; ++j) {
        scanf("%d %d", &cmd, &option);

        switch (cmd) {
        case CMD_ROTATE:
          rotate(option);
          break;
        case CMD_MOVE:
          move(option);
          break;
        default:
          printf("cmd error\n");
          break;
        }
      }

      int answer, result;
      scanf("%d", &answer);
      result = land();
      if (answer != result) {
        --score; // wrong answer
      }
    }

    printf("#%d %d\n", tc, score);
    totalScore += (double)score / blockCnt * 100;
    if (tc == 2)
    {
      printf("");
    }
  }

  printf("Total Score : %.3lf\n", totalScore / testCnt);

  return 0;
}