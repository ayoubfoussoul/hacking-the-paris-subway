#include "Graph.hpp"
#define INF 0x3f3f3f3f

using namespace std;

Graph::Graph(Network network, tm now){

  // adding stations of the network and creating edges
  for(unordered_map<string, Dataset>::iterator it = network.getData()->begin(); it != network.getData()->end(); it++){
    for(auto stationData: *(it->second.getStations())){
      string station = get<1>(stationData);
      string previousStation_A = get<0>(stationData);
      string previousStation_R = get<2>(stationData);
      string line = it->second.getLine();

      vertices_.insert(station);

      if(previousStation_A != ""){ // edge in R dir
        tm arr_tm = network.estimateNextArrivalTime(line, 'R', station, now);
        int diff = (arr_tm.tm_hour - now.tm_hour)*60 + (arr_tm.tm_min - now.tm_min);
        long totalDuration_R = diff + network.duration(line, previousStation_A, station); // for the edge(station -> previous station in R direction)
        edges_[station].push_back(make_tuple(previousStation_A, line, 'R' , totalDuration_R));
      }
      if(previousStation_R !=  ""){ // edge in A dir
        tm arr_tm = network.estimateNextArrivalTime(line, 'A', station, now);
        int diff = (arr_tm.tm_hour - now.tm_hour)*60 + (arr_tm.tm_min - now.tm_min);
        long totalDuration_A = diff + network.duration(line, station, previousStation_R); // for the edge(station -> previous station in R direction)
        edges_[station].push_back(make_tuple(previousStation_R, line, 'A' , totalDuration_A));
      }
    }
  }

}


// Graph::~Graph(){}


unordered_map<string, tuple<string, string, char, long>> Graph::getOptimalPath(string source, string target){
  unordered_map<string, long> distancesToSource;
  for(set<string>::iterator it = vertices_.begin(); it != vertices_.end(); it++){
    if(*it == source)
      distancesToSource[*it] = 0;
    else
      distancesToSource[*it] = INF;
  }

  priority_queue <pair<long,string>, vector<pair<long,string>>, greater<pair<long,string>>> Q;
  Q.push({0, source});
  unordered_map<string, tuple<string, string, char, long>> path; // (station, <target, line, direction, distance>)
  while(!Q.empty()){
        string u = Q.top().second;
        Q.pop();
        for(auto c : edges_[u]){
            string v = get<0>(c);
            long w = get<3>(c);
            if(distancesToSource[v] > distancesToSource[u] + w){
                distancesToSource[v] = distancesToSource[u]+w;
                path[v] = make_tuple(u, get<1>(c), get<2>(c), get<3>(c));
                Q.push({distancesToSource[v],v});
            }
        }
  }
  return path;
}

void Graph::print(){
  cout << "***** Graph *****" << endl << endl;
  for(set<string>::iterator it = vertices_.begin(); it != vertices_.end(); it++){
    cout << "Station : " << *it << endl;
    cout << "Edges :" << endl;
    for(auto edge : edges_[*it]){
        int dur = (int) get<3>(edge);
        string h = (dur / 60 < 10) ? "0" + to_string(dur / 60) : to_string(dur / 60);
        string min =  (dur % 60 < 10) ? "0" + to_string(dur % 60) : to_string(dur % 60);

        cout << *it << " -> " << get<0>(edge) << " | Line : " << get<1>(edge) << " | Direction : " << get<2>(edge) << " | Distance : " << h << ":" << min << endl;
    }
    cout << endl;
  }
  cout << endl;
}

void Graph::print_path(unordered_map<string, tuple<string, string, char, long>> path, string source, string target){
  cout << "***** Path *****" << endl << endl << "From [" << source << "] To [" << target << "]" << endl << endl;
  string curr = target;
  tuple<string, string, char, long> edge;
  stack<string> output;

  int pathExists = true;
  if(path.find(target) == path.end()){
    cout << "Path not found!" << endl;
    pathExists = false;
  }
  while (curr != source && pathExists){
    edge = path[curr];
    int dur = (int) get<3>(edge);
    string h = (dur / 60 < 10) ? "0" + to_string(dur / 60) : to_string(dur / 60);
    string min =  (dur % 60 < 10) ? "0" + to_string(dur % 60) : to_string(dur % 60);
    if(dur == 300)
      output.push(get<0>(edge) + " -> " + curr + " | Line : " + get<1>(edge) + " | Direction : " + get<2>(edge) + " | Pas d'informations sur le prochain arrivé!");
    else
      output.push(get<0>(edge) + " -> " + curr + " | Line : " + get<1>(edge) + " | Direction : " + get<2>(edge) + " | Le prochain viens dans : " + h + "h " + min + "min");
    curr = get<0>(edge);
  }
  while (!output.empty() && pathExists) {
        cout << output.top() << endl;
        output.pop();
  }
}
