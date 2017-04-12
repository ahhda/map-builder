#include "map.h"
#include <algorithm>
#include <fstream>

Map::Map()
{
	grid = Grid(800, 600, 10);;
	lButtonPressed = false;
	wallCount = 0;
	walls.clear();
	origin.x = 20;
	origin.y = 40;
	wall wall1, wall2, wall3, wall4;
	wall1.st.x = 20;
	wall1.st.y = 40;
	wall1.end.x = 780;
	wall1.end.y = 40;
	wall2.st.x = 780;
	wall2.st.y = 40;
	wall2.end.x = 780;
	wall2.end.y = 560;
	wall3.st.x = 780;
	wall3.st.y = 560;
	wall3.end.x = 20;
	wall3.end.y = 560;
	wall4.st.x = 20;
	wall4.st.y = 560;
	wall4.end.x = 20;
	wall4.end.y = 40;
	walls.push_back(wall1);
	walls.push_back(wall2);
	walls.push_back(wall3);
	walls.push_back(wall4);
	wallCount = walls.size();

}

void Map::addWall(CvPoint st, CvPoint end)
{
	wall temp;
	temp.st = st;
	temp.end = end;
	walls.push_back(temp);
	wallCount = walls.size();
}

bool checkPointinLine(CvPoint p, wall w)
{
	if(p.x == w.st.x && p.y == w.st.y)
		return true;
	if(p.x == w.end.x && p.y == w.end.y)
		return true;
	float x1 = w.st.x;
	float y1 = w.st.y;
	float x2 = w.end.x;
	float y2 = w.end.y;
	float x3 = p.x;
	float y3 = p.y;
	float slope1 = (y1 - y2)/(x1 - x2);
	float slope2 = (y1 - y3)/(x1 - x3);
	if(slope1 == slope2)
		return true;
	return false;
}

void Map::deleteWall(CvPoint p)
{
	for (int i = 0; i < wallCount; ++i)
	{
		if(checkPointinLine(p, walls[i]))
		{
			walls.erase(walls.begin() + i);
			wallCount = walls.size();
			return;
		}
	}
}

void Map::addRobot(CvPoint p)
{
	for (int i = 0; i < robots.size(); ++i)
	{
		if(p.x == robots[i].x && p.y == robots[i].y)
		{
			robots.erase(robots.begin() + i);
			return;
		}
	}
	robots.push_back(p);
}

vector<pair<CvPoint, CvPoint> > drawstraightdiagonals(CvPoint startPoint, CvPoint current){
	vector<pair<CvPoint, CvPoint> > pointstodraw;
	int x1 = startPoint.x;
	int y1 = startPoint.y;
	int x2 = current.x;
	int y2 = current.y;
	//cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
	int hor = abs(x2-x1)/10;
	int vert = abs(y2-y1)/10;
	if(hor == vert){
		CvPoint oldpoint, newpoint;
		newpoint = startPoint;
		for(int i=1;i<=vert;i++){
			if(y1 < y2){
				oldpoint = newpoint;
				if(((x2 < x1) && (y2 < y1)) || ((x1 < x2) && (y1 < y2))){
					newpoint.x = x1+i*10;
				}
				else{
					newpoint.x = x1-i*10;
				}
				pointstodraw.push_back(make_pair(oldpoint, newpoint));
				oldpoint = newpoint;
				if(((x2 < x1) && (y2 < y1)) || ((x1 < x2) && (y1 < y2))){
					newpoint.y = y1+i*10;
				}
				else{
					newpoint.y = y1+i*10;
				}
				pointstodraw.push_back(make_pair(oldpoint, newpoint));
			}
			else{
				oldpoint = newpoint;
				if(((x2 < x1) && (y2 < y1)) || ((x1 < x2) && (y1 < y2))){
					newpoint.y = y1-i*10;
				}
				else{
					newpoint.y = y1-i*10;
				}
				
				pointstodraw.push_back(make_pair(oldpoint, newpoint));
				oldpoint = newpoint;
				if(((x2 < x1) && (y2 < y1)) || ((x1 < x2) && (y1 < y2))){
					newpoint.x = x1-i*10;
				}
				else{
					newpoint.x = x1+i*10;
				}
				
				pointstodraw.push_back(make_pair(oldpoint, newpoint));
			}
		}
	}
	return pointstodraw;
}

