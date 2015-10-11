// Lab4.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctype.h>
#include <regex>
#include <utility>
#include <thread>
#include <mutex>
#include <queue>
#include <map>
#include <array>
#define pii pair< int, int >
using namespace std;
class XML_File{
private:
	string fileLocation, content;
	ifstream inFile;
public:
	XML_File(string s){
		inFile.open(s, fstream::in);
		ostringstream ss;
		ss << inFile.rdbuf();
		content = ss.str();
		inFile.close();
	}
	string getContent(){
		return content;
	}
};
class REGEX_util{
public:
	static pair<string, string> getSingleData(string text, string rgx){
		pair<string, string> p;
		regex pattern(rgx);
		sregex_token_iterator it(text.begin(), text.end(), pattern);
		sregex_token_iterator it_end;
		int j = 0;
		while (it != it_end){
			if (j == 1){
				p.first = *it;
			}
			else if (j == 2){
				p.second = *it;
			}
			++it;
			j++;
		}
		return p;
	}
};

class String_util{
public:
	static string getRawNode(string nodeName, string& text){
		string s = "<" + nodeName + ">";
		size_t pos = text.find(s);
		s = "</" + nodeName + ">";
		size_t pos2 = text.find(s);
		pos2 = (pos2 - pos) + s.size();
		string str = text.substr(pos + s.size(), pos2 - s.size() * 2);
		text.erase(pos, pos2);
		return str;
	}
	static bool has_only_spaces(const std::string& str) {
		return str.find_first_not_of(' ') == str.npos;
	}
};

class Dist_Util {
public:
	static double Radians(double value)
	{
		double pi = 3.14159;
		double radians = value * pi / 180.0;
		return radians;
	}
	static double Haversine(pair<double, double> coor1, pair<double, double> coor2)
	{
		double dLatitude = Radians(coor2.first - coor1.first);
		double dLongitude = Radians(coor2.second - coor1.second);
		double nA = pow(sin(dLatitude / 2.0), 2.0) + cos(Radians(coor1.first)) * cos(Radians(coor2.first)) * pow(sin(dLongitude / 2.0), 2.0);
		double nC = 2.0 * atan2(sqrt(nA), sqrt(1.0 - nA));
		double radius = 6371;
		double distance = radius * nC;
		return distance;
	}


};


class Location{
private:
	string _nodeStr;
	string _Address;
	bool _startNode;
	pair<double, double> _x_y;

public:
	Location(string s){
		_nodeStr = s;
		populateLoc();
	}
	string getStr(){
		return _nodeStr;
	}
	pair<double,double> getCoordinates(){
		return _x_y;
	}
	void populateLoc(){
		pair<string, string> temp;
		pair<double, double> x_y;
		stringstream ss(_nodeStr);
		string line;
		while (getline(ss, line)){
			temp = (REGEX_util::getSingleData(line, "[^<^>]+"));
			if (!String_util::has_only_spaces(temp.second)){
				if (temp.first == "Latitude"){
					x_y.first = stod(temp.second);
				}
				else if (temp.first == "Longitude"){
					x_y.second = stod(temp.second);
				}
				else if (temp.first == "Address"){
					_Address = temp.second;
				}
				else if (temp.first == "City"){
					_Address += ", " + temp.second;
				}
				else if (temp.first == "State"){
					_Address += " " + temp.second;
				}

			}

		}
		_x_y = x_y;
	}
	string getAddress(){ return _Address; }

};
class Edge{
private:
	int A;
	int B;
	double wieght;
public:
	Edge(int a, int b, double w){
		A = a;
		B = b;
		wieght = w;
	}
	int getA(){return A;}
	int getB(){return B;}
	double getWieght(){return wieght;}
	void printEdge(){
		cout << A << " -> " << B << " = " << wieght << "\n";
	}

};
class Graph{
private:
	vector<pair<int, int>> _vertices;
	vector<Edge> _edges;
	vector<Location> _locs;

public:
	Graph(string s){
		loadLocsFromNodes(s);
		loadEdges();

	}
	void loadLocsFromNodes(string str){
		XML_File xmlFile(str);
		string strBuff = xmlFile.getContent();
		int count = 0;
		int compare = numOfCommands(strBuff);
		while (count != compare){
			string temp = String_util::getRawNode("Location", strBuff);
			Location loc(temp);
			_locs.push_back(loc);
			count++;
		}
	}
	void loadEdges(){
		for(int i = 0; i < _locs.size(); i++){
			for(int j = 0; j < _locs.size(); j++){
				Edge e(i, j, Dist_Util::Haversine(_locs[i].getCoordinates(), _locs[j].getCoordinates()));
				if(e.getWieght() != 0)
					_edges.push_back(e);
			}
		}

	}
	void printAddresses(){
		for (auto l : _locs){
			cout << l.getAddress() << "\n";
		}

	}
	void printGraphCoor(){
		for (auto l : _locs){
			cout << l.getCoordinates().first << "  " << l.getCoordinates().second << "\n";
		}

	}
	void printDistancesFrom0(){
		for (int i = 1; i < _locs.size(); i++){
			cout << Dist_Util::Haversine(_locs[0].getCoordinates(), _locs[i].getCoordinates());
			cout << "\n";
		}
	}
	int numOfCommands(string str){
		int i = 0;
		string s = "/Location";
		const size_t step = s.size();
		size_t count(0);
		size_t pos(0);
		while ((pos = str.find(s, pos)) != string::npos){
			++pos;
			++i;
		}

		return i;
	}
	void printEdges(){
		for(int i = 0; i < _edges.size(); i++){
			_edges[i].printEdge();
		}
	}
	vector<Location> getLocs(){return _locs;}
	vector<Edge> getEdges(){return _edges;}

};

