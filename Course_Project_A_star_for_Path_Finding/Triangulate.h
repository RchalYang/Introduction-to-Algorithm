#pragma once
#include "common.h"
using namespace std;

class Delaunay
{

  private:
	vector<Polygon> polygonV;   //store all polygons
	vector<Point2D> vertexV;	//store all vertexs
	vector<Line2D> edgeV;		//store all edges
	int outEdgeVecNmu;			//number of vertexs of the outside boundary
	vector<Line2D> lineStack;   //stack for lines
	vector<Triangle> triangleV; //created triangles

	Circle circumCircle(Point2D &p1, Point2D &p2, Point2D &p3);
	Point2D findDT(Line2D line);						 //find the DT point of the edge(the closest avaliable point)
	void initData(vector<Polygon> polyV);				 //initialize data
	Line2D getInitOutEdge();							 //get the first line to put in stack
	double lineAngle(Point2D s, Point2D o, Point2D e);   //calculate the anlge angle-soe
	Rectangle circleBounds(Circle &c);					 //get the rectangle bound of the circle
	bool isVisiblePointOfLine(Point2D vec, Line2D line); //judge whether point vec and this line can build a legal triangle
	bool isVisibleIn2Point(Point2D pa, Point2D pb);		 //judge whether the line ab is intersected with any existed line

  public:
	Delaunay()
	{
		vector<Polygon>().swap(polygonV);
		vector<Point2D>().swap(vertexV);
		vector<Line2D>().swap(edgeV);
		vector<Triangle>().swap(triangleV);
		vector<Line2D>().swap(lineStack);
	}

	vector<Triangle> createDelaunay(vector<Polygon> polyV);
	int indexOfVector(Line2D line, vector<Line2D> &vec)
	{
		int i = 0;
		while (i < vec.size() && !(vec[i] == line))
			i++;
		if (i < vec.size())
			return i;
		else
			return -1;
	}
};

vector<Triangle> Delaunay::createDelaunay(vector<Polygon> polyV)
{
	//initialize the stack
	initData(polyV);
	Line2D initEdge = getInitOutEdge();
	lineStack.push_back(initEdge);

	Line2D edge;

	do
	{
		edge = lineStack.at(lineStack.size() - 1);
		lineStack.erase(lineStack.begin() + lineStack.size() - 1);

		Point2D p3 = findDT(edge);

		if (p3.first == -1 && p3.second == -1)
			continue;

		Line2D line13(edge.start, p3);
		Line2D line32(p3, edge.end);

		//create and put the triangle into the vector
		Triangle trg(edge.start, edge.end, p3);
		triangleV.push_back(trg);

		/* if the newly created edge isn't the edge of the given polygons,
		   if the edge is in the stack, erase it
		   else put it into the stack */
		int index;
		if (indexOfVector(line13, edgeV) < 0)
		{
			index = indexOfVector(line13, lineStack);
			if (index > -1)
			{
				lineStack.erase(lineStack.begin() + index);
			}
			else
			{
				lineStack.push_back(line13);
			}
		}

		if (indexOfVector(line32, edgeV) < 0)
		{
			index = indexOfVector(line32, lineStack);
			if (index > -1)
			{
				lineStack.erase(lineStack.begin() + index);
			}
			else
			{
				lineStack.push_back(line32);
			}
		}

	} while (!lineStack.empty());

	return triangleV;
}

void Delaunay::initData(vector<Polygon> polyV)
{
	Polygon poly;
	for (int i = 0; i < polyV.size(); i++)
	{
		poly = polyV[i];
		for (int j = 0; j < (poly.vertexs).size(); j++)
		{
			vertexV.push_back(poly.vertexs[j]);
		}

		if (poly.vertexs.size() < 3)
			continue;
		Point2D p1 = poly.vertexs[0];
		Point2D p2;
		for (int i = 1; i < poly.vertexs.size(); i++)
		{
			p2 = poly.vertexs[i];
			Line2D temp(p1, p2);
			edgeV.push_back(temp);
			p1 = p2;
		}

		p2 = poly.vertexs[0];
		Line2D temp(p1, p2);
		edgeV.push_back(temp);

		for (int j = 0; j < (poly.inner_Edge).size(); j++)
		{
			edgeV.push_back((poly.inner_Edge)[j]);
		}
	}

	outEdgeVecNmu = polyV[0].vertexs.size();
}

Line2D Delaunay::getInitOutEdge()
{
	Line2D initEdge = edgeV[0];
	bool flag;
	int loopIdx = 0;

	do
	{
		flag = false;
		loopIdx++;
		for (Point2D testPoint : vertexV)
		{
			if (testPoint == initEdge.start || testPoint == initEdge.end)
			{
				continue;
			}
			if (initEdge.classifyPoint(testPoint) == OnLine)
			{
				flag = true;
				initEdge = edgeV[loopIdx];
				break;
			}
		}

	} while (flag && loopIdx < outEdgeVecNmu - 1);

	return initEdge;
}

