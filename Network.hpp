#pragma once

#include <string>
#include <vector>
#include <utility>
#include <time.h>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <cmath>
#include <stdlib.h>
#include <utility>
#include <limits>
#include "Dataset.hpp"

typedef std::unordered_map<std::string, std::vector<std::tuple<long, double>>> Cloud; // moment, error

class Network{
private:
  std::vector<std::string> lines_;
  std::unordered_map<std::string, Dataset> datasets_;

public:
  Network(std::unordered_map<std::string, std::pair<std::string, std::string>> files);
  //~Network();

  static int differenceTime(tm &t1, tm &t2);
  std::unordered_map<std::string, Dataset>* getData();

  tm estimateNextArrivalTime(std::string line, char dir, std::string station, tm now); // if not found in data it returns 5 hours to keep the graph working
  int duration(std::string line, std::string previousStation_A, std::string station); // in minutes
  void drawEstimatedCityTraffic();
  void compareToNominalFrequencices();
  void print();
};