class Dykstra
{
	struct comp
	{
        bool operator()(const pii &a, const pii &b)
        {
            return a.second > b.second;
        }
	};
	private:

		priority_queue<pair<int, int>, vector<pair<int,int>>, comp> Q;
		vector<vector<pair<int, double>>> G;
		vector<pair<int, double>> distVec;
		vector<double> D;
		vector<bool> F;
		vector<bool> V;
	public:

		Dykstra(Graph g, int startingNode){
			initVecs(g.getLocs().size());
			initBoolVec(F);
				for (int i = 0; i < g.getEdges().size(); i++){

					G[g.getEdges()[i].getA()].push_back(make_pair(g.getEdges()[i].getB(), g.getEdges()[i].getWieght()));
					G[g.getEdges()[i].getB()].push_back(make_pair(g.getEdges()[i].getA(), g.getEdges()[i].getWieght()));
				}
				for (int i = 0; i < g.getLocs().size(); i++)
					D[i] = (double)(1 << 20);
				D[startingNode] = 0;
				Q.push(make_pair(startingNode, 0));
				while (!Q.empty()){
					int u = Q.top().first;
					Q.pop();
					if (F[u])
						continue;
					int sz = G[u].size();
					for (int i = 0; i < sz; i++){
						int v = G[u][i].first;
						double w = G[u][i].second;
						if (!F[v] && D[u] + w < D[v]){
							D[v] = D[u] + w;
							Q.push(make_pair(v, D[v]));
						}
					}
					F[u] = true;
				}


			cout << "From " << g.getLocs()[startingNode].getAddress() << "\n";
			for (int i = 0; i < g.getLocs().size(); i++)
        cout << "\n...To -> " << g.getLocs()[i].getAddress() << ", min weight = " << D[i] << endl;

		}
		void initVecs(int size){;
			G = vector<vector<pair<int, double>>> (size*2);
			D = vector<double> (size);
			F = vector<bool> (size);
			V = vector<bool>(size);
		}
		bool vecIsTrue(vector<bool> v){
			for (int i = 0; i < v.size(); i++){
				if (v[i] == false)
					return false;
			}
			return true;
		}
		void initBoolVec(vector<bool> &v){
			for (int i = 0; i < v.size(); i++)
				v[i] = false;
		}

};
int main()
{
	Graph g("Coordinates.xml");
	//starting node is second argument
	Dykstra d(g, 1);
	return 0;
}

