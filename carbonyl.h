#ifndef _CARBONYL_H_
#define _CARBONYL_H_

#include "murmur3.h"
#include "utils.h"

class Carbonyl : public Base {
public:
  Carbonyl(int w, int d, int M, double E)
      : w(w), d(d), M(M), E(E), e(0), udist(0, 1) {
    if (this->w % 2 != 0) {
      this->w += 1;
    }
    key = new uint32_t *[this->w];
    val = new double *[this->w];

    for (int i = 0; i < this->w; i++) {
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
    n_item = 0;
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
      if (step > 0) {
        n_cuckoo += 1;
      }
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
    }
    return false;
  }

  void findMin(uint32_t idx, uint32_t &pos_0, uint32_t &pos_1) {
    if (fabs(val[idx][0]) < fabs(val[idx][1])) {
      pos_0 = 0;
      pos_1 = 1;
    } else {
      pos_0 = 1;
      pos_1 = 0;
    }
    for (int i = 2; i < d; i++) {
      if (fabs(val[idx][i]) < fabs(val[idx][pos_0])) {
        pos_1 = pos_0;
        pos_0 = i;
      } else if (fabs(val[idx][i]) < fabs(val[idx][pos_1])) {
        pos_1 = i;
      }
    }
  }

  bool mergeItem(Item &item, uint32_t idx) {
    double curMin = 0;
    int stepMin = -1;
    bool ret = false;

    Item itemCur = item;
    uint32_t idxCur = idx;

    for (int step = 0; step <= M; step++) {
      if (step > 0) {
        n_cuckoo += 1;
      }
      uint32_t pos_0, pos_1;
      findMin(idxCur, pos_0, pos_1);
      // cout << "idx: " << idxCur << endl;
      // cout << "pos_0: " << pos_0 << " " << val[idxCur][pos_0] << endl;
      // cout << "pos_1: " << pos_1 << " " << val[idxCur][pos_1] << endl;

      if (stepMin < 0 ||
          fabs(itemCur.val) * fabs(val[idxCur][pos_0]) < curMin) {
        curMin = fabs(itemCur.val) * fabs(val[idxCur][pos_0]);
        stepMin = step;
      } else if (fabs(val[idxCur][pos_0]) * fabs(val[idxCur][pos_1]) < curMin) {
        curMin = fabs(val[idxCur][pos_0]) * fabs(val[idxCur][pos_1]);
        stepMin = step;
      } else if (udist(e) < E) {
        break;
      }

      itemCur.key = key[idxCur][pos_0];
      itemCur.val = val[idxCur][pos_0];
      idxCur = otherIdx(itemCur, idxCur);
    }

    itemCur = item;
    idxCur = idx;

    for (int step = 0; step < stepMin; step++) {
      uint32_t pos_0, pos_1;
      findMin(idxCur, pos_0, pos_1);
      // cout << "\tidx: " << idxCur << endl;
      // cout << "\tpos_0: " << pos_0 << " " << val[idxCur][pos_0] << endl;
      // cout << "\tpos_1: " << pos_1 << " " << val[idxCur][pos_1] << endl;

      swap(itemCur.key, key[idxCur][pos_0]);
      swap(itemCur.val, val[idxCur][pos_0]);
      idxCur = otherIdx(itemCur, idxCur);
    }

    uint32_t pos_0, pos_1;
    findMin(idxCur, pos_0, pos_1);
    if (fabs(key[idxCur][pos_0]) == 0) {
      ret = true;
    }

    if (fabs(itemCur.val) < fabs(val[idxCur][pos_1])) {
      bool replace = false;
      if (udist(e) <
          fabs(itemCur.val) / (fabs(itemCur.val) + fabs(val[idxCur][pos_0]))) {
        replace = true;
      }

      if (replace) {
        key[idxCur][pos_0] = itemCur.key;
        val[idxCur][pos_0] =
            sgn(itemCur.val) * (fabs(itemCur.val) + fabs(val[idxCur][pos_0]));
      } else {
        key[idxCur][pos_0] = key[idxCur][pos_0];
        val[idxCur][pos_0] = sgn(val[idxCur][pos_0]) *
                             (fabs(itemCur.val) + fabs(val[idxCur][pos_0]));
      }
    } else {
      bool replace = false;
      if (udist(e) < fabs(val[idxCur][pos_0]) / (fabs(val[idxCur][pos_0]) +
                                                 fabs(val[idxCur][pos_1]))) {
        replace = true;
      }

      if (replace) {
        key[idxCur][pos_1] = key[idxCur][pos_0];
        val[idxCur][pos_1] =
            sgn(val[idxCur][pos_0]) *
            (fabs(val[idxCur][pos_0]) + fabs(val[idxCur][pos_1]));
      } else {
        key[idxCur][pos_1] = key[idxCur][pos_1];
        val[idxCur][pos_1] =
            sgn(val[idxCur][pos_1]) *
            (fabs(val[idxCur][pos_0]) + fabs(val[idxCur][pos_1]));
      }
      key[idxCur][pos_0] = itemCur.key;
      val[idxCur][pos_0] = itemCur.val;
    }
    return ret;
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
      return mergeItem(item, idx_0 % w);
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

  void shrink1() {
    assert(w % 2 == 0);

    vector<Item> items;
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < d; j++) {
        if (key[i][j] != 0) {
          Item item;
          item.key = key[i][j];
          item.val = val[i][j];
          items.push_back(item);
        }
      }
    }
    w /= 2;