Point2D Delaunay::findDT(Line2D line)
{

	Point2D p1 = line.start;
	Point2D p2 = line.end;

	// search all visible points
	vector<Point2D> allVPoint; //store all visible points of this edge
	for (Point2D vt : vertexV)
	{
		if (vt.first == 300 && vt.second == 100 && line.start.first== 300 && line.start.second == 50) {
			int qwe = 0;
		}
		if (isVisiblePointOfLine(vt, line))
		{
			allVPoint.push_back(vt);
		}
	}

	if (allVPoint.size() == 0)
	{
		Point2D temp;
		return temp;
	}

	Point2D p3 = allVPoint[0];

	bool loopSign = false;
	do
	{
		loopSign = false;

		//build Tri-p1p2p3's circum circle and it's bound box
		Circle circle = circumCircle(p1, p2, p3);
		Rectangle boundsBox = circleBounds(circle);

		//traverse all the points in the bound box
		//use bound box to judge is much easier than the circle = =
		double angle132 = abs(lineAngle(p1, p3, p2)); // angle-p1p3p2
		for (Point2D vec : allVPoint)
		{
			if ((vec == p1) || (vec == p2) || (vec == p3))
			{
				continue;
			}
			//not in the bound box
			if (boundsBox.contains(vec.first, vec.second) == false)
			{
				continue;
			}
			// calculate the angle
			double a1 = abs(lineAngle(p1, vec, p2));
			// the angle a1 is larger than the angle132 means this point is closer to the edge than
			// the former one
			if (a1 > angle132)
			{
				p3 = vec;
				loopSign = true;
				break;
			}
		}
	} while (loopSign);

	//p3 is the closest avaliable point of the edge
	return p3;
}

Circle Delaunay::circumCircle(Point2D &p1, Point2D &p2, Point2D &p3)
{
	double slope1, slope2, midPointX1, midPointX2, midPointY1, midPointY2;
	double dx, dy, rsqr, drsqr;
	double XCenter, YCenter, r;
	// check whether these points are on the same line
	if ((abs(p1.second - p2.second) < EPSILON) && (abs(p2.second - p3.second) < EPSILON))
	{
		Circle tempC;
		return tempC;
	}

	midPointX1 = (double)(p1.first + p2.first) / 2.0;
	midPointX2 = (double)(p2.first + p3.first) / 2.0;
	midPointY1 = (double)(p1.second + p2.second) / 2.0;
	midPointY2 = (double)(p2.second + p3.second) / 2.0;

	if (abs(p2.second - p1.second) < EPSILON)
	{ //line p2p1 is paralled with x axis
		XCenter = (p2.first + p1.first) / 2.0;
		slope2 = -(double)(p3.first - p2.first) / (p3.second - p2.second); //slope of the midperpendicular of p3p2
		YCenter = slope2 * (XCenter - midPointX2) + midPointY2;
	}
	else if (abs(p3.second - p2.second) < EPSILON)
	{ //line p2p3 is paralled with x axis
		XCenter = (p3.first + p2.first) / 2.0;
		slope1 = -(p2.first - p1.first) / (p2.second - p1.second); //slope of the midperpendicular of p2p1
		YCenter = slope1 * (XCenter - midPointX1) + midPointY1;
	}
	else
	{
		slope1 = -(double)(p2.first - p1.first) / (p2.second - p1.second); //slope of the midperpendicular of p2p1
		slope2 = -(double)(p3.first - p2.first) / (p3.second - p2.second); //slope of the midperpendicular of p3p2
		XCenter = (slope1 * midPointX1 - slope2 * midPointX2 + midPointY2 - midPointY1) / (slope1 - slope2);
		YCenter = slope1 * (XCenter - midPointX1) + midPointY1;
	}

	dx = p2.first - XCenter;
	dy = p2.second - YCenter;
	rsqr = dx * dx + dy * dy;
	r = sqrt(rsqr); //radius of the circle

	Point2D temp(XCenter, YCenter);
	Circle tempC(temp, r);
	return tempC;
}

double Delaunay::lineAngle(Point2D s, Point2D o, Point2D e)
{
	double cosfi, fi, norm;
	double dsx = s.first - o.first;
	double dsy = s.second - o.second;
	double dex = e.first - o.first;
	double dey = e.second - o.second;

	cosfi = dsx * dex + dsy * dey; //scale product
	norm = (dsx * dsx + dsy * dsy) * (dex * dex + dey * dey);
	cosfi /= sqrt(norm);

	if (cosfi >= 1.0)
		return 0;
	if (cosfi <= -1.0)
		return Pi;

	fi = acos(cosfi);
	if (dsx * dey - dsy * dex > 0)//use cross product to determine the relative location of lines
		return fi;
	return -fi;
}

Rectangle Delaunay::circleBounds(Circle &c)
{
	Rectangle temp(c.center.first - c.r, c.center.second - c.r, c.r * 2, c.r * 2);
	return temp;
}

bool Delaunay::isVisiblePointOfLine(Point2D vec, Line2D line)
{
	if ((vec == line.start) || (vec == line.end))
	{
		return false;
	}
	// always use the point on the right or the algorithm would turn out to be dead
	if (line.classifyPoint(vec) != OnRight)
	{
		return false;
	}

	//ensure line-p3p1 is not intersected with any restrain edge(edge of given polygons)
	if (isVisibleIn2Point(line.start, vec) == false)
	{
		return false;
	}

	//do the same with line-p3p2
	if (isVisibleIn2Point(line.end, vec) == false)
	{
		return false;
	}

	return true;
}

bool Delaunay::isVisibleIn2Point(Point2D pa, Point2D pb)
{
	Line2D linepapb(pa, pb);
	for (Line2D lineTmp : edgeV)
	{
		//intersected or not
		if (linepapb.intersection(lineTmp))
		{
			//whether the intersect point is the end of the restrain edge
			if (!(pa == lineTmp.start) && !(pa == lineTmp.end) && !(pb == lineTmp.start) && !(pb == lineTmp.end))
			{
				return false;
			}
		}
	}
	return true;
}
