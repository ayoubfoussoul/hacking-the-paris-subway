#pragma once

#include <string>
#include <set>
#include <vector>
#include <tuple>
#include <time.h>
#include <utility>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iostream>
#include "Network.hpp"

typedef std::vector<std::tuple<std::string, std::string, char, long>> Edges; // (target station,line,direction,distance [in minutes])

class Graph
{
private:
  std::set<std::string> vertices_;
  std::unordered_map<std::string,Edges> edges_;

public:
  Graph(Network network, tm now);
  // vvvv constructor to test without a network vvvvv
  Graph(std::set<std::string> vertices, std::unordered_map<std::string,Edges> edges): vertices_(vertices), edges_(edges){};
  //~Graph();
  std::unordered_map<std::string, std::tuple<std::string, std::string, char, long>> getOptimalPath(std::string source, std::string target); //  vector of (station, to , dir , line)
  void print();
  static void print_path(std::unordered_map<std::string, std::tuple<std::string, std::string, char, long>> path, std::string source, std::string target);
};
