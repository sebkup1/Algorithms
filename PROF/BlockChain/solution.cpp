#define MAXSERVER	5
#define IMAGESIZE	400000
#define MAXHASH 15000

#include <limits>

extern int calcHash(unsigned char buf[], int pos, int len);

struct transaction {
  unsigned short int exchangeId;
  unsigned int ammount;
};
struct block {
  short int tc;
  unsigned int hash;
  unsigned int parentHash;
  unsigned short int parent;
  unsigned int random;
  unsigned short int childCount;
  int child[20];
  unsigned short int transN;
  transaction transaction[16];
};

unsigned char(*blockchainimage)[IMAGESIZE];
block blocks[MAXSERVER][108 * MAXHASH];

void addToLocalQueue(int hash, int localChilds[16], int& localChildsCount) {
  localChilds[localChildsCount++] = hash;
}

bool isInQueue(int hash, int localChilds[16], int localChildsCount) {
  for (int i = 0; i < localChildsCount; i++) {
    if (localChilds[i] == hash) return true;
  }
  return false;
}

short int TestCase = 0;
short int serwers;

unsigned int indForHash(int hash) {
  return hash % (107 * MAXHASH);
}

int getIndFromHash(int hash, int server) {
  // dopuki nastêpny lub koniec 
  int ind = indForHash(hash);
  if (blocks[server][ind].tc == TestCase) {
    while (ind != 108 * MAXHASH) {
      if (blocks[server][ind].hash == hash &&
        blocks[server][ind].tc == TestCase) {
        return ind;
      }
      else if (blocks[server][ind].tc != TestCase) {
        return -1;
      }
      ind++;
    }
  }
  else
    return -1;
}

void syncBlockChain(int S, unsigned char blockchainimage[MAXSERVER][IMAGESIZE]) {
  serwers = S;
  TestCase++;
  for (unsigned short int server = 0; server < S; server++) {

    // block image size
    unsigned int bisize = 0;
    unsigned short int temp = blockchainimage[server][0];
    bisize |= (temp << 24);
    temp = blockchainimage[server][1];
    bisize |= (temp << 16);
    temp = blockchainimage[server][2];
    bisize |= (temp << 8);
    temp = blockchainimage[server][3];
    bisize |= temp;
    unsigned int it = 4;

    // get blocks
    while (true) {
      unsigned int parent, blockBeg, blockEnd;
      parent = 0;
      blockBeg = it;

      // get one block
      temp = blockchainimage[server][it];
      parent |= (temp << 24);
      temp = blockchainimage[server][it + 1];
      parent |= (temp << 16);
      temp = blockchainimage[server][it + 2];
      parent |= (temp << 8);
      temp = blockchainimage[server][it + 3];
      parent |= temp;

      unsigned int random = 0;
      temp = blockchainimage[server][it + 4];
      random |= (temp << 8);
      temp = blockchainimage[server][it + 5];
      random |= temp;

      unsigned int transN = 0;
      transN = blockchainimage[server][it + 6];

      blockEnd = it;
      int blockHash = calcHash(blockchainimage[server], blockBeg, 7 + 3 * transN);
      unsigned int ind = indForHash(blockHash);

      while (blocks[server][ind].tc == TestCase && blocks[server][ind].hash != blockHash) {
        ind++;
      }
      it += 7;

      blocks[server][ind].transN = 0;

      for (short int trans = 0; trans < transN; trans++) {
        blocks[server][ind].transaction[blocks[server][ind].transN].exchangeId = blockchainimage[server][it];
        blocks[server][ind].transaction[blocks[server][ind].transN].ammount = 0;
        blocks[server][ind].transaction[blocks[server][ind].transN].ammount |= (blockchainimage[server][it + 1] << 8);
        blocks[server][ind].transaction[blocks[server][ind].transN].ammount |= blockchainimage[server][it + 2];
        blocks[server][ind].transN++;
        it += 3;
      }

      blocks[server][ind].parent = parent;
      blocks[server][ind].random = random;

      if (blocks[server][ind].tc != TestCase) {
        blocks[server][ind].tc = TestCase;
        blocks[server][ind].childCount = 0;
        blocks[server][ind].hash = blockHash;
      }

      if (parent != 0) {
        unsigned int parentInd = indForHash(parent);
        while (blocks[server][parentInd].hash != parent && blocks[server][parentInd].tc == TestCase) {
          parentInd++;
        }

        // wczesne dodanie parenta, zanim zostanie odczytany z obrazu
        if (blocks[server][parentInd].hash != parent) {
          blocks[server][parentInd].hash = parent;
          blocks[server][parentInd].tc = TestCase;
          blocks[server][parentInd].childCount = 0;
        }

        blocks[server][parentInd].child[blocks[server][parentInd].childCount++] = blockHash;
      }

      if (it >= bisize + 4) break;
    }
  }
}

bool checkIfValidBlock(int hash, int& server, int& ind) {
  short int ocurance = 0;
  for (unsigned short int i = 0; i < serwers; i++) {
    int lind = getIndFromHash(hash, i);
    if (lind != -1 && blocks[i][lind].tc == TestCase) {
      ocurance++;
      server = i;
      ind = lind;
    }
  }

  short int blocksMin = serwers / 2 + 1;
  if (ocurance >= blocksMin) return true;
  else return false;
}

int getAmountFromBlock(int ind, int exchangeid, int server, int hash) {
  unsigned int sum = 0;
  int lserver = server, lind = ind;
  int transN = blocks[server][ind].transN;

  for (int i = 0; i < transN; i++) {
    if (blocks[server][ind].transaction[i].exchangeId == exchangeid) {
      sum += blocks[server][ind].transaction[i].ammount;
    }
  }

  int localChilds[16];
  int localChildsCount = 0;

  for (int s = 0; s < serwers; s++) {
    int index = getIndFromHash(hash, s);
    if (index == -1) continue;

    int childs = blocks[s][index].childCount;
    for (int i = 0; i < childs; i++) {
      if (isInQueue(blocks[s][index].child[i], localChilds, localChildsCount)) continue;
      if (!checkIfValidBlock(blocks[s][index].child[i], server, ind)) continue;

      sum += getAmountFromBlock(ind, exchangeid, server, blocks[s][index].child[i]);
      addToLocalQueue(blocks[s][index].child[i], localChilds, localChildsCount);
    }
  }
  return sum;
}

int calcAmount(int hash, int exchangeid) {
  int server, ind;
  if (!checkIfValidBlock(hash, server, ind)) return 0;

  return getAmountFromBlock(ind, exchangeid, server, hash);
}