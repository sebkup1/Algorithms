#define MAX_FILE_SYSTEM 10000
#include <stdio.h>


struct Item
{
  int id;
  int pid;
  int size;
  int origSize;
  //Item* parent;
};

struct File : Item
{
  //int origSize;
  File(int id, int pid, int size)
  {
    if (id == 97502)
    {
      printf("");
    }

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
    //Item::parent = nullptr;
    this->pid = pid;
    this->size = 0;
  }
};

Item** fileSystem = nullptr;
Directory* root = nullptr;
int fileSystemEnd;

void ADD(Item* item)
{
  fileSystem[fileSystemEnd++] = item;
}

Item* GET(int id)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i] != nullptr && fileSystem[i]->id == id)
      return fileSystem[i];
  }
}

int GET_IND(int id)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i] != nullptr && fileSystem[i]->id == id)
      return i;
  }
}


int countSize(int pid)
{
  int sum = 0;
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i] != nullptr && fileSystem[i]->pid == pid)
    {
      if (fileSystem[i]->size > 0)
      {
        sum += fileSystem[i]->size;
      }
      else
      {
        sum += countSize(fileSystem[i]->id);
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
    if (fileSystem[i] != nullptr && fileSystem[i]->size > 0)
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
    if (fileSystem[i] != nullptr && fileSystem[i]->pid == item->id)
    {
      if (fileSystem[i]->size > 0)
      {
        fileSystem[i]->size += curFileSystemSize / curFilesCount;
      }
      else
      {
        infectDirectory(fileSystem[i], curFileSystemSize, curFilesCount);
      }
    }
  }
}

void recoverDirectory(Item* item)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i] != nullptr && fileSystem[i]->pid == item->id)
    {
      if (fileSystem[i]->size > 0)
      {
        //File* item = (File*)fileSystem[id - 10000];
        fileSystem[i]->size = fileSystem[i]->origSize;
      }
      else
      {
        recoverDirectory(fileSystem[i]);
      }
    }
  }
}

void removeDirectory(int id)
{
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i] != nullptr && fileSystem[i]->pid == id)
    {
      if (fileSystem[i]->size > 0)
      {
        delete fileSystem[i];
        fileSystem[i] = nullptr;
      }
      else
      {
        removeDirectory(i);
      }
    }
  }
}

//=============================================================
//to implement
void init(){
  fileSystemEnd = 0;
  if (fileSystem != nullptr) delete[] fileSystem;

  fileSystem = new Item*[MAX_FILE_SYSTEM];

  for (int i = 0; i < MAX_FILE_SYSTEM; i++) fileSystem[i] = nullptr;

  root = new Directory(10000, -1);

  ADD(root);
}

int add(int id, int pid, int fileSize){
  if (fileSize > 0) {
    ADD(new File(id, pid, fileSize));
  }
  else {
    ADD(new Directory(id, pid));
  }
  return countSize(pid);
}

int move(int id, int pid){
  for (int i = 0; i < fileSystemEnd; i++)
  {
    if (fileSystem[i] != nullptr && fileSystem[i]->id == id)
    {
      fileSystem[i]->pid = pid;
      return countSize(pid);
    }
  }
}

int infect(int id){
  if (id == 97502)
  {
    printf("");
  }

  Item* item = GET(id);
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
  if (id == 97502)
  {
    printf("");
  }

  Item* item = GET(id);
  if (item->size > 0)
  {
    //File* item = (File*)fileSystem[id - 10000];
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
  int ind = GET_IND(id);
  int size;

  if (fileSystem[ind]->size > 0){
    size = fileSystem[ind]->size;
    delete fileSystem[ind];
    fileSystem[ind] = nullptr;
    return size;
  }
  else
  {
    size = countSize(id);
    removeDirectory(id);
    if (fileSystem[ind]->pid != -1)
    {
      delete fileSystem[ind];
      fileSystem[ind] = nullptr;
    }

    return size;
  }
}