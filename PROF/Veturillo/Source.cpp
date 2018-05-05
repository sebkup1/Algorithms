#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <limits>
#include <list>

using namespace std;

#include <stdio.h>
#include <limits.h>

// Number of vertices in the graph
#define V 101
#define timeLimit 1001

bool bikesPoints[V];
int graph[V][V];

int curTCGraphNodesCount;

unsigned int visited[V][timeLimit][2];

int test_case;
int T;
int M;
int nodesCount;
int edgesCount;
int bikesPointsCount;
unsigned int minCost;

struct State {
  short int curNode;
  short int time;
  unsigned int cost;
  bool onBike;
  State(int cN, int t, int c, bool onB) : curNode(cN), time(t), cost(c), onBike(onB) {}
  State() /*: curNode(0), time(0), cost(0), onBike(false)*/ {};
};

// queue ////////
State qu[21000000]; // less than 256 MB global memory
unsigned int head = 0, tail = 0;
void addToQueue(int cN, int t, int c, bool onB) {
  qu[tail].curNode = cN;
  qu[tail].time = t;
  qu[tail].cost = c;
  qu[tail].onBike = onB;
  tail++;
}

State& qHead() {
  return qu[head];
}

bool qEmpty() {
  return (head != tail);
}

void qPopFront() {
  head++;
}
//////////////

list<State> queue;

int main(int argc, char** argv) {
  freopen("sample_input.txt", "r", stdin);
  int size = sizeof(State);
  std::ios::sync_with_stdio(false);

  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    // input data
    cin >> nodesCount;
    cin >> M;
    cin >> edgesCount;
    cin >> bikesPointsCount;

    // initialization
    minCost = UINT_MAX;
    head = tail = 0;

    for (int i = 0; i <= nodesCount; i++) {
      bikesPoints[i] = false;
      for (int j = 0; j <= nodesCount; j++) {
        graph[i][j] = 0;
      }
      for (int j = 0; j <= M; j++) {
        for (int k = 0; k < 2; k++) {
          visited[i][j][k] = UINT_MAX;
        }
      }
    }

    int n1, n2, dist;
    for (int i = 0; i < edgesCount; i++) {
      cin >> n1;
      cin >> n2;
      cin >> dist;
      graph[n1][n2] = graph[n2][n1] = dist;
    }

    for (int i = 0; i < bikesPointsCount; i++) {
      int point;
      cin >> point;
      bikesPoints[point] = true;
    }

    //queue.push_back(State(1, 0, 0, false));
    addToQueue(1, 0, 0, false);

    //while (!queue.empty()) {
    while (qEmpty()) {
      //State* state = &queue.front();
      State* state = &qHead();

      visited[state->curNode][state->time][state->onBike] = state->cost;

      // check edge conditions
      if (state->cost > minCost) {
        //queue.pop_front();
        qPopFront();
        continue;
      }

      // check end cond
      if (state->curNode == nodesCount && state->time < M && state->onBike == false) {
        if (minCost > state->cost)
          minCost = state->cost;
        //queue.pop_front();
        qPopFront();
        continue;
      }

      int node = state->curNode;
      int time, cost;

      for (int i = 1; i <= nodesCount; i++) { // find adjacent nodes
        if (graph[node][i]) {
          if (!state->onBike) {
            // Foot
            time = state->time + graph[node][i] * 19;
            cost = state->cost;
            if (time < M && cost < minCost && visited[i][time][false] > cost)
              //queue.push_back(State(i, time, cost, false));
              addToQueue(i, time, cost, false);

            // get Taxi
            time = state->time + graph[node][i] * 3;
            cost = state->cost + graph[node][i] * 2110;
            if (time < M && cost < minCost && visited[i][time][false] > cost)
              //queue.push_back(State(i, time, cost, false));
              addToQueue(i, time, cost, false);

            // getBike
            if (bikesPoints[node]) {
              time = state->time + graph[node][i] * 7;
              cost = state->cost + graph[node][i] * 7 * 79;
              if (time < M && cost < minCost && visited[i][time][true] > cost)
                //queue.push_back(State(i, time, cost, true));
                addToQueue(i, time, cost, true);
            }
          }
          else {
            if (bikesPoints[node]) { // leave bike
              // Foot
              time = state->time + graph[node][i] * 19;
              cost = state->cost;
              if (time < M && cost < minCost && visited[i][time][false] > cost)
                //queue.push_back(State(i, time, cost, false));
                addToQueue(i, time, cost, false);

              // get Taxi
              time = state->time + graph[node][i] * 3;
              cost = state->cost + graph[node][i] * 2110;
              if (time < M && cost < minCost && visited[i][time][false] > cost)
                //queue.push_back(State(i, time, cost, false));
                addToQueue(i, time, cost, false);
            }
            // continue bike
            time = state->time + graph[node][i] * 7;
            cost = state->cost + graph[node][i] * 7 * 79;
            if (time < M && cost < minCost && visited[i][time][true] > cost)
              //queue.push_back(State(i, time, cost, true));
              addToQueue(i, time, cost, true);
          }
        }
      }

      //queue.pop_front();
      qPopFront();
    }

    if (minCost == UINT_MAX)
      cout << "#" << test_case << " " << -1 << endl;
    else
      cout << "#" << test_case << " " << minCost << endl;
  }

  return 0; // Your program should return 0 on normal termination.
}