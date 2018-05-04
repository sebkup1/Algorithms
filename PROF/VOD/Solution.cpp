#include <iostream>
#define MAX_N				20000
#define MAX_M				128
#define MAX_BLOCKCOUNT		1250
#define MAX_DATALENGTH		5120000
#define FRAMESIZE			256

using namespace std;

struct huffman
{
  int symbol, codeword, codewordLength;
};

int M, N, nextFrame;
int processedData[MAX_BLOCKCOUNT][16][FRAMESIZE];
huffman huffmanCodes[22][MAX_M];
int codesLengthCount[22];
bool DeCodedBlocks[MAX_BLOCKCOUNT];
bool DeCodedFrames[MAX_BLOCKCOUNT][16];
int maxCodeLength;
int minCodeLength;

unsigned char *Data;
int* Size;

void reDifferenceFrame(int block, int dirFrame);

void Init(int gN, int *size, unsigned char *data, int gM, struct huffman *code)
{
  Data = data;
  Size = size;
  M = gM;
  N = gN;

  for (int j = 0; j < 22; j++)
  {
    for (int i = 0; i < MAX_M; i++)
    {
      huffmanCodes[j][i].codeword = -1;
      huffmanCodes[j][i].symbol = -1;
      huffmanCodes[j][i].codewordLength = -1;
    }

    codesLengthCount[j] = 0;
  }

  for (int i = 0; i < N / 16; i++)
  {
    DeCodedBlocks[i] = false;
  }

  maxCodeLength = 0;
  minCodeLength = 21;


  for (int i = 0; i < gM; i++)
  {
    if (code[i].codewordLength > maxCodeLength)
      maxCodeLength = code[i].codewordLength;

    if (code[i].codewordLength < minCodeLength)
      minCodeLength = code[i].codewordLength;

    huffmanCodes[code[i].codewordLength][codesLengthCount[code[i].codewordLength]] = code[i];
    codesLengthCount[code[i].codewordLength]++;
  }

}

void myGoto(int frame)
{
  int block = frame / 16;
  int dirFrame = frame % 16;
  dirFrame = 0; // to mo¿na zoptymalizowaæ

    // Huffman decoding
  int blockSize = Size[block];

  int dataPos = 0;
  int blockIt = 0;

  while (blockIt != block)
  {
    dataPos += Size[blockIt];
    blockIt++;
  }

  int blockEnd = dataPos + Size[blockIt];

  unsigned int cur4Bdata = 0;
  int bitsInWord;

  int Bpos = 0;

  // take 4 bytes from encoded data
  cur4Bdata = Data[dataPos++];
  cur4Bdata <<= 8;
  Bpos++;
  cur4Bdata |= Data[dataPos++];
  cur4Bdata <<= 8;
  Bpos++;
  cur4Bdata |= Data[dataPos++];
  cur4Bdata <<= 8;
  Bpos++;
  cur4Bdata |= Data[dataPos++];
  Bpos++;

  bitsInWord = 32;

  int decodedWords = 0;

  while (Bpos <= blockSize)
  {
    bool found = false;
    int i;
    for (i = minCodeLength; i <= maxCodeLength && i <= bitsInWord; i++)
    {
      unsigned int curPotentialCode = cur4Bdata;
      curPotentialCode >>= (32 - i);

      for (int j = 0; j < codesLengthCount[i]; j++)
      {
        if (huffmanCodes[i][j].codeword == curPotentialCode)
        {
          processedData[block][dirFrame][decodedWords++] = huffmanCodes[i][j].symbol;

          if (decodedWords % 256 == 0)
          {
            decodedWords = 0;
            dirFrame++;
            if (dirFrame == 16)
              int y = 556;

            if (dirFrame > 1)
            {
              reDifferenceFrame(block, dirFrame - 1);
            }

          }

          bitsInWord -= i;
          found = true;
          break;
        }
      }
      if (found)
        break;
    }

    if (!found)
      cout << "code not found" << endl;

    int offset = 32 - bitsInWord;
    cur4Bdata <<= i;

    while (bitsInWord <= 24)
    {
      if (Bpos < blockSize)
      {
        int newWord = Data[dataPos++];
        newWord <<= (offset - 8);
        offset -= 8;
        cur4Bdata |= newWord;
        bitsInWord += 8;
        Bpos++;
      }
      else
      {
        if (dirFrame == 16)
        {
          Bpos++;
          break;
        }
        else
        {
          break;
        }
      }

    }
  }

  DeCodedBlocks[block] = true;
}

void Goto(int frame)
{
  nextFrame = frame;

  int block = nextFrame / 16;
  int dirFrame = nextFrame % 16;

  if (!DeCodedBlocks[block])
  {
    myGoto(nextFrame);
  }
}

void reDifferenceFrame(int block, int dirFrame)
{
  // ReDifference 
  for (int i = 0; i < 256; i++)
    processedData[block][dirFrame][i] = processedData[block][dirFrame][i] + processedData[block][dirFrame - 1][i] - 128;
}

int Tick(unsigned char *screen)
{
  int block = nextFrame / 16;
  int dirFrame = nextFrame % 16;

  if (!DeCodedBlocks[block])
  {
    myGoto(nextFrame);
  }


  for (int i = 0; i < 256; i++)
    screen[i] = processedData[block][dirFrame][i];

  nextFrame++;

  int toReturnFrame = nextFrame;

  if (nextFrame > N - 1)
  {
    nextFrame--;
  }

  toReturnFrame--;

  return toReturnFrame;
}
