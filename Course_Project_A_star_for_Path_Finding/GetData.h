#pragma once
#include "common.h"
#include <fstream>
#include <string>
#include <sstream>

Map GetMapFromCSV(string filename);
vector<Polygon> GetPolygonData(string filename, int& rows, int& cols,Point2D& start,Point2D& dst);

Map GetMapFromCSV(string filename) {

	Map data(0);
	string line, number;
	ifstream in(filename);
	if (!in.good())
	{
		cout << "Cannot open file" << endl;
		return data;
	}

	while (std::getline(in,line))
	{
		vector<int> singleLine(0);
		istringstream is(line);
		while (std::getline(is, number, ','))
		{
			int tag = number[0]-48;
			singleLine.push_back(tag);
		}
		data.push_back(singleLine);
	}
	return data;
}

vector<Polygon> GetPolygonData(string filename, int& rows, int& cols, Point2D& start, Point2D& dst) {
	vector<Polygon> data;
	ifstream in(filename);
	in >> start.first >> start.second >> dst.first >> dst.second;
	in >> rows>> cols;
	int n;
	in >> n;
	for (int i = 0; i < n; i++) {
		int m;
		in >> m;
		Polygon p;
		for (int j = 0; j < m; j++) {
			Point2D tempP;
			in >> tempP.first >> tempP.second;
			(p.vertexs).push_back(tempP);
		}
		in >> m;
		for (int j = 0; j < m; j++) {
			int stx,sty, enx,eny;
			in >> stx >> sty >> enx >> eny;
			Point2D st(stx, sty), en(enx, eny);
			Line2D tempL(st, en);
			p.inner_Edge.push_back(tempL);
		}

		data.push_back(p);
	}
	return data;
}