#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

#define MAX_GROUND_SIZE 25
int s1r, s1c, s2r, s2c, g1r, g1c, g2r, g2c;
int cur1r, cur1c, cur2r, cur2c;
int N;
unsigned int minMoves = UINT_MAX;
unsigned int moves;

struct State{
  int p1r, p1c, p2r, p2c;
  int moves;
  State* next;
  State(int r1, int c1, int r2, int c2, int m) {
    p1r = r1;
    p1c = c1;
    p2r = r2;
    p2c = c2;
    moves = m;
    next = nullptr;
  }
};

State* queueHead;
State* queueEnd;

int ground[MAX_GROUND_SIZE + 1][MAX_GROUND_SIZE + 1];
int visited[MAX_GROUND_SIZE + 1][MAX_GROUND_SIZE + 1][MAX_GROUND_SIZE + 1][MAX_GROUND_SIZE + 1];

int processTanksBfs(bool secStood = false);
bool colision();

int main(int argc, char** argv)
{
  int test_case;
  int T;

  freopen("sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    for (int r1 = 0; r1 <= MAX_GROUND_SIZE+1; r1++){
      for (int c1 = 0; c1 <= MAX_GROUND_SIZE+1; c1++){
        for (int r2 = 0; r2 <= MAX_GROUND_SIZE+1; r2++){
          for (int c2 = 0; c2 <= MAX_GROUND_SIZE+1; c2++){
            visited[r1][c1][r2][c2] = 0;
          }
        }
        ground[r1][c1] = 1;
      }
    }
    moves = 0;
    minMoves = UINT_MAX;
    cin >> N;
    cin >> s1r;
    cin >> s1c;
    cin >> g1r;
    cin >> g1c;
    cin >> s2r;
    cin >> s2c;
    cin >> g2r;
    cin >> g2c;

    cur1r = s1r;
    cur1c = s1c;
    cur2r = s2r;
    cur2c = s2c;

    for (int r = 1; r <= N; r++) {
      for (int c = 1; c <= N; c++) {
        cin >> ground[r][c];
      }
    }

    queueHead = new State(cur1r, cur1c, cur2r, cur2c, moves);
    queueEnd = queueHead;
    queueHead->next = nullptr;

    visited[cur1r][cur1c][cur2r][cur2c] = true;

    while (queueHead != nullptr) {
      State* temp = queueHead;
      cur1c = temp->p1c;
      cur1r = temp->p1r;
      cur2c = temp->p2c;
      cur2r = temp->p2r;
      moves = temp->moves;
      processTanksBfs(false);

      queueHead = queueHead->next;
      delete temp;
    }

    int ret = minMoves;
    unsigned int max = UINT_MAX;
    if (minMoves == max) ret = -1;

    cout << "#" << test_case << " " << ret << endl;
  }

  return 0; // Your program should return 0 on normal termination.
}

void secondTankMove(bool firstStood);

//Bfs
int processTanksBfs(bool secStood) {
  if (colision()) return 0;
  if (moves >= minMoves) return 0;

  // destination found
  if (cur1c == g1c && cur1r == g1r && cur2c == g2c && cur2r == g2r) {
    if (moves < minMoves) {
      minMoves = moves;
    }
    return 0;
  }

  // destination found for this tank
  if (cur1c == g1c && cur1r == g1r){
    secondTankMove(true);
    return 0;
  }

  int r, c;
  int prevR = cur1r;
  int prevC = cur1c;

  //tank 1
  // up left
  r = prevR - 1;
  c = prevC - 1;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // up
  r = prevR - 1;
  c = prevC;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // up right
  r = prevR - 1;
  c = prevC + 1;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // left
  r = prevR;
  c = prevC - 1;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // stood
  if (!secStood){
    r = prevR;
    c = prevC;
    if (!ground[r][c]) {
      cur1r = r;
      cur1c = c;
      secondTankMove(true);
    }
  }

  // right
  r = prevR;
  c = prevC + 1;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // left down
  r = prevR + 1;
  c = prevC - 1;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // down
  r = prevR + 1;
  c = prevC;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  // down right
  r = prevR + 1;
  c = prevC + 1;
  if (!ground[r][c]) {
    cur1r = r;
    cur1c = c;
    secondTankMove(false);
  }

  cur1r = prevR;
  cur1c = prevC;
  return 0;
}

void secondTankMove(bool firstStood) {
  int r, c;
  int prevR = cur2r;
  int prevC = cur2c;

  // destination found for this tank
  if (cur2c == g2c && cur2r == g2r) {
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
    return;
  }

  //tank 2
  // up left
  r = prevR - 1;
  c = prevC - 1;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // up
  r = prevR - 1;
  c = prevC;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // up right
  r = prevR - 1;
  c = prevC + 1;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // left
  r = prevR;
  c = prevC - 1;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // stood
  if (!firstStood) {
    r = prevR;
    c = prevC;
    if (!ground[r][c]) {
      cur2r = r;
      cur2c = c;
      if (!visited[cur1r][cur1c][cur2r][cur2c]) {
        queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
        queueEnd = queueEnd->next;
        visited[cur1r][cur1c][cur2r][cur2c] = true;
      }
    }
  }

  // right
  r = prevR;
  c = prevC + 1;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // left down
  r = prevR + 1;
  c = prevC - 1;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // down
  r = prevR + 1;
  c = prevC;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  // down right
  r = prevR + 1;
  c = prevC + 1;
  if (!ground[r][c]) {
    cur2r = r;
    cur2c = c;
    if (!visited[cur1r][cur1c][cur2r][cur2c]) {
      queueEnd->next = new State(cur1r, cur1c, cur2r, cur2c, moves + 1);
      queueEnd = queueEnd->next;
      visited[cur1r][cur1c][cur2r][cur2c] = true;
    }
  }

  cur2r = prevR;
  cur2c = prevC;
}

bool colision() {
  if ((cur1r - 1 == cur2r && cur1c - 1 == cur2c) || // left up
    (cur1r - 1 == cur2r && cur1c == cur2c) || //up
    (cur1r - 1 == cur2r && cur1c + 1 == cur2c) || // right up
    (cur1r == cur2r && cur1c - 1 == cur2c) || // left
    (cur1r == cur2r && cur1c == cur2c) || // center
    (cur1r == cur2r && cur1c + 1 == cur2c) || // right
    (cur1r + 1 == cur2r && cur1c - 1 == cur2c) || // left down
    (cur1r + 1 == cur2r && cur1c == cur2c) || // down
    (cur1r + 1 == cur2r && cur1c + 1 == cur2c)) // right down
  {
    return true;
  }
  return false;
}