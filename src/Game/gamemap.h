#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <vector>
#include "../Utils/micropather.h"
#include "editor.h"

class C_GameMap : public micropather::Graph
{
private:
    int MAPX, MAPY;
    int maxDir;

public:
    std::vector<void*> path;
    micropather::MicroPather* aStar;
    float totalCost;

    C_GameMap()
    {
        aStar = NULL;
    }

	virtual ~C_GameMap()
	{
		if (aStar) delete aStar;
	}

	void init()
	{
        MAPX = Editor->getWordlWidth();
        MAPY = Editor->getWordlHeigh();
        if (aStar) delete aStar;
        aStar = new micropather::MicroPather( this, MAPX*MAPY/4, 6 );
        maxDir = 8;
	}

	bool getPath(int sx, int sy, int ex, int ey)
	{
	    int result = aStar->Solve( XYToNode( sx, sy ), XYToNode( ex, ey ), &path, &totalCost );
	    if ( result == micropather::MicroPather::SOLVED ) return true;
	    return false;
	}

	int Passable( int x, int y, int nx, int ny )
	{
		if (    nx >= 0 && nx < MAPX
			 && ny >= 0 && ny < MAPY )
		{
		    int dx=nx-x, dy=ny-y;
            if ( abs(dx)+abs(dy)==2 ) //ход по диагонали
            {
                if ( Editor->getTile(nx,ny)->isPassable //доступна целевая клетка
                    && Editor->getTile(x+dx, y)->isPassable // и пересекаемая диагональ
                    && Editor->getTile(x, y+dy)->isPassable // свободна
                    ) return 1;
                return 0;
            }
			else
                if ( Editor->getTile(nx,ny)->isPassable ) return 1;
		}
		return 0;
	}

	void NodeToXY( void* node, int* x, int* y )
	{
		int index = (int)node;
		*y = index / MAPX;
		*x = index - *y * MAPX;
	}

	void* XYToNode( int x, int y )
	{
		return (void*) ( y*MAPX + x );
	}


	virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd )
	{
		int xStart, yStart, xEnd, yEnd;
		NodeToXY( nodeStart, &xStart, &yStart );
		NodeToXY( nodeEnd, &xEnd, &yEnd );

		int dx = xStart - xEnd;
		int dy = yStart - yEnd;
		return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
	}

	virtual void  AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors )
	{
		int x, y;
		//					E  N  W   S     NE  NW  SW SE
		const int dx[8] = { 1, 0, -1, 0,	1, -1, -1, 1 };
		const int dy[8] = { 0, -1, 0, 1,	-1, -1, 1, 1 };
		const float cost[8] = { 1.0f, 1.0f, 1.0f, 1.0f,
								1.41f, 1.41f, 1.41f, 1.41f };

		NodeToXY( node, &x, &y );

		for( int i=0; i<maxDir; ++i )
		{
			int nx = x + dx[i];
			int ny = y + dy[i];

			int pass = Passable( x, y, nx, ny );
			if ( pass > 0 ) {
				// Normal floor
				micropather::StateCost nodeCost = { XYToNode( nx, ny ), cost[i] * (float)(pass) };
				neighbors->push_back( nodeCost );
			}
		}
	}

	virtual void  PrintStateInfo( void* node )
	{
		int x, y;
		NodeToXY( node, &x, &y );
		printf( "(%2d,%2d)", x, y );
    }

	void PrintPath()
	{
	    printf("path: \n");
	    for(int i=0, imax=path.size(); i<imax; ++i)
	    {
	        PrintStateInfo( path[i] );
	    }
	    printf("\n");
	}
};

#endif // GAMEMAP_H_INCLUDED
