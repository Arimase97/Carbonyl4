#ifndef _USS_H_
#define _USS_H_

#include "murmur3.h"
#include "utils.h"

class USS : public Base {
public:
  USS(int w) : w(w), e(0), udist(0, 1) {}

  void clear() {
    uss.clear();
    iter.clear();
  }

  bool insert(Item &item) {
    Item cell;
    if (iter.count(item.key) > 0) {
      cell = *iter[item.key];
      uss.erase(iter[cell.key]);
      iter.erase(cell.key);
      if (item.type == 0) {
        cell.val = 0;
      }
    } else if (uss.size() < w) {
      cell.key = 0;
      cell.val = 0;
    } else {
      cell = *uss.begin();
      uss.erase(iter[cell.key]);
      iter.erase(cell.key);
    }

    if (udist(e) < fabs(item.val) / (fabs(item.val) + fabs(cell.val))) {
      cell.key = item.key;
      cell.val = sgn(item.val) * (fabs(item.val) + fabs(cell.val));
    } else {
      cell.key = cell.key;
      cell.val = sgn(cell.val) * (fabs(item.val) + fabs(cell.val));
    }
    uss.insert(cell);
    iter[cell.key] = uss.find(cell);

    return true;
  }

  double query(uint32_t keyItem) {
    if (iter.count(keyItem) > 0) {
      return (*iter[keyItem]).val;
    } else {
      return 0;
    }
  }

  double complexity() { return 0; }

public:
  default_random_engine e;
  uniform_real_distribution<double> udist;

  const int w;
  set<Item> uss;
  unordered_map<uint32_t, set<Item>::iterator> iter;
};

#endif
