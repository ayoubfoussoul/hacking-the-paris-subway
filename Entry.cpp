#include "Entry.hpp"
#include <iostream>

using namespace std;

Entry::Entry(string station, int numArrivals_A, string* arrivals_A, int numArrivals_R, string* arrivals_R) : station_(station), numArrivals_A_(numArrivals_A), numArrivals_R_(numArrivals_R){
    arrivals_A_ = new tm[numArrivals_A_];
    for(int i = 0; i < numArrivals_A_; i++){
      strptime((arrivals_A[i]+"00").c_str(), "%Y%m%d%H%M%S", &arrivals_A_[i]);
    }
    arrivals_R_ = new tm[numArrivals_R_];
    for(int i = 0; i < numArrivals_R_; i++){
      strptime((arrivals_R[i]+"00").c_str(), "%Y%m%d%H%M%S", &arrivals_R_[i]);
    }
}


void Entry::print(){
  cout << "***** Entry *****" << endl <<endl;
  cout << "Station : "  << station_ << endl << endl;
  cout << "Number of arrivals in A direction : " << numArrivals_A_ << endl;
  cout << "Arrivals in A direction :" << endl;
  for(int i = 0; i < numArrivals_A_; i++){
    cout << "==> " << pr_time(&arrivals_A_[i]) << endl;
  }
  cout << endl << endl;
  cout << "Number of arrivals in R direction : " << numArrivals_R_ << endl;
  cout << "Arrivals in R direction :" << endl;
  for(int i = 0; i < numArrivals_R_; i++){
    cout << "==> " << pr_time(&arrivals_R_[i]) << endl;
  }
  cout << endl;

}

Entry::Entry(const Entry& other) : station_(other.station_), numArrivals_A_(other.numArrivals_A_), numArrivals_R_(other.numArrivals_R_){
  arrivals_A_ = new tm[numArrivals_A_];
  for(int i = 0; i < numArrivals_A_; i++)
    arrivals_A_[i] = other.arrivals_A_[i];

  arrivals_R_ = new tm[numArrivals_R_];
  for(int i = 0; i < numArrivals_R_; i++)
    arrivals_R_[i] = other.arrivals_R_[i];
}
Entry& Entry::operator=(const Entry& other){
  return *this = Entry(other);
}

Entry::~Entry(){
  delete[] arrivals_A_;
  delete[] arrivals_R_;
}

std::string Entry::getEntryStation(){
  return station_;
}

int Entry::getEntryNumArrivalsA(){
  return numArrivals_A_;
}

int Entry::getEntryNumArrivalsR(){
  return numArrivals_R_;
}

string Entry::getFirstArrivalA(){
  char buffer[20];
  strftime(buffer, 20, "%Y%m%d%H%M" ,&arrivals_A_[0]);
  return buffer;
}

string Entry::getFirstArrivalR(){
  char buffer[20];
  strftime(buffer, 20, "%Y%m%d%H%M" ,&arrivals_R_[0]);
  return buffer;
}

string Entry::getSecondArrivalA(string first){
  char buffer[20];
  strftime(buffer, 20, "%Y%m%d%H%M" ,&arrivals_A_[0]);
  if(first == buffer){
    strftime(buffer, 20, "%Y%m%d%H%M" ,&arrivals_A_[1]);
  }
  return buffer;
}

string Entry::getSecondArrivalR(string first){
  char buffer[20];
  strftime(buffer, 20, "%Y%m%d%H%M" ,&arrivals_R_[0]);
  if(first == buffer){
    strftime(buffer, 20, "%Y%m%d%H%M" ,&arrivals_R_[1]);
  }
  return buffer;
}

string Entry::pr_time(tm* t){
  string s = asctime(t);
  s[s.length()-1] = '\0';
  return s;
}
