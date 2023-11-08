#ifndef _EXPERIMENT_H_
#define _EXPERIMENT_H_

#include "dataloader.h"
#include "eval.h"
#include "utils.h"

template <int n_item> class Experimenter {
public:
  void experiment(Dataloader *data, Base *base, Evaluator *eval) {
    data->clear();
    base->clear();
    truth.clear();

    for (int i = 0; i < n_item; i++) {
      Item item;
      data->next(item);

      if (item.type == 0) {
        truth[item.key] = item.val;
      } else {
        truth[item.key] += item.val;
      }

      base->insert(item);
    }

    eval->eval(truth, base);
  }

public:
  unordered_map<uint32_t, double> truth;
};

#endif
