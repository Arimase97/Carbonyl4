#ifndef _COCO_H_
#define _COCO_H_

#include "murmur3.h"
#include "utils.h"

class Coco : public Base {
public:
  Coco(int w, int d) : w(w), d(d), e(0), udist(0, 1) {
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
  }

  bool insert(Item &item) {
    uint32_t idx;
    int posMin = -1;
    double valueMin = 0;
    bool ret = false;

    for (int i = 0; i < d; i++) {
      MurmurHash3_x86_32(&item.key, sizeof(item.key), i + idxSeed, &idx);
      // MurmurHash3_x86_32(&item.key, sizeof(item.key), 0 + idxSeed, &idx);
      idx %= w;
      if (key[idx][i] == item.key) {
        if (item.type == 0) {
          val[idx][i] = item.val;
        } else {
          val[idx][i] += item.val;
        }
        return true;
      } else if (posMin == -1 || fabs(val[idx][i]) < valueMin) {
        valueMin = fabs(val[idx][i]);
        posMin = i;
      }
    }

    MurmurHash3_x86_32(&item.key, sizeof(item.key), posMin + idxSeed, &idx);
    // MurmurHash3_x86_32(&item.key, sizeof(item.key), 0 + idxSeed, &idx);
    idx %= w;
    if (key[idx][posMin] == 0) {
      ret = true;
    }
    if (udist(e) < fabs(item.val) / (fabs(item.val) + fabs(val[idx][posMin]))) {
      key[idx][posMin] = item.key;
      val[idx][posMin] =
          sgn(item.val) * (fabs(item.val) + fabs(val[idx][posMin]));
    } else {
      key[idx][posMin] = key[idx][posMin];
      val[idx][posMin] =
          sgn(val[idx][posMin]) * (fabs(item.val) + fabs(val[idx][posMin]));
    }
    return ret;
  }

  double query(uint32_t keyItem) {
    uint32_t idx;

    for (int i = 0; i < d; i++) {
      MurmurHash3_x86_32(&keyItem, sizeof(keyItem), i + idxSeed, &idx);
      // MurmurHash3_x86_32(&keyItem, sizeof(keyItem), 0 + idxSeed, &idx);
      idx %= w;
      if (key[idx][i] == keyItem) {
        return val[idx][i];
      }
    }
    return 0;
  }

  double complexity() { return 0; }

public:
  default_random_engine e;
  uniform_real_distribution<double> udist;

  const uint32_t idxSeed = 0x1000;

  const int w;
  const int d;
  uint32_t **key;
  double **val;
};

#endif
