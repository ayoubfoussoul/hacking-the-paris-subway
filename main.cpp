#include <string>
#include <vector>
#include <time.h>
#include "Network.hpp"
#include "Graph.hpp"
#include "tests.hpp"

using namespace std;

tm estimateNextArrivalTime(Network network, string line, char dir, string station, tm now){
  return network.estimateNextArrivalTime(line, dir, station, now);
}
int duration(Network network, string line, string previousStation_A, string station){
  return network.duration(line, previousStation_A, station);
}
void estimateOptimalPath(Network network, string from, string to, tm now){
  Graph g(network, now);
  unordered_map<string, tuple<string, string, char, long>> path = g.getOptimalPath(from, to);
  Graph::print_path(path, from, to);
}
void drawEstimatedCityTraffic(Network network){
  network.drawEstimatedCityTraffic();
}
void compareToNominalFrequencices(Network network){
  network.compareToNominalFrequencices();
}

int main(){

  //****** TESTING STRUCTURES *******

  //testEntry();
  //testRawEntry();
  //testGraph();
  //testDataset();
  //testNetwork();

  //****** TESTING FUNCTIONALITIES ******

  // functions estimateNextArrivalTime() and duration() are used and have been test manualy in Graph.cpp but you can test them using the following network.

  //city traffic
  unordered_map<string, pair<string, string>> files;
  files["RER B"] = make_pair("RER_B_data.csv","RER_B_structure.csv");
  files["RER A"] = make_pair("RER_A_data.csv","RER_A_structure.csv");
  files["BUS 91"] = make_pair("BUS_91_data.csv","BUS_91_structure.csv");
  Network net(files);
  drawEstimatedCityTraffic(net);
  //Compare to nominal frequencies
  compareToNominalFrequencices(net);
  //path calculation
  time_t now = time(0);
  estimateOptimalPath(net, "Sartrouville", "Denfert Rochereau", *localtime(&now));

}
