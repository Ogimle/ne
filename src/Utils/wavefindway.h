#ifndef WAVEFINDWAY_H_INCLUDED
#define WAVEFINDWAY_H_INCLUDED

#include <string.h>
#include <vector>
#include "../Game/editor.h"

typedef struct waypoint_t
{
    waypoint_t(int x, int y, int costa, int owner) { X=x; Y=y; CostA=costa; Owner=owner;}
    int X;
    int Y;
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
    irr::u16 getCost(int x, int y) { return movecost[y][x]; }

    bool getPath(int sx,int sy,int tx,int ty)
    {
        int x,y;

        // запоминаем конечную точку маршрута, чтобы не передавать
        // через аргументы в addPoint
        endPointX = tx; endPointY = ty;

        memset(fillmap,0,sizeof(fillmap)); // очищаем fillmap
        candidates.clear(); // очищаем список кандидатов на лучший путь
        pbuf.clear(); // очищаем буфер цепочек путей

        /*
            так уж повелось в реляционных базах что индексы нумеруются с 1
            а я использую реляцию, то придеться ввести виртуальный индекс
            и индексы в pbuf будут равны pidx-1
        */
        pidx = 1;

        // первая точка в пути стартовая, имеет 0 веса и не имеет предка
        pbuf.push_back(waypoint_t(sx,sy,0,0));

        /*
            упарился я уже memset может заполнить память нормально только нулями,
            поэтому ставлю тут обходную заплатку, делаю точку старта непроходимой,
            что поможет избежать лишних условий в addPoint после цикла верну "стоимость"
        */
        int saveCost = movecost[sy][sx];
        movecost[sy][sx] = 0;

        while(getPoint(&x,&y)) // Цикл, пока есть точки в буфеpе
        {
            //Пеpебоp 8-х соседних клеток
            addPoint(x  ,y-1, movecost[y-1][x  ]+2, pidx); // N
            addPoint(x  ,y+1, movecost[y+1][x  ]+2, pidx); // S
            addPoint(x+1,y  , movecost[y  ][x+1]+2, pidx); // E
            addPoint(x-1,y  , movecost[y  ][x-1]+2, pidx); // W
            addPoint(x+1,y-1, movecost[y-1][x+1]+3, pidx); // NE
            addPoint(x-1,y+1, movecost[y+1][x-1]+3, pidx); // SW
            addPoint(x+1,y+1, movecost[y+1][x+1]+3, pidx); // SE
            addPoint(x-1,y-1, movecost[y-1][x-1]+3, pidx); // NW

            pidx++;
         }

         movecost[sy][sx] = saveCost; // ворачиваю обещанное

        if(candidates.size()==0) return false; //путей нет

        //вычисляем самый дешевый путь
        int theBestID = candidates[0];
        for(int i=1, imax=candidates.size(); i<imax; ++i)
            if ( pbuf[theBestID].CostA > pbuf[ candidates[i] ].CostA ) theBestID = candidates[i];

        path.clear(); // зачищаем старый путь
        path.push_back( waypoint_t(tx,ty,0,0) ); // закидываем в путь конечную точку, а тож мы ее обнуляли постоянно
        int idx = theBestID-1;
        while(idx > -1 ) // выбираем по отбратной связи через Owner все точки следования
        {
            path.push_back( waypoint_t(pbuf[idx].X, pbuf[idx].Y, 0, 0) );
            idx = pbuf[idx].Owner-1;
        }

        //testCost(tx,ty);
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
                if (movecost[i][j]==1001)
                    printf("A ");
                else if (movecost[i][j]==1000)
                    printf("X ");
                else
                    printf("%d ", movecost[i][j]);
            }
            printf("\n");
        }
        printf("-------------------\n");
    }

private:
    int MAPX, MAPY; //размер карты, максимум 256х256

    irr::u16 endPointX, endPointY; // конечная точка маршрута

    std::vector<int> candidates; // список путей кандидатов на звание лучший

    std::vector<waypoint_t> pbuf; // буффер всех точек попавших в поиск
    int pidx;

    irr::u16 movecost[256][256]; // кеш карты, в ячейках храним цену проходимости
    irr::u16 fillmap[256][256];  // ячейки содержат индексы pbuf

    void addPoint(irr::u16 x, irr::u16 y, int cost, int owner)
    {
        //клетка в пределах карты и доступна
        // мы еще не были в этой точке
        if ( x>-1 && x<MAPX && y>-1 && y<MAPY && movecost[y][x] != 0 && fillmap[y][x]==0)
        {
            // пишем как следующую точку маршрута
            pbuf.push_back( waypoint_t(x, y, pbuf[owner-1].CostA+cost, owner) );

            if( x==endPointX && y==endPointY ) // какой-то из маршрутов дошел до конца
            {
                // добавляем кандидата на лучший путь
                candidates.push_back(pbuf.size());
                // но конечную точку не занимаем, чтобы остальные могли дойти
                return;
            }
            fillmap[y][x] = pbuf.size();
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
