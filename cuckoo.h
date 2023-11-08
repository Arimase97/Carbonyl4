#ifndef _CUCKOO_H_
#define _CUCKOO_H_

#include "murmur3.h"
#include "utils.h"

class Cuckoo : public Base {
public:
  Cuckoo(int w, int d, int M) : w(w), d(d), M(M), e(0), udist(0, 1) {
    key = new uint32_t *[w];
    val = new double *[w];

    for (int i = 0; i < w; i++) {
      key[i] = new uint32_t[d];
      val[i] = new double[d];
    }
  }

  void clear() {
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < d; j++) {
        key[i][j] = 0;
        val[i][j] = 0;
      }
    }
    n_cuckoo = 0;
  }

  bool findItem(Item &item, uint32_t idx) {
    for (int i = 0; i < d; i++) {
      if (key[idx][i] == item.key) {
        if (item.type == 0) {
          val[idx][i] = item.val;
        } else {
          val[idx][i] += item.val;
        }
        return true;
      }
    }
    return false;
  }

  bool findEmpty(Item &item, uint32_t idx) {
    for (int i = 0; i < d; i++) {
      if (key[idx][i] == 0) {
        key[idx][i] = item.key;
        val[idx][i] = item.val;
        return true;
      }
    }
    return false;
  }

  double queryValue(uint32_t keyItem, uint32_t idx) {
    for (int i = 0; i < d; i++) {
      if (key[idx][i] == keyItem) {
        return val[idx][i];
      }
    }
    return 0;
  }

  int otherIdx(Item &item, uint32_t idx) {
    uint32_t idx_0, idx_1;
    MurmurHash3_x86_32(&item.key, sizeof(item.key), 0 + idxSeed, &idx_0);
    MurmurHash3_x86_32(&item.key, sizeof(item.key), 1 + idxSeed, &idx_1);
    if (idx == idx_0 % w) {
      idx = idx_1 % w;
    } else {
      idx = idx_0 % w;
    }
    return idx;
  }

  bool cuckooItem(Item &item, uint32_t idx) {
    int idxCur = idx;
    Item itemCur = item;
    for (int step = 0; step <= M; step++) {
      int posEmpty = -1;
      for (int i = 0; i < d; i++) {
        if (key[idxCur][i] == 0) {
          posEmpty = i;
        }
      }
      if (posEmpty >= 0) {
        key[idxCur][posEmpty] = itemCur.key;
        val[idxCur][posEmpty] = itemCur.val;
        return true;
      } else {
        uint32_t posRand = floor(udist(e) * d);
        swap(key[idxCur][posRand], itemCur.key);
        swap(val[idxCur][posRand], itemCur.val);
        idxCur = otherIdx(itemCur, idxCur);
      }
      n_cuckoo += 1;
    }
    return false;
  }

  bool insert(Item &item) {
    uint32_t idx_0, idx_1;
    MurmurHash3_x86_32(&item.key, sizeof(item.key), 0 + idxSeed, &idx_0);
    MurmurHash3_x86_32(&item.key, sizeof(item.key), 1 + idxSeed, &idx_1);
    if (udist(e) < 1.0 / 2.0) {
      swap(idx_0, idx_1);
    }

    n_item += 1;
    if (findItem(item, idx_0 % w)) {
      return true;
    } else if (findItem(item, idx_1 % w)) {
      return true;
    } else if (findEmpty(item, idx_0 % w)) {
      return true;
    } else if (findEmpty(item, idx_1 % w)) {
      return true;
    } else {
      return cuckooItem(item, idx_0 % w);
    }
  }

  double query(uint32_t keyItem) {
    uint32_t idx_0, idx_1;
    MurmurHash3_x86_32(&keyItem, sizeof(keyItem), 0 + idxSeed, &idx_0);
    MurmurHash3_x86_32(&keyItem, sizeof(keyItem), 1 + idxSeed, &idx_1);

    double val_0 = queryValue(keyItem, idx_0 % w);
    double val_1 = queryValue(keyItem, idx_1 % w);
    if (val_0 != 0) {
      return val_0;
    } else if (val_1 != 0) {
      return val_1;
    } else {
      return 0;
    }
  }

  double complexity() { return n_cuckoo / n_item; }

public:
  default_random_engine e;
  uniform_real_distribution<double> udist;

  const uint32_t idxSeed = 0x1000;

  const int w;
  const int d;
  uint32_t **key;
  double **val;

  double n_cuckoo;
  int n_item;

  const int M;
};

#endif