    clear();
    for (auto item : items) {
      insert(item);
    }
  }

  void shrink2() {
    assert(w % 2 == 0);

    double error_1 = 0;
    double error_2 = 0;
    w /= 2;
    for (int i = 0; i < w; i++) {
      vector<Item> items;
      for (int j = 0; j < d; j++) {
        Item item;
        item.key = key[i][j];
        item.val = val[i][j];
        if (item.key != 0) {
          items.push_back(item);
        }
        item.key = key[w + i][j];
        item.val = val[w + i][j];
        if (item.key != 0) {
          items.push_back(item);
        }
        key[i][j] = key[w + i][j] = 0;
        val[i][j] = val[w + i][j] = 0;
      }
      // map<uint32_t, double> mp_1;
      // for (int i = 0; i < items.size(); i++) {
      //   mp_1[items[i].key] = items[i].val;
      // }
      while (items.size() > d) {
        sort(items.rbegin(), items.rend());
        Item item_1 = items[items.size() - 1];
        Item item_2 = items[items.size() - 2];
        // error_1 += 2 * fabs(item_1.val) * fabs(item_2.val);
        if (udist(e) <
            fabs(item_1.val) / (fabs(item_1.val) + fabs(item_2.val))) {
          items[items.size() - 2].key = item_1.key;
          items[items.size() - 2].val =
              sgn(item_1.val) * (fabs(item_1.val) + fabs(item_2.val));
        } else {
          items[items.size() - 2].key = item_2.key;
          items[items.size() - 2].val =
              sgn(item_2.val) * (fabs(item_1.val) + fabs(item_2.val));
        }
        items.pop_back();
      }
      for (int j = 0; j < items.size(); j++) {
        key[i][j] = items[j].key;
        val[i][j] = items[j].val;
      }
      // map<uint32_t, double> mp_2;
      // for (int j = 0; j < items.size(); j++) {
      //   mp_2[key[i][j]] = val[i][j];
      // }
      // for (auto [key, val] : mp_1) {
      //   error_2 += pow(val - mp_2[key], 2);
      // }
    }
    // cout << endl << error_1 << " " << error_2 << endl;
  }

  void shrink3() {
    assert(w % 2 == 0);

    w /= 2;
    double error_1 = 0;
    double error_2 = 0;
    for (int i = 0; i < w; i++) {
      vector<Item> items;
      for (int j = 0; j < d; j++) {
        Item item;
        item.key = key[i][j];
        item.val = val[i][j];
        if (item.key != 0) {
          items.push_back(item);
        }
        item.key = key[w + i][j];
        item.val = val[w + i][j];
        if (item.key != 0) {
          items.push_back(item);
        }
        key[i][j] = key[w + i][j] = 0;
        val[i][j] = val[w + i][j] = 0;
      }

      if (items.size() > d) {
        vector<pair<double, int>> prio;
        for (int j = 0; j < items.size(); j++) {
          prio.push_back(make_pair(fabs(items[j].val) / udist(e), j));
        }
        nth_element(prio.rbegin(), prio.rbegin() + prio.size() - (d + 1),
                    prio.rend());

        // for (int j = 0; j < items.size(); j++) {
        //   int pos = prio[j].second;
        //   if (fabs(items[pos].val) < prio[d].first) {
        //     error_1 +=
        //         (prio[d].first - fabs(items[pos].val)) *
        //         fabs(items[pos].val);
        //   }
        // }
        // for (int j = 0; j < items.size(); j++) {
        //   int pos = prio[j].second;
        //   if (fabs(items[pos].val) < prio[d].first) {
        //     if (prio[j].first > prio[d].first) {
        //       // if (fabs(items[pos].val) / udist(e) > prio[d].first) {
        //       error_2 += pow(prio[d].first - fabs(items[pos].val), 2);
        //     } else {
        //       error_2 += pow(items[pos].val, 2);
        //     }
        //   }
        // }

        for (int j = 0; j < d; j++) {
          int pos = prio[j].second;
          key[i][j] = items[pos].key;
          if (fabs(items[pos].val) > prio[d].first) {
            val[i][j] = items[pos].val;
          } else {
            val[i][j] = sgn(items[pos].val) * prio[d].first;
          }
        }
      } else {
        for (int j = 0; j < items.size(); j++) {
          key[i][j] = items[j].key;
          val[i][j] = items[j].val;
        }
      }
    }
    // cout << endl << error_1 << " " << error_2 << endl;
  }

  void shrink4() {
    assert(w % 2 == 0);

    w /= 2;
    double error_1 = 0;
    double error_2 = 0;
    for (int i = 0; i < w; i++) {
      vector<Item> items;
      for (int j = 0; j < d; j++) {
        Item item;
        item.key = key[i][j];
        item.val = val[i][j];
        if (item.key != 0) {
          items.push_back(item);
        }
        item.key = key[w + i][j];
        item.val = val[w + i][j];
        if (item.key != 0) {
          items.push_back(item);
        }
        key[i][j] = key[w + i][j] = 0;
        val[i][j] = val[w + i][j] = 0;
      }
      if (items.size() > d) {
        sort(items.rbegin(), items.rend());
        double weight = 0;
        for (int j = 0; j < items.size(); j++) {
          weight += fabs(items[j].val);
        }
        int k_1 = 0, k_2 = d;
        while (true) {
          if (k_2 * fabs(items[k_1].val) >= weight) {
            key[i][k_1] = items[k_1].key;
            val[i][k_1] = items[k_1].val;
            weight -= fabs(items[k_1].val);
            k_1 += 1;
            k_2 -= 1;
          } else {
            break;
          }
        }

        random_shuffle(items.begin() + k_1, items.end());
        // for (int j = k_1; j < items.size(); j++) {
        //   error_1 += (1 / (k_2 * fabs(items[j].val) / weight) - 1) *
        //              pow(items[j].val, 2);
        // }

        double prob[items.size() + 1];
        for (int j = k_1; j < items.size(); j++) {
          if (j == k_1) {
            prob[j] = 0;
          }
          prob[j + 1] = prob[j] + k_2 * fabs(items[j].val) / weight;
        }

        double r = udist(e);
        for (int j = k_1; j < items.size(); j++) {
          double thresh_1 = floor(prob[j]) + r;
          double thresh_2 = floor(prob[j + 1]) + r;
          if ((prob[j] < thresh_1 && thresh_1 < prob[j + 1]) ||
              (prob[j] < thresh_2 && thresh_2 < prob[j + 1])) {
            key[i][k_1] = items[j].key;
            val[i][k_1] = sgn(items[j].val) * weight / k_2;
            k_1 += 1;
            // error_2 += pow(weight / k_2 - fabs(items[j].val), 2);
          } else {
            // error_2 += pow(fabs(items[j].val), 2);
          }
        }
        assert(k_1 == d);
      } else {
        for (int j = 0; j < items.size(); j++) {
          key[i][j] = items[j].key;
          val[i][j] = items[j].val;
        }
      }
    }
    // cout << endl << error_1 << " " << error_2 << endl;
  }

public:
  default_random_engine e;
  uniform_real_distribution<double> udist;

  const uint32_t idxSeed = 0x1000;

  int w;
  const int d;
  uint32_t **key;
  double **val;

  double n_cuckoo;
  int n_item;

  const int M;
  const double E;
};

#endif
