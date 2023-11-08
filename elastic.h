#ifndef _ELASTIC_H_
#define _ELASTIC_H_

#include "murmur3.h"
#include "utils.h"

class Elastic : public Base {
public:
  Elastic(int w, int d) : w(w), d(d) {
    key = new uint32_t *[w];
    val = new double *[w];

    for (int i = 0; i < w; i++) {
      key[i] = new uint32_t[d];
      val[i] = new double[d];
    }
    vote = new double[w];
  }

  void clear() {
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < d; j++) {
        key[i][j] = 0;
        val[i][j] = 0;
      }
      vote[i] = 0;
    }
  }

  bool insert(Item &item) {
    uint32_t idx;
    int posMin = 0;
    bool ret = false;

    MurmurHash3_x86_32(&item.key, sizeof(item.key), idxSeed, &idx);
    idx %= w;

    for (int i = 0; i < d; i++) {
      if (key[idx][i] == item.key) {
        if (item.type == 0) {
          val[idx][i] = item.val;
        } else {
          val[idx][i] += item.val;
        }
        return true;
      } else if (fabs(val[idx][i]) < fabs(val[idx][posMin])) {
        posMin = i;
      }
    }

    if (key[idx][posMin] == 0) {
      key[idx][posMin] = item.key;
      val[idx][posMin] = item.val;
      ret = true;
    } else {
      vote[idx] += fabs(item.val);
      if (vote[idx] > lambda * fabs(val[idx][posMin])) {
        key[idx][posMin] = item.key;
        val[idx][posMin] = item.val;
        vote[idx] = 0;
      }
    }
    return ret;
  }

  double query(uint32_t keyItem) {
    uint32_t idx;
    MurmurHash3_x86_32(&keyItem, sizeof(keyItem), idxSeed, &idx);
    idx %= w;

    for (int i = 0; i < d; i++) {
      if (key[idx][i] == keyItem) {
        return val[idx][i];
      }
    }
    return 0;
  }

  double complexity() { return 0; }

public:
  const uint32_t idxSeed = 0x1000;

  const int w;
  const int d;

  uint32_t **key;
  double **val;
  double *vote;

  const double lambda = 8;
};

#endif
