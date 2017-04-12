#ifndef MAP_H
#define MAP_H

#include "grid.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

#define returnPixel1C(image, x, y) ((uchar*)(image->imageData + image->widthStep*(y)))[x]

struct wall
{
	CvPoint st;
	CvPoint end;
};

struct mapwall
{
	double x1;
	double y1;
	double x2;
	double y2;
};

struct feature
{
	double val1;
	double val2;
	double val3;
	double val4;
};

typedef struct cell{
	float occ;
	float total;
	float val;
	cell():occ(0),total(0),val(50.0f)
	{};
}cell;

class Map
{
private:
	vector<wall> walls;
	vector<mapwall> mapwalls;
	vector<feature> features;
	vector<CvPoint> robots;
	int wallCount;
public:
	Grid grid;
	CvPoint startPoint;
	CvPoint origin;
	bool lButtonPressed;
	void addWall(CvPoint st, CvPoint end);
	void addRobot(CvPoint p);
	void deleteWall(CvPoint p);
	Map();
	// ~Map();
	void run();
	void drawMap(CvPoint current);
	void generateFeatures();
	void saveMap();
	void saveOmap();
};

#endif