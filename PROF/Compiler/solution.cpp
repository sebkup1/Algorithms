#include <iostream>
#define MAX_CMD 4096
#define MAX_STR (MAX_CMD * 2 + 1)

using namespace std;

extern bool save(int cmd, int line);

int conver2int(const char*, int size);

void run(const char* str)
{
  int i = 0;

  while (str[i] != 0)
  {
    i++;
  }

  char* newStr = new char[++i];

  i = 0;
  while (str[i] != 0)
  {
    newStr[i] = str[i];
    i++;
  }

  newStr[i] = 0;

  int instCounter = 0;
  save(1 << 31, instCounter++);

  i = 0;
  while (str[i] != 0)
  {
    // find multiplicatons
    if (str[i] == '*')
    {
      bool firstIsReg;
      // get first operand 
      int tempIt = i;
      while (str[tempIt] != '-' && str[tempIt] != '+' && tempIt > 0)  tempIt--;

      char sign;
      if (tempIt == 0)
        sign = '+';
      else
        sign = newStr[tempIt];

      int lhsPos = tempIt;

      if (tempIt > 0)
      tempIt++;

      //    reg or constVal
      if (str[tempIt] == 'R')
      {
        firstIsReg = true;
        
        tempIt++;
      }
      else
      {
        firstIsReg = false;
      }

      int firstOperand = conver2int(str + tempIt, i - tempIt);

      

      // get second operand
      tempIt = i;

      while (str[i] != '+' && str[i] != '-' && str[i] != 0) i++;

      i--;

      int secOperatnd = conver2int(str + tempIt + 1, i - tempIt);

      int cmd, ri;

      // Load R18, 0
      cmd = 1 << 30;
      ri = 18 << 18;
      cmd |= ri;
      save(cmd, instCounter++);

      // load R17, constVal
      cmd = 1 << 30;
      ri = 17;
      ri <<= 18;
      cmd |= ri;
      cmd |= secOperatnd;
      save(cmd, instCounter++);

      // Load R19, 2 instr before
      cmd = 1 << 30;
      ri = 19 << 18;
      cmd |= ri;
      cmd |= (instCounter + 6);
      save(cmd, instCounter++);

      // Load R20, 3 instr ahead
      cmd = 1 << 30;
      ri = 20 << 18;
      cmd |= ri;
      cmd |= (instCounter + 1);
      save(cmd, instCounter++);


      // REGISTER
      if (firstIsReg)
      {
        if (sign == '+')
        {
          // ADD R16, reg
          cmd = 1 << 27;
          ri = 16;
          ri <<= 18;
          firstOperand <<= 13;
          cmd |= ri;
          cmd |= firstOperand;
          save(cmd, instCounter++);
        }
        else if (sign == '-')
        {
          // SUB R16, reg
          cmd = 1 << 26;
          ri = 16;
          ri <<= 18;
          firstOperand <<= 13;
          cmd |= ri;
          cmd |= firstOperand;
          save(cmd, instCounter++);
        }
      }
      else
      {
        if (sign == '+')
        {
          // ADD R16, constVal1
          cmd = 1 << 29;
          ri = 16;
          ri <<= 18;
          cmd |= ri;
          cmd |= firstOperand;
          save(cmd, instCounter++);
        }
        else if (sign == '-')
        {
          // SUB R16, constVal1
          cmd = 1 << 28;
          ri = 16;
          ri <<= 18;
          cmd |= ri;
          cmd |= firstOperand;
          save(cmd, instCounter++);
        }

      }


      // ADD R18, 1
      cmd = 1 << 29;
      ri = 18;
      ri <<= 18;
      cmd |= ri;
      cmd |= 1;
      save(cmd, instCounter++);


      // CJMP R18, R17, 2 instr ahead
      cmd = 1 << 24;
      cmd |= ri; // prevoiusly shifted
      ri = 17;
      ri <<= 13;
      cmd |= ri;
      int rk = (19) << 8;
      cmd |= rk;
      save(cmd, instCounter++);

      // JMP 3 instr before
      cmd = 1 << 25;
      rk = (20) << 8;
      cmd |= rk;
      save(cmd, instCounter++);


      while (lhsPos <= i)
      {
        newStr[lhsPos] = ' ';
        lhsPos++;
      }

    }

    if (str[i] == 0) break;
    i++;
  }


  //////////////////////////////////////////
  // now only additions and substractions
  i = 0;

  //add first substract
  if (newStr[i] == 'R')
  {
    while (newStr[i] != '-' && newStr[i] != '+' && newStr[i] != ' ')  i++;

    if (newStr[i] != 0) i--;

    int reg = conver2int(newStr + 1, i);

    // ADD R16, reg
    int cmd = 0;
    cmd = 1 << 27;
    int ri = 16;
    ri <<= 18;
    reg <<= 13;
    cmd |= ri;
    cmd |= reg;
    save(cmd, instCounter++);
  }
  else
  {
    while (newStr[i] != '-' && newStr[i] != '+' && newStr[i] != ' ')  i++;

    int val = conver2int(newStr, i);
    int cmd = 1 << 29;
    int ri = 16;
    ri <<= 18;
    cmd |= ri;
    cmd |= val;
    save(cmd, instCounter++);
  }


  // next substract
  while (newStr[i] != 0)
  {
    if (newStr[i] == '+' || newStr[i] == '-')
    {
      char sign = newStr[i];
      int previ = ++i;
      while (newStr[i] != '-' && newStr[i] != '+' && newStr[i] != ' ' && newStr[i] != 0)  i++;


      if (newStr[previ] == 'R')
      {
        while (newStr[i] != '-' && newStr[i] != '+' && newStr[i] != ' ' && newStr[i] != 0)  i++;

        i--;

        if (sign == '+')
        {
          // ADD R16, reg
          int reg = conver2int(newStr + previ + 1, i - previ);
          int cmd = 0;
          cmd = 1 << 27;
          int ri = 16;
          ri <<= 18;
          reg <<= 13;
          cmd |= ri;
          cmd |= reg;
          save(cmd, instCounter++);
        }
        else if (sign == '-')
        {
          // SUB R16, reg
          int reg = conver2int(newStr + previ + 1, i - previ);
          int cmd = 0;
          cmd = 1 << 26;
          int ri = 16;
          ri <<= 18;
          reg <<= 13;
          cmd |= ri;
          cmd |= reg;
          save(cmd, instCounter++);
        }
        else
        {
          cout << "bad 5" << endl;
        }


      }
      else
      {
        while (newStr[i] != '-' && newStr[i] != '+' && newStr[i] != ' ' && newStr[i] != 0)  i++;


        if (sign == '+')
        {
          // ADD R16, val
          int val = conver2int(newStr + previ, i - previ);
          int cmd = 1 << 29;
          int ri = 16;
          ri <<= 18;
          cmd |= ri;
          cmd |= val;
          save(cmd, instCounter++);
        }
        else if (sign == '-')
        {
          // SUB R16, val
          int val = conver2int(newStr + previ, i - previ);
          int cmd = 1 << 28;
          int ri = 16;
          ri <<= 18;
          cmd |= ri;
          cmd |= val;
          save(cmd, instCounter++);
        }
        else
        {
          cout << "Bad 8";
        }

        if (newStr[i] != 0) i--;
      }

    }

    i++;
  }



  int cmd = 1 << 23;
  int ri = 16;
  ri <<= 18;
  cmd |= ri;

  save(cmd, instCounter++);
}


int conver2int(const char* str, int size)
{
  int place = size - 1;
  int dec = 0;
  int toReturn = 0;

  while (place >= 0)
  {
    int powInd = 0;
    int tens = 10;

    while (powInd + 1 < dec)
    {
      tens = 10 * tens;
      powInd++;
    }

    if (dec != 0)
      toReturn += tens * (str[place] - 48);
    else
      toReturn += (str[place] - 48);

    place--;
    dec++;
  }

  return toReturn;
}