bool isdiagonal(CvPoint p1, CvPoint p2){
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;
	if(abs(x1 - x2) == 0 || abs(y1-y2) == 0){
		return false;
	}
	else{
		return true;
	}
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	Map *m = (Map*) userdata;
	Grid g = m->grid;
	if(event == EVENT_LBUTTONDOWN)
	{
		if(m->lButtonPressed == true)
			return;
		m->lButtonPressed = true;
		m->startPoint = g.getNearestGridPoint(cvPoint(x, y));
	}
	else if(event == EVENT_RBUTTONDOWN)
	{
		if(m->lButtonPressed == true)
		{
			CvPoint endPoint = g.getNearestGridPoint(cvPoint(x, y));
			m->lButtonPressed = false;
			if(isdiagonal(m->startPoint, endPoint)){
				vector<pair<CvPoint, CvPoint> > pointstodraw = drawstraightdiagonals(m->startPoint, endPoint);
				for(int i=0;i<pointstodraw.size();i++){
					m->addWall(pointstodraw[i].first, pointstodraw[i].second);
				}
			}
			else{
				m->addWall(m->startPoint, endPoint);
			}
		}
		else
		{
			CvPoint p = g.getNearestGridPoint(cvPoint(x, y));
			m->deleteWall(p);
		}
	}
	else if(event == EVENT_MOUSEMOVE)
	{
		CvPoint current = g.getNearestGridPoint(cvPoint(x, y));
		m->drawMap(current);
	}
	else if(event == EVENT_MBUTTONDOWN)
	{
		CvPoint current = g.getNearestGridPoint(cvPoint(x, y));
		m->addRobot(current);
	}
}

void Map::drawMap(CvPoint current)
{
	IplImage *img = grid.getGridAsImage();
	cvCircle(img, current, 4, CV_RGB(255, 0, 0));
	for (int i = 0; i < wallCount; ++i)
		cvLine(img, walls[i].st, walls[i].end, CV_RGB(0, 255, 0));
	for (int i = 0; i < robots.size(); ++i)
		cvCircle(img, robots[i], 4, CV_RGB(255, 75, 0), -1);
	if(lButtonPressed == true)
	{
		if(!isdiagonal(startPoint, current)){
			cvLine(img, startPoint, current, CV_RGB(255, 255, 0));
		}
		else{
			vector<pair<CvPoint, CvPoint> > pointstodraw = drawstraightdiagonals(startPoint, current);
			if(pointstodraw.size() <= 0){
				cvLine(img, startPoint, current, CV_RGB(255, 255, 0));
			}
			else{
				for(int i=0;i<pointstodraw.size();i++){
					cvLine(img, pointstodraw[i].first, pointstodraw[i].second, CV_RGB(255, 255, 0));
				}
			}
		}
	}
	cvShowImage("img", img);
}

void Map::run()
{
	cvNamedWindow("img");
	drawMap(cvPoint(0, 0));
	while(1)
	{
		setMouseCallback("img", CallBackFunc, (void*)this);
		char c = cvWaitKey(50);
		if(c == 's' || c == 'S')
		{
			saveMap();
			// saveOmap();
		}
		if(c == 27)
			exit(0);
	}
}

