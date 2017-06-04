#include "common.h"
#include "AStar.h"
#include "GetData.h"
#include "Triangulate.h"

int main()
{
	deque<Point> path(0);

	Point2D start;
	Point2D dst;
	vector<Triangle> trianS;
	Delaunay de;
	int width, length;
	vector<Polygon> polys = GetPolygonData("d:\\test8.txt", width, length, start,dst);
	trianS = de.createDelaunay(polys);

	deque<Point2D> path2 = AStarWithNavMesh(trianS, start, dst);

	ofstream out("d:\\out.txt");
	out << trianS.size()<<endl;

	for (Triangle temp : trianS) {
		for (int i = 0; i < 3; i++) {
			out << temp.vertexs[i].first<<" "<< temp.vertexs[i].second <<endl;
		}
		out << endl;
	}
	out.close();

	out.open("d:\\path.txt");
	for (Point2D p : path2) {
		out << p.first << " " << p.second << endl;
	}
	out.close();
}