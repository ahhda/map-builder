#ifndef GRID_H
#define GRID_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

using namespace std;
using namespace cv;

class Grid
{
private:
	int width;
	int height;
	int scale;
	int gridWidth;
	int gridHeight;
	IplImage *gridImg;
	vector<vector<CvPoint> > gridPoints;
	void generateGrid();
public:
	Grid();
	Grid(int width, int height, int scale);
	// ~Grid();
	int getWidth();
	int getHeight();
	int getScale();
	void setWidth(int width);
	void setHeight(int height);
	void setScale(int scale);
	CvPoint getNearestGridPoint(CvPoint p);
	IplImage* getGridAsImage();
	void displayGrid();
};

#endif