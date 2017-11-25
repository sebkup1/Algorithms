#include <iostream>
using namespace std;

#define MAX_WIN 500
#define WIDTH   160
#define HEIGHT  120

extern void DisplayUpdate(char dispbuf[HEIGHT][WIDTH]);

struct Window
{
  bool valid;
  Window* parent;
  int handle;
  int parentHandleId;
  int absVisibleX, absVisibleY; // ablolut values
  int relX, relY; // relative to parent values
  int realX, realY;
  int width;
  int height;
  char color[HEIGHT][WIDTH];
  Window* innerWindows[HEIGHT][WIDTH];

  Window() : valid(false) {};


  void fillRect(int x, int y, int width, int height, char color)
  {
    int fillXBeg = realX + x;
    int fillYBeg = realY + y;

    int fillWidthEnd = fillXBeg + width;
    int fillHeightEnd = fillYBeg + height;

    if (fillXBeg < this->absVisibleX)
    {
      //fillWidthEnd -= (this->absVisibleX - fillXBeg);
      //fillWidthEnd++;
      fillXBeg = this->absVisibleX;
    }

    if (fillYBeg < this->absVisibleY)
    {
      //fillHeightEnd -= (this->absVisibleY - fillYBeg);
      //fillHeightEnd++;
      fillYBeg = this->absVisibleY;
    }

    if (fillWidthEnd > absVisibleX + this->width)
      fillWidthEnd -= fillWidthEnd - absVisibleX - this->width;

    if (fillHeightEnd > absVisibleY + this->height)
      fillHeightEnd -= fillHeightEnd - absVisibleY - this->height;

    for (int i = fillYBeg; i < fillHeightEnd; i++)
    {
      for (int j = fillXBeg; j < fillWidthEnd; j++)
      {
        this->color[i][j] = color;
      }
    }
  }
};

Window* findRoot(Window* window);
void drawWindowRecursively(Window& window);
char dispbuf[HEIGHT][WIDTH];
bool processed[HEIGHT][WIDTH];
Window* pixelOwner[HEIGHT][WIDTH];
Window windows[MAX_WIN];

int lastAddedWinHandle;
int lastActivatedWindowIndex;


static void createWindow(Window& window, int handle, int parentHandleId, int x, int y, int width, int height, char color)
{
  window.valid = true;
  window.handle = handle;
  window.relX = x;
  window.relY = y;
  window.parentHandleId = parentHandleId;

  if (handle != -1)
  {
    window.parent = &windows[parentHandleId];

    window.realX = window.parent->realX + x;
    window.realY = window.parent->realY + y;

    if (window.realX < window.parent->absVisibleX)
    {
      window.absVisibleX = window.parent->absVisibleX;
      window.width = width -
        (window.parent->absVisibleX - window.realX);
    }
    else
    {
      window.absVisibleX = window.parent->realX + x;
      window.width = width;
    }

    if (window.realY < window.parent->absVisibleY)
    {
      window.absVisibleY = window.parent->absVisibleY;
      window.height = height -
        (window.parent->absVisibleY - window.realY);
    }
    else
    {
      window.absVisibleY = window.parent->realY + y;
      window.height = height;
    }

    // cut window if exceedes parent bounds
    if (window.absVisibleX + window.width > window.parent->absVisibleX + window.parent->width)
    {
      window.width -= (window.absVisibleX + window.width - window.parent->absVisibleX - window.parent->width);
    }

    if (window.absVisibleY + window.height > window.parent->absVisibleY + window.parent->height)
    {
      window.height -= (window.absVisibleY + window.height - window.parent->absVisibleY - window.parent->height);
    }

    for (int i = window.absVisibleY; i < window.absVisibleY + window.height; i++)
    {
      for (int j = window.absVisibleX; j < window.absVisibleX + window.width; j++)
      {
        //if (window.parent->innerWindows[i][j] == window.parent)
        window.parent->innerWindows[i][j] = &window;

        window.innerWindows[i][j] = &window;
        window.color[i][j] = color;
      }
    }

  }
  else
  {
    window.absVisibleX = window.absVisibleY = 0;
    window.parent = nullptr;
    window.width = width;
    window.height = height;
    window.realX = x;
    window.realY = y;

    for (int i = window.absVisibleY; i < window.absVisibleY + window.height; i++)
    {
      for (int j = window.absVisibleX; j < window.absVisibleX + window.width; j++)
      {
        window.innerWindows[i][j] = &window;
        window.color[i][j] = color;
      }
    }
  }

}


