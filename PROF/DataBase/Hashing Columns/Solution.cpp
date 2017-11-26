#define MODULO 50000

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

  Record() = default;
};

struct HashChainLink
{
  HashChainLink* next;
  int id;

  HashChainLink()
  {
    this->next = nullptr;
    this->id = -1;
  }

  HashChainLink(HashChainLink* next, int id)
  {
    this->id = id;
    this->next = next;
  }
};

Record DBTable[50000];
HashChainLink NameHash[50000];
HashChainLink NumberHash[50000];
HashChainLink BirthdayHash[50000];
HashChainLink EmailHash[50000];
HashChainLink MemoHash[50000];

int lastInsertedRow;
bool equals(char* first, char* sec);
void copyStr(char* orig, char* dest);
void cleanHashTable(HashChainLink* hashTable);
void createRecord(int id, char* gname, char* gnumber, char* gbirthday, char* gemail, char* gmemo);
unsigned long hashFun(char *str);
void cleanHashTable(HashChainLink* hashTable);
void removeOldLinkFromHash(int hash, HashChainLink* hashTable, int mainTableIndex);
void putIntoHashTable(int hash, HashChainLink* hashTable, int mainTableIndex);

void InitDB()
{
  for (int i = 0; i < 50000; i++)
  {
    DBTable[i].valid = false;

    NameHash[i].id = -1;
    NumberHash[i].id = -1;
    BirthdayHash[i].id = -1;
    EmailHash[i].id = -1;
    MemoHash[i].id = -1;

    cleanHashTable(NameHash);
    //[i].next = nullptr;
    NumberHash[i].next = nullptr;
    BirthdayHash[i].next = nullptr;
    EmailHash[i].next = nullptr;
    MemoHash[i].next = nullptr;

  }
  lastInsertedRow = 0;
}

void Add(char* name, char* number, char* birthday, char* email, char* memo)
{
  createRecord(lastInsertedRow, name, number, birthday, email, memo);

  // name
  int hash = hashFun(name) % MODULO;
  putIntoHashTable(hash, NameHash, lastInsertedRow);

  // number
  hash = hashFun(number) % MODULO;
  putIntoHashTable(hash, NumberHash, lastInsertedRow);

  // birthday
  hash = hashFun(birthday) % MODULO;
  putIntoHashTable(hash, BirthdayHash, lastInsertedRow);

  // email
  hash = hashFun(email) % MODULO;
  putIntoHashTable(hash, EmailHash, lastInsertedRow);

  // memo
  hash = hashFun(memo) % MODULO;
  putIntoHashTable(hash, MemoHash, lastInsertedRow);

  lastInsertedRow++;
}

int Delete(FIELD field, char* str)
{
  int deletedRows = 0;
  MyVector result;
  result.count = 0;

  int hash = hashFun(str) % MODULO;
  HashChainLink* link = nullptr;

  switch (field)
  {
  case NAME:
    link = &NameHash[hash];
    break;
  case NUMBER:
    link = &NumberHash[hash];
    break;
  case BIRTHDAY:
    link = &BirthdayHash[hash];
    break;
  case EMAIL:
    link = &EmailHash[hash];
    break;
  case MEMO:
    link = &MemoHash[hash];
    break;
  }

  char* tempField = 0;
  while (link != nullptr)
  {
    switch (field)
    {
    case NAME:
      tempField = DBTable[link->id].name;
      break;
    case NUMBER:
      tempField = DBTable[link->id].number;
      break;
    case BIRTHDAY:
      tempField = DBTable[link->id].birthday;
      break;
    case EMAIL:
      tempField = DBTable[link->id].email;
      break;
    case MEMO:
      tempField = DBTable[link->id].memo;
      break;
    }

    if (equals(tempField, str))
    {
      if (DBTable[link->id].valid)
        deletedRows++;

      DBTable[link->id].valid = false;

      link->id = -1;
    }
    link = link->next;
  }

  return deletedRows;
}

