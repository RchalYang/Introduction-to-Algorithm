#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

int main(int, char**)
{
	int rows, cols;
	
	ifstream in("D:\\out.txt");
	ifstream in2("d:\\test8.txt");
	Point start, dstp;
	in2 >> start.x >> start.y >> dstp.x >> dstp.y;

	in2 >> rows >> cols;
	Mat dst(rows,cols,CV_8UC3,Scalar(255,255,255));

	circle(dst, start, 3, CV_RGB(0, 255, 0));
	circle(dst, dstp, 3, CV_RGB(0, 255, 0));


	int num;
	in >> num;
	for (int i = 0; i < num; i++) {
		Point temp1;
		in >> temp1.x>>temp1.y;

		Point temp2;
		in >> temp2.x >> temp2.y;

		Point temp3;
		in >> temp3.x >> temp3.y;

		line(dst, temp1, temp2, Scalar(255, 255, 0));
		line(dst, temp2, temp3, Scalar(255, 255, 0));
		line(dst, temp3, temp1, Scalar(255, 255, 0));
	}

	in.close();

	int n;
	in2 >> n;
	for (int i = 0; i < n; i++) {
		int m;
		in2 >> m;
		vector<Point > points;
		for (int j = 0; j < m; j++) {

			int x, y;
			in2 >> x >> y;
			Point temp(x, y);
			points.push_back(temp);
		}
		Point p1 = points[points.size() - 1];
		Point p2;

		for (int i = 0; i < m; i++) {
			p2 = points[i];
			line(dst, p1, p2, Scalar(255, 0, 0));
			p1 = p2;
		}

		in2 >> m;
		for (int j = 0; j < m; j++) {
			int x, y;
			in2 >> x >> y;
			Point s1(x, y);
			in2 >> x >> y;
			Point s2(x, y);
			line(dst, s1, s2, Scalar(255, 0, 0));
		}

	}

	in2.close();

	in.open("d:\\path.txt");
	vector< Point > path(0);
	int x, y;
	while (in >> x >> y) {
		Point temp(x, y);
		path.push_back(temp);
	}

	for (int i = 1; i < path.size(); i++) {
		line(dst, path[i-1], path[i], Scalar(0, 0, 255));
	}

	imwrite("D:\\testout.jpg", dst);
//	waitKey(0);

	return 0;
}