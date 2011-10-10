#ifndef WAVEFINDWAY_H_INCLUDED
#define WAVEFINDWAY_H_INCLUDED

#include <string.h>
#include <vector>
#include "../Game/editor.h"

typedef struct waypoint_t
{
    waypoint_t(int x, int y, int cost, int costa, int owner) { X=x; Y=y; Cost=cost; CostA=costa; Owner=owner;}
    int X;
    int Y;
    int Cost;  // стоимость точки
    int CostA; // общая стоимость до этой точки
    int Owner; // предыдущая точка пути
};

class C_WaveFindWay
{
public:
    std::vector<waypoint_t> path;

    void init()
    {
        MAPX = Editor->getWordlWidth();
        MAPY = Editor->getWordlHeigh();
        Editor->fillCostMap(movecost);
    }

    void setCost(int x, int y, int c) { movecost[y][x] = c; }
    irr::u8 getCost(int x, int y) { return movecost[y][x]; }

    bool getPath(int sx,int sy,int tx,int ty)
    {
        int x,y;

        memset(fillmap,0,sizeof(fillmap)); // Вначале очищаем fillmap

        pbuf.clear();
        // стартовая позиция
        pidx = 1;
        /*
            так уж повелось в реляционных базах что индексы нумеруются с 1
            а я использую реляцию, то придеться ввести виртуальный индекс
            и индексы в pbuf будут равны pidx-1
        */
        pbuf.push_back(waypoint_t(sx,sy,0,0,0));

        /*
            упарился я уже memset может заполнить память нормально только нулями,
            поэтому ставлю тут обходную заплатку, делаю точку старта непроходимой,
            что поможет избежать лишних условий в addPoint после цикла верну "стоимость"
        */
        int saveCost = movecost[sy][sx];
        movecost[sy][sx] = 0;

        int foundedPath = 8; // в идеале можно найти 8 путей

        while(getPoint(&x,&y)) // Цикл, пока есть точки в буфеpе
        {
            if( x==tx && y==ty )
            {
                if (!foundedPath--) break; // все пут нашелись и в pbuf[ fillmap[y][x]-1 ] его конечная точка
            }

            //Пеpебоp 8-х соседних клеток
            addPoint(x  ,y-1, movecost[y-1][x  ], 2, pidx); // N
            addPoint(x+1,y-1, movecost[y-1][x+1], 3, pidx); // NE
            addPoint(x+1,y  , movecost[y  ][x+1], 2, pidx); // E
            addPoint(x+1,y+1, movecost[y+1][x+1], 3, pidx); // SE
            addPoint(x  ,y+1, movecost[y+1][x  ], 2, pidx); // S
            addPoint(x-1,y+1, movecost[y+1][x-1], 3, pidx); // SW
            addPoint(x-1,y  , movecost[y  ][x-1], 2, pidx); // W
            addPoint(x-1,y-1, movecost[y-1][x-1], 3, pidx); // NW

            pidx++;
         }

         movecost[sy][sx] = saveCost; // ворачиваю обещанное

        if(fillmap[ty][tx]==0) return false; //путь не найден

        path.clear(); // зачищаем старый путь
        int idx = fillmap[ty][tx]-1;
        printf("---\n");
        while(idx > -1 ) // выбираем по отбратной связи через Owner все точки следования
        {
            printf("%d: (%d,%d->%d,%d) %d %d\n", idx, sx,sy,tx,ty, pbuf[idx].X, pbuf[idx].Y);
            path.push_back( waypoint_t(pbuf[idx].X, pbuf[idx].Y, pbuf[idx].Cost, 0,0) );
            idx = pbuf[idx].Owner-1;
        }
        testCost(tx,ty);
         // но ессно надо учитывать что точки следования в обратном порядке и надо использовать
         // реверсный итератор для их обхода, ну или считить за запросить путь от финиша к старту
        return true;
    }//getPath

    void testCost(int x, int y)
    {
        printf("----movecost-----\n");
        for(int i=y-5, imax=y+5; i<imax; ++i)
        {
            for(int j=x-5, jmax=x+5; j<jmax; ++j)
            {
                if (movecost[i][j]==99)
                    printf("x ");
                else
                    printf("%d ", movecost[i][j]);
            }
            printf("\n");
        }
        printf("-------------------\n");
    }

private:
    int MAPX, MAPY;

    std::vector<waypoint_t> pbuf;
    int pidx;

    irr::u8 movecost[256][256]; // кеш карты, в ячейках храним цену проходимости
    int fillmap[256][256];  // Pазмеp == pазмеpу лабиpинта


    void addPoint(int x,int y, int cost, int koef, int owner)
    {
        //клетка в пределах карты и доступна
        if ( x>-1 && x<MAPX && y>-1 && y<MAPY && movecost[y][x] != 0)
        {
            int full_cost = pbuf[owner-1].CostA+cost+koef; // полная стоимость нового пути

            if ( fillmap[y][x]==0 ) // мы еще не были в этой точке
            {
                pbuf.push_back( waypoint_t(x, y, cost, full_cost, owner) );
                fillmap[y][x] = pbuf.size(); // создаем новую точку маршрута
                return;
            }

            // иначе если стоимость нового маршрута дешевле
            if ( full_cost < pbuf[ fillmap[y][x] ].CostA )
            {
                pbuf.push_back( waypoint_t(x, y, cost, full_cost, owner) );
                fillmap[y][x] = pbuf.size(); // обрываем старый маршрут новым
            }
         }
    }

    bool getPoint(int *x,int *y)
    {
        int idx = pidx-1;
        if (idx == pbuf.size()) return false;
        *x = pbuf[idx].X;
        *y = pbuf[idx].Y;
        return true;
    }

};

#endif // WAVEFINDWAY_H_INCLUDED
