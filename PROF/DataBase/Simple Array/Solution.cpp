typedef enum
{
  NAME,
  NUMBER,
  BIRTHDAY,
  EMAIL,
  MEMO
} FIELD;

typedef struct
{
  int count;
  char str[20];
} RESULT;

struct MyVector
{
  int count;
  int indexes[50000];
};

struct Record
{
  bool valid;
  char name[20];
  char number[20];
  char birthday[20];
  char email[20];
  char memo[20];

  Record() {}
  Record(char* gname, char* gnumber, char* gbirthday, char* gemail, char* gmemo) : valid(true)
  {
    int i = 0;
    while (i < 20)
    {
      name[i] = gname[i];
      number[i] = gnumber[i];
      birthday[i] = gbirthday[i];
      email[i] = gemail[i];
      memo[i] = gmemo[i];
      i++;
    }
  }
};

Record DBTable[50000];
int lastInsertedRow;
bool equals(char* first, char* sec);
void copyStr(char* orig, char* dest);

void InitDB()
{
  for (int i = 0; i < 50000; i++)
  {
    DBTable[i].valid = false;
  }
  lastInsertedRow = 0;
}

void Add(char* name, char* number, char* birthday, char* email, char* memo)
{
  DBTable[lastInsertedRow++] = Record(name, number, birthday, email, memo);
}

int Delete(FIELD field, char* str)
{
  int deletedRows = 0;
  for (int i = 0; i < lastInsertedRow; i++)
  {
    if (!DBTable[i].valid) continue;

    switch (field)
    {
    case NAME:
      if (equals(DBTable[i].name, str))
      {
        DBTable[i].valid = false;
        deletedRows++;
      }
      break;
    case NUMBER:
      if (equals(DBTable[i].number, str))
      {
        DBTable[i].valid = false;
        deletedRows++;
      }
      break;
    case BIRTHDAY:
      if (equals(DBTable[i].birthday, str))
      {
        DBTable[i].valid = false;
        deletedRows++;
      }
      break;
    case EMAIL:
      if (equals(DBTable[i].email, str))
      {
        DBTable[i].valid = false;
        deletedRows++;
      }
      break;
    case MEMO:
      if (equals(DBTable[i].memo, str))
      {
        DBTable[i].valid = false;
        deletedRows++;
      }
      break;
    default:
      break;
    }
  }

  return deletedRows;
}

int Change(FIELD field, char* str, FIELD changefield, char* changestr)
{
  int changedRows = 0;
  MyVector result;
  result.count = 0;

  for (int i = 0; i < lastInsertedRow; i++)
  {
    if (!DBTable[i].valid) continue;

    switch (field)
    {
    case NAME:
      if (equals(DBTable[i].name, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case NUMBER:
      if (equals(DBTable[i].number, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case BIRTHDAY:
      if (equals(DBTable[i].birthday, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case EMAIL:
      if (equals(DBTable[i].email, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case MEMO:
      if (equals(DBTable[i].memo, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    default:
      break;
    }
  }

  // modify records
  for (int i = 0; i < result.count; i++)
  {
    switch (changefield)
    {
    case NAME:
      copyStr(changestr, DBTable[result.indexes[i]].name);
      break;
    case NUMBER:
      copyStr(changestr, DBTable[result.indexes[i]].number);
      break;
    case BIRTHDAY:
      copyStr(changestr, DBTable[result.indexes[i]].birthday);
      break;
    case EMAIL:
      copyStr(changestr, DBTable[result.indexes[i]].email);
      break;
    case MEMO:
      copyStr(changestr, DBTable[result.indexes[i]].memo);
      break;
    }

  }
  return result.count;
}

RESULT Search(FIELD field, char* str, FIELD returnfield)
{
  int changedRows = 0;
  MyVector result;
  result.count = 0;

  for (int i = 0; i < lastInsertedRow; i++)
  {
    if (!DBTable[i].valid) continue;

    switch (field)
    {
    case NAME:
      if (equals(DBTable[i].name, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case NUMBER:
      if (equals(DBTable[i].number, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case BIRTHDAY:
      if (equals(DBTable[i].birthday, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case EMAIL:
      if (equals(DBTable[i].email, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    case MEMO:
      if (equals(DBTable[i].memo, str))
      {
        result.indexes[result.count++] = i;
      }
      break;
    default:
      break;
    }
  }

  // preper final results
  RESULT res;
  res.count = 0;

  if (result.count == 1)
  {
    switch (returnfield)
    {
    case NAME:
      copyStr(DBTable[result.indexes[0]].name, res.str);
      break;
    case NUMBER:
      copyStr(DBTable[result.indexes[0]].number, res.str);
      break;
    case BIRTHDAY:
      copyStr(DBTable[result.indexes[0]].birthday, res.str);
      break;
    case EMAIL:
      copyStr(DBTable[result.indexes[0]].email, res.str);
      break;
    case MEMO:
      copyStr(DBTable[result.indexes[0]].memo, res.str);
      break;
    }
  }

  res.count = result.count;
  return res;
}

bool equals(char* first, char* sec)
{
  int i = 0;
  while (first[i] != 0 && sec[i] != 0)
  {
    if (first[i] != sec[i])
      return false;
    i++;
  }

  if (first[i] != sec[i])
    return false;

  return true;
}

void copyStr(char* orig, char* dest)
{
  int i = 0;
  while (orig[i] != 0)
  {
    dest[i] = orig[i];
    i++;
  }
  dest[i] = 0;
}