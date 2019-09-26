#pragma once
#include <string>
#include <sstream>
#include <time.h>
#include "Entry.hpp"

class RawEntry: public Entry{
private:
  int numPerturbations_;
  tm moment_;
  std::string* arrivalsDescriptions_A_;
  std::string* arrivalsDescriptions_R_;
  std::string* perturbations_;

public:
  RawEntry(std::string data);
  RawEntry(const RawEntry& other);
  RawEntry& operator=(const RawEntry& other);
  RawEntry();
  ~RawEntry();
  std::string getMoment();
  tm get_tm_moment();
  std::string getFirstDescriptionA();
  std::string getFirstDescriptionR();
  void print();
};