int Change(FIELD field, char* str, FIELD changefield, char* changestr)
{
  int changedRows = 0;
  MyVector result;
  result.count = 0;

  int hash = hashFun(str) % MODULO;
  HashChainLink* link = nullptr;

  switch (field)
  {
  case NAME:
    link = &NameHash[hash];
    break;
  case NUMBER:
    link = &NumberHash[hash];
    break;
  case BIRTHDAY:
    link = &BirthdayHash[hash];
    break;
  case EMAIL:
    link = &EmailHash[hash];
    break;
  case MEMO:
    link = &MemoHash[hash];
    break;
  }

  char* tempField = 0;
  while (link != nullptr)
  {
    switch (field)
    {
    case NAME:
      tempField = DBTable[link->id].name;
      break;
    case NUMBER:
      tempField = DBTable[link->id].number;
      break;
    case BIRTHDAY:
      tempField = DBTable[link->id].birthday;
      break;
    case EMAIL:
      tempField = DBTable[link->id].email;
      break;
    case MEMO:
      tempField = DBTable[link->id].memo;
      break;
    }

    if (link->id != -1 && DBTable[link->id].valid && equals(tempField, str))
    {
      result.indexes[result.count++] = link->id;
    }

    link = link->next;
  }

  int newHash = hashFun(changestr) % MODULO;
  HashChainLink* oldLink = nullptr;

  // modify records
  for (int i = 0; i < result.count; i++)
  {
    Record* record = &DBTable[result.indexes[i]];

    int oldHash;
    switch (changefield)
    {
    case NAME:
      oldHash = hashFun(record->name) % MODULO;
      copyStr(changestr, record->name);
      removeOldLinkFromHash(oldHash, NameHash, result.indexes[i]);
      putIntoHashTable(newHash, NameHash, result.indexes[i]);
      break;
    case NUMBER:
      oldHash = hashFun(record->number) % MODULO;
      copyStr(changestr, record->number);
      removeOldLinkFromHash(oldHash, NumberHash, result.indexes[i]);
      putIntoHashTable(newHash, NumberHash, result.indexes[i]);
      break;
    case BIRTHDAY:
      oldHash = hashFun(record->birthday) % MODULO;
      copyStr(changestr, record->birthday);
      removeOldLinkFromHash(oldHash, BirthdayHash, result.indexes[i]);
      putIntoHashTable(newHash, BirthdayHash, result.indexes[i]);
      break;
    case EMAIL:
      oldHash = hashFun(record->email) % MODULO;
      copyStr(changestr, record->email);
      removeOldLinkFromHash(oldHash, EmailHash, result.indexes[i]);
      putIntoHashTable(newHash, EmailHash, result.indexes[i]);
      break;
    case MEMO:
      oldHash = hashFun(record->memo) % MODULO;
      copyStr(changestr, record->memo);
      removeOldLinkFromHash(oldHash, MemoHash, result.indexes[i]);
      putIntoHashTable(newHash, MemoHash, result.indexes[i]);
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

  int hash = hashFun(str) % MODULO;
  HashChainLink* link = nullptr;

  switch (field)
  {
  case NAME:
    link = &NameHash[hash];
    break;
  case NUMBER:
    link = &NumberHash[hash];
    break;
  case BIRTHDAY:
    link = &BirthdayHash[hash];
    break;
  case EMAIL:
    link = &EmailHash[hash];
    break;
  case MEMO:
    link = &MemoHash[hash];
    break;
  }

  char* tempField = 0;
  if (link->id != -1)
  {
    while (link != nullptr)
    {
      switch (field)
      {
      case NAME:
        tempField = DBTable[link->id].name;
        break;
      case NUMBER:
        tempField = DBTable[link->id].number;
        break;
      case BIRTHDAY:
        tempField = DBTable[link->id].birthday;
        break;
      case EMAIL:
        tempField = DBTable[link->id].email;
        break;
      case MEMO:
        tempField = DBTable[link->id].memo;
        break;
      }

      if (equals(tempField, str))
      {
        if (link->id != -1 && DBTable[link->id].valid)
          result.indexes[result.count++] = link->id;
      }

      link = link->next;
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

void putIntoHashTable(int hash, HashChainLink* hashTable, int mainTableIndex)
{
  HashChainLink* link = &hashTable[hash];
  HashChainLink* prevLink = nullptr;

  if (link->id == -1 && link->next == nullptr)
  {
    link->id = mainTableIndex;
    link->next = nullptr;
  }
  else
  {
    while (link != nullptr)
    {
      prevLink = link;
      link = link->next;
    }

    prevLink->next = new HashChainLink(nullptr, mainTableIndex);
  }
}

void removeOldLinkFromHash(int hash, HashChainLink* hashTable, int mainTableIndex)
{
  HashChainLink* link = &hashTable[hash];

  while (link != nullptr)
  {
    if (link->id == mainTableIndex)
    {
      link->id = -1;
      break;
    }
    link = link->next;
  }
}

void cleanHashTable(HashChainLink* hashTable)
{
  HashChainLink* link = &hashTable[0];
  HashChainLink* prevLink = nullptr;

  while (link != nullptr)
  {
    prevLink = link;
    link = link->next;
    if (prevLink != &hashTable[0])
      delete prevLink;
  }

  hashTable[0].next = nullptr;
}

void createRecord(int id, char* gname, char* gnumber, char* gbirthday, char* gemail, char* gmemo)
{
  DBTable[id].valid = true;
  int i = 0;
  while (i < 20)
  {
    DBTable[id].name[i] = gname[i];
    DBTable[id].number[i] = gnumber[i];
    DBTable[id].birthday[i] = gbirthday[i];
    DBTable[id].email[i] = gemail[i];
    DBTable[id].memo[i] = gmemo[i];
    i++;
  }
}

unsigned long hashFun(char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % MODULO;
}

