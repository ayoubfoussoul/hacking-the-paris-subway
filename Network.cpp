#include "Network.hpp"

using namespace std;

Network::Network(unordered_map<string, pair<string, string>> files){ // (line, (file of line data, file of line structure))
  for(unordered_map<string, pair<string, string>>::iterator it = files.begin(); it != files.end(); it++){
    lines_.push_back(it->first);
    Dataset ds(it->first, it->second.first , it->second.second);
    datasets_.emplace(it->first, ds);
  }
}

double distA(string arrival, Entry &entry){
  struct tm tm;
  strptime((arrival + "00").c_str(), "%Y%m%d%H%M%S", &tm);
  double min = numeric_limits<double>::max();
  for(int i = 0 ; i < entry.getEntryNumArrivalsA() ; i ++){
    int diff = abs(Network::differenceTime(tm, entry.arrivals_A_[i])); // in minutes
    if(min > diff)
      min = diff;
  }
  if(min != numeric_limits<double>::max()) // means not max
    return min;
  return -1;
}

double distR(string arrival, Entry &entry){
  struct tm tm;
  strptime((arrival + "00").c_str(), "%Y%m%d%H%M%S", &tm);
  double min = numeric_limits<double>::max();
  for(int i = 0 ; i < entry.getEntryNumArrivalsR() ; i ++){
    int diff = abs(Network::differenceTime(tm, entry.arrivals_R_[i])); // in minutes
    if(min > diff)
      min = diff;
  }
  if(min != numeric_limits<double>::max()) // means not max
    return min;
  return -1;
}


tm Network::estimateNextArrivalTime(string line, char dir, string station, tm now){
  tm res = now;
  res.tm_hour += 5;
  for(auto & entry : *datasets_.at(line).getRealData()){
    if(entry.getEntryStation() == station){
      if(dir == 'A'){
        int count = entry.getEntryNumArrivalsA();
        while(--count > 0)
          if(entry.arrivals_A_[count].tm_hour > now.tm_hour || (entry.arrivals_A_[count].tm_hour == now.tm_hour && entry.arrivals_A_[count].tm_min >= now.tm_min))
            res = entry.arrivals_A_[count];
      }
      else{
        int count = entry.getEntryNumArrivalsR();
        while(--count > 0)
          if(entry.arrivals_R_[count].tm_hour > now.tm_hour || (entry.arrivals_R_[count].tm_hour == now.tm_hour && entry.arrivals_R_[count].tm_min >= now.tm_min))
            res = entry.arrivals_R_[count];
      }
    }
  }
  return res;
}

int Network::duration(string line, string previousStation_A, string station){
    Dataset* dataset = &datasets_.at(line);
    int indSt, indStA;
    int a=0;
    int b=0;
    for(int i=0;i<dataset->getRealData()->size();i++){
        if(dataset->getRealData()->at(i).getEntryStation() == station){
            indSt = i;
            a = 1;
        }
        else if(dataset->getRealData()->at(i).getEntryStation() == previousStation_A){
            indStA = i;
            b = 1;
        }
        else if (a==1 && b==1) {
            break;
        }
    }
    if(a != 1 || b != 1) return 1500;
    Entry st = dataset->getRealData()->at(indSt);
    Entry stA = dataset->getRealData()->at(indStA);
    double res = 0, count = 0;
    if( st.getEntryNumArrivalsA()!= 0 && stA.getEntryNumArrivalsA()!= 0 ){
      for (int j=0;j<stA.getEntryNumArrivalsA();j++) {
        for (int i=0;i<st.getEntryNumArrivalsA();i++) {
          int diff = differenceTime( stA.arrivals_A_[j] , st.arrivals_A_[i]);
          if(diff > 0 && diff < 15){
            res += diff;
            count++;
            break;
          }
        }
      }
    }

    else if( st.getEntryNumArrivalsR()!= 0 && stA.getEntryNumArrivalsR()!= 0 ){
      int res = 0, count = 0;
      for (int j=0 ; j < st.getEntryNumArrivalsR() ; j++) {
        for (int i=0 ; i < stA.getEntryNumArrivalsR() ; i++) {
          int diff = differenceTime(st.arrivals_R_[j], stA.arrivals_R_[i]);
          if(diff > 0 && diff < 15){
            res += diff;
            count++;
            break;
          }
        }
      }
    }
    if(res != 0) return res / count;
    return 1500;
}

int Network::differenceTime(tm &t1, tm &t2){
  int min1 = t1.tm_min, min2 = t2.tm_min, h1 = t1.tm_hour, h2 = t2.tm_hour, d1 =t1.tm_mday, d2= t2.tm_mday, m1 = t1.tm_mon, m2 = t2.tm_mon, y1 = t1.tm_year, y2 = t2.tm_year;
  return (min2 - min1) + (h2 - h1)*60 + (d2 - d1)*1440 + (m2 - m1)*43200 + (y1 - y2)*525600;
}

