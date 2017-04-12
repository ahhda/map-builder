#include "grid.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

Grid::Grid()
{
	this->width = 800;
	this->height = 600;
	this->scale = 10;
	gridImg = cvCreateImage(cvSize(this->width, this->height), 8, 3);
	generateGrid();
}

Grid::Grid(int width, int height, int scale)
{
	printf("INITIALISING GRID...");
	this->width = width;
	this->height = height;
	this->scale = scale;
	gridImg = cvCreateImage(cvSize(width, height), 8, 3);
	generateGrid();
	printf("DONE\n");
}

int Grid::getWidth()
{
	return this->width;
}

int Grid::getHeight()
{
	return this->height;
}

int Grid::getScale()
{
	return this->scale;
}

void Grid::setWidth(int width)
{
	this->width = width;
}

void Grid::setHeight(int height)
{
	this->height = height;
}

void Grid::setScale(int scale)
{
	this->scale = scale;
}

void Grid::generateGrid()
{
	this->gridWidth = width/scale - 1;
	this->gridHeight = height/scale - 1;
	gridPoints.clear();
	gridPoints.resize(gridWidth, vector<CvPoint> (gridHeight));
	cvZero(gridImg);
	for (int i = 0; i < gridWidth; ++i)
	{
		for (int j = 0; j < gridHeight; ++j)
		{
			CvPoint temp;
			temp.x = (i + 1) * scale;
			temp.y = (j + 1) * scale;
			gridPoints[i][j] = temp;
			cvCircle(gridImg, temp, 1, CV_RGB(255, 255, 255));
		}
	}
}

IplImage* Grid::getGridAsImage()
{
	IplImage *img = cvCloneImage(gridImg);
	return img;
}

void Grid::displayGrid()
{
	cvShowImage("grid", gridImg);
}

double euclideanDistance(CvPoint p1, CvPoint p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

CvPoint Grid::getNearestGridPoint(CvPoint p)
{
	int i = p.x/scale - 1;
	int j = p.y/scale - 1;
	CvPoint nearest;
	double distance = 1000000;
	for (int ii = -1; ii < 2; ++ii)
	{
		for (int jj = -1; jj < 2; ++jj)
		{
			int tempi = i + ii;
			int tempj = j + jj;
			if(tempi < 0 || tempi >= gridWidth)
				continue;
			if(tempj < 0 || tempj >= gridHeight)
				continue;
			if(distance > euclideanDistance(p, gridPoints[tempi][tempj]))
			{
				distance = euclideanDistance(p, gridPoints[tempi][tempj]);
				nearest = gridPoints[tempi][tempj];
			}
		}
	}
	return nearest;
}

