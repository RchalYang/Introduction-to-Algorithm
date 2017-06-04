#pragma once
#include "common.h"
#include <queue>

using namespace std;

int getCost(vector<Triangle> &trians, int f, int sideId);
pair<Point2D, int> getFurthestWayPoint(vector<Triangle> &triangs, deque<int> &pathT, pair<Point2D, int> current, Point2D end);
deque<Point2D> AStarWithNavMesh(vector<Triangle> &triangles, Point2D start, Point2D Dst);

deque<Point2D> AStarWithNavMesh(vector<Triangle> &triangles, Point2D start, Point2D Dst)
{

	//path in the form of point
	deque<Point2D> pathArr(0);

	//link triangles
	for (int i = 0; i < triangles.size(); i++)
	{
		for (int k = 0; k < 3; k++)
		{
			for (int j = i + 1; j < triangles.size(); j++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (triangles[i].sides[k] == triangles[j].sides[m])
					{
						triangles[i].link[k] = j;
						triangles[j].link[m] = i;
					}
				}
				if (triangles[i].link[k] != -1)
				{
					break;
				}
			}
		}
	}
	//find triangles the startPoint and destination locating
	int startId = 0;
	while (startId < triangles.size() && !triangles[startId].isPointIn(start))
		startId++;
	int dstId = 0;
	while (dstId<triangles.size()&&!triangles[dstId].isPointIn(Dst))
		dstId++;
	//one of the point is unreachable
	if (startId == triangles.size() || startId == triangles.size()){
		return pathArr;
	}

	//store the index of the triangles to present the triangle path
	deque<int> pathTriangle(0);
	triangleInfo startT, dstT;
	vector<triangleInfo> open(0);

	//initialize the open list
	{
		startT.plc = startId;
		triangles[startId].parent = startId;
		triangles[startT.plc].status = PointInOpen;

		dstT.plc = dstId;
		triangles[dstId].parent = dstId;
	}

	if (startT.plc == dstT.plc)
	{
		//the startPoint and destination are in the same triangles
		pathArr.push_back(start);
		pathArr.push_back(Dst);
		return pathArr;
	}
	open.push_back(startT);

	while (!open.empty())
	{
		//find the triangle with minimum F
		triangleInfo minP = open[open.size() - 1];
		int minPP = open.size() - 1;
		for (int i = open.size() - 1; i >= 1; i--)
		{
			if (open[i].F < minP.F)
			{
				minP = open[i];
				minPP = i;
			}
		}

		//put this point into close list
		open.erase(open.begin() + minPP);
		triangles[minP.plc].status = PointInClose;

		int flag = 0;
		for (int i = 0; i < 3; i++)
		{
			//get the index of the triangle to go
			int nextTId = triangles[minP.plc].link[i];
			//there is no avaliable triangles on that direction
			if (nextTId == -1)
			{
				continue;
			}
			if (triangles[nextTId].status == PointInClose)
			{
				continue;
			}
			//the next triangle is already in openList
			if (triangles[nextTId].status == PointInOpen)
			{
				int place = 0;
				int tempG;
				while (open[place].plc != nextTId)
					place++;
				//get new Cost(G)
				tempG = getCost(triangles, minP.plc, i) + minP.G;

				if (open[place].G > tempG)
				{
					open[place].F = tempG + open[place].H;
					open[place].G = tempG;
					triangles[nextTId].parent = minP.plc;
				}
				continue;
			}

			//neither in open or close
			triangleInfo next;
			next.plc = nextTId;
			triangles[nextTId].parent = minP.plc;
			triangles[nextTId].status = PointInOpen;
			//update cost information
			next.G = getCost(triangles, minP.plc, i) + minP.G;
			next.H = abs(triangles[next.plc].getCenter().first - Dst.first) + abs(triangles[next.plc].getCenter().second - Dst.second);
			next.F = next.G + next.H;

			//already arrived destination
			open.push_back(next);
			if (next.plc == dstId)
			{
				flag = 1;
				break;
			}
		}
		if (flag)
		{
			break;
		}
	}
	//retrace to find the path
	int current = dstId;

	if (triangles[dstId].parent == dstId) {
		return pathArr;
	}

	while (current != startId)
	{
		pathTriangle.push_front(current);
		current = triangles[current].parent;
	}
	pathTriangle.push_front(startId);

	pathArr.push_back(start);
	//the first part of the pair indicate the corner point
	//the second part of the pair indicate the triangle the corner point located in
	pair<Point2D, int> wayPoint(start, 0);
	while (!(wayPoint.first == Dst))
	{
		//get the next corner point
		wayPoint = getFurthestWayPoint(triangles, pathTriangle, wayPoint, Dst);
		pathArr.push_back(wayPoint.first);
	}
	return pathArr;
}

