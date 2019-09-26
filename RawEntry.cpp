#include "RawEntry.hpp"
#include <iostream>

using namespace std;

RawEntry::RawEntry(string data) : Entry("", 0, 0, 0, 0){
  // converting data to stream
  stringstream iss(data);

  // getting moment
  string val;
  getline(iss,val,',');
  strptime((val+"00").c_str(), "%Y%m%d%H%M%S", &moment_); // redo

  // getting station name
  getline(iss,val,',');
  station_ = val;

  // getting number of arrivals R
  getline(iss,val,',');
  numArrivals_R_ = atoi(val.c_str());

  // getting arrivals R
  arrivals_R_ = new tm[numArrivals_R_];
  arrivalsDescriptions_R_ = new string[numArrivals_R_];
  for(int i = 0; i < numArrivals_R_; i++){
    getline(iss,val,',');
    strptime((val+"00").c_str(), "%Y%m%d%H%M%S", &arrivals_R_[i]);
    getline(iss,val,',');
    arrivalsDescriptions_R_[i] = val;
  }

  // skiping empty fields
  getline(iss,val,',');
  while (val == "")
    getline(iss,val,',');

  // getting number of arrivals A
  numArrivals_A_ = atoi(val.c_str());

  // getting arrivals A
  arrivals_A_ = new tm[numArrivals_A_];
  arrivalsDescriptions_A_ = new string[numArrivals_A_];
  for(int i = 0; i < numArrivals_A_; i++){
      getline(iss,val,',');
      strptime((val+"00").c_str(), "%Y%m%d%H%M%S", &arrivals_A_[i]);
      getline(iss,val,',');
      arrivalsDescriptions_A_[i] = val;
  }
  // skiping empty fields
  getline(iss,val,',');
  while (val == "")
      getline(iss,val,',');

  // getting number of perturbations
  numPerturbations_ = atof(val.c_str());

  // getting perturbations
  perturbations_ = new string[numPerturbations_];
  for(int i = 0; i < numPerturbations_; i++){
      getline(iss,val,',');
      perturbations_[i] = val;
  }

}

RawEntry::RawEntry(const RawEntry& other) : Entry("", 0, 0, 0, 0){

  station_ = other.station_ ;
  numArrivals_A_ = other.numArrivals_A_;
  numArrivals_R_ = other.numArrivals_R_;
  numPerturbations_ = other.numPerturbations_;
  moment_ = other.moment_;

  arrivals_A_ = new tm[numArrivals_A_];
  for(int i = 0; i < numArrivals_A_; i++)
    arrivals_A_[i] = other.arrivals_A_[i];

  arrivals_R_ = new tm[numArrivals_R_];
  for(int i = 0; i < numArrivals_R_; i++)
    arrivals_R_[i] = other.arrivals_R_[i];

  arrivalsDescriptions_A_ = new string[numArrivals_A_];
  for(int i = 0; i < numArrivals_A_; i++)
    arrivalsDescriptions_A_[i] = other.arrivalsDescriptions_A_[i];

  arrivalsDescriptions_R_ = new string[numArrivals_R_];
  for(int i = 0; i < numArrivals_R_; i++)
    arrivalsDescriptions_R_[i] = other.arrivalsDescriptions_R_[i];

  perturbations_ = new string[numPerturbations_];
  for(int i = 0; i < numPerturbations_; i++)
    perturbations_[i] = other.perturbations_[i];
}
RawEntry& RawEntry::operator=(const RawEntry& other){
  return *this = RawEntry(other);
}

RawEntry::~RawEntry(){
  delete[] arrivalsDescriptions_A_;
  delete[] arrivalsDescriptions_R_;
  delete[] perturbations_;
};

string RawEntry::getMoment(){
  char buffer[20];
  strftime(buffer, 20, "%Y%m%d%H%M" ,&moment_);
  return buffer;
}

string RawEntry::getFirstDescriptionA(){
  return arrivalsDescriptions_A_[0];
}

string RawEntry::getFirstDescriptionR(){
  return arrivalsDescriptions_R_[0];
}

tm RawEntry::get_tm_moment(){
  return moment_;
}

void RawEntry::print(){
  cout << "***** Raw Entry *****" << endl << endl;
  cout << "Station : "  << station_ << endl;
  cout << "Moment : "  << pr_time(&moment_) << endl << endl;
  cout << "Number of arrivals in A direction : " << numArrivals_A_ << endl;
  cout << "Arrivals in A direction :" << endl;
  for(int i = 0; i < numArrivals_A_; i++){
    cout << "==> " << pr_time(&arrivals_A_[i]) << " : " << arrivalsDescriptions_A_[i] << "" << endl;
  }
  cout << endl;
  cout << "Number of arrivals in R direction : " << numArrivals_R_ << endl;
  cout << "Arrivals in R direction :" << endl;
  for(int i = 0; i < numArrivals_R_; i++){
    cout << "==> " << pr_time(&arrivals_R_[i]) << " : " << arrivalsDescriptions_R_[i] << "" << endl;
  }
  cout << endl;
  cout << "Number of perturbations : " << numPerturbations_ << endl;
  cout << "Perturbations :" << endl;
  for(int i = 0; i < numPerturbations_; i++){
    cout << "==> " << perturbations_[i] << endl;
  }
}
