#define MAX_FILE_SYSTEM 10000


struct Item
{
  int id;
  int pid;
  int size;
  int origSize;
};

struct File : Item
{
  File(int id, int pid, int size)
  {
    this->id = id;
    this->pid = pid;
    this->size = size;
    origSize = size;
  }

};

struct Directory : Item
{
  Directory(int id, int pid)
  {
    this->id = id;
    this->pid = pid;
    this->size = 0;
  }
};

Item fileSystem[10000];
Directory* root = nullptr;
int fileSystemEnd;


Item* getItem(int id)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].id == id)
      return &fileSystem[i];
  }
}

int getItemIndex(int id)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].id == id)
      return i;
  }
}


int countSize(int pid)
{
  int sum = 0;
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].pid == pid)
    {
      if (fileSystem[i].size > 0)
      {
        sum += fileSystem[i].size;
      }
      else
      {
        sum += countSize(fileSystem[i].id);
      }
    }
  }
  return sum;
}

int countFiles()
{
  int sum = 0;
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].size > 0)
    {
      sum++;
    }
  }
  return sum;
}

void infectDirectory(Item* item, int curFileSystemSize, int curFilesCount)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].pid == item->id)
    {
      if (fileSystem[i].size > 0)
      {
        fileSystem[i].size += curFileSystemSize / curFilesCount;
      }
      else
      {
        infectDirectory(&fileSystem[i], curFileSystemSize, curFilesCount);
      }
    }
  }
}

void recoverDirectory(Item* item)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].pid == item->id)
    {
      if (fileSystem[i].size > 0)
      {
        fileSystem[i].size = fileSystem[i].origSize;
      }
      else
      {
        recoverDirectory(&fileSystem[i]);
      }
    }
  }
}

void removeDirectory(int id)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].pid == id)
    {
      if (fileSystem[i].size > 0)
      {
        fileSystem[i].id = -1;
      }
      else
      {
        // bug którego d³ugo szuka³em: removeDirectory(i)
        removeDirectory(fileSystem[i].id);
      }
    }
  }
}

//=============================================================
//to implement
void init(){

  fileSystemEnd = 0;

  for (int i = 0; i < MAX_FILE_SYSTEM; i++) fileSystem[i].id = -1;

  // root
  fileSystem[fileSystemEnd++] = Directory(10000, -1);

}

int add(int id, int pid, int fileSize){

  if (fileSize > 0) {
    fileSystem[fileSystemEnd++] = File(id, pid, fileSize);
  }
  else {
    fileSystem[fileSystemEnd++] = Directory(id, pid);
  }
  return countSize(pid);
}

int move(int id, int pid){

  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i].id == -1) continue;

    if (fileSystem[i].id == id)
    {
      fileSystem[i].pid = pid;
      return countSize(pid);
    }
  }
}

int infect(int id){

  Item* item = getItem(id);
  if (item->size > 0)
  {
    int newSize;
    int wholeSize = countSize(10000);
    int files = countFiles();

    if (files == 0)
      newSize = 0;
    else
      newSize = wholeSize / files;

    item->size += newSize;
    return item->size;
  }
  else
  {
    infectDirectory(item, countSize(10000), countFiles());
    return countSize(id);
  }
}

int recover(int id){

  Item* item = getItem(id);
  if (item->size > 0)
  {
    item->size = item->origSize;
    return item->size;
  }
  else
  {
    recoverDirectory(item);
    return countSize(id);
  }
}

int remove(int id){
  int ind = getItemIndex(id);
  int size;

  if (fileSystem[ind].size > 0){
    size = fileSystem[ind].size;
    fileSystem[ind].id = -1;
    return size;
  }
  else
  {
    size = countSize(id);
    removeDirectory(id);
    if (fileSystem[ind].pid != -1)
    {
      fileSystem[ind].id = -1;
    }

    return size;
  }
}