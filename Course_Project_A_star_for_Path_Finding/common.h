#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <deque>
#include <cmath>

using namespace std;

//describe map status in basic AStar algorithm
#define MapObstacle 1
#define MapAvailable 0
#define PointInClose -1
#define PointInOpen -2
#define PointHaveNotUsed -3
//basic AStar moving panalty
#define CostStraight 10
#define CostCrossing 14
//used const data
#define EPSILON 0.000001
#define Pi 3.1415926535
//whether two line is intersected
#define INTERSECT 500
//describe point's relative location to a line
#define OnLine 100
#define OnRight 200
#define OnLeft 300

//first should be Y, Second should be X
typedef std::pair<int, int> Point;
typedef std::vector<Point> VecP;
typedef std::vector< std::vector<int> > Map;

//structure to store path cost information
struct EstimateDis {
	int F;
	int G;
	int H;
	Point parent;
};
typedef std::map<Point,EstimateDis> EstimaeMap;

//orientation information
const int orientation[][2] = { {-1,-1},{-1,0},{-1,1},
								{0,-1},{0,1},
								{1,-1},{1,0},{1,1} };

class Point2D {
public:
	int first;
	int second;
	Point2D(int x, int y) {
		first = x;
		second = y;
	}
	Point2D() {
		first = -1;
		second = -1;
	}
	
	double length() {
		return sqrt(first*first + second*second);
	}

	friend bool operator== (Point2D& a, Point2D& b) {
		return ((a.first == b.first) && (a.second == b.second));
	}

	// vector minus
	friend Point2D operator - (Point2D& a, Point2D& b) {
		Point2D temp(a.first - b.first, a.second - b.second);
		return temp;
	}

	//cross product
	friend long Crossmultiply(Point2D& a, Point2D& b ) {
		return a.first*b.second - a.second*b.first;
	}

};


class Line2D {
public:
	// two end of the line
	Point2D start;
	Point2D end;

	Line2D() {
	}

	Line2D(Point2D a, Point2D b) {
		start = a;
		end = b;
	}

	//find point's relative position  to line
	int classifyPoint(Point2D pt) {
		Point2D startPt(pt.first - start.first, pt.second - start.second);
		Point2D startEnd(end.first - start.first, end.second - start.second);
		double product = startPt.first*startEnd.first + startPt.second*startEnd.second;
		double absV = startEnd.length()*startPt.length();
		double crossProduct = startEnd.first * startPt.second - startPt.first*startEnd.second;

		if (abs(abs(product) - absV) < EPSILON) {
			return OnLine;
		}
		// using cross production's properties
		if (crossProduct < 0) 
			return OnLeft;
		else
			return OnRight;
		
	}
	//to judge whether two line are intersected
	//firstly judge the relative location using the relative location of four end points of these lines
	bool intersection(Line2D& oline) {
		if (!((max(this->start.first, this->end.first) >= min(oline.start.first, oline.end.first)) && (max(oline.start.first, oline.end.first) >= min(this->start.first, this->end.first)))) {
			return false;
		}
		if (!((max(this->start.second, this->end.second) >= min(oline.start.second, oline.end.second)) &&
			(max(oline.start.second, oline.end.second) >= min(this->start.second, this->end.second))))
		{
			return false;
		}
		long temp1 = Crossmultiply(start - oline.start, oline.end - oline.start);
		long temp2 = Crossmultiply(oline.end - oline.start, end - oline.start);

		if ((temp1>=0 && temp2 < 0) || (temp1 < 0 && temp2 >= 0)) {
			return false;
		}
		temp1 = Crossmultiply((oline.start - start), (end - start));
		temp2 = Crossmultiply(end - start, oline.end - start);
		if ((temp1 >= 0 && temp2 < 0) || (temp1 < 0 && temp2 >= 0)) {
			return false;
		}
		return true;
	}

	double length() {
		return sqrt((start.first - end.first)*(start.first - end.first) + (start.second - end.second)*(start.second - end.second));
	}

	friend bool operator == (Line2D& a, Line2D& b) {
		return ((a.start == b.start && a.end == b.end) || (a.start == b.end && a.end == b.start));
	}
	
};

class Triangle {
public:
	std::vector<Line2D>  sides;
	std::vector<Point2D> vertexs;
	//store the triangles this triangle links to
	std::vector<int> link;
	//to judge whether this triangle is in open list or close list
	int status;
	//store the parent node in AStar path finding
	int parent;

	Triangle() {}
	Triangle(Point2D a,Point2D b, Point2D c) {
		vector<int>(3, -1).swap(link);
		vector<Line2D>().swap(sides);
		vector<Point2D>().swap(vertexs);
		vertexs.push_back(a);
		vertexs.push_back(b);
		vertexs.push_back(c);
		Line2D temp(a, b);
		sides.push_back( temp );
		temp.start = b; temp.end = c;
		sides.push_back( temp );
		temp.start = c; temp.end = a;
		sides.push_back( temp );
		status = PointHaveNotUsed;
	}

	//get the center Point's location
	Point2D getCenter() {
		int x = (vertexs[0].first + vertexs[1].first + vertexs[2].first)/3;
		int y = (vertexs[0].second + vertexs[1].second + vertexs[2].second) / 3;
		Point2D temp(x,y);
		return temp;
	}
	
	//to judge whether the point is in the triangle
	bool isPointIn(Point2D testPoint) {
		// the point on the triangle's sides are assumed in this triangle
		if ((sides[0].classifyPoint(testPoint) == OnLine && testPoint.first< max(sides[0].start.first, sides[0].end.first) && testPoint.first > min(sides[0].start.first, sides[0].end.first))
			|| (sides[1].classifyPoint(testPoint) == OnLine && testPoint.first< max(sides[1].start.first, sides[1].end.first) && testPoint.first > min(sides[1].start.first, sides[1].end.first))
			|| (sides[2].classifyPoint(testPoint) == OnLine && testPoint.first< max(sides[2].start.first, sides[2].end.first) && testPoint.first > min(sides[2].start.first, sides[2].end.first))
			) {
			return true;
		}

		//if point are on the same side of three sides, it's in the triangle
		if (sides[0].classifyPoint(testPoint) == OnLeft && sides[1].classifyPoint(testPoint) == OnLeft && sides[2].classifyPoint(testPoint) == OnLeft) {
			return true;
		}
		if (sides[0].classifyPoint(testPoint) == OnRight && sides[1].classifyPoint(testPoint) == OnRight && sides[2].classifyPoint(testPoint) == OnRight) {
			return true;
		}

		return false;
	}

};

//information in NavMesh based Path Finding
struct triangleInfo{
	int plc;
	int G;
	int H;
	int F;
	triangleInfo() {
		G = 0;
		H = 0;
		F = 0;
	}
};

class Circle {
public:
	Point2D center;
	double r;
	Circle():center(-1,-1) {
		r = -1;
	}
	Circle(Point2D c, double r1) {
		center = c;
		r = r1;
	}
};

class Rectangle {
public:
	int leftTopx;
	int leftTopy;
	int width;
	int length;
	Rectangle() {
		leftTopx = -1;
		leftTopy = -1;
		width = -1;
		length = -1;
	}
	Rectangle(int x,int y,int w, int l) {
		leftTopx = x;
		leftTopy = y;
		width = w;
		length = l;
	}
	//judge whehter the point is in the rectangle
	bool contains(int x, int y) {
		return (x >= leftTopx && x <= leftTopx + width && y >= leftTopy && y <= leftTopy + length);
	}

};

class Polygon {
public:
	std::vector<Point2D> vertexs;
	std::vector<Line2D> inner_Edge;
};