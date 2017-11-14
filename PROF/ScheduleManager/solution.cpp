typedef enum{
  SEARCH_TYPE_TITLE,
  SEARCH_TYPE_LOCATION,
  SEARCH_TYPE_DATE
} SEARCH_TYPE;

typedef struct{
  int count;
  int id_list[10000];
} RESULT;

struct MyVector{
  int count;
  int indexes_list[10000];
};

long long dateToLongLong(char* date);
bool equals(char* bigger, char* smaler);
void update(int id, char* title, char* location, long long startDate, long long endDate);

struct Schedule
{
  int id;
  char title[20];
  char location[20];
  long long startDate;
  long long endDate;

  Schedule() {};
  Schedule(int ID, char* tit, char* loc, char* start_date, char* end_date) : id(ID)
  {
    int i = 0;
    while (tit[i] != 0)
    {
      title[i] = tit[i];
      i++;
    }
    title[i] = 0;

    i = 0;
    while (loc[i] != 0)
    {
      location[i] = loc[i];
      i++;
    }
    location[i] = 0;

    startDate = dateToLongLong(start_date);
    endDate = dateToLongLong(end_date);
  }

  Schedule(int ID, char* tit, char* loc, long long  start_date, long long end_date) : id(ID)
  {
    int i = 0;
    while (tit[i] != 0)
    {
      title[i] = tit[i];
      i++;
    }
    title[i] = 0;

    i = 0;
    while (loc[i] != 0)
    {
      location[i] = loc[i];
      i++;
    }
    location[i] = 0;

    startDate = start_date;
    endDate = end_date;
  }
};

Schedule ScheduleTable[10000];
int lastAddedSchedule;

void Init() {
  for (int i = 0; i < 10000; i++)
  {
    ScheduleTable[i].id = -1;
  }
  lastAddedSchedule = 0;
}

void Add(int id, char* title, char* location, char* start_date, char* end_date) {

  ScheduleTable[lastAddedSchedule++] = Schedule(id, title, location, start_date, end_date);

  update(id, title, location, ScheduleTable[lastAddedSchedule-1].startDate, ScheduleTable[lastAddedSchedule-1].endDate);

}

int Modify(int id, char* title, char* location, char* start_date, char* end_date) {

  int i;
  for (i = 0; i < lastAddedSchedule; i++)
  {
    if (ScheduleTable[i].id == id)
    {
      ScheduleTable[i] = Schedule(id, title, location, start_date, end_date);
      update(id, title, location, ScheduleTable[i].startDate, ScheduleTable[i].endDate);
      return 0;
    }
  }

  return 1;
}

void update(int id, char* title, char* location, long long s2start, long long s2end)
{
  MyVector toMerge; // with list of indexes
  toMerge.count = 0;

  for (int i = 0; i < lastAddedSchedule; i++)
  {
    if ((equals(ScheduleTable[i].title, title) && equals(ScheduleTable[i].location, location)) && 
      (
      ScheduleTable[i].startDate == s2start ||
      ScheduleTable[i].startDate == s2end ||
      ScheduleTable[i].endDate == s2start ||
      ScheduleTable[i].endDate == s2end ||
      // 1: ScheduleTable[i] - S1;
      (ScheduleTable[i].startDate < s2start && (ScheduleTable[i].endDate > s2start ||
      // 3:
      s2end < ScheduleTable[i].endDate)) ||
      // 2: 
      (ScheduleTable[i].startDate > s2start && (s2end > ScheduleTable[i].startDate ||
      // 4;
      s2end > ScheduleTable[i].endDate))
      ))
    {
      toMerge.indexes_list[toMerge.count++] = i;
    }
  }

  if (toMerge.count > 1)
  {
    long long start = 201912312359;
    long long end = 0;
    int lowestId = ScheduleTable[toMerge.indexes_list[0]].id;
    int lowestIndex = toMerge.indexes_list[0];

    for (int i = 0; i < toMerge.count; i++)
    {
      if (ScheduleTable[toMerge.indexes_list[i]].startDate < start)
        start = ScheduleTable[toMerge.indexes_list[i]].startDate;
      if (ScheduleTable[toMerge.indexes_list[i]].endDate > end)
        end = ScheduleTable[toMerge.indexes_list[i]].endDate;
    }

    ScheduleTable[lowestIndex] = Schedule(lowestId, title, location, start, end);

    // delete rest (id = -1)
    for (int i = 1; i < toMerge.count; i++)
    {
      ScheduleTable[toMerge.indexes_list[i]].id = -1;
    }

  }
}


int Delete(int id) {

  for (int i = 0; i < lastAddedSchedule; i++)
  {
    if (ScheduleTable[i].id == id)
    {
      ScheduleTable[i].id = -1;
      return 0;
    }
  }

  return 1;
}

RESULT Search(SEARCH_TYPE type, char* str){

  RESULT result;
  result.count = 0;

  int relatedDate;

  if (type == SEARCH_TYPE_DATE)
    relatedDate = dateToLongLong(str);


  for (int i = 0; i < lastAddedSchedule; i++)
  {
    if (ScheduleTable[i].id == -1) continue;

    switch (type)
    {
    case SEARCH_TYPE_TITLE:
      if (equals(ScheduleTable[i].title, str))
        result.id_list[result.count++] = ScheduleTable[i].id;
      break;
    case SEARCH_TYPE_LOCATION:
      if (equals(ScheduleTable[i].location, str))
        result.id_list[result.count++] = ScheduleTable[i].id;
      break;
    case SEARCH_TYPE_DATE:
    {
      if (ScheduleTable[i].startDate / 10000 <= relatedDate && ScheduleTable[i].endDate / 10000 >= relatedDate)
      {
        result.id_list[result.count++] = ScheduleTable[i].id;
      }
    }
    break;
    }
  }

  return result;
}


// helpers
long long dateToLongLong(char* date)
{
  long long ret = date[0] - '0';
  int i = 1;
  while (date[i] >= '0' && date[i] <= '9')
  {
    ret *= 10;
    ret += date[i] - '0';
    i++;
  }
  return ret;
}

bool equals(char* stored, char* question)
{
  int i = 0;
  while (stored[i] != 0)
  {
    if (stored[i] != question[i])
    {
      return false;
    }
    i++;
  }

  if (stored[i] != question[i])
  {
    return false;
  }

  return true;
}