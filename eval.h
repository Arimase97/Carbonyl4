#ifndef _EVAL_H_
#define _EVAL_H_

#include "utils.h"

class Evaluator {
public:
  virtual void eval(unordered_map<uint32_t, double> &, Base *) = 0;
};

template <int K> class TopKEvaluator : public Evaluator {
public:
  void eval(unordered_map<uint32_t, double> &truth, Base *base) {
    double are = 0;
    double aae = 0;
    double mse = 0;
    double recall = 0;

    vector<pair<double, uint32_t>> topk;
    for (auto [key, freq] : truth) {
      topk.push_back(make_pair(fabs(freq), key));
    }

    sort(topk.rbegin(), topk.rend());
    topk.resize(K);

    for (auto [_, key] : topk) {
      double freq = truth[key];
      double esti = base->query(key);
      are += fabs(esti - freq) / fabs(freq);
      aae += fabs(esti - freq);
      mse += pow(esti - freq, 2);
      if (esti != 0) {
        recall += 1.0 / K;
      }
    }
    are /= K;
    aae /= K;
    mse /= K;

    cout << " " << are;
    cout << " " << aae;
    cout << " " << mse;
    cout << " " << recall;
  }
};

class ErrorEvaluator : public Evaluator {
public:
  void eval(unordered_map<uint32_t, double> &truth, Base *base) {
    double are = 0;
    double aae = 0;
    double mse = 0;
    double bias = 0;

    int n_query = 0;
    for (auto [key, freq] : truth) {
      if (freq != 0) {
        double esti = base->query(key);
        n_query += 1;

        are += fabs(esti - freq) / fabs(freq);
        aae += fabs(esti - freq);
        mse += pow(esti - freq, 2);
        bias += esti - freq;
      }
    }
    are /= n_query;
    aae /= n_query;
    mse /= n_query;
    bias /= n_query;

    cout << " " << are;
    cout << " " << aae;
    cout << " " << mse;
    // cout << " " << bias;
  }
};

template <int n_eval, int n_item> class SubsetEvaluator : public Evaluator {
public:
  void eval(unordered_map<uint32_t, double> &truth, Base *base) {
    default_random_engine e(0);
    uniform_int_distribution<int> udist(0, truth.size() - 1);

    double are = 0;
    double aae = 0;
    double mse = 0;

    vector<uint32_t> keys;
    for (auto [key, freq] : truth) {
      keys.push_back(key);
    }

    for (int i = 0; i < n_eval; i++) {
      double freq = 0;
      double esti = 0;
      for (int j = 0; j < n_item; j++) {
        uint32_t key = keys[udist(e)];
        freq += truth[key];
        esti += base->query(key);
      }

      are += fabs(esti - freq) / fabs(freq);
      aae += fabs(esti - freq);
      mse += pow(esti - freq, 2);
    }

    are /= n_eval;
    aae /= n_eval;
    mse /= n_eval;

    cout << " " << are;
    cout << " " << aae;
    cout << " " << mse;
  }

public:
};

#endif