int getCost(vector<Triangle> &trians, int f, int sideId)
{

	int s = trians[f].link[sideId];
	int sideIdS = 0;
	while (trians[s].link[sideIdS] != f)
		sideIdS++;
	int pointId = (sideIdS + 2) % 3; // get the point on the opposite side
	Point2D pointAgainst = trians[s].vertexs[pointId];
	//get the center of the adjacent triangles
	Point2D centerF = trians[f].getCenter();
	Point2D centerS = trians[s].getCenter();
	Line2D betweenCenter(centerF, centerS);

	int sideIdS2 = pointId;
	//the pointAgainst's relative location toward the line(make up with the two center)
	//determine which edge would the path would intersectes with 
	if (betweenCenter.classifyPoint(pointAgainst) != OnLeft)
	{
		sideIdS2 = (pointId - 1 + 3) % 3;
	}
	//the cost is defined using the distance between two mid point of the edge where the path intersect with
	Point2D midP1((trians[s].sides[sideIdS].start.first + trians[s].sides[sideIdS].end.first) / 2, (trians[s].sides[sideIdS].start.second + trians[s].sides[sideId].end.second) / 2);
	Point2D midP2((trians[s].sides[sideIdS2].start.first + trians[s].sides[sideIdS2].end.first) / 2, (trians[s].sides[sideIdS2].start.second + trians[s].sides[sideId].end.second) / 2);
	Point2D tempL = midP1 - midP2;
	int cost = (int)sqrt(tempL.first * tempL.first + tempL.second * tempL.second);

	return cost;
}

pair<Point2D, int> getFurthestWayPoint(vector<Triangle> &triangs, deque<int> &pathT, pair<Point2D, int> current, Point2D end)
{
	//using the corner point method to find the path guided by the triangle path
	int cellP = current.second;
	Point2D startPt = current.first; //temporary start point
	Triangle tri = triangs[pathT[cellP]];
	int lastTri = cellP;
	int startIndex = cellP; //indicate the triangle that the temp start point located in

	int sideId = 0;
	while (triangs[pathT[cellP]].link[sideId] != pathT[cellP + 1])
		sideId++;

	Line2D outSide = triangs[pathT[cellP]].sides[sideId]; //away side
	Point2D lastPtA = outSide.start;
	Point2D lastPtB = outSide.end;
	Line2D lastLineA(startPt, lastPtA);
	Line2D lastLineB(startPt, lastPtB);
	Point2D testPtA, testPtB;
	for (int i = startIndex + 1; i < pathT.size(); i++)
	{
		tri = triangs[pathT[i]];

		if (i == pathT.size() - 1)
		{
			testPtA = end;
			testPtB = end;
		}
		else
		{

			int sideId = 0;
			while (tri.link[sideId] != pathT[i + 1])
				sideId++;

			outSide = tri.sides[sideId];

			testPtA = outSide.start;
			testPtB = outSide.end;
		}

		if (!(lastPtA == testPtA))
		{
			if (lastLineB.classifyPoint(testPtA) == OnRight)
			{
				//pointA is on the right side of pointB means there is a corner point
				return make_pair(lastPtB, lastTri);
			}
			else
			{
				if (lastLineA.classifyPoint(testPtA) != OnLeft)
				{
					lastPtA = testPtA;
					lastTri = i;
					//update the corner point's information
					lastLineA.end = lastPtA;
				}
			}
		}

		if (!(lastPtB == testPtB))
		{
			if (lastLineA.classifyPoint(testPtB) == OnLeft)
			{
				//pointB is on the left side of pointA means there is a corner point
				return make_pair(lastPtA, lastTri);
			}
			else
			{
				if (lastLineB.classifyPoint(testPtB) != OnRight)
				{
					lastPtB = testPtB;
					lastTri = i;
					//update the corner point's information
					lastLineB.end = lastPtB;
				}
			}
		}
	}
	return make_pair(end, pathT.size() - 1); 
}