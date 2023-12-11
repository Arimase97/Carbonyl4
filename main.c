#include "carbonyl.h"
#include "coco.h"
#include "cuckoo.h"
#include "dataloader.h"
#include "elastic.h"
#include "eval.h"
#include "experiment.h"
#include "uss.h"
#include "utils.h"
#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

const int default_n = 10000000;

const int default_d = 4;

const int default_K = 1000;
const int default_N = 10000;
const int default_S = 10;

const int default_M = 100;
const double default_E = 0.02;

const double default_L1 = 1.2;
const double default_L2 = 3.0;

const char *default_data = "./zipf_0.9.dat";
const char *zipfian_data[11] = {
    "/share/zipf_2022/zipf_0.0.dat", "/share/zipf_2022/zipf_0.1.dat",
    "/share/zipf_2022/zipf_0.2.dat", "/share/zipf_2022/zipf_0.3.dat",
    "/share/zipf_2022/zipf_0.4.dat", "/share/zipf_2022/zipf_0.5.dat",
    "/share/zipf_2022/zipf_0.6.dat", "/share/zipf_2022/zipf_0.7.dat",
    "/share/zipf_2022/zipf_0.8.dat", "/share/zipf_2022/zipf_0.9.dat",
    "/share/zipf_2022/zipf_1.0.dat",
};

void comprativeExperimemt(double mem, Dataloader *data, Evaluator *eval) {
  Experimenter<default_n> *expr = new Experimenter<default_n>();

  int w1 = floor(mem / (default_d * (4 + 8)));
  int w2 = floor(mem / (4 + 8 + 8 * 4));
  int w3 = floor(mem / (default_d * (8 + 4) + 8));

  Base *carb4 = new Carbonyl(w1, default_d, default_M, default_E);
  Base *cuckoo = new Cuckoo(w1, default_d, default_M);
  Base *coco = new Coco(w1, default_d);
  Base *uss = new USS(w2);
  Base *elastic = new Elastic(w3, default_d);

  cout << "Ours";
  expr->experiment(data, carb4, eval);
  cout << endl;

  cout << "Cuckoo";
  expr->experiment(data, cuckoo, eval);
  cout << endl;

  cout << "Coco";
  expr->experiment(data, coco, eval);
  cout << endl;

  cout << "USS";
  expr->experiment(data, uss, eval);
  cout << endl;

  cout << "Elastic";
  expr->experiment(data, elastic, eval);
  cout << endl;
}

void parameterExperiment(Dataloader *data, Base *carb1, Base *carb2) {
  Experimenter<default_n> *expr = new Experimenter<default_n>();

  Evaluator *errEval = new ErrorEvaluator();
  Evaluator *topEval = new TopKEvaluator<default_K>();
  Evaluator *setEval = new SubsetEvaluator<default_N, default_S>();

  cout << "Error";
  expr->experiment(data, carb1, errEval);
  cout << " " << carb1->complexity() << endl;

  cout << "TopK";
  expr->experiment(data, carb2, topEval);
  cout << " " << carb2->complexity() << endl;

  cout << "Subset";
  expr->experiment(data, carb1, setEval);
  cout << " " << carb1->complexity() << endl;
}

