#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
using namespace std;

enum COMMAND {
  CMD_ADD = 1,
  CMD_MOVE,
  CMD_INFECT,
  CMD_RECOVER,
  CMD_REMOVE
};

extern void init();
extern int add(int id, int pid, int fileSize);
extern int move(int id, int pid);
extern int infect(int id);
extern int recover(int id);
extern int remove(int id);

int run() {
  int score = 0;
  int N;
  scanf("%d", &N);

  for (int i = 0; i < N; i++) {
    int cmd;
    int ret;
    scanf("%d", &cmd);
    int myId;

    switch (cmd) {
    case CMD_ADD: {
                    int id, pid, fileSize;
                    scanf("%d%d%d", &id, &pid, &fileSize);
                    ret = add(id, pid, fileSize);
                    break;
    }
    case CMD_MOVE: {
                     int id, pid;
                     scanf("%d%d", &id, &pid);
                     ret = move(id, pid);
                     break;
    }
    case CMD_INFECT: {
                       int id;
                       scanf("%d", &id);
                       myId = id;
                       ret = infect(id);
                       break;
    }
    case CMD_RECOVER: {
                        int id;
                        scanf("%d", &id);
                        ret = recover(id);
                        break;
    }
    case CMD_REMOVE: {
                       int id;
                       scanf("%d", &id);
                       ret = remove(id);
                       break;
    }
    }

    int checkSum;
    scanf("%d", &checkSum);
    if (ret == checkSum)
    {
      score++;
    }
    else
    {
      printf("");
    }
  }
  return score;
}

int main() {
  setbuf(stdout, NULL);
  freopen("C:\\Users\\s.kupis\\Documents\\Visual Studio 2013\\Projects\\AntiVir\\AntiVir\\sample_input.txt", "r", stdin);
  int totalScore = 0;

  int T;
  scanf("%d", &T);
  for (int t = 1; t <= T; t++) {
    init();
    int score = run();
    printf("#%d %d\n", t, score);
    totalScore += score;
  }
  printf("Total Score : %d\n", totalScore);

  return 0;
}