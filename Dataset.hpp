#pragma once

#include <string>
#include <sstream>
#include <time.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include "Entry.hpp"
#include "RawEntry.hpp"

typedef std::tuple <std::string, std::string, std::string> station; // (previous station A, station, previous station R)  [station = "" if does not exist]

class Dataset
{
private:
  std::string line_;
  std::vector<RawEntry> nominalData_; // table of raw entrys (raw entry is a time/station/station data on that time : see raweEntry.hpp)
  std::vector<Entry> realData_; // real entries : station / actual times of arrivals
  std::vector<station> stations_; // in this case numStations will be stations_.size()
  void fillRealData();
  bool verify(std::string val);
  bool isValidNumber(std::string val, int size);

public:
  Dataset( std::string line, std::string dataFile, std::string linePlan );
  //~Dataset();

  std::vector<station>* getStations();
  std::vector<RawEntry>* getNominalData();
  int getNominalDataSize();
  std::vector<Entry>* getRealData();
  std::string getLine();
  void print();

};