void compressExperiment(double mem, Dataloader *data, Evaluator *eval) {
  Experimenter<default_n> *expr = new Experimenter<default_n>();

  int w = floor(mem / (default_d * (4 + 8)));
  Carbonyl *carb1 = new Carbonyl(w, default_d, default_M, default_E);
  Carbonyl *carb2 = new Carbonyl(w, default_d, default_M, default_E);
  Carbonyl *carb3 = new Carbonyl(w, default_d, default_M, default_E);
  Carbonyl *carb4 = new Carbonyl(w, default_d, default_M, default_E);

  cout << "Shrink1";
  expr->experiment(data, carb1, eval);
  auto beg1 = system_clock::now();
  carb1->shrink1();
  auto end1 = system_clock::now();
  eval->eval(expr->truth, carb1);
  cout << endl;

  cout << "Shrink2";
  expr->experiment(data, carb2, eval);
  auto beg2 = system_clock::now();
  carb2->shrink2();
  auto end2 = system_clock::now();
  eval->eval(expr->truth, carb2);
  cout << " "
       << 1.0 * duration_cast<nanoseconds>(end1 - beg1).count() /
              duration_cast<nanoseconds>(end2 - beg2).count()
       << endl;

  cout << "Shrink3";
  expr->experiment(data, carb3, eval);
  auto beg3 = system_clock::now();
  carb3->shrink3();
  auto end3 = system_clock::now();
  eval->eval(expr->truth, carb3);
  cout << " "
       << 1.0 * duration_cast<nanoseconds>(end1 - beg1).count() /
              duration_cast<nanoseconds>(end3 - beg3).count()
       << endl;

  cout << "Shrink4";
  expr->experiment(data, carb4, eval);
  auto beg4 = system_clock::now();
  carb4->shrink4();
  auto end4 = system_clock::now();
  eval->eval(expr->truth, carb4);
  cout << " "
       << 1.0 * duration_cast<nanoseconds>(end1 - beg1).count() /
              duration_cast<nanoseconds>(end4 - beg4).count()
       << endl;
}

void memoryZipfErrorExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);
  Evaluator *eval = new ErrorEvaluator();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryZipfErrorExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryZipfTopKExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);
  Evaluator *eval = new TopKEvaluator<default_K>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryZipfTopKExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryZipfSubsetExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);
  Evaluator *eval = new SubsetEvaluator<default_N, default_S>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryZipfSubsetExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}
void memoryCAIDAErrorExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new CaidaDataloader<default_n>("./normalized_001.dat");
  Evaluator *eval = new ErrorEvaluator();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCAIDAErrorExpr " << load << endl;
    double mem = data->card() / load * (4 + 8);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryCAIDATopKExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new CaidaDataloader<default_n>("./normalized_001.dat");
  Evaluator *eval = new TopKEvaluator<default_K>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCAIDATopKExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryCAIDASubsetExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new CaidaDataloader<default_n>("./normalized_001.dat");
  Evaluator *eval = new SubsetEvaluator<default_N, default_S>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCAIDASubsetExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryWordErrorExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new WordDataloader<default_n>("./webdocs_form00.dat");
  Evaluator *eval = new ErrorEvaluator();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryWordErrorExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryWordTopKExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new WordDataloader<default_n>("./webdocs_form00.dat");
  Evaluator *eval = new TopKEvaluator<default_K>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryWordTopKExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryWordSubsetExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new WordDataloader<default_n>("./webdocs_form00.dat");
  Evaluator *eval = new SubsetEvaluator<default_N, default_S>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryWordSubsetExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryCriteoErrorExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new CriteoDataloader<default_n>("./criteo.dat");
  Evaluator *eval = new ErrorEvaluator();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCriteoErrorExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryCriteoTopKExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new CriteoDataloader<default_n>("./criteo.dat");
  Evaluator *eval = new TopKEvaluator<default_K>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCriteoTopKExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void memoryCriteoSubsetExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new CriteoDataloader<default_n>("./criteo.dat");
  Evaluator *eval = new SubsetEvaluator<default_N, default_S>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCriteoSubsetExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void ratioZipfErrorExpr(double minRatio, double maxRatio, double stepRatio) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);
  Evaluator *eval = new ErrorEvaluator();

  for (double ratio = minRatio; ratio <= maxRatio + 1e-6; ratio += stepRatio) {
    cout << "ratioZipfErrorExpr " << ratio << endl;
    double mem = data->card() / default_L1 * (8 + 4);
    data->reset(ratio);
    comprativeExperimemt(mem, data, eval);
  }
}

void skewZipfErrorExpr(int minSkew, int maxSkew, int stepSkew) {
  Evaluator *eval = new ErrorEvaluator();

  for (int skew = minSkew; skew <= maxSkew; skew += stepSkew) {
    cout << "skewZipfErrorExpr " << skew << endl;
    Dataloader *data = new ZipfDataloader<default_n>(zipfian_data[skew]);

    double mem = data->card() / default_L1 * (8 + 4);
    comprativeExperimemt(mem, data, eval);
  }
}