double euclideandistance(CvPoint p1, CvPoint p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

void Map::generateFeatures()
{
	double distance = 100000;
	for (int i = 0; i < wallCount; ++i)
	{
		if(distance > euclideandistance(cvPoint(0, 0), walls[i].st))
		{
			distance = euclideandistance(cvPoint(0, 0), walls[i].st);
			origin = walls[i].st;
		}
		if(distance > euclideandistance(cvPoint(0, 0), walls[i].end))
		{
			distance = euclideandistance(cvPoint(0, 0), walls[i].end);
			origin = walls[i].end;
		}
	}
	mapwalls.clear();
	features.clear();
	for (int i = 0; i < wallCount; ++i)
	{
		mapwall temp;
		temp.x1 = (walls[i].st.x - origin.x)/20.0;
		temp.y1 = (walls[i].st.y - origin.y)/20.0;
		temp.x2 = (walls[i].end.x - origin.x)/20.0;
		temp.y2 = (walls[i].end.y - origin.y)/20.0;
		mapwalls.push_back(temp);
	}
	for (int i = 0; i < mapwalls.size(); ++i)
	{
		double X1, Y1, X2, Y2;
		double x1, y1, x2, y2;
		X1 = mapwalls[i].x1;
		X2 = mapwalls[i].x2;
		Y1 = mapwalls[i].y1;
		Y2 = mapwalls[i].y2;
		x1 = mapwalls[i].x1;
		x2 = mapwalls[i].x2;
		y1 = mapwalls[i].y1;
		y2 = mapwalls[i].y2;
		if(x1 > x2)
		{
			swap(x1, x2);
			swap(y1, y2);
			swap(X1, X2);
			swap(Y1, Y2);
		}
		if(y1 > y2)
		{
			swap(x1, x2);
			swap(y1, y2);
			swap(X1, X2);
			swap(Y1, Y2);
		}
		if(X1 == X2)
		{
			while(y1 < y2)
			{
				feature temp;
				temp.val1 = x1;
				temp.val2 = y1;
				temp.val3 = 1.000000;
				temp.val4 = 500;
				features.push_back(temp);
				y1 += 1.266667;
				if(X1 <= x1 && x1 <= X2 && Y1 <= y1 && y1 <= Y2)
				{}
				else
					break;
			}
		}
		if(Y1 == Y2)
		{
			while(x1 < x2)
			{
				feature temp;
				temp.val1 = x1;
				temp.val2 = y1;
				temp.val3 = 1.000000;
				temp.val4 = 500;
				features.push_back(temp);
				x1 += 1.266667;
				if(X1 <= x1 && x1 <= X2 && Y1 <= y1 && y1 <= Y2)
				{}
				else
					break;
			}
		}
	}
}

void Map::saveMap()
{
	generateFeatures();
	FILE *fp = fopen("scene.map", "w");
	fprintf(fp, "NUMWALLS = %d\n", (int)mapwalls.size());
	fprintf(fp, "NUMFEATURES = %d\n", (int)features.size());
	fprintf(fp, "DESCRIPTORLENGTH = 1\n");
	fprintf(fp, "GRIDMAP = scene.omap\n");
	fprintf(fp, "GRIDMAPTYPE = 3\n");
	fprintf(fp, "MAXNUMROBOTS = %d\n", (int)robots.size());
	fprintf(fp, "NUMROBOTS = %d\n", (int)robots.size());
	fprintf(fp, "\n");
	fprintf(fp, "FOOTPRINT = -0.25 0.25 0.25 0.25 0.25 -0.25 -0.25 -0.25\n");
	fprintf(fp, "\n");
	fprintf(fp, "# >> Motion\nalfa1 = 0.04\nalfa2 = 0.04\nalfa3 = 0.04\nalfa4 = 0.04\n#drifttrans = 1.0\nVABSMAX = 0.35\nWABSMAX = 0.25\n\n");
	fprintf(fp, "# >> Camera\nf = 300\nI = 0.1\ndistMAX = 5.0\ndistMIN = 0.0\nWIDTH = 320\nHEIGHT = 240\nsigmar = 3.3\nsigmac = 3.3\nsigmad = 1\ncamx = -0.08\ncamy = 0.08\ncamz = 1.0\n#Cx = 160.0\n#Cy = 120.0\n#Cxp = 160.0\n#sigma2f = 0 #5.45\n#sigma2I = 0 #0.00000002\n#sigma2Cx = 0 #1.77\n#sigma2Cy = 0 #1.77\n#sigma2Cxp = 0 #1.77\n#sigma2r = 11 #0.12\n#sigma2c = 11 #0.12\n#sigma2cp = 0 #0.12\n#sigma2d = 1\n#betaMAX  = 0.78539816\n#gammaMAX = 1.04\n#camera position\n#rx = 0\n#ry = 0\n#rz = 0\n\n");
	fprintf(fp, "# Laser\nNUMPOINTS = 300\nLASERMINANGLE = -120\nLASERMAXANGLE =  120\nLASERMAXDIST = 6.0\nLASERMINDIST = 0.0\nLASERSIGMA = 0.1\nLASERX = 0\nLASERY = 0\nLASERZ = 0\n#APERTURE = 0.00\n#GAMMAMAX = 3.1416\n#REALERROR = 0.1\n\n");
	fprintf(fp, "# >> Initial poses\n");
	for (int i = 0; i < robots.size(); ++i)
		fprintf(fp, "R%d = %.6lf %.6lf 0.000000\n", i + 1, (robots[i].x - origin.x)/20.0, (robots[i].y - origin.y)/20.0);
	fprintf(fp, "\n# >> Walls\n");
	for (int i = 0; i < mapwalls.size(); ++i)
		fprintf(fp, "WALL%d = %.6lf %.6lf %.6lf %.6lf\n", i + 1, mapwalls[i].x1,mapwalls[i].y1, mapwalls[i].x2, mapwalls[i].y2);
	fprintf(fp, "\n# >> Features\n");
	for (int i = 0; i < features.size(); ++i)
		fprintf(fp, "FEAT%d = %.6lf %.6lf %.6lf %.6lf\n", i+1, features[i].val1, features[i].val2, features[i].val3, features[i].val4);
	fclose(fp);
	printf("Map saved as scene.map\n");
}

void Map::saveOmap()
{
	IplImage *img = cvCreateImage(cvSize(540, 420), 8, 1);
	cvZero(img);
	for (int i = 0; i < mapwalls.size(); ++i)
	{
		double x1 = (mapwalls[i].x1/38.0)*540;
		double y1 = (mapwalls[i].y1/26.0)*420;
		double x2 = (mapwalls[i].x2/38.0)*540;
		double y2 = (mapwalls[i].y2/26.0)*420;
		cvLine(img, cvPoint((int)x1, (int)y1), cvPoint((int)x2, (int)y2), CV_RGB(255,255,255));
	}
	for (int i = 0; i < robots.size(); ++i)
	{
		double x = (robots[i].x/800.0)*540.0;
		double y = (robots[i].y/600.0)*420.0;
		CvPoint pt;
		pt.x = (int)x;
		pt.y = (int)y;
		cvFloodFill(img, pt, CV_RGB(255,255,255));
	}
	ofstream outfile;
	outfile.open("scene.omap", ios::out | ios::binary);
	unsigned short width = 540;
	unsigned short height = 420;
	float realWidth = 54;
	float realHeight = 42;
	float xorigin = -8;
	float yorigin = -8;
	float resolution = 0.1;
	int totalsize = 226800;
	cell* cells = new cell[totalsize]();
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			if(returnPixel1C(img, i, j) != 0)
				cells[j*width + i].val = 0.0f;
			else
				cells[j*width + i].val = 50.0f;
		}
	}
	for (int cnt = 0; cnt < mapwalls.size(); ++cnt)
	{
		double x1 = (mapwalls[cnt].x1/38.0)*540;
		double y1 = (mapwalls[cnt].y1/26.0)*420;
		double x2 = (mapwalls[cnt].x2/38.0)*540;
		double y2 = (mapwalls[cnt].y2/26.0)*420;
		for (int i = x1; i <= x2; ++i)
		{
			for (int j = y1; j <= y2; ++j)
			{
				cells[j*width + i].val = 100.0f;
			}
		}
	}
	if (outfile.good())
	{
		outfile.write((char *) &width, sizeof width);
		outfile.write((char *) &height, sizeof height);
		outfile.write((char *) &realWidth, sizeof realWidth);
		outfile.write((char *) &realHeight, sizeof realHeight);
		outfile.write((char *) &xorigin, sizeof xorigin);
		outfile.write((char *) &yorigin, sizeof yorigin);
		outfile.write((char *) &resolution, sizeof resolution);
		outfile.write((char *) &totalsize, sizeof totalsize);
		outfile.write((char *) cells, totalsize*sizeof(cell));
		outfile.close();
	}
}