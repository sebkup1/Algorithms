#include <iostream>
#define MAXCHEF     32
#define MAXORDER    10000

// API
extern int startcook(int chefid, int orderid);
extern int finishcook(int chefid);

using namespace std;

struct Order;

struct Chef
{
  int chiefId;
  Order* order;
  Chef() : order(nullptr) {}

};

struct Order
{
  bool valid;
  Chef* chef;
  int orderid;
  int ordertime;
  int price;
  int cookingtime;
  int waitingtime;
  Order() : chef(nullptr) {}

};


static Order orders[MAXORDER + 1];

static int ordersCount, chefsCount, lowestOrder;
static Chef chefs[MAXCHEF + 1];

Order* findBestFreeOrder();
Order* findWorseOccupiedOrder();

void createOrder(int orderId, Chef* gchef, int gordertime, int gprice, int gcookingtime, int gwaitingtime) {

  if (gcookingtime > gwaitingtime)
  {
    orders[orderId - 1].valid = false;
  }

  orders[orderId - 1].valid = true;
  orders[orderId - 1].chef = gchef;
  orders[orderId - 1].orderid = orderId;
  orders[orderId - 1].ordertime = gordertime;
  orders[orderId - 1].price = gprice;
  orders[orderId - 1].cookingtime = gcookingtime;
  orders[orderId - 1].waitingtime = gwaitingtime;
}


// User Implementation

void init(int N) {
  chefsCount = N;
  ordersCount = lowestOrder = 0;

  for (int i = 0; i < MAXCHEF; i++)
  {
    chefs[i].order = nullptr;
    chefs[i].chiefId = i + 1;
  }

  for (int i = 0; i < MAXORDER; i++)
  {
    orders[i].chef = nullptr;
    orders[i].valid = false;
  }

}

void takeorder(int orderid, int ordertime, int price, int cookingtime, int waitingtime) {

  // create order
  createOrder(orderid, nullptr, ordertime, price, cookingtime, waitingtime);
  ordersCount++;

  if (ordersCount != orderid)
    cout << "orders mismach" << endl;

}

void tick(int currenttime) {
  int lowest;

  if (lowestOrder == 0)
    lowest = -1;
  else
    lowest = lowestOrder;

  // decrement
  for (int i = lowestOrder; i < ordersCount; i++)
  {
    if (orders[i].valid && currenttime != orders[i].ordertime)
    {
      orders[i].waitingtime--;

      if (orders[i].chef != nullptr)
      {
        orders[i].cookingtime--;

        if (orders[i].cookingtime == 0)
        {
          // finishcook
          if (!finishcook(orders[i].chef->chiefId))
            std::cout << "chef " << orders[i].chef->chiefId << " hasn't finished order " << i << endl;

          orders[i].valid = false;
          orders[i].chef->order = nullptr;
          orders[i].chef = nullptr;
        }

      }

      if (orders[i].waitingtime < orders[i].cookingtime)
      {
        if (orders[i].chef != nullptr)
          cout << "coœ nie tak" << endl;

        orders[i].chef = nullptr;
        orders[i].valid = false;

      }
    }

    if (!orders[i].valid && lowest + 1 == i)
      lowest++;
  }

  lowestOrder = lowest;


  // get best orders for free chefs
  int i;
  for (i = 0; i < chefsCount; i++)
  {
    if (chefs[i].order == nullptr)
    {
      Order* bestOrder = findBestFreeOrder();
      if (bestOrder == nullptr)
        break;
      else
      {
        bestOrder->chef = &chefs[i];
        chefs[i].order = bestOrder;
        startcook(chefs[i].chiefId, bestOrder->orderid);
      }

    }
  }

  // all chefs are busy
  if (i == chefsCount)
  {
    Order* bestOrder = findBestFreeOrder();
    Order* worseOrder = findWorseOccupiedOrder();

    while (bestOrder != nullptr && worseOrder != nullptr)
    {
      double ratioB = (double)bestOrder->price / (double)bestOrder->cookingtime;
      double ratioW = (double)worseOrder->price / (double)worseOrder->cookingtime;

      if (ratioB > ratioW)
      {
        finishcook(worseOrder->chef->chiefId);
        startcook(worseOrder->chef->chiefId, bestOrder->orderid);
        bestOrder->chef = worseOrder->chef;
        worseOrder->chef = nullptr;
      }
      else
        break;

      bestOrder = findBestFreeOrder();
      worseOrder = findWorseOccupiedOrder();
    }
  }

}


Order* findBestFreeOrder()
{
  int index = -1;
  double maxRatio = 0;

  for (int i = lowestOrder; i < ordersCount; i++)
  {
    if (orders[i].valid && orders[i].chef == nullptr)
    {
      double ratio = (double)orders[i].price / (double)orders[i].cookingtime;
      if (maxRatio < ratio)
      {
        maxRatio = ratio;
        index = i;
      }
    }
  }

  if (index == -1)
    return nullptr;
  else
    return &orders[index];
}

Order* findWorseOccupiedOrder()
{
  int index = -1;
  double minRatio = 1001;

  for (int i = lowestOrder; i < ordersCount; i++)
  {
    if (orders[i].valid && orders[i].chef != nullptr)
    {
      double ratio = (double)orders[i].price / (double)orders[i].cookingtime;
      if (minRatio > ratio)
      {
        minRatio = ratio;
        index = i;
      }
    }
  }

  if (index == -1)
    return nullptr;
  else
    return &orders[index];
}