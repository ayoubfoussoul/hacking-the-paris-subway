#pragma once

#include <string>
#include <time.h>

class Entry{
protected:
  std::string station_;
  int numArrivals_A_; // A
  int numArrivals_R_; // R

public:
  tm* arrivals_A_;
  tm* arrivals_R_;
  Entry(std::string station, int numArrivals_A, std::string* arrivals_A, int numArrivals_R, std::string* arrivals_R);
  Entry(const Entry& other);
  Entry& operator=(const Entry& other);
  ~Entry();
  std::string getEntryStation();
  int getEntryNumArrivalsA();
  int getEntryNumArrivalsR();
  std::string getFirstArrivalA();
  std::string getFirstArrivalR();
  std::string getSecondArrivalA(std::string first);
  std::string getSecondArrivalR(std::string first);
  void print();
  static std::string pr_time(tm* s);
};