void Network::drawEstimatedCityTraffic(){
  cout << endl << "***** Estimation of the city traffic *****" << endl << endl;
  int maxStars = 20;
  long maxDist = 0, minDist = numeric_limits<long>::max();
  for(string line : lines_){
    if(line.substr(0,3) == "BUS"){
      cout << "Estimation of the city traffic for " << line << " line:" << endl;

      unordered_map<string,long> errorsA, errorsR; // (station => error)
      for(auto & rawEntry : *datasets_.at(line).getNominalData()){
        double distanceA = 0, distanceR = 0;
        for(Entry e : *datasets_.at(line).getRealData()){
           if(e.getEntryStation() == rawEntry.getEntryStation()){
             if(rawEntry.getEntryNumArrivalsA() > 0)
              distanceA = distA(rawEntry.getFirstArrivalA(), e);
             if(rawEntry.getEntryNumArrivalsR() > 0)
              distanceR = distR(rawEntry.getFirstArrivalR(), e);
             break;
           }
        }
        errorsA[rawEntry.getEntryStation()] += distanceA;
        errorsR[rawEntry.getEntryStation()] += distanceR;

        if(errorsA[rawEntry.getEntryStation()] > maxDist) maxDist = errorsA[rawEntry.getEntryStation()];
        if(errorsR[rawEntry.getEntryStation()] > maxDist) maxDist = errorsR[rawEntry.getEntryStation()];
        if(errorsA[rawEntry.getEntryStation()] < minDist) minDist = errorsA[rawEntry.getEntryStation()];
        if(errorsR[rawEntry.getEntryStation()] < minDist) minDist = errorsR[rawEntry.getEntryStation()];
      }
      // display data
      for(station st : *datasets_.at(line).getStations()){
          long errorA = errorsA[get<1>(st)], errorR = errorsR[get<1>(st)];
          double dist = (double) maxDist - minDist;
          if(get<0>(st) != "" && errorA == 0.){
            cout << "[";
            for(int i = 0; i < maxStars - 1; i++){
                if(i == maxStars / 2 - 1)
                  cout << "NA";
                else
                  cout << " ";
            }
            cout << "] ";
            cout << "From " << get<0>(st) << " to " << get<1>(st) << " (dir A)" << endl;
          }
          if(get<2>(st) != "" && errorR == 0.){
            cout << "[";
            for(int i = 0; i < maxStars - 1; i++){
              if(i == maxStars / 2 - 1)
                cout << "NA";
              else
                cout << " ";
            }
            cout << "] ";
            cout << "From " << get<2>(st) << " to " << get<1>(st) << " (dir R)" << endl;
          }
          if(get<0>(st) != "" && errorA != 0){
            cout << "[";
            for(int i = 0; i < maxStars; i++){
              if(i <= (int) (((errorA-minDist)  / dist) * maxStars))
                cout << "*";
              else
                cout << " ";
            }
            cout << "] ";
            cout << "From " << get<0>(st) << " to " << get<1>(st) << " (dir A)" << endl;
          }
          if(get<2>(st) != "" && errorR != 0){

            cout << "[";
            for(int i = 0; i < maxStars; i++){
              if(i <= (int) (((errorR-minDist)  / dist) * maxStars))
                cout << "*";
              else
                cout << " ";
            }
            cout << "] ";
            cout << "From " << get<2>(st) << " to " << get<1>(st) << " (dir R) "<< endl;
          }
        }
    }
  }
}

double absError(RawEntry &r, vector<Entry> &ve){ // average error in the rawEntry
  int ind;

  for(int i = 0; i < ve.size(); i++){
    if(ve[i].getEntryStation() == r.getEntryStation()){
      ind = i;
      break;
    }
  }
  int minR = ve[ind].getEntryNumArrivalsR();
  if(minR > r.getEntryNumArrivalsR())
    minR = r.getEntryNumArrivalsR();
  int minA = ve[ind].getEntryNumArrivalsA();
  if(minA > r.getEntryNumArrivalsA())
    minA = r.getEntryNumArrivalsA();
  double res = 0.;
  int counter = 0;
  for(int i = 0; i < minA; i++){
    char buffer[20];
    strftime(buffer, 20, "%Y%m%d%H%M" , &r.arrivals_A_[i]);
    double err = distA(buffer, ve[ind]);
    res += err;
    counter++;
  }
  for(int i = 0; i < minR; i++){
    char buffer[20];
    strftime(buffer, 20, "%Y%m%d%H%M" , &r.arrivals_R_[i]);
    double err = distR(buffer, ve[ind]);
    res += err;
    counter++;
  }
  if(counter == 0) return 0;
  return res / counter;
}

void Network::compareToNominalFrequencices(){
  cout << endl << "***** Comapraison to nominal frequencies *****" << endl << endl;
  Cloud stationsError;
  for(auto & line : lines_){

    for(auto & rawEntry : *datasets_.at(line).getNominalData()){
      double err = absError(rawEntry, *datasets_.at(line).getRealData());
      string station = rawEntry.getEntryStation();
      long moment = atol(rawEntry.getMoment().c_str());
      // recording errors
      stationsError[station].emplace_back(moment, err);
    }

  }

  // printing results for lines and stations
  for(auto & line : lines_){
    std::vector<station>* stations = datasets_.at(line).getStations();
    cout << endl << "Average absolute error in minutes for (line " << line << ") stations : " << endl << endl;
    double sum = 0, numSum = 0;
    for(station st : *stations){
      // calculating the total error for a given station
      double totalError = 0;
      for(auto & point : stationsError[get<1>(st)]){
        totalError += get<1>(point);
      }
      //

      if(stationsError[get<1>(st)].size() == 0)
        cout << get<1>(st) << " : NA " << endl;
      else{
        sum += (totalError / stationsError[get<1>(st)].size());
        numSum++;
        cout << get<1>(st) << " : " << (totalError / stationsError[get<1>(st)].size()) << " min" << endl;
      }
    }
    cout << endl << "==> Average absolute error in minutes for line " << line << " is : "<< sum/numSum << " min" << endl << endl;
  }
}

std::unordered_map<std::string, Dataset>* Network::getData(){
  return &datasets_;
}

void Network::print(){
  cout << "***** Network *****" << endl << endl;
  for(unordered_map<string, Dataset>::iterator it = datasets_.begin(); it != datasets_.end(); it++){
    cout << endl << "Line : "<< it->first << endl << endl << "Dataset : " << endl;
    it->second.print();
  }
}
