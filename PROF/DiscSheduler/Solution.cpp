#include <stdio.h>

enum AddrState  {
  Empty,
  Processed,
  ToProcess
};

int curHeadPos;
AddrState disc[100000];
int trackSize;

//fcfs
int* queue;
int qPos;
int qEnd;

//look dir
bool lookDir;

void init(int track_size, int head){
  trackSize = track_size;

  for (int i = 0; i < track_size; i++)
  {
    disc[i] = AddrState::Empty;
  }

  curHeadPos = head;

  //fcfs
  queue = new int[track_size];
  qPos = 0;
  qEnd = 0;

  //look
  lookDir = true; // left
}

void request(int track){
  //fcfs
  queue[qEnd] = track;
  qEnd++;
  disc[track] = ToProcess;
}

int fcfs(){
  while (disc[queue[qPos]] == Processed)
  {
    qPos++;
  }
  disc[queue[qPos]] = Processed;
  curHeadPos = queue[qPos];
  return curHeadPos;
}

int sstf(){
  int head = curHeadPos;
  int offset = 0;
  while (disc[offset + curHeadPos] != ToProcess)
  {
    if (offset + curHeadPos >= 0 && offset + curHeadPos < trackSize){
      if (offset == 0)
      {
        offset--;
      }
      else if (offset < 0)
      {
        if (-offset + curHeadPos >= trackSize)
        {
          offset--;
          continue;
        }
        offset = -offset;
      }
      else
      {
        if (-offset - 1 + curHeadPos< 0)
        {
          offset++;
          continue;
        }
        offset = -offset - 1;
      }
      continue;
    }
    else if (offset + curHeadPos < 0)
    {
      offset = -offset + 1;
      continue;
    }
    else if (offset + curHeadPos >= trackSize)
    {
      offset = -offset - 1;
      continue;
    }
    else
    {
      printf("Ÿle");
    }
  }
  
  curHeadPos = offset + curHeadPos;

  if (curHeadPos < 0)
  {
    new int;
  }
  disc[curHeadPos] = Processed;

  return curHeadPos;
}

int look(){
  int offset = 0;
  while (disc[offset + curHeadPos] != ToProcess)
  {
    if (lookDir) // true - left
    {
      offset--;
    }
    else
    {
      offset++;
    }

    if (offset + curHeadPos < 0)
    {
      offset = 0;
      lookDir = false;
      continue;
    }
    if (offset + curHeadPos >= trackSize)
    {
      offset = 0;
      lookDir = true;
    }
  }


  disc[offset + curHeadPos] = Processed;
  int pos = offset + curHeadPos;
  curHeadPos = offset + curHeadPos;
  return curHeadPos;
}

int clook(){
  int offset = 0;
  while (disc[offset + curHeadPos] != ToProcess)
  {
    offset--;

    if (offset + curHeadPos < 0)
    {
      offset = trackSize - curHeadPos - 1;
    }
  }

  disc[offset + curHeadPos] = Processed;
  int pos = offset + curHeadPos;
  curHeadPos = offset + curHeadPos;
  return curHeadPos;

}