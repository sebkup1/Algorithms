#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

long minSum;

struct OrcsGroup
{
  int size;
  int toll;
  OrcsGroup(int s, int t) : size(s), toll(t) {};
  OrcsGroup() { size = 0; toll = 0; }
};

class GroupNode
{
public:
  int size;
  int battles;

  GroupNode* next;

  void add(GroupNode* newNode)
  {
    if (next != nullptr)
      next->add(newNode);
    else
      next = newNode;
  }

  GroupNode(int size) : size(size), battles(0), next(nullptr) {}
};

GroupNode* copy(GroupNode* node)
{
  if (node == nullptr) return nullptr;

  GroupNode* newNode = new GroupNode(*node);

  if (node->next != nullptr)
    newNode->next = copy(node->next);

  return newNode;
}

int countOwnTrups(GroupNode* node)
{
  int trups = 0;
  if (node == nullptr) return trups;

  if (node->next != nullptr)
    return node->size + countOwnTrups(node->next);
  else
    return node->size;

}

GroupNode* batleAftermatch(int enemyOrcs, GroupNode* head)
{
  if (head == nullptr) return nullptr;
  GroupNode* temp = head;
  GroupNode* newHead = head;
  GroupNode* toDelete;

  if (temp->size > enemyOrcs)
  {
    temp->size -= enemyOrcs;
    goto IncNumberOfBattles; 
  }

  while (true)
  {
    if (enemyOrcs == 0) break;

    if (temp->size <= enemyOrcs)
    {
      enemyOrcs -= temp->size;
      toDelete = temp;
      temp = temp->next;
      newHead = temp;
      delete toDelete;
      if (temp == nullptr) return nullptr;
    }
    else
    {
      temp->size -= enemyOrcs;
      break;
    }

  }

  // 3 batles
IncNumberOfBattles:
  temp = newHead;
  while (true)
  {
    if (temp == nullptr) break;

    temp->battles++;
    if (temp->battles == 3)
    {
      newHead = temp->next;
      GroupNode* toDelete = temp;
      temp = temp->next;
      delete toDelete;
      continue;
    }
    temp = temp->next;
  }

  return newHead;
}

void deleteGroup(GroupNode* group)
{
  if (group == nullptr) return;

  if (group->next != nullptr)
    deleteGroup(group->next);

  delete group;
}

GroupNode* first = nullptr;

void processGroups(int N, int currentGroup, OrcsGroup* orcsGroups, GroupNode* head, int moneySpend)
{
  // last Group
  if (currentGroup == N)
  {
    if (minSum > moneySpend)
      minSum = moneySpend;
    return;
  }

  if (moneySpend > minSum) return;

  int money;
  GroupNode* newHead;

  // batel (if number of own orc grater or equal than enemy)
  int ownTrups = countOwnTrups(head);
  if (ownTrups >= orcsGroups[currentGroup].size)
  {
    money = moneySpend;
    newHead = copy(head);
    newHead = batleAftermatch(orcsGroups[currentGroup].size, newHead);
    processGroups(N, currentGroup + 1, orcsGroups, newHead, money);
    deleteGroup(newHead);
  }

  // hire
  money = moneySpend;
  money += ((orcsGroups[currentGroup].toll) * 2);
  newHead = copy(head);

  if (newHead != nullptr)
  {
    newHead->add(new GroupNode(orcsGroups[currentGroup].size));
  }
  else
  {
    newHead = new GroupNode(orcsGroups[currentGroup].size);
  }
  processGroups(N, currentGroup + 1, orcsGroups, newHead, money);
  deleteGroup(newHead);

  // pass
  money = moneySpend;
  money += orcsGroups[currentGroup].toll;
  newHead = copy(head);
  processGroups(N, currentGroup + 1, orcsGroups, newHead, money);
  deleteGroup(newHead);
}

int main(int argc, char** argv)
{
  int moneySum = 0;
  int test_case;
  int T;
  int N;

  freopen("C:\\Users\\Sebastian\\Documents\\Visual Studio 2012\\Projects\\mordor\\Debug\\sample_input.txt", "r", stdin);

  std::ios::sync_with_stdio(false);


  cin >> T;
  for (test_case = 1; test_case <= T; test_case++)
  {
    minSum = LONG_MAX;
    cin >> N;

    OrcsGroup* orcsGroup = new OrcsGroup[N];

    for (int i = 0; i < N; i++)
    {
      cin >> orcsGroup[i].size;
      cin >> orcsGroup[i].toll;
    }

    processGroups(N, 0, orcsGroup, first, 0);


    cout << "#" << test_case << " " << minSum << endl;

    delete[] orcsGroup;
  }

  return 0; // Your program should return 0 on normal termination.
}