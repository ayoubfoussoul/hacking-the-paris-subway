#include "tests.hpp"
#include <iostream>

using namespace std;

void testEntry(){
  Entry e("Station example", 3, new string[3]{"201905012021","201905012026","201905012047"}, 2, new string[2]{"201905012021","201905012026"});
  e.print();
}

void testRawEntry(){
  string data = "201905012021,Saint Remy les Chevreuse,6,201905012026,20:26 D√©part Voie 1,201905012047,20:47 D√©part Voie 2,201905012108,21:08 D√©part Voie 2,201905012129,21:29 D√©part Voie 2,201905012150,21:50 D√©part Voie 2,201905012211,22:11 D√©part Voie 2,,,,,6,201905012021,Train terminus V.2,201905012035,Train terminus V.2,201905012057,Train terminus V.2,201905012118,Train terminus V.2,201905012139,Train terminus V.2,201905012200,Train terminus V.2,,,,,0,,,,";
  RawEntry e(data);
  e.print();
}

void testGraph(){
  std::set<std::string> vertices;
  vertices.insert("Station1");
  vertices.insert("Station2");
  vertices.insert("Station3");
  vertices.insert("Station4");
  vertices.insert("Station5");

  std::unordered_map<std::string,Edges> edges;
  edges["Station1"].push_back(make_tuple("Station3", "RER B", 'A', 9623));
  edges["Station1"].push_back(make_tuple("Station2", "RER C", 'A', 767));
  edges["Station2"].push_back(make_tuple("Station3", "RER D", 'R', 342));
  edges["Station3"].push_back(make_tuple("Station2", "RER D", 'A', 5876));
  edges["Station1"].push_back(make_tuple("Station5", "RER E", 'A', 20));
  edges["Station5"].push_back(make_tuple("Station4", "RER D", 'A', 13));
  edges["Station4"].push_back(make_tuple("Station3", "RER C", 'A', 100));
  edges["Station1"].push_back(make_tuple("Station2", "RER E", 'A', 7696));

  Graph g(vertices, edges);
  unordered_map<string, tuple<string, string, char, long>> path = g.getOptimalPath("Station1", "Station3");

  g.print();

  // initialisation using a network
  unordered_map<string, pair<string, string>> files;
  files["RER B"] = make_pair("RER_B_data.csv","RER_B_structure.csv");
  files["RER A"] = make_pair("RER_A_data.csv","RER_A_structure.csv");
  files["BUS 91"] = make_pair("BUS_91_data.csv","BUS_91_structure.csv");
  Network net(files);
  tm now;
  strptime(string("201905121212").c_str(), "%Y%m%d%H%M%S", &now);
  Graph g2(net, now);
  g2.print();

}

void testDataset(){
  Dataset data("RER B", "RER_B_data.csv","RER_B_structure.csv");
  data.print();
}

void testNetwork(){
  unordered_map<string, pair<string, string>> files;
  files["RER B"] = make_pair("RER_B_data.csv","RER_B_structure.csv");
  files["RER A"] = make_pair("RER_A_data.csv","RER_A_structure.csv");
  files["BUS 91"] = make_pair("BUS_91_data.csv","BUS_91_structure.csv");
  Network net(files);
  net.print();
}
