#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <stdio.h>

typedef enum{
  CMD_INIT,
  CMD_ADD,
  CMD_MODIFY,
  CMD_DELETE,
  CMD_SEARCH
} COMMAND;

typedef enum{
  SEARCH_TYPE_TITLE,
  SEARCH_TYPE_LOCATION,
  SEARCH_TYPE_DATE
} SEARCH_TYPE;

typedef struct{
  int count;
  int id_list[10000];
} RESULT;

extern void Init();
extern void Add(int id, char* title, char* location, char* start_date, char* end_date);
extern int Modify(int id, char* title, char* location, char* start_date, char* end_date);
extern int Delete(int id);
extern RESULT Search(SEARCH_TYPE type, char* str);

static int Score, ScoreIdx;

static void cmd_init(){
  Score = 1000;
  scanf("%d", &ScoreIdx);
  Init();
}

static void cmd_add(){
  int newid;
  char newTitle[20], newLocation[20], newStartDate[13], newEndDate[13];
  scanf("%d %s %s %s %s", &newid, newTitle, newLocation, newStartDate, newEndDate);
  Add(newid, newTitle, newLocation, newStartDate, newEndDate);
}

static void cmd_modify(){
  int modifyid, check, result;
  char newTitle[20], newLocation[20], newStartDate[13], newEndDate[13];
  scanf("%d %s %s %s %s %d", &modifyid, newTitle, newLocation, newStartDate, newEndDate, &check);

  result = Modify(modifyid, newTitle, newLocation, newStartDate, newEndDate);
  if (result != check)
    Score -= ScoreIdx;
}

static void cmd_delete(){
  int modifyid, check, result;
  scanf("%d %d", &modifyid, &check);

  result = Delete(modifyid);
  if (result != check)
    Score -= ScoreIdx;
}

static void cmd_search(){
  int searchType, checkCnt, check;
  char str[20];
  scanf("%d %s %d %d", &searchType, str, &checkCnt, &check);

  RESULT result = Search((SEARCH_TYPE)searchType, str);

  if (result.count != checkCnt)
    Score -= ScoreIdx;
  else{
    int checkSum = 0;
    for (int i = 0; i < result.count; ++i)
      checkSum += result.id_list[i];
    if (checkSum != check)
      Score -= ScoreIdx;
  }
}

static void run(){
  int N;
  scanf("%d", &N);
  for (int i = 0; i < N; i++){
    int cmd;
    scanf("%d", &cmd);
    switch (cmd){
    case CMD_INIT:   cmd_init();   break;
    case CMD_ADD:    cmd_add();    break;
    case CMD_MODIFY: cmd_modify(); break;
    case CMD_DELETE: cmd_delete(); break;
    case CMD_SEARCH: cmd_search(); break;
    default: break;
    }
  }
}

int main(){
  setbuf(stdout, NULL);
  freopen("C:\\Users\\Sebastian\\source\\repos\\ScheduleManager\\ScheduleManager\\sample_input.txt", "r", stdin);

  int T;
  scanf("%d", &T);

  int TotalScore = 0;
  for (int tc = 1; tc <= T; tc++){
    run();
    if (Score < 0)	Score = 0;

    TotalScore += Score;
    printf("#%d %d\n", tc, Score);
  }
  printf("Total Score = %d\n", TotalScore);

  return 0;
}