#include "Dataset.hpp"

using namespace std;

Dataset::Dataset( string line, string dataFile, string linePlan ) : line_(line){

   ifstream dataF(dataFile);
   ifstream planF(linePlan);

  if(dataF.fail()){
     cout<<"Cannot read from file " << dataFile << " !"<< endl;
     exit(1);
   }
   else if(planF.fail()){
     cout<<"Cannot read from file " << linePlan << " !"<< endl;
     exit(1);
   }
   else{

        string tmpStation, station, prevA, prevR;
        while(getline(planF, tmpStation)){
            stringstream iss(tmpStation);
            getline(iss ,station,',');
            getline(iss ,prevA,',');
            getline(iss ,prevR,',');
            stations_.push_back(make_tuple(prevA, station, prevR));
        }
        string data;
        getline(dataF,data); // skip first line
        while (getline(dataF,data)){
            if(verify(data)) // verifies if the line is in the right format means the moment is indicated in data
              nominalData_.emplace_back(data);
        }
    }
    fillRealData();
}

bool Dataset::verify(string data){
  // the string should be like NNNNNNNNNNNN,?,number,NNNNNNNNNNNN,?,NNNNNNNNNNNN,?,... (number times),,,,number,NNNNNNNNNNNN,?,NNNNNNNNNNNN,?,...
  stringstream iss(data);
  string val;
  getline(iss,val,',');
  if(!isValidNumber(val, 12))
    return false;
  getline(iss,val,',');
  getline(iss,val,',');
  if(!isValidNumber(val, 1))
    return false;
  int num = atoi(val.c_str());
  for(int i = 0; i < num; i++){
    getline(iss,val,',');
    if(!isValidNumber(val, 12))
      return false;
    getline(iss,val,',');
  }
  for(int i = 0; i < 8-num; i++){
    getline(iss,val,',');
    getline(iss,val,',');
  }
  getline(iss,val,',');
  if(!isValidNumber(val, 1))
    return false;
  num = atoi(val.c_str());
  for(int i = 0; i < num; i++){
    getline(iss,val,',');
    if(!isValidNumber(val, 12))
      return false;
    getline(iss,val,',');
  }
  for(int i = 0; i < 8-num; i++){
    getline(iss,val,',');
    getline(iss,val,',');
  }
  getline(iss,val,',');
  if(!isValidNumber(val, 1))
    return false;
  return true;
}

bool Dataset::isValidNumber(string val, int size){
  if(val.size() != size){
    return false;
  }
  else{
    for(char c : val)
      if(!isdigit(c))
        return false;
  }
  return true;
}

int campare(string T1, string T2){
  return stol(T1) - stol(T2);
}

bool smallDifference(string T1, string T2){
  return (stol(T1) < stol(T2) + 3 ) && (stol(T2) < stol(T1) + 3);
}

void Dataset::fillRealData(){
  unordered_map<string, tuple<vector<string>,vector<string>>> realData;
  bool aQuaisA, aQuaisR;

  for(RawEntry re : nominalData_){
    if(re.getEntryNumArrivalsA() > 0){
      if(re.getFirstDescriptionA() != "Train sans arrêt"){
        vector<string>* v = &get<0>(realData[re.getEntryStation()]);
        string moment(re.getMoment()), first(re.getFirstArrivalA()), station(re.getEntryStation()), back;
        if(v->size() == 0){
          v->push_back(first);
        }
        else{

          back = v->back();
          if((campare(moment,back) < 0 && campare(back, first) <= 0) || (campare(moment, first) < 0 && campare(first, back) <= 0) || (campare(moment, first) == 0 && campare(first, back) < 0)){ // update back
            v->pop_back();
            v->push_back(first);
          }
          else if((campare(moment, back) == 0 && campare(back, first) < 0) || (campare(moment, back) > 0 && campare(back, first) <= 0)){
            if(smallDifference(back, first) && !aQuaisA){
              v->pop_back();
              v->push_back(first);
            }
            else{
              v->push_back(first);
            }
          }
        }
        aQuaisA = (re.getFirstDescriptionA() == "Train à quai");
      }
    }

    if(re.getEntryNumArrivalsR() > 0){
      if(re.getFirstDescriptionR() != "Train sans arrêt"){
        vector<string>* v = &get<1>(realData[re.getEntryStation()]);
        string moment(re.getMoment()), first(re.getFirstArrivalR()), station(re.getEntryStation()), back;

        if(v->size() == 0){
          v->push_back(first);
        }
        else{

          back = v->back();
          if((campare(moment,back) < 0 && campare(back, first) <= 0) || (campare(moment, first) < 0 && campare(first, back) <= 0) || (campare(moment, first) == 0 && campare(first, back) < 0)){ // update back
            v->pop_back();
            v->push_back(first);
          }
          else if((campare(moment, back) == 0 && campare(back, first) < 0) || (campare(moment, back) > 0 && campare(back, first) <= 0)){
            if(smallDifference(back, first) && !aQuaisR){
              v->pop_back();
              v->push_back(first);
            }
            else{
              v->push_back(first);
            }
          }
        }
        aQuaisR = (re.getFirstDescriptionR() == "Train à quai");
      }
    }

  }

  for(auto st : realData){
    string arrivalsA[get<0>(st.second).size()];
    string arrivalsR[get<1>(st.second).size()];
    for(int i = 0; i < get<0>(st.second).size(); i++){
      arrivalsA[i] = get<0>(st.second).at(i);
    }
    for(int i = 0; i < get<1>(st.second).size(); i++){
      arrivalsR[i] = get<1>(st.second).at(i);
    }
    Entry test(st.first, get<0>(st.second).size(), arrivalsA, get<1>(st.second).size(), arrivalsR);
    realData_.push_back(test);
  }


}

vector<station>* Dataset::getStations(){
    return &stations_;
}
string Dataset::getLine(){
    return line_;
}

vector<RawEntry>* Dataset::getNominalData(){
  return &nominalData_;
}

int Dataset::getNominalDataSize(){
  return nominalData_.size();
}

vector<Entry>* Dataset::getRealData(){
  return &realData_;
}

void Dataset::print(){
  cout << "***** Dataset for line : " << line_ << " *****" << endl << endl;
  cout << "Stations : " << endl;
  for(station station: stations_){
    cout << "Previous in A direction : " << get<0>(station) << " | station : " << get<1>(station) << " | Previous in R direction : " <<get<2>(station) << endl;
  }
  cout << endl;
  cout << "Raw Entries : " << endl;
  for(RawEntry entry: nominalData_){
    cout << endl;
    entry.print();
  }

  cout << endl;
  cout << "Real Entries : " << endl;

  for(Entry entry: realData_){
    cout << endl;
    entry.print();
  }
}
