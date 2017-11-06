#include <iostream>

struct Word
{
  char* text;
  int size;
  Word* next;
  Word* prev;

  Word(char* inText, Word* iPrev, Word* iNext) : prev(iPrev), next(iNext)
  {
    text = new char[30];
    int i = size = 0;
    while (inText[i] != 0)
    {
      text[i] = inText[i];
      size++;
      i++;
    }
    text[i] = inText[i];
  }
};

void deleteWords(Word*);
void revertDataUpdate(Word*, int);
void printAll();
int cursorPosition;
int curCISPos;
int cachedWordsPosition;
bool cached = false;
int prevCursorPos;

Word* head = nullptr;
Word* consecInsertedBegin;
Word* consecInsertedEnd;
Word* oldCachedBegin;
Word* oldCachedEnd;

void init()
{
  cursorPosition = 0;
  curCISPos = -1;
  prevCursorPos = -1;
  cachedWordsPosition = -1;

  // clearing
  if (head != nullptr)
    deleteWords(head);

  head = nullptr;
  consecInsertedBegin = nullptr;
  consecInsertedEnd = nullptr;
  oldCachedBegin = nullptr;
  oldCachedEnd = nullptr;
  cached = false;
}

void cmd_input(char ch[])
{
  // find word
  Word* it = head;
  Word* last = nullptr;
  int curPos = 0;

  if (cursorPosition == 0 && head == nullptr)
  {
    head = new Word(ch, nullptr, nullptr);
    cursorPosition = head->size;
    revertDataUpdate(head, 0);
    return;
  }

  while (it != nullptr)
  {
    if (cursorPosition == 0)
    {
      Word* newWord = new Word(ch, nullptr, head);
      head->prev = newWord;
      head = newWord;
      revertDataUpdate(newWord, 0);
      return;
    }

    //is beside the words
    if (curPos == cursorPosition)
    {
      Word* newWord = new Word(ch, it->prev, it);

      if (it->prev != nullptr)
      {
        it->prev->next = newWord;
        it->prev = newWord;
      }
      else
      {
        head = newWord;
      }

      //if (it->next != nullptr)
      //  it->next->prev = newWord;

      cursorPosition = curPos + newWord->size;
      revertDataUpdate(newWord, 1);
      return;
    }

    // is in the middle of the word
    if (curPos + it->size > cursorPosition)
    {
      // split current
      int pos = cursorPosition - curPos;
      Word* newWord = new Word(&it->text[pos], nullptr, it->next);
      it->text[pos] = 0;
      it->size = pos;

      // join new in
      Word* middleNewWord = new Word(ch, it, newWord);
      it->next = middleNewWord;
      newWord->prev = middleNewWord;

      //consecInsertedBegin = middleNewWord;
      //consecInsertedEnd = middleNewWord;
      cursorPosition += middleNewWord->size;
      revertDataUpdate(middleNewWord, 1);
      printAll();
      return;
    }

    curPos += it->size;

    if (it->next == nullptr)
    {
      last = it;
    }
    it = it->next;
  }

  if (it == nullptr) //push back
  {
    last->next = new Word(ch, last, nullptr);
    cursorPosition = curPos + last->next->size;
    revertDataUpdate(last->next, 0);
  }

}

void cmd_revert()
{
  if (!cached) // pop data to cache
  {
    // begin
    if (consecInsertedBegin->prev != nullptr)
    {
      consecInsertedBegin->prev->next = consecInsertedEnd->next;
      oldCachedBegin = consecInsertedBegin->prev;
    }
    else
    {
      head = consecInsertedEnd->next;
      oldCachedBegin = nullptr;
    }

    // end
    if (consecInsertedEnd->next != nullptr)
    {
      consecInsertedEnd->next->prev = consecInsertedBegin->prev;
      oldCachedEnd = consecInsertedEnd->next;
    }
    else
    {
      oldCachedEnd = nullptr;
    }

    prevCursorPos = cursorPosition;

    if (head == nullptr) cursorPosition = 0;
    else
    {
      Word* it = head;
      int curPos = 0;
      while (it != oldCachedBegin)
      {
        curPos += it->size;
        it = it->next;
      }

      cursorPosition = curPos + it->size;
    }
    cached = true;
    cachedWordsPosition = cursorPosition;
  }
  else // get words from cache
  {
    // insert in previous place
    if (oldCachedBegin != nullptr)
    {
      oldCachedBegin->next = consecInsertedBegin;
    }
    else
    {
      if (head != nullptr)
      {
        head->prev = consecInsertedEnd;
        consecInsertedEnd->next = head;
      }

      head = consecInsertedBegin;
      curCISPos = prevCursorPos;
    }

    if (oldCachedEnd != nullptr)
    {
      oldCachedEnd->prev = consecInsertedEnd;
    }


    cursorPosition = prevCursorPos;
    curCISPos = prevCursorPos;
    cached = false;
  }
  printAll();

}

void cmd_movecursor(int mCursor)
{
  cursorPosition = mCursor;
  printAll();
}

void get_substring(int mPosition, int mLength, char res[])
{
  Word* it = head;
  printAll();
  Word* last = nullptr;
  int curPos = 0;
  while (true)
  {
    curPos += it->size;
    if (curPos >= mPosition)
    {
      //cur word offset
      int offSet = curPos - mPosition;

      int curWordPos = it->size - offSet;
      int i;
      for (i = 0; i < mLength; i++)
      {
        if (curWordPos >= it->size)
        {
          it = it->next;
          if (it == nullptr)
          {
            res[i] = 0;
            return;
          }
          curWordPos = 0;
        }

        res[i] = it->text[curWordPos];
        curWordPos++;
      }

      res[mLength] = 0;

      return;
    }

    if (it->next != nullptr)
      it = it->next;
    else
    {
      last = it;
    }
  }
}

void revertDataUpdate(Word* recentWord, int status)
{
  if (cursorPosition - recentWord->size != curCISPos)
  {
    // clean cache

    cached = false;
    consecInsertedBegin = recentWord;
  }

  consecInsertedEnd = recentWord;
  curCISPos = cursorPosition;
}

void deleteWords(Word* word)
{
  if (word->next != nullptr)
    deleteWords(word->next);
  delete word;
}

void printAll()
{
  bool val = false;
  if (val)
  {
    std::cout << "--------------" << std::endl;
    Word* it = head;
    while (it != nullptr)
    {
      std::cout << it->text;
      std::cout << std::endl;
      it = it->next;
    }
  }


}
