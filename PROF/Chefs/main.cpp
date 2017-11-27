#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAXCHEF     32
#define MAXORDER    10000

#define OK          1
#define NOTOK       0

#define SAMPLE_CUTLINE     51
#define EVAL_CUTLINE       74      // it can be changed in evaluation.

struct Order {
	int ordertime;
	int price;
	int cookingtime;
	int waitingtime;
	int chefid;
	int done;
};

static int M;                       // the number of orders
static int orderidx;
static Order order[MAXORDER + 1];

struct Chef {
	int orderid;
	int startcookingtime;
};

static int N;                       // the number of chefs
static Chef chef[MAXCHEF + 1];

static int endtime;
static int currenttime;
static int earned;
static int total;
static int okay;

extern void init(int N);
extern void takeorder(int orderid, int ordertime, int price, int cookingtime, int waitingtime);
extern void tick(int currenttime);

// The below commented code is reference code for testcase generation.
// It is provided for your reference and it does not work.
//
// int M;  // the number of orders
//
// for (int idx = 1; idx <= M; ) {
//   order[idx].ordertime = rand() % (MAXTIME + 1);
//   order[idx].price = rand() % (MAXPRICE - MINPRICE + 1) + MINPRICE;
//   order[idx].cookingtime = rand() % (MAXCOOKINGTIME - MINCOOKINGTIME + 1) + MINCOOKINGTIME;
//   order[idx].waitingtime = rand() % (MAXWAITINGTIME - order[idx].cookingtime + 1) + order[idx].cookingtime;
//   order[idx].chefid = -1;
//   order[idx].done = NOTOK;
//
//   if (order[idx].ordertime + order[idx].waitingtime > MAXTIME) continue;
//   idx++;
// }
//
// sort(order + 1, order + M + 1);  // sort by ordertime

int startcook(int chefid, int orderid) {
	if (chefid <= 0 || chefid > N)
		return okay = NOTOK;

	if (orderid <= 0 || orderid > M || order[orderid].ordertime > currenttime || order[orderid].chefid != -1)
		return okay = NOTOK;

	if (chef[chefid].orderid != -1) {
		order[chef[chefid].orderid].cookingtime -= currenttime - chef[chefid].startcookingtime;
		order[chef[chefid].orderid].chefid = -1;
	}

	chef[chefid].orderid = orderid;
	chef[chefid].startcookingtime = currenttime;

	order[orderid].chefid = chefid;

	return OK;
}

int finishcook(int chefid) {
	if (chefid <= 0 || chefid > N || chef[chefid].orderid == -1)
		return okay = NOTOK;

	int orderid = chef[chefid].orderid;
	if (order[orderid].done == OK)
		return okay = NOTOK;

	int elapsedtime = currenttime - chef[chefid].startcookingtime;
	if (elapsedtime < order[orderid].cookingtime)
		return okay = NOTOK;

	if (currenttime > order[orderid].ordertime + order[orderid].waitingtime)
		return okay = NOTOK;

	earned += order[orderid].price;
	order[orderid].done = OK;

	chef[chefid].orderid = -1;

	return OK;
}

static void read() {
	scanf("%d %d %d", &N, &M, &endtime);

	total = 0;
	for (int idx = 1; idx <= M; ++idx) {
		scanf("%d %d %d %d", &order[idx].ordertime,
			&order[idx].price,
			&order[idx].cookingtime,
			&order[idx].waitingtime);
		order[idx].chefid = -1;
		order[idx].done = NOTOK;
		total += order[idx].price;
	}
}

static void doticking(int currenttime) {
	while (orderidx <= M && order[orderidx].ordertime == currenttime) {
		takeorder(orderidx,
			order[orderidx].ordertime,
			order[orderidx].price,
			order[orderidx].cookingtime,
			order[orderidx].waitingtime);
		orderidx++;
	}

	tick(currenttime);
}

int main() {
	setbuf(stdout, NULL);

	int TC, totalscore;

	freopen("sample_input.txt", "r", stdin);
	scanf("%d", &TC);

	totalscore = 0;
	for (int testcase = 1; testcase <= TC; ++testcase) {
		read();

		for (int idx = 1; idx <= N; ++idx)
			chef[idx].orderid = -1;

		earned = 0;
		orderidx = 1;

		init(N);

		for (currenttime = 0, okay = OK; okay == OK && currenttime <= endtime; ++currenttime)
			doticking(currenttime);

		int point = okay == OK ? earned * 100 / total : 0;
		totalscore += point >= EVAL_CUTLINE ? 100 : 0;

		printf("#%d %d\n", testcase, earned * 100 / total);
	}

	printf("total score = %d\n", totalscore / TC);
	return 0;
}