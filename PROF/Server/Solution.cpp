#define MAX_ACCOUNT 50000

struct Account
{
  bool logged;
  Account* next;
  int defaultTime;
  int remainingTime;
  char password[11];
  char id[11];
};

Account ACCOUNTS[MAX_ACCOUNT];
int hashes[MAX_ACCOUNT];
int accountsCount;
int lowestAccount;
int colisions = 0;

int hash(char*);
bool compareStrs(char*, char*);

void Init()
{
  accountsCount = 0;
  lowestAccount = 0;
  for (int i = 0; i < MAX_ACCOUNT; i++)
  {
    ACCOUNTS[i].logged = false;
    ACCOUNTS[i].next = nullptr;
    ACCOUNTS[accountsCount].remainingTime = 0;
    ACCOUNTS[accountsCount].defaultTime = 0;
    hashes[i] = -1;
  }

}

void NewAccount(char id[11], char password[11], int defaulttime)
{
  ACCOUNTS[accountsCount].defaultTime = defaulttime;
  ACCOUNTS[accountsCount].remainingTime = defaulttime;
  ACCOUNTS[accountsCount].logged = true;

  int i = 0;
  while (password[i] != 0)
  {
    ACCOUNTS[accountsCount].password[i] = password[i];
    i++;
  }
  ACCOUNTS[accountsCount].password[i] = 0;

  i = 0;
  while (id[i] != 0)
  {
    ACCOUNTS[accountsCount].id[i] = id[i];
    i++;
  }
  ACCOUNTS[accountsCount].id[i] = 0;

  int hashI = hash(id);
  if (hashes[hashI] != -1)
  {
    colisions++;
    Account* it = &ACCOUNTS[hashes[hashI]];
    while (it->next != nullptr)
    {
      it = it->next;
    }
    it->next = &ACCOUNTS[accountsCount];
  }
  else
  {
    hashes[hashI] = accountsCount;
  }

  accountsCount++;
}

void Logout(char id[11])
{
  int hashI = hash(id);

  Account* it = &ACCOUNTS[hashes[hashI]];
  while (it != nullptr)
  {
    if (compareStrs(id, it->id))
    {
      it->logged = false;
      return;
    }
    it = it->next;
  }

}

void Connect(char id[11], char password[11])
{
  int hashI = hash(id);

  Account* it = &ACCOUNTS[hashes[hashI]];
  while (it != nullptr)
  {
    if (compareStrs(id, it->id) && compareStrs(password, it->password))
    {
      if (!it->logged)
        return;

      it->remainingTime = it->defaultTime;
      return;
    }

    it = it->next;
  }

}

int Tick()
{
  int logggedOutAccounts = 0;

  for (int i = lowestAccount; i < accountsCount; i++)
  {
    if (ACCOUNTS[i].logged)
    {
      ACCOUNTS[i].remainingTime--;
      if (ACCOUNTS[i].remainingTime == 0)
      {
        ACCOUNTS[i].logged = false;
        ACCOUNTS[i].id[0] = 0;
        logggedOutAccounts++;
      }
    }
    if (!ACCOUNTS[i].logged && lowestAccount + 1 == i) //bez tego jest chyba trochê szybciej
      lowestAccount = i;
  }

  return logggedOutAccounts;
}

int hash(char* str)
{
  long long  hash = 5381;
  int i = 0;
  while (str[i] != 0)
  {
    hash = ((hash << 5) + hash) + str[i];
    i++;
  }

  if (hash < 0)
    hash = -hash;

  hash %= (MAX_ACCOUNT);

  return hash;
}

bool compareStrs(char* first, char* sec)
{
  int i = 0;
  while (first[i] != 0 && sec[i] != 0)
  {
    if (first[i] != sec[i]) return false;
    i++;
  }

  if (first[i] != sec[i]) return false;

  return true;
}