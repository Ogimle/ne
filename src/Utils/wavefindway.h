#ifndef WAVEFINDWAY_H_INCLUDED
#define WAVEFINDWAY_H_INCLUDED

#include <vector>
#include "../Game/editor.h"

struct Point {
    int X;
    int Y;
    int w;

    Point(int x_ = 0, int y_ = 0, int w_ = 0)
    {
        X = x_;
        Y = y_;
        w = w_;
    }

};

#define ADD_POINT {									\
    if (distance[y][x] == 0) {                      \
                                                    \
        distance[y][x] = w;                    		\
                                                    \
        points[points_count].X = x;             	\
        points[points_count].Y = y;             	\
        points[points_count].w = w;                 \
        points_count ++;        		            \
    }												\
}

class C_WaveFindWay
{
public:
    std::vector<Point> path;

    void init()
    {
        MAPX = Editor->getWordlWidth();
        MAPY = Editor->getWordlHeigh();
        MAX_PATH = 500;
        Editor->fillCostMap(movecost);
    }

    void setCost(int x, int y, int c) { movecost[y][x] = c; }
    int getCost(int x, int y) { return movecost[y][x]; }

    bool getPath(int sx,int sy,int tx,int ty)
    {
		if (sx < 0 || sy < 0 || sx >= MAPX || sy >= MAPY) return false;
		if (sx == tx && sy == ty ) return false;

		makeWave(tx,ty);
		if (distance[sy][sx] == 0) return false;

		int x = sx;
		int y = sy;

		path.clear();
		path.push_back( Point(sx,sy,1) );

		while (distance[y][x] != 1)
		{
			int lx = -1;
			int hx = 1;
			if (x + lx < 0) lx = 0;
			if (x + hx >= MAPX) hx = 0;

			int ly = -1;
			int hy = 1;
			if (y + ly < 0) ly = 0;
			if (y + hy >= MAPY) hy = 0;

			int min_x = lx;
			int min_y = ly;

			for (int j = ly; j<=hy; j++)
				for (int i = lx; i<=hx; i++)
				{

					if (i == 0 && j == 0) continue;
					if (distance[y + j][x + i] == 0 || distance[y + min_y][x + min_x] <= distance[y + j][x + i]) continue;

					min_x = i;
					min_y = j;
				}

			x += min_x;
			y += min_y;
			path.push_back( Point(x,y,1) );
			if (path.size()>500) return false;
		}

		return true;
    }//getPath

    void testCost(irr::IrrlichtDevice* Device)
    {
        irr::video::SMaterial material;
        material.setTexture(0, 0);
        material.Lighting = false;
        material.NormalizeNormals = true;
        material.setFlag(irr::video::EMF_BACK_FACE_CULLING, true);

        for (int y=0; y<MAPY; y++)
        for (int x=0; x<MAPX; x++)
        if (movecost[y][x]>0 && movecost[y][x]<10000)
        {

            irr::core::triangle3df t(
                irr::core::vector3df(-x,  0.1, y),
                irr::core::vector3df(-x-1,0.1, y),
                irr::core::vector3df(-x,  0.1, y+1)
            );
            irr::core::triangle3df t2(
                irr::core::vector3df(-x-1,  0.1, y),
                irr::core::vector3df(-x-1,  0.1, y+1),
                irr::core::vector3df(-x,    0.1, y+1)
            );
            Device->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
            Device->getVideoDriver()->setMaterial(material);
            Device->getVideoDriver()->draw3DTriangle(t, irr::video::SColor(0,0,255,0));
            Device->getVideoDriver()->draw3DTriangle(t2, irr::video::SColor(0,0,255,0));
        }
    }

private:
    int MAPX, MAPY; //размер карты, максимум 256х256
    int MAX_PATH; // думаю дольше 500 мувпоинтов не стоит кататься

    int movecost[256][256]; // кеш карты, в ячейках храним цену проходимости
    int distance[256][256];  //
    Point points[256*256];
    int points_count;

	void addPoint(int x, int y, int w)
	{
		distance[y][x] = w;

		points[points_count].X = x;
		points[points_count].Y = y;
		points[points_count].w = w;

		points_count ++;
	}

	void makeWave(int sx, int sy)
	{
		if (sx < 0 || sy < 0 || sx >= MAPX || sy >= MAPY) return;

		//memset(distance, 0, sizeof(distance));
		points_count = 0;

		int *src = (int *)movecost;
		int *dst = (int *)distance;

		for (int i=0, imax=256*256; i<imax; ++i) dst[i] = src[i];

		addPoint(sx, sy, 1);

		int point_index = 0;

		Point *point;
		int x;
		int y;
		int w;

		while (point_index < points_count)
		{
			point = &points[point_index];
			x = point->X;
			y = point->Y;
			w = point->w + 1;

			x--;
			if (x >= 0) ADD_POINT;

			x += 2;
			if (x < MAPX) ADD_POINT;

			x -= 1;
			y--;
			if (y >= 0) ADD_POINT;

			y += 2;
			if (y < MAPY) ADD_POINT;

			point_index ++;
		}
	}

};

#endif // WAVEFINDWAY_H_INCLUDED
