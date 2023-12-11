#ifndef _UTILS_H_
#define _UTILS_H_

#include <arpa/inet.h>
#include <bits/stdc++.h>

using namespace std;

class Item {
public:
  uint8_t type = false;
  uint32_t key = 0;
  double val = 0;

  bool operator<(const Item &other) const {
    return fabs(val) < fabs(other.val) ||
           (fabs(val) == fabs(other.val) && key < other.key);
  }
};

class Base {
public:
  virtual void clear() = 0;
  virtual bool insert(Item &) = 0;
  virtual double query(uint32_t) = 0;
  virtual double complexity() = 0;
};

double sgn(double val) { return val >= 0 ? 1.0 : -1.0; }

#endif
