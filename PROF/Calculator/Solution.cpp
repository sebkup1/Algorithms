#define MAX_N 1000

extern int my_strlen(const char *a);
extern void my_strcpy(char *a, const char *b);
extern int my_strcmp(const char *a, const char *b);
extern char *my_strcat(char *a, const char *b);

int numcmp(const char *a, const char *b);
void add(char *r, const char *a, const char *b);
void sub(char *r, const char *a, const char *b);
void mul(char *r, const char *a, const char *b);
void div(char *r, const char *a, const char *b);

char temp[MAX_N];
char temp2[MAX_N];
char temp3[MAX_N];
char wynikDzieleniaLok[MAX_N];

struct Factorial {
  char nom[MAX_N], den[MAX_N];
  char sign;
};

Factorial factorials[MAX_N];
unsigned int tmLastind;

Factorial oneFactorial;

void parseOne(char s, int b, int e, const char *str) {
  factorials[tmLastind].sign = s;
  char sign = '*';
  int beg = b, end = b;
  while (end != e) {
    end = beg;
    while (end != e && str[end] >= '0' && str[end] <= '9')  {
      end++;
    }
    int tempi = beg;
    while (tempi < end) {
      temp[tempi - beg] = str[tempi];
      tempi++;
      temp[tempi - beg] = 0;
    }

    if (sign == '*') {
      mul(factorials[tmLastind].nom, factorials[tmLastind].nom, temp);
    }
    else if (sign == '/') {
      mul(factorials[tmLastind].den, factorials[tmLastind].den, temp);
    }
    sign = str[end];
    beg = end + 1;
  }
}

void run(char *rst, const char *str)
{
  // I. parse input to table
  tmLastind = -1;
  int beg = 0, end = 0;
  char sign = '+';
  while (str[end] != 0) {
    end = beg;
    while (str[end] != '+' && str[end] != '-' && str[end] != 0) {
      end++;
    }
    tmLastind++;
    int i = 1;
    while (factorials[tmLastind].den[i] != 0 || factorials[tmLastind].nom[i] != 0) {
      factorials[tmLastind].den[i] = 0;
      factorials[tmLastind].nom[i] = 0;
      i++;
    }
    factorials[tmLastind].den[0] = '1';
    factorials[tmLastind].nom[0] = '1';
    parseOne(sign, beg, end, str);

    sign = str[end];
    beg = end + 1;
  }

  // II. calculate to one factorial
  int i = 0;
  while (oneFactorial.den[i] != 0 || oneFactorial.nom[i] != 0) {
    oneFactorial.den[i] = 0;
    oneFactorial.nom[i] = 0;
    i++;
  }

  oneFactorial.den[0] = '1';
  oneFactorial.nom[0] = 0;

  for (i = 0; i <= tmLastind; i++) {
    int u = 0;
    while (temp[u] != 0) {
      temp[u] = 0; //czyszczenie
      u++;
    }
    temp[0] = '1';

    for (int j = 0; j <= tmLastind; j++) {
      if (j == i) continue;
      mul(temp, temp, factorials[j].den);
    }

    mul(temp, temp, factorials[i].nom);

    // add or sub nominator multiplied by denominator
    if (factorials[i].sign == '-') {
      sub(oneFactorial.nom, oneFactorial.nom, temp);
    }
    else {
      add(oneFactorial.nom, oneFactorial.nom, temp);
    }

    // multiply denominators
    mul(oneFactorial.den, oneFactorial.den, factorials[i].den);
  }

  // III. calculate result from oneFactorial
  div(rst, oneFactorial.nom, oneFactorial.den);

  int decPoints = 0;
  mul(temp, rst, oneFactorial.den);
  if (my_strcmp(temp, oneFactorial.nom)){
    my_strcpy(wynikDzieleniaLok, rst);
    my_strcat(rst, ".");
  }
  else return;

  my_strcpy(temp3, oneFactorial.nom);

  while (true) {
    mul(temp2, wynikDzieleniaLok, oneFactorial.den);
    sub(temp, temp3, temp2);

    if (!my_strcmp(temp, "0") || decPoints == 20) return;
    decPoints++;
    my_strcat(temp, "0");
    div(temp3, temp, oneFactorial.den);
    my_strcat(rst, temp3);
    my_strcpy(wynikDzieleniaLok, temp3);
    my_strcpy(temp3, temp); // przepisuwanie
  }
}

int numcmp(const char *a, const char *b)
{
  if (a[0] != '-' && b[0] != '-') {
    int c = my_strlen(a);
    int d = my_strlen(b);
    if (c > d)
      return 1;
    if (c < d)
      return -1;
    do {
      if (*b < *a)
        return 1;
      if (*a < *b++)
        return -1;
    } while (*++a != 0);
  }
  else if (a[0] == '-' && b[0] == '-')
    return numcmp(&b[1], &a[1]);
  else if (a[0] == '-')
    return -1;
  else
    return 1;
  return 0;
}