void refresh()
{
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      processed[i][j] = false;
    }
  }

  // manage piksel owners
  Window* currentWindow = &windows[lastActivatedWindowIndex];
  Window* temp, *prevTemp, *temp2;

  while (currentWindow != nullptr)
  {
    for (int i = currentWindow->absVisibleY; i < currentWindow->absVisibleY + currentWindow->height; i++)
    {
      for (int j = currentWindow->absVisibleX; j < currentWindow->absVisibleX + currentWindow->width; j++)
      {
        if (!processed[i][j])
        {
          temp = currentWindow->innerWindows[i][j];
          prevTemp = currentWindow;

          while (temp != prevTemp)
          {
            if (!temp->valid)
            {
              temp = prevTemp;
              break;
            }

            temp2 = temp;
            temp = temp->innerWindows[i][j];
            prevTemp = temp2;
          }

          pixelOwner[i][j] = temp;
          processed[i][j] = true;
        }
      }
    }
    currentWindow = currentWindow->parent;
  }

  // preper dispbuf
  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      temp = pixelOwner[i][j];
      char a = temp->color[i][j];
      dispbuf[i][j] = a;
    }
  }
}

void InitWinManager(char bgcolor)
{
  for (int i = 0; i < MAX_WIN; i++)
    windows[i].valid = false;

  lastAddedWinHandle = 0;
  lastActivatedWindowIndex = lastAddedWinHandle;

  createWindow(windows[lastAddedWinHandle], -1, 0, 0, 0, WIDTH, HEIGHT, bgcolor);

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      pixelOwner[i][j] = &windows[lastAddedWinHandle];
    }
  }

  refresh();
  DisplayUpdate(dispbuf);
}

int CreateWindow(int parent, int x, int y, int width, int height, char color)
{
  if (parent == -1) parent++;

  pixelOwner;
  lastAddedWinHandle++;

  createWindow(windows[lastAddedWinHandle], lastAddedWinHandle, parent, x, y, width, height, color);

  if (windows[lastAddedWinHandle].parentHandleId == 0)
    lastActivatedWindowIndex = lastAddedWinHandle;
  //else
  //  lastActivatedWindowIndex = findRoot(&windows[lastAddedWinHandle])->handle;

  //drawWindowRecursively(windows[0]);

  refresh();
  DisplayUpdate(dispbuf);

  return lastAddedWinHandle;
}

void deleteWindow(int handle)
{
  for (int i = 0; i < lastAddedWinHandle; i++)
  {
    if (windows[i].parentHandleId == handle)
    {
      deleteWindow(windows[i].handle);
      windows[i].valid = false;
    }
  }
}

void DestroyWindow(int handle)
{
  if (handle == 15)
    cout << "";

  Window* win = &windows[handle];

  if (win->valid)
  {
    win->valid = false;

    deleteWindow(handle);

    if (!windows[lastActivatedWindowIndex].valid)
    {
      Window* temp = &windows[lastActivatedWindowIndex];
      while (!temp->valid)
      {
        temp = temp->parent;
      }
      lastActivatedWindowIndex = temp->handle;

      if (lastActivatedWindowIndex == -1)
        lastActivatedWindowIndex = 0;


    }

    //Constrain 12
    drawWindowRecursively(windows[lastActivatedWindowIndex]);
  }
  else
  {
    cout << "no such window :( DestroyWindow \n";
  }

  refresh();
  DisplayUpdate(dispbuf);
}

void FillRect(int handle, int x, int y, int width, int height, char color)
{
  Window* win = &windows[handle];
  if (win->valid)
  {
    win->fillRect(x, y, width, height, color);
  }
  else
  {
    cout << "no such window :( FillRect";
  }

  refresh();
  DisplayUpdate(dispbuf);
}

void ActiveWindow(int handle)
{
  // find root
  lastActivatedWindowIndex = findRoot(&windows[handle])->handle;
  if (lastActivatedWindowIndex == -1)
    lastActivatedWindowIndex = 0;
  refresh();
  DisplayUpdate(dispbuf);
}

void Click(int x, int y)
{
  // find root
  if (pixelOwner[y][x]->handle != -1)
  {
    lastActivatedWindowIndex = findRoot(pixelOwner[y][x])->handle;
    if (lastActivatedWindowIndex == -1)
      lastActivatedWindowIndex = 0;
  }
  refresh();
  DisplayUpdate(dispbuf);
}

Window* findRoot(Window* window)
{
  Window* temp = window;

  if (window->handle == -1) return window;

  while (temp->parent->handle != -1)
  {
    temp = temp->parent;
  }

  return temp;
}

void drawWindowRecursively(Window& window)
{
  for (int k = 0; k < MAX_WIN; k++)
  {
    if (windows[k].valid && windows[k].parent == &window)
    {
      drawWindowRecursively(windows[k]);

      for (int i = windows[k].absVisibleY; i < windows[k].absVisibleY + windows[k].height; i++)
      {
        for (int j = windows[k].absVisibleX; j < windows[k].absVisibleX + windows[k].width; j++)
        {
          windows[k].parent->innerWindows[i][j] = &windows[k];
        }
      }

    }
  }

  if (window.parent != nullptr)
    for (int i = window.absVisibleY; i < window.absVisibleY + window.height; i++)
    {
      for (int j = window.absVisibleX; j < window.absVisibleX + window.width; j++)
      {
        window.parent->innerWindows[i][j] = &window;
      }
    }
}