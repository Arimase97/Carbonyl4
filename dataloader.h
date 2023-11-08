#ifndef _DATALOADER_H_
#define _DATALOADER_H_

#include "utils.h"

class Dataloader {
public:
  Dataloader(string filename) : filename(filename), data(filename) {}

  virtual void clear() = 0;
  virtual bool next(Item &) = 0;
  virtual int card() = 0;
  virtual void reset(double) = 0;

public:
  ifstream data;
  string filename;
};

template <int n_item> class ZipfDataloader : public Dataloader {
public:
  ZipfDataloader(string filename, double R = 0.5)
      : e(0), udist(0, 1), edist(1), ndist(0, 1), Dataloader(filename),
        ratio(R) {
    uint32_t key;
    while (keys.size() < n_item &&
           Dataloader::data.read(reinterpret_cast<char *>(&key), sizeof(key))) {
      keys.push_back(key);
      freq[key] += 1;
    }
    Dataloader::data.close();

    // cout << "Data. : " << keys.size() << " " << freq.size() << endl;
  }

  void reset(double R) { ratio = R; }

  void clear() {
    n_key = 0;
    e.seed(0);
  }

  bool next(Item &item) {
    if (n_key < n_item) {
      item.key = keys[n_key++];

      if (udist(e) < ratio) {
        item.type = 0;
        item.val = mean * edist(e);
      } else {
        item.type = 1;
        item.val = var * ndist(e);
      }
      return true;
    }
    return false;
  }

  int card() { return freq.size(); }

public:
  default_random_engine e;
  uniform_real_distribution<double> udist;
  exponential_distribution<double> edist;
  normal_distribution<double> ndist;

  unordered_map<uint32_t, double> freq;

  vector<uint32_t> keys;
  int n_key = 0;

  const double mean = 10.0;
  const double var = 10.0;

  double ratio;
};

template <int n_item> class CaidaDataloader : public Dataloader {
public:
#pragma pack(push, 1)
  struct Packet {
    uint32_t srcIP;
    uint32_t dstIP;
    uint16_t srcPort;
    uint16_t dstPort;
    uint8_t protocol;

    uint16_t length;
    double timestamp;
  };
#pragma pack(pop)

  CaidaDataloader(string filename, double R = 0.001)
      : Dataloader(filename), thresh(R) {
    Packet pkt;
    while (
        pkts.size() < n_item &&
        Dataloader::data.read(reinterpret_cast<char *>(&pkt), sizeof(Packet))) {
      pkts.push_back(pkt);
      freq[pkt.srcIP] += 1;
    }
    Dataloader::data.close();

    // cout << "Data. : " << pkts.size() << " " << freq.size() << endl;
  }

  void reset(double R) { thresh = R; }

  void clear() {
    n_key = 0;
    lastTS.clear();
  }

  bool next(Item &item) {
    if (n_key < n_item) {
      Packet pkt = pkts[n_key++];
      item.key = pkt.srcIP;
      item.val = pkt.length;

      if (pkt.timestamp - lastTS[pkt.srcIP] > thresh) {
        item.type = 0;
      } else {
        item.type = 1;
      }
      lastTS[pkt.srcIP] = pkt.timestamp;
      return true;
    }
    return false;
  }

  int card() { return freq.size(); }

public:
  unordered_map<uint32_t, double> freq, lastTS;

  vector<Packet> pkts;
  int n_key = 0;

  double thresh;
};

template <int n_item> class WordDataloader : public Dataloader {
public:
  WordDataloader(string filename, double R = 10000)
      : Dataloader(filename), thresh(R) {
    uint64_t key;
    while (keys.size() < n_item &&
           Dataloader::data.read(reinterpret_cast<char *>(&key), sizeof(key))) {
      keys.push_back(key);
      freq[key] += 1;
    }
    Dataloader::data.close();

    // cout << "Data. : " << keys.size() << " " << freq.size() << endl;
  }

  void reset(double R) { thresh = R; }

  void clear() {
    n_key = 0;
    lastTS.clear();
  }

  bool next(Item &item) {
    if (n_key < n_item) {
      item.key = keys[n_key++];
      item.val = 1;

      if (n_key - lastTS[item.key] > thresh) {
        item.type = 0;
      } else {
        item.type = 1;
      }
      lastTS[item.key] = n_key;
      return true;
    }
    return false;
  }

  int card() { return freq.size(); }

public:
  unordered_map<uint32_t, double> freq, lastTS;

  vector<uint32_t> keys;
  int n_key = 0;

  double thresh;
};

template <int n_item> class CriteoDataloader : public Dataloader {
public:
  CriteoDataloader(string filename, double R = 0.5)
      : e(0), udist(0, 1), Dataloader(filename), ratio(R) {
    uint32_t key;
    double val;
    while (keys.size() < n_item && (Dataloader::data >> key >> val)) {
      keys.push_back(key);
      vals.push_back(val);
      freq[key] += val;
    }
    Dataloader::data.close();

    // cout << "Data. : " << keys.size() << " " << freq.size() << endl;
  }

  void reset(double R) { ratio = R; }

  void clear() { n_key = 0; }

  bool next(Item &item) {
    if (n_key < n_item) {
      item.key = keys[n_key];
      item.val = vals[n_key];
      n_key += 1;

      if (udist(e) < ratio) {
        item.type = 0;
      } else {
        item.type = 1;
      }

      return true;
    }
    return false;
  }

  int card() { return freq.size(); }

public:
  default_random_engine e;
  uniform_real_distribution<double> udist;

  unordered_map<uint32_t, double> freq;

  vector<uint32_t> keys;
  vector<double> vals;
  int n_key = 0;

  double ratio;
};

#endif