void add(char *r, const char *a, const char *b)
{
  if (a[0] != '-' && b[0] != '-') {
    if (0 <= numcmp(a, b)) {
      int tmp[MAX_N];
      int pos_a = my_strlen(a);
      int pos_b = my_strlen(b);
      int carry = 0;
      int len = 0;
      while (0 < pos_a) {
        int num1 = a[--pos_a] - '0';
        int num2 = (0 < pos_b) ? b[--pos_b] - '0' : 0;
        int num3 = carry + num1 + num2;
        tmp[len++] = num3 % 10;
        carry = num3 / 10;
      }
      if (0 < carry)
        tmp[len++] = 1;
      for (int i = 0; i < len; ++i)
        r[i] = tmp[len - 1 - i] + '0';
      r[len] = 0;
    }
    else
      add(r, b, a);
  }
  else if (a[0] == '-' && b[0] == '-') {
    add(&r[1], &a[1], &b[1]);
    r[0] = '-';
  }
  else if (a[0] == '-')
    sub(r, b, &a[1]);
  else
    sub(r, a, &b[1]);
}

void sub(char *r, const char *a, const char *b)
{
  if (a[0] != '-' && b[0] != '-') {
    if (0 <= numcmp(a, b)) {
      int tmp[MAX_N];
      int pos_a = my_strlen(a);
      int pos_b = my_strlen(b);
      int carry = 0;
      int len = 0;
      while (0 < pos_a) {
        int num1 = a[--pos_a] - '0';
        int num2 = (0 < pos_b) ? b[--pos_b] - '0' : 0;
        int num3 = 10 - carry + num1 - num2;
        tmp[len++] = num3 % 10;
        carry = 1 - (num3 / 10);
      }
      while (1 < len && tmp[len - 1] == 0)
        --len;
      for (int i = 0; i < len; ++i)
        r[i] = tmp[len - 1 - i] + '0';
      r[len] = 0;
    }
    else {
      sub(&r[1], b, a);
      r[0] = '-';
    }
  }
  else if (a[0] == '-' && b[0] == '-')
    sub(r, &b[1], &a[1]);
  else if (a[0] == '-') {
    add(&r[1], &a[1], b);
    r[0] = '-';
  }
  else
    add(r, a, &b[1]);
}

void mul(char *r, const char *a, const char *b)
{
  if (numcmp(a, "0") == 0 || numcmp(b, "0") == 0)
    my_strcpy(r, "0");
  else if (a[0] != '-' && b[0] != '-') {
    char cpy_a[MAX_N + 1];
    my_strcpy(cpy_a, a);
    my_strcpy(r, "0");
    for (int i = my_strlen(b) - 1; i >= 0; --i) {
      for (int j = 0; j < b[i] - '0'; ++j)
        add(r, r, cpy_a);
      my_strcat(cpy_a, "0");
    }
  }
  else if (a[0] == '-' && b[0] == '-')
    mul(r, &a[1], &b[1]);
  else {
    if (a[0] == '-')
      mul(&r[1], &a[1], b);
    else
      mul(&r[1], a, &b[1]);
    r[0] = '-';
  }
}

void div(char *r, const char *a, const char *b)
{
  if (numcmp(a, "0") == 0 || numcmp(b, "0") == 0)
    my_strcpy(r, "0");
  else if (numcmp(((a[0] != '-') ? a : &a[1]), ((b[0] != '-') ? b : &b[1])) == -1)
    my_strcpy(r, "0");
  else if (a[0] != '-' && b[0] != '-') {
    char cpy_a[MAX_N + 1];
    char cpy_b[MAX_N + 1];
    char itr[MAX_N + 1];
    my_strcpy(cpy_a, a);
    my_strcpy(r, "");
    int c = (my_strlen(a) > my_strlen(b)) ? my_strlen(a) - my_strlen(b) : 1;
    for (int i = 0; i < c; ++i) {
      my_strcpy(cpy_b, b);
      for (int j = 0; j < c - i - 1; ++j)
        my_strcat(cpy_b, "0");
      my_strcpy(itr, "0");
      while (0 <= numcmp(cpy_a, "0")) {
        add(itr, itr, "1");
        sub(cpy_a, cpy_a, cpy_b);
      }
      sub(itr, itr, "1");
      add(cpy_a, cpy_a, cpy_b);
      my_strcat(r, itr);
    }
  }
  else if (a[0] == '-' && b[0] == '-')
    div(r, &a[1], &b[1]);
  else {
    if (a[0] == '-')
      div(&r[1], &a[1], b);
    else
      div(&r[1], a, &b[1]);
    r[0] = '-';
  }
}