void param_D_ZipfErrorExpr(int minD, int maxD, int stepD) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);

  for (int d = minD; d <= maxD; d += stepD) {
    cout << "param_D_ZipfErrorExpr " << d << endl;
    double mem1 = data->card() / default_L1 * (8 + 4);
    double mem2 = data->card() / default_L2 * (8 + 4);
    int w1 = floor(mem1 / (d * (4 + 8)));
    int w2 = floor(mem2 / (d * (4 + 8)));

    Base *carb1 = new Carbonyl(w1, d, default_M, default_E);
    Base *carb2 = new Carbonyl(w2, d, default_M, default_E);
    parameterExperiment(data, carb1, carb2);
  }
}

void param_M_ZipfErrorExpr(double minM, double maxM, double stepM) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);

  for (double M = minM; M <= maxM + 1e-6; M += stepM) {
    cout << "param_M_ZipfErrorExpr " << M << endl;
    double mem1 = data->card() / default_L1 * (8 + 4);
    double mem2 = data->card() / default_L2 * (8 + 4);
    int w1 = floor(mem1 / (default_d * (4 + 8)));
    int w2 = floor(mem2 / (default_d * (4 + 8)));

    Base *carb1 = new Carbonyl(w1, default_d, M, default_E);
    Base *carb2 = new Carbonyl(w2, default_d, M, default_E);
    parameterExperiment(data, carb1, carb2);
  }
}

void param_E_ZipfErrorExpr(double minE, double maxE, double stepE) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);

  for (double E = minE; E <= maxE + 1e-6; E += stepE) {
    cout << "param_E_ZipfErrorExpr " << E << endl;
    double mem1 = data->card() / default_L1 * (8 + 4);
    double mem2 = data->card() / default_L2 * (8 + 4);
    int w1 = floor(mem1 / (default_d * (4 + 8)));
    int w2 = floor(mem2 / (default_d * (4 + 8)));

    Base *carb1 = new Carbonyl(w1, default_d, default_M, E);
    Base *carb2 = new Carbonyl(w2, default_d, default_M, E);
    parameterExperiment(data, carb1, carb2);
  }
}

void memoryCompressErrorExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);
  Evaluator *eval = new ErrorEvaluator();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCompressErrorExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    compressExperiment(mem, data, eval);
  }
}

void memoryCompressTopKExpr(double minLoad, double maxLoad, double stepLoad) {
  Dataloader *data = new ZipfDataloader<default_n>(default_data);
  Evaluator *eval = new TopKEvaluator<default_K>();

  for (double load = minLoad; load <= maxLoad + 1e-6; load += stepLoad) {
    cout << "memoryCompressTopKExpr " << load << endl;
    double mem = data->card() / load * (8 + 4);
    compressExperiment(mem, data, eval);
  }
}

int main() {
  memoryZipfErrorExpr(0.9, 1.2, 0.03);
  memoryZipfTopKExpr(1.0, 5.0, 0.4);
  memoryZipfSubsetExpr(0.9, 1.2, 0.03);

  memoryCAIDAErrorExpr(0.9, 1.2, 0.03);
  memoryCAIDATopKExpr(1.0, 5.0, 0.4);
  memoryCAIDASubsetExpr(0.9, 1.2, 0.03);

  memoryWordErrorExpr(0.9, 1.2, 0.03);
  memoryWordTopKExpr(1.0, 5.0, 0.4);
  memoryWordSubsetExpr(0.9, 1.2, 0.03);

  memoryCriteoErrorExpr(0.9, 1.2, 0.03);
  memoryCriteoTopKExpr(1.0, 5.0, 0.4);
  memoryCriteoSubsetExpr(0.9, 1.2, 0.03);

  ratioZipfErrorExpr(0.0, 1.0, 0.1);
  skewZipfErrorExpr(0, 10, 1);

  param_D_ZipfErrorExpr(1, 11, 1);
  param_M_ZipfErrorExpr(0, 20, 2);
  param_E_ZipfErrorExpr(0, 0.5, 0.05);

  memoryCompressErrorExpr(1.0, 2.0, 0.1);
  memoryCompressTopKExpr(2.0, 4.0, 0.2);

  return 0;
}
