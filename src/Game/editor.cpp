#include <stdio.h>
#include "editor.h"

//const int max_vert_in_buffer = (65536/3/2) * 4; // в 16битный индексный буфер опишет только 65536/3 треугольников /2 квадратов * 4 вершин
const int max_vert_in_buffer = 65536; // в 16битный индексный буфер опишет только 65536 треугольников

irr::u8 tileTmpTable[5][5];

void printTileTmpTable( const irr::c8 *name, const irr::u8 a[5][5], xz_key koord=xz_key(0,0) )
{
    printf("%s at (%d,%d):\n", name, koord.X, koord.Y);

    printf("%d %d %d %d %d\n", a[0][0], a[0][1], a[0][2], a[0][3], a[0][4]);
    printf("%d %d %d %d %d\n", a[1][0], a[1][1], a[1][2], a[1][3], a[1][4]);
    printf("%d %d %d %d %d\n", a[2][0], a[2][1], a[2][2], a[2][3], a[2][4]);
    printf("%d %d %d %d %d\n", a[3][0], a[3][1], a[3][2], a[3][3], a[3][4]);
    printf("%d %d %d %d %d\n", a[4][0], a[4][1], a[4][2], a[4][3], a[4][4]);
}

const float tex_co[16][4] =
{
    //1 ряд
    { 0.0000f, 0.0000f, 0.1240f, 0.1240f }, //1/1   1
    { 0.3750f, 0.1250f, 0.4990f, 0.2500f }, //2/4   2
    { 0.2500f, 0.0000f, 0.3740f, 0.1240f }, //1/3   3
    { 0.2500f, 0.1250f, 0.3740f, 0.2500f }, //2/3   4
    { 0.3750f, 0.0000f, 0.4990f, 0.1240f }, //1/4   5
    { 0.8750f, 0.1250f, 1.0000f, 0.2500f }, //2/8   6
    { 0.7500f, 0.1250f, 0.8740f, 0.2500f }, //2/7   7
    { 0.7500f, 0.0000f, 0.8740f, 0.1240f }, //1/7   8
    //2 ряд
    { 0.8750f, 0.0000f, 1.0000f, 0.1240f }, //1/8   9
    { 0.5000f, 0.0000f, 0.6240f, 0.1240f }, //1/5   10
    { 0.6250f, 0.0000f, 0.7490f, 0.1240f }, //1/6   11
    { 0.6250f, 0.1250f, 0.7490f, 0.2500f }, //2/6   12
    { 0.5000f, 0.1250f, 0.6240f, 0.2500f }, //2/5   13
    { 0.0000f, 0.1250f, 0.1240f, 0.2500f }, //2/1   14
    { 0.1250f, 0.1250f, 0.2490f, 0.2500f }, //2/2   15
    { 0.1250f, 0.0000f, 0.2490f, 0.1240f }  //1/2   16
};

//текстурные координаты для тайлов в ширину
const float tex_x_l[16] = {0.0000f, 0.064450, 0.126950, 0.189450, 0.251950, 0.314450, 0.376950, 0.439450, 0.501950, 0.564450, 0.626950, 0.689450, 0.751950, 0.814450, 0.876950, 0.939450};
const float tex_x_r[16] = {0.06250, 0.125000, 0.187500, 0.250000, 0.312500, 0.375000, 0.437500, 0.500000, 0.562500, 0.625000, 0.687500, 0.750000, 0.812500, 0.875000, 0.937500, 1.00000f};

//текстурные координаты для тайлов в высоту
const float tex_y_u[4]  = {0.000f, 0.25800, 0.5080, 0.7580};
const float tex_y_d[4]  = {0.2500, 0.50000, 0.7500, 1.000f};

const int tailBaseTable[25][16] =
{
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//0
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//1
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//2
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//3
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//4
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//5
    { 9, 1, 6, 8, 4, 5, 6,15, 8, 9, 1,14, 4, 5,14,16},//6
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,16},//7
    {10, 1, 2, 7, 5, 5, 6, 7,15, 1,10, 2,13,13, 6,16},//8
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//9
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//10
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,16},//11
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//12
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,16},//13
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//14---
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//15
    {12, 5, 7, 3, 4, 5,15, 7, 8, 4,13, 3,12,13, 8,16},//16
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,16},//17
    {11, 6, 2, 3, 8,15, 6, 7, 8,14, 2,11, 3, 7,14,16},//18
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//19
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//20
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//21
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//22
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//23
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16} //24
};

const int tailEqualTable[25][16] =
{
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//0
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//1
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//2
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//3
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//4
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//5
    {16,10,16,16,12,13, 2,16, 3, 0,16,16,16,16,11, 7},//6
    {16, 0,11,16,12,12,11, 3, 3, 0, 0,16,16,12,11, 3},//7
    {16, 9,11,16,16, 4,14, 3,16,16, 0,16,16,12,16, 8},//8
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//9
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//10
    {16,10,16,11, 0,10, 2, 2,11, 0,16,16, 0,10,11, 2},//11
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//12
    {16, 9, 0,12,16, 4, 9,12, 4,16, 0, 0,16,12, 9, 4},//13
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//14---
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//15
    {16,16,16,11, 9, 1,16, 2,14,16,16,16, 0,10,16, 6},//16
    {16,16,10, 0, 9, 1, 1,10, 9,16,16, 0, 0,10, 9, 1},//17
    {16,16,10,12,16,16, 1,13, 4,16,16, 0,16,16, 9, 5},//18
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//19
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//20
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//21
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//22
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},//23
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16} //24
};

const int tailDiffTable[25][16] =
{
    { 9, 1, 6, 8, 4, 5, 6,15, 8, 9, 1,14, 4, 5,14,15},
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,15},
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,15},
    { 1, 1, 6,15, 5, 5, 6,15,15, 1, 1, 6, 5, 5, 6,15},
    {10, 1, 2, 7, 5, 5, 6, 7,15, 1,10, 2,13,13, 6,15},
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,15},
    {23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23},
    {19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19},
    {24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24},
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,15},
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,15},
    {18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18},
    {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
    {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20},
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,15},
    { 4, 5,15, 8, 4, 5,15,15, 8, 4, 5, 8, 4, 5, 8,15},
    {22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22},
    {17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17},
    {21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21},
    { 2, 6, 2, 7,15,15, 6, 7,15, 6, 2, 2, 7, 7, 6,15},
    {12, 5, 7, 3, 4, 5,15, 7, 8, 4,13, 3,12,13, 8,15},
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,15},
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,15},
    { 3,15, 7, 3, 8,15,15, 7, 8, 8, 7, 3, 3, 7, 8,15},
    {11, 6, 2, 3, 8,15, 6, 7, 8,14, 2,11, 3, 7,14,15}
};

C_Editor::C_Editor(irr::IrrlichtDevice* device) : camera(device)
{
    Device = device;
    minimap = NULL;
    Skydome = NULL;
    root_sn = NULL;
    root_bsn = NULL;
    for(irr::u8 i=0;i<9;i++)
    {
        regions[i].land = NULL;
    }
}

C_Editor::~C_Editor()
{
    //dtor
}

bool C_Editor::createMap(const irr::c8 *fname, irr::u8 size, irr::u8 worldtype)
{
    map_file = "./res/maps/";
    map_file.append(fname);

    worldSize = size;

    if      (worldSize == 0) { worldWidth = 256;   worldHeight = 8;  regions_count = 2; regions_per_x = 2; regions_per_z = 1; }
    else if (worldSize == 1) { worldWidth = 128;   worldHeight = 32; regions_count = 4; regions_per_x = 4; regions_per_z = 1; }
    else if (worldSize == 2) { worldWidth = 64;    worldHeight = 64; regions_count = 4; regions_per_x = 2; regions_per_z = 2; }
    else                     { worldWidth = 96;    worldHeight = 96; regions_count = 9; regions_per_x = 3; regions_per_z = 3; }

    minimap = new C_MiniMap( Device->getGUIEnvironment(), Device->getGUIEnvironment()->getRootGUIElement(), -1, irr::core::rect<irr::s32>(0, 0, worldWidth*2, worldHeight*2) );
    minimap->setRelativePosition( irr::core::position2di(10, 60) );
    minimap->setMiniMapSize(worldWidth, worldHeight);

    camera.camSet(worldWidth, worldHeight);

    worldType = worldtype;

    Device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);

    irr::io::path pth("./res/tex/skydome_earth.jpg");
    if (worldType==1) pth = "./res/tex/skydome_cosmo.jpg";

    Skydome = Device->getSceneManager()->addSkyDomeSceneNode(Device->getVideoDriver()->getTexture(pth),32,8,0.95f,2.0f,100);
    Skydome->setVisible(true);
    Device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);


    clearLand();

    root_sn = Device->getSceneManager()->addEmptySceneNode();
    root_bsn = Device->getSceneManager()->addEmptySceneNode();
    createLand(worldType);

    return true;
}

void C_Editor::fillMiniMap()
{
    if ( !minimap->lockMapTex() ) return;
    for(irr::u16 z=0, zmax=worldHeight; z<zmax; z++)
    for(irr::u16 x=0, xmax=worldWidth;  x<xmax; x++)
    {
        xz_key tkey(x,z);
        irr::u16 b_idx = ((tile_t*)tiles[tkey])->tex_id / 16 ;
        irr::u16 t_idx = ((tile_t*)tiles[tkey])->tex_id % 16 ;

        if (b_idx==0 && t_idx==15) // земля
        {
            minimap->setPixelEx(x,z, irr::video::SColor(255, 116, 65, 2), true);
        }
        else if (b_idx==0) // вода
        {
            minimap->setPixelEx(x,z, irr::video::SColor(255, 5, 55, 114), true);
        }
        else if (b_idx==1) // камни
        {
            minimap->setPixelEx(x,z, irr::video::SColor(255, 116, 116, 116), true);
        }
        else if (b_idx==2) // трава
        {
            minimap->setPixelEx(x,z, irr::video::SColor(255, 46, 89, 17), true);
        }

        tile_t* tile = (tile_t*)tiles[tkey];
        if ( !tile->isBuildable )
        {
            if ( tile->isPassable )
                minimap->setPixelEx(x,z, irr::video::SColor(255, 255, 0, 0), true);
            else
                minimap->setPixelEx(x,z, irr::video::SColor(255, 0, 0, 0), true);
        }
    }
    minimap->unlockMapTex();
}

bool C_Editor::fillTileTmpTable(xz_key t_, bool buildCheck)
{
    int sx = 0, sy = 0;

    for(int cy=0;cy<5;cy++)
    {
        sy = t_.Y-2+cy;
        for(int cx=0; cx<5; cx++)
        {
            sx = t_.X-2+cx;
            if ( sx > -1  && sx < worldWidth && sy > -1 && sy < worldHeight )
            {
                xz_key tkey(sx,sy);
                if ( buildCheck && cx>0&&cx<4 && cy>0&&cy<4 && !((tile_t*)tiles[tkey])->isBuildable ) return false;
                //tileTmpTable[cy][cx] = tiles.find(tkey)->getValue()->tex_id;
                tileTmpTable[cy][cx] = ((tile_t*)tiles[tkey])->tex_id;
            }
            else
                tileTmpTable[cy][cx] = -1;
        }
    }
    #if _DEBUG
    //printTileTmpTable("fillTileTmpTable",  tileTmpTable, t_);
    #endif
    return true;
}//fillTileTmpTable

void C_Editor::prepareTileTmpTable(int b, bool isEarth)
{
    int
        b_idx = 0, //нормер ряда с 16ю текстрами вида ландшафта
        t_idx = 0, //номер текструры в ряду
        cnt = 0;

    if ( isEarth )
    {
        tileTmpTable[2][2] = b*16+15;
        for (int y=0; y<5; y++)
        {
            for (int x=0; x<5; x++)
            {
                if ( tileTmpTable[y][x] > -1 )
                {
                    b_idx = int( tileTmpTable[y][x] / 16 );
                    t_idx = int( tileTmpTable[y][x] % 16 );
                    if ( tailBaseTable[cnt][t_idx] != 16 )
                        tileTmpTable[y][x] = b_idx*16 + tailBaseTable[cnt][t_idx];
                }
                cnt++;
            }
        }
    }
    else
    {
        tileTmpTable[2][2] = b * 16;
        for (int y=0; y<5; y++)
        {
            for (int x=0; x<5; x++)
            {
                if ( tileTmpTable[y][x] > -1 )
                {
                    b_idx = int( tileTmpTable[y][x] / 16 );
                    t_idx = int( tileTmpTable[y][x] % 16 );

                    if ( b_idx == b) // Если прежняя земля имеет тот же тип, что и активная
                    {
                        if ( tailEqualTable[cnt][t_idx] != 16 ) tileTmpTable[y][x] = b_idx*16 + tailEqualTable[cnt][t_idx];
                    }
                    else // Если заменяемая и замещающая земля имеют разные типы
                    {
                        // Если число матрици = 16, то ничего не делаем
                        // Если < 16, то рисуем заменяемой землей
                        // Если > 16, то рисуем замещающей (с хитрым индексом секрет которого не разгадан мной)

                        if (tailDiffTable[cnt][t_idx] < 16)
                        {
                            tileTmpTable[y][x] = b_idx*16 + tailDiffTable[cnt][t_idx];
                        }
                        else if (tailDiffTable[cnt][t_idx] > 16)
                        {
                            tileTmpTable[y][x] = b*16 + tailDiffTable[cnt][t_idx]-16;
                        }
                    }
                }
                cnt++;
            }
        }
    }
    #if _DEBUG
    //printTileTmpTable("prepareTileTmpTable",  tileTmpTable);
    #endif
}//prepareTileTmpTable

void C_Editor::placeTileTmpTable(xz_key t_)
{
    irr::u16 sx=0, sy=0, b_idx = 0, t_idx = 0;

    for(irr::u8 cy=0; cy<5; cy++)
    {
        sy = t_.Y-2+cy;
        for(irr::u8 cx=0; cx<5; cx++)
        {
            sx = t_.X-2+cx;
            if ( sx > -1 && sx < worldWidth && sy > -1 && sy < worldHeight )
            {
                xz_key tkey(sx,sy);
                if ( ((tile_t*)tiles[tkey])->isBuildable && ((tile_t*)tiles[tkey])->tex_id != tileTmpTable[cy][cx] )
                {
                    ((tile_t*)tiles[tkey])->tex_id = tileTmpTable[cy][cx];

                    b_idx = tileTmpTable[cy][cx] / 16 ;
                    t_idx = tileTmpTable[cy][cx] % 16 ;

                    //if ( isEarth )
                    //    setTileTex(sx, sy, b_idx, 15);
                    //else
                    setTileTex( tkey, b_idx, t_idx );

                    if (b_idx==0 && t_idx==15) // земля
                    {
                        minimap->setPixel(sx,sy, irr::video::SColor(255, 116, 65, 2), true);
                        setTileFlag( t_, true, true, true );
                    }
                    else if (b_idx==0) // вода
                    {
                        minimap->setPixel(sx,sy, irr::video::SColor(255, 5, 55, 114), true);
                        setTileFlag( t_, false, true, true );
                    }
                    else if (b_idx==1) // камни
                    {
                        minimap->setPixel(sx,sy, irr::video::SColor(255, 116, 116, 116), true);
                        setTileFlag( t_, true, true, true );
                    }
                    else if (b_idx==2) // трава
                    {
                        minimap->setPixel(sx,sy, irr::video::SColor(255, 46, 89, 17), true);
                        setTileFlag( t_, true, true, true );
                    }
                }
            }
        }
    }

    #if _DEBUG
    //printTileTmpTable("placeTileTmpTable",  tileTmpTable);
    #endif
}//placeTileTmpTable

void C_Editor::set2dTile(xz_key t_, int b_, bool isEarth,  bool buildCheck)
{
    if ( !((tile_t*)tiles[t_])->isBuildable ) return;

    if (!fillTileTmpTable(t_, buildCheck)) return;
    prepareTileTmpTable(b_, isEarth);
    placeTileTmpTable(t_);
}//set2dTile

bool C_Editor::deleteDynObj(irr::s32 x, irr::s32 y)
{
    irr::scene::ISceneCollisionManager * cm = Device->getSceneManager()->getSceneCollisionManager();
    irr::core::line3d<irr::f32> ray = cm->getRayFromScreenCoordinates(irr::core::position2di(x, y), camera.csn);

    irr::core::vector3df outCollisionPoint;
    irr::core::triangle3df outTriangle;

    irr::scene::ISceneNode* sn = cm->getSceneNodeAndCollisionPointFromRay(ray, outCollisionPoint, outTriangle, 0, root_bsn);

    if (sn)
    {
        for(irr::u16 i=0, imax = buildings.size(); i<imax; i++) //перебираем здания
        {
            if (sn==buildings[i]->msn) // если найдено
            {
                for(irr::u16 t=0, tmax=buildings[i]->tiles.size(); t<tmax; t++) // освобождаем тайлы
                {
                    xz_key tile = buildings[i]->tiles[t];
                    minimap->setPixel(tile.X,tile.Y, irr::video::SColor(255, 116, 65, 2), true);
                    setTileFlag( tile, true, true, true );
                }

                buildings[i]->msn->remove(); // удаляем ноду со сцены
                buildings.erase(i); // удаляем объект здания из пула

                return true;
            }
        }
    }

    return false;
}//deleteBuilding

void C_Editor::deleteStatObj()
{
    xz_key tile_pos(-posMapClick.X, posMapClick.Z), sht;

    for(irr::u8 sb=0; sb<SB_COUNT; sb++)
        if (regions[regMapClick].objs[sb].mb_shifts.find(tile_pos))
        {
            switch(sb)
            {
                case SB_WALL_1:
                case SB_WALL_2:
                    sht = regions[regMapClick].objs[sb].mb_shifts[tile_pos];
                    deleteModelBuffer( regions[regMapClick].objs[sb].msn, Device->getSceneManager()->getMesh("./res/mdl/wall_1.x"), sht );
                    regions[regMapClick].objs[sb].mb_deleted[tile_pos] = sht;
                    setTileFlag( tile_pos, true, true, true);
                break;

                case SB_WALL_3:
                case SB_WALL_4:
                    sht = regions[regMapClick].objs[sb].mb_shifts[tile_pos];
                    deleteModelBuffer( regions[regMapClick].objs[sb].msn, Device->getSceneManager()->getMesh("./res/mdl/wall_3.x"), sht );
                    regions[regMapClick].objs[sb].mb_deleted[tile_pos] = sht;
                    setTileFlag( tile_pos, true, true, true);
                break;

                case SB_TREE_1:
                    sht = regions[regMapClick].objs[sb].mb_shifts[tile_pos];
                    deleteModelBuffer( regions[regMapClick].objs[sb].msn, Device->getSceneManager()->getMesh("./res/mdl/tree_1.x"), sht );
                    regions[regMapClick].objs[sb].mb_deleted[tile_pos] = sht;
                    setTileFlag( tile_pos, true, true, true);
                break;
            }
            minimap->setPixel(tile_pos.X,tile_pos.Y, irr::video::SColor(255, 116, 65, 2), true);
        }
}//deleteFromMapClick

void C_Editor::genBuildingsMetaTriSelectors()
{
    //stat obj
    for(irr::u8 rc=0; rc<9; rc++)
    {
        regions[rc].buildings_selector =  Device->getSceneManager()->createMetaTriangleSelector();
        for(irr::u8 sb=0; sb<SB_COUNT; sb++)
        {
            if (sb != SB_TREE_1 && regions[rc].objs[sb].msn)
            {
                regions[rc].buildings_selector->addTriangleSelector(
                    Device->getSceneManager()->createTriangleSelector(
                        regions[rc].objs[sb].msn->getMesh(),
                        regions[rc].objs[sb].msn)
                );
            }
        }
    }

    //dyn obj
    buildings_selector =  Device->getSceneManager()->createMetaTriangleSelector();
    for(irr::u16 i=0, imax = buildings.size(); i<imax; i++) //перебираем здания
    {
        buildings_selector->addTriangleSelector(
            Device->getSceneManager()->createTriangleSelector(
                buildings[i]->msn->getMesh(),
                buildings[i]->msn)
        );
    }
}

bool C_Editor::setBase(irr::scene::IAnimatedMeshSceneNode* node)
{
    irr::core::vector3df pos;
    irr::core::vector3df tile3d;
    xz_key tile;

    // 1. выбираем тайлы по коорд. костей и проверяем возможность установки здания
    irr::core::array< irr::scene::IBoneSceneNode* > bones;
    bones.push_back( node->getJointNode("Ar_entrance") );
    bones.push_back( node->getJointNode("Ar_t1") );
    bones.push_back( node->getJointNode("Ar_t2") );
    bones.push_back( node->getJointNode("Ar_t3") );
    bones.push_back( node->getJointNode("Ar_t4") );
    bones.push_back( node->getJointNode("Ar_t5") );
    bones.push_back( node->getJointNode("Ar_t6") );
    bones.push_back( node->getJointNode("Ar_t7") );
    bones.push_back( node->getJointNode("Ar_t8") );
    bones.push_back( node->getJointNode("Ar_t9") );
    bones.push_back( node->getJointNode("Ar_t10") );
    bones.push_back( node->getJointNode("Ar_t11") );
    bones.push_back( node->getJointNode("Ar_t12") );
    bones.push_back( node->getJointNode("Ar_t13") );
    bones.push_back( node->getJointNode("Ar_t14") );


    irr::core::array< irr::core::vector3df > btiles;
    for(irr::u16 i=0, imax=bones.size(); i<imax; i++)
    {
        pos = bones[i]->getAbsolutePosition();
        tile3d = irr::core::vector3df( -pos.X, pos.Y, pos.Z );
        if ( tile3d.Z<0 || tile3d.Z>(worldHeight-1) || tile3d.X<0 || tile3d.X>(worldWidth-1) ) return false;
        if (!((tile_t*)tiles[xz_key(tile3d.X,tile3d.Z)])->isBuildable) return false;
        btiles.push_back( tile3d );
    }

    irr::u16 bid = buildings.size();
    buildings.push_back( new diobj_t );
    buildings[bid]->ab = AB_BASE;
    buildings[bid]->msn = addAnimatedModel(AB_BASE, node->getPosition(), node->getRotation());

    buildings[bid]->entrance = xz_key( btiles[0].X, btiles[0].Z );

    for(irr::u16 i=0, imax=btiles.size(); i<imax; i++)
    {
        xz_key tile(btiles[i].X,btiles[i].Z);
        buildings[bid]->tiles.push_back( tile );
        set2dTile(tile, 0, true, false);
        minimap->setPixel(tile.X,tile.Y, irr::video::SColor(255, 0, 0, 0), true);

        if (btiles[i].Y>1) // непроходимый непрозрачный занятый тайл
            setTileFlag( tile, false, false, false);
        else if (btiles[i].Y>0.5) // непроходимый прозрачный занятый тайл
            setTileFlag( tile, false, true, false);
        else // проходимый, прозрачный, занятый тайл
            setTileFlag( tile, true, true, false);
    }
    minimap->setPixel(buildings[bid]->entrance.X,buildings[bid]->entrance.Y, irr::video::SColor(255, 255, 0, 0), true);

    return true;

} //setBase

bool C_Editor::setPlant(animate_building_t tip, irr::scene::IAnimatedMeshSceneNode* node)
{
    irr::core::vector3df pos;
    irr::core::vector3df tile3d;
    xz_key tile;

    // 1. выбираем тайлы по коорд. костей и проверяем возможность установки здания
    irr::core::array< irr::scene::IBoneSceneNode* > bones;
    bones.push_back( node->getJointNode("Ar_entrance") );
    bones.push_back( node->getJointNode("Ar_t1") );
    bones.push_back( node->getJointNode("Ar_t2") );
    bones.push_back( node->getJointNode("Ar_t3") );
    bones.push_back( node->getJointNode("Ar_t4") );
    bones.push_back( node->getJointNode("Ar_t5") );

    irr::core::array< irr::core::vector3df > btiles;
    for(irr::u16 i=0, imax=bones.size(); i<imax; i++)
    {
        pos = bones[i]->getAbsolutePosition();
        tile3d = irr::core::vector3df( -pos.X, pos.Y, pos.Z );
        if ( tile3d.Z<0 || tile3d.Z>(worldHeight-1) || tile3d.X<0 || tile3d.X>(worldWidth-1) ) return false;
        if (!((tile_t*)tiles[ xz_key(tile3d.X,tile3d.Z) ])->isBuildable) return false;
        btiles.push_back( tile3d );
    }

    irr::u16 bid = buildings.size();
    buildings.push_back( new diobj_t );
    buildings[bid]->ab = tip;
    buildings[bid]->msn = addAnimatedModel(tip, node->getPosition(), node->getRotation());

    buildings[bid]->entrance = xz_key( btiles[0].X, btiles[0].Z );

    for(irr::u16 i=0, imax=btiles.size(); i<imax; i++)
    {
        xz_key tile(btiles[i].X,btiles[i].Z);
        buildings[bid]->tiles.push_back( tile );
        set2dTile(tile, 0, true, false);
        minimap->setPixel(tile.X,tile.Y, irr::video::SColor(255, 0, 0, 0), true);

        if (btiles[i].Y>1) // непроходимый непрозрачный занятый тайл
            setTileFlag( tile, false, false, false);
        else if (btiles[i].Y>0.5) // непроходимый прозрачный занятый тайл
            setTileFlag( tile, false, true, false);
        else // проходимый, прозрачный, занятый тайл
            setTileFlag( tile, true, true, false);
    }
    minimap->setPixel(buildings[bid]->entrance.X,buildings[bid]->entrance.Y, irr::video::SColor(255, 255, 0, 0), true);

    return true;
}//setPlant

irr::scene::IAnimatedMeshSceneNode* C_Editor::addAnimatedModel(animate_building_t tip, irr::core::vector3df pos, irr::core::vector3df rot)
{
    irr::io::path mdl_pth;
    irr::io::path tex_pth;

    if (tip==AB_BASE)
    {
        mdl_pth = "./res/mdl/base.x";
        tex_pth = "./res/tex/base.png";
    }
    else if (tip==AB_CHASSIS)
    {
        mdl_pth = "./res/mdl/plant_chassis.x";
        tex_pth = "./res/tex/plant_chassis.png";
    }
    else if (tip==AB_ELECTRONICS)
    {
        mdl_pth = "./res/mdl/plant_electronics.x";
        tex_pth = "./res/tex/plant_electronics.png";
    }
    else if (tip==AB_NUCLEARS)
    {
        mdl_pth = "./res/mdl/plant_nuclears.x";
        tex_pth = "./res/tex/plant_nuclears.png";
    }
    else if (tip==AB_PHASERS)
    {
        mdl_pth = "./res/mdl/plant_phasers.x";
        tex_pth = "./res/tex/plant_phasers.png";
    }
    else if (tip==AB_CANNON)
    {
        mdl_pth = "./res/mdl/plant_cannon.x";
        tex_pth = "./res/tex/plant_cannon.png";
    }
    else if (tip==AB_MISSILES)
    {
        mdl_pth = "./res/mdl/plant_missiles.x";
        tex_pth = "./res/tex/plant_missiles.png";
    }
    else
        return 0;

    irr::scene::IAnimatedMeshSceneNode* msn = Device->getSceneManager()->addAnimatedMeshSceneNode( Device->getSceneManager()->getMesh(mdl_pth), root_bsn );
    msn->setPosition( pos );
    msn->setRotation( rot );
    msn->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    msn->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
    msn->setMaterialTexture(0, Device->getVideoDriver()->getTexture(tex_pth));

    irr::scene::ITriangleSelector* mts = Device->getSceneManager()->createTriangleSelector(Device->getSceneManager()->getMesh(mdl_pth), msn);
    msn->setTriangleSelector(mts);
    //msn->setDebugDataVisible(irr::scene::EDS_BBOX | irr::scene::EDS_NORMALS);
    mts->drop();

    return msn;
}//addAnimatedModel

bool C_Editor::setDecor(static_building_t tip, irr::scene::IAnimatedMeshSceneNode* node, irr::u16 x, irr::u16 z)
{
    xz_key tile_pos(x,z);
    irr::core::vector3df pos = irr::core::vector3df(-x,0,z);

    if (node != 0)
    {
        irr::scene::IBoneSceneNode* bone = node->getJointNode("Ar_t");
        pos = bone->getAbsolutePosition();
        tile_pos = xz_key( -int(pos.X), int(pos.Z) );
        pos = node->getPosition();
    }

    if (!((tile_t*)tiles[tile_pos])->isBuildable) return false;

    irr::io::path pth;
    irr::io::path tex;
    if (tip==SB_WALL_1)
    {
        pth = "./res/mdl/wall_1.x";
        tex = "./res/tex/wall.png";
    }
    else if (tip==SB_WALL_2)
    {
        pth = "./res/mdl/wall_2.x";
        tex = "./res/tex/wall.png";
    }
    else if (tip==SB_WALL_3)
    {
        pth = "./res/mdl/wall_3.x";
        tex = "./res/tex/wall.png";
    }
    else if (tip==SB_WALL_4)
    {
        pth = "./res/mdl/wall_4.x";
        tex = "./res/tex/wall.png";
    }
    else if (tip==SB_TREE_1)
    {
        pth = "./res/mdl/tree_1.x";
        tex = "./res/tex/tree_1.jpg";
    }

    irr::u8 reg_id = x&&z ? getRegionByPos(pos): regMapClick;

    if ( !regions[reg_id].objs[tip].msn)
    {
        irr::scene::IMeshSceneNode* wall = createEmptyMeshSceneNode();
        regions[reg_id].objs[tip].msn = wall;
        regions[reg_id].objs[tip].mb_shifts[tile_pos] = appendModelBuffer(wall, Device->getSceneManager()->getMesh(pth), pos);
        wall->setMaterialTexture(0, Device->getVideoDriver()->getTexture(tex) );
        wall->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        wall->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
        if (tip==SB_TREE_1)
        {
            wall->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
            wall->setMaterialType( irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL );
        }
    }
    else
    {
        try
        {
            if ( regions[reg_id].objs[tip].mb_deleted.find(tile_pos) )
            {
                xz_key shift = regions[reg_id].objs[tip].mb_deleted[tile_pos];
                insertModelBuffer(regions[reg_id].objs[tip].msn, Device->getSceneManager()->getMesh(pth), shift, pos);
                regions[reg_id].objs[tip].mb_deleted.remove(tile_pos);
            }
            else
                regions[reg_id].objs[tip].mb_shifts[tile_pos] = appendModelBuffer(regions[reg_id].objs[tip].msn, Device->getSceneManager()->getMesh(pth), pos);
        }
        catch(...)
        {
            printf("except: %d %d\n", tile_pos.X, tile_pos.Y);
        }
    }

    xz_key tst = regions[reg_id].objs[tip].mb_shifts[tile_pos];

    minimap->setPixel(tile_pos.X,tile_pos.Y, irr::video::SColor(255, 0, 0, 0), true);
    if (tip == SB_TREE_1)
    {
        setTileFlag( tile_pos, false, true, false); //сквозь деревья стрелять можно
    }
    else if (tip == SB_WALL_1 || tip == SB_WALL_2)
        setTileFlag( tile_pos, false, false, false); //сквозь стены ходить и стрелять нельзя
    else if (tip == SB_WALL_3 || tip == SB_WALL_4)
        setTileFlag( tile_pos, false, true, false); //сквозь маленькие стены ходить нельзя но видеть можно


    return true;
}

void C_Editor::setTileTex(xz_key t_, int texType, int texNum)
{
    //смещение вершины
    irr::u16
        r_wdh = worldWidth/regions_per_x,
        x = t_.X - int(t_.X/r_wdh)*r_wdh,
        r_hgt = worldHeight/regions_per_z,
        z = t_.Y - int(t_.Y/r_hgt)*r_hgt;

    irr::u8 reg_id = getRegionByPos(irr::core::vector3df(t_.X, 0, t_.Y));

    irr::video::S3DVertex* vbuf = (irr::video::S3DVertex*)regions[reg_id].land->getMesh()->getMeshBuffer(0)->getVertices();

    irr::u32 vert_idx = z * r_wdh * 4 + x * 4;

    //определяем новые координаты текстуры для этого полигона
    float texShift = 0.25f * texType;
    vbuf[vert_idx  ].TCoords = irr::core::vector2df(tex_co[texNum][0], tex_co[texNum][1]+texShift); //0,0
    vbuf[vert_idx+1].TCoords = irr::core::vector2df(tex_co[texNum][2], tex_co[texNum][1]+texShift); //1,0
    vbuf[vert_idx+2].TCoords = irr::core::vector2df(tex_co[texNum][2], tex_co[texNum][3]+texShift); //1,1
    vbuf[vert_idx+3].TCoords = irr::core::vector2df(tex_co[texNum][0], tex_co[texNum][3]+texShift); //0,1

    regions[reg_id].land->getMesh()->getMeshBuffer(0)->setDirty(irr::scene::EBT_VERTEX);
}//setTileTex

void C_Editor::setTileFlag( xz_key t_, bool passable, bool transparent, bool buildable)
{
    ((tile_t*)tiles[t_])->isPassable = passable;
    ((tile_t*)tiles[t_])->isTransparent = transparent;
    ((tile_t*)tiles[t_])->isBuildable = buildable;
}

xz_key C_Editor::appendModelBuffer( irr::scene::IMeshSceneNode* node, irr::scene::IMesh* mesh, irr::core::vector3df pos )
{
    xz_key shifts; // смещения в буферах вершин и индексов с которых начинается скопированный меш

    //получаем манипулятор и матрица для транслирования моделей
    irr::scene::IMeshManipulator * mm = Device->getSceneManager()->getMeshManipulator();
    irr::core::matrix4 trans_mat;
    irr::scene::SMeshBuffer *src = 0, *dst = 0; //здесь будем хранить сслыку на буферы новой копии модели

    // 1. определяем меш приемника и делаем временную копию меша источника
    irr::scene::SMesh *dmes = (irr::scene::SMesh *)node->getMesh();
    irr::scene::IMesh *tmes = mm->createMeshCopy( mesh );

    // 2. транслировать копию в координаты тайла
    trans_mat.makeIdentity();
    trans_mat.setTranslation( pos );
    mm->transform( tmes, trans_mat );

    // 3. утрамбовать копию в конец приемника
    for(int mb_i=0, mb_max=tmes->getMeshBufferCount(); mb_i<mb_max; mb_i++)
    {
        src = (irr::scene::SMeshBuffer *)tmes->getMeshBuffer(mb_i);
        dst = (irr::scene::SMeshBuffer *)dmes->getMeshBuffer( dmes->getMeshBufferCount()-1 );
        //если у приемника нет места, то создаем новый буфер
        if ( (max_vert_in_buffer - dst->getVertexCount()) < src->getVertexCount() )
        {
            dst = new irr::scene::SMeshBuffer;
            dmes->addMeshBuffer(dst);
            dst->drop();
        }

        shifts = xz_key( dst->getVertexCount(), dst->getIndexCount() );
        dst->append(src->getVertices(), src->getVertexCount(), src->getIndices(), src->getIndexCount());
        dst->setDirty();
    }

    // 4. удалить временную копию
    delete tmes;

    return shifts;
}//appendModelBuffer

void C_Editor::insertModelBuffer( irr::scene::IMeshSceneNode* node, irr::scene::IMesh* mesh, xz_key shift, irr::core::vector3df pos )
{
    //получаем манипулятор и матрица для транслирования моделей
    irr::scene::IMeshManipulator * mm = Device->getSceneManager()->getMeshManipulator();
    irr::core::matrix4 trans_mat;
    irr::scene::SMeshBuffer *src = 0, *dst = 0; //здесь будем хранить сслыку на буферы новой копии модели

    // 1. определяем меш приемника и делаем временную копию меша источника
    irr::scene::SMesh *dmes = (irr::scene::SMesh *)node->getMesh();
    irr::scene::IMesh *tmes = mm->createMeshCopy( mesh );

    // 2. транслировать копию в координаты тайла
    trans_mat.makeIdentity();
    trans_mat.setTranslation( pos );
    mm->transform( tmes, trans_mat );

    // 3. определяем целевой мешбуфер и копируем вершины и индексы
    xz_key t_(-pos.X, pos.Z);
    src = (irr::scene::SMeshBuffer *)tmes->getMeshBuffer(0);

    irr::u16
        r_wdh = worldWidth/regions_per_x, //ширина региона
        x = t_.X - int(t_.X/r_wdh)*r_wdh, //позиция икс тайла в регионе
        r_hgt = worldHeight/regions_per_z,//высота региона
        z = t_.Y - int(t_.Y/r_hgt)*r_hgt, //позиция зет тайла в регионе
        mb_size = src->getVertexCount(), //размер буфера вершин модели
        mb_id = int((z*r_wdh*mb_size+x*mb_size)/max_vert_in_buffer);

    dst = (irr::scene::SMeshBuffer *)dmes->getMeshBuffer( mb_id );

    for(irr::u16 i=0; i<mb_size; i++)
    {
        dst->Vertices[i+shift.X] = src->Vertices[i];
        dst->Indices[i+shift.X] = i+shift.X;
    }

    dst->setDirty();

    // 4. удалить временную копию
    delete tmes;

}//insertModelBuffer

void C_Editor::deleteModelBuffer(irr::scene::IMeshSceneNode* node, irr::scene::IMesh* mesh, xz_key shift)
{
    irr::scene::SMeshBuffer *src = (irr::scene::SMeshBuffer *)node->getMesh()->getMeshBuffer(0);
    irr::scene::SMeshBuffer *dst = (irr::scene::SMeshBuffer *)mesh->getMeshBuffer(0);
    for(irr::u16 i=shift.Y, max=shift.Y+dst->getIndexCount(); i<max; i++)
        src->Indices[i] = 0;
    src->setDirty();
}//delene from buffer

irr::scene::IMeshSceneNode* C_Editor::createEmptyMeshSceneNode()
{
        //создаем МЕШ контейнер c буфером по умолчанию
        irr::scene::SMesh * dmes = new irr::scene::SMesh;
        dmes->setHardwareMappingHint(irr::scene::EHM_STATIC);
        irr::scene::SMeshBuffer * dst = new irr::scene::SMeshBuffer;
        dmes->addMeshBuffer(dst);
        dst->drop();

        irr::scene::IMeshSceneNode * sn =  Device->getSceneManager()->addMeshSceneNode( dmes, root_sn );
        dmes->drop();

        sn->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        sn->setMaterialFlag(irr::video::EMF_BILINEAR_FILTER, false);
        sn->setAutomaticCulling(irr::scene::EAC_OFF);
        sn->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
        return sn;
}//createEmptyMeshSceneNode

void C_Editor::createLand(irr::u8 worldType)
{
    irr::u16
        r_wdh = worldWidth/regions_per_x,
        r_hgt = worldHeight/regions_per_z;

    irr::u32
        v_cnt = r_wdh*r_hgt*4, //число вершин
        i_cnt = r_wdh*r_hgt*6; //число индексов

    irr::u8 rc = 0; //счетчик регионов

    //цвет для вершин по умолчанию
    irr::video::SColor poly_col(255,255,255,255);


    for(irr::u8 rcz=0; rcz<regions_per_z; rcz++)
    for(irr::u8 rcx=0; rcx<regions_per_x; rcx++)
    {
        //создаем меш под буфера и указатель на текущий генерируемый буффер
        irr::scene::SMesh *dmes = new irr::scene::SMesh;
        irr::scene::SMeshBuffer *buffer = new irr::scene::SMeshBuffer;
        dmes->addMeshBuffer(buffer); buffer->drop();
        irr::u32 v = 0;

        for(irr::u16 z=r_hgt*rcz, zmax=r_hgt*rcz+r_hgt; z<zmax; z++)
        for(irr::u16 x=r_wdh*rcx, xmax=r_wdh*rcx+r_wdh; x<xmax; x++)
        {
            buffer->Vertices.push_back( irr::video::S3DVertex(  -x, 0,   z,   0,1,0, poly_col, tex_co[15][0], tex_co[15][1]) );//00
            buffer->Vertices.push_back( irr::video::S3DVertex(-x-1, 0,   z,   0,1,0, poly_col, tex_co[15][2], tex_co[15][1]) );//10
            buffer->Vertices.push_back( irr::video::S3DVertex(-x-1, 0, z+1,   0,1,0, poly_col, tex_co[15][2], tex_co[15][3]) );//11
            buffer->Vertices.push_back( irr::video::S3DVertex(  -x, 0, z+1,   0,1,0, poly_col, tex_co[15][0], tex_co[15][3]) );//01

            v += 4;

            if (v == v_cnt) // вершины закончились или буффер заполнен, приступаем к индексированию вершин
            {
                irr::u16 p_cnt = 0; //номер первой вершины полигона из buffer->Vertices
                for(irr::u16 i=0; i<i_cnt; i+=6) //за один шаг цикла описываем 2 треугольника из которых состоит полигон
                {
                    buffer->Indices.push_back( p_cnt );
                    buffer->Indices.push_back( p_cnt+1 );
                    buffer->Indices.push_back( p_cnt+2 );
                    buffer->Indices.push_back( p_cnt );
                    buffer->Indices.push_back( p_cnt+2 );
                    buffer->Indices.push_back( p_cnt+3 );
                    p_cnt += 4;
                }
                buffer->recalculateBoundingBox();
            }
        }
        dmes->setHardwareMappingHint(irr::scene::EHM_STATIC, irr::scene::EBT_VERTEX);
        dmes->setHardwareMappingHint(irr::scene::EHM_NEVER, irr::scene::EBT_INDEX);

        regions[rc].land = Device->getSceneManager()->addMeshSceneNode(dmes, root_sn);
        regions[rc].land_selector = Device->getSceneManager()->	createOctreeTriangleSelector(dmes, regions[rc].land);
        dmes->drop();

        regions[rc].land->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        regions[rc].land->setMaterialFlag(irr::video::EMF_BILINEAR_FILTER, false);
        regions[rc].land->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, false);
        regions[rc].land->setAutomaticCulling(irr::scene::EAC_OFF);
        regions[rc].land->setMaterialFlag(irr::video::EMF_FOG_ENABLE, true);
        if (worldType==0)
            regions[rc].land->setMaterialTexture(0, Device->getVideoDriver()->getTexture("./res/tex/ground.png"));
        else
            regions[rc].land->setMaterialTexture(0, Device->getVideoDriver()->getTexture("./res/tex/kosmos.jpg"));

        rc++;
    }

    // заполняем карту тайлами по умолчанию
    for(irr::u16 z=0; z<96; z++)
        for(irr::u16 x=0; x<256; x++)
        {
            tiles.insert( xz_key(x,z), new tile_t(15) );
        }
}//createLand

void C_Editor::clearScene()
{
    clearLand();

    if (minimap) minimap->remove();;
    if (Skydome) Skydome->remove();
    if (root_sn) root_sn->remove();
    if (root_bsn) root_bsn->remove();

    minimap = NULL;
    Skydome = NULL;
    root_sn = NULL;
    root_bsn = NULL;

    for(irr::u16 i, imax=buildings.size(); i<imax; i++) delete buildings[i];
    buildings.clear();

    for(irr::u16 r=0; r<9; r++)
    for(irr::u16 o=0; o<SB_COUNT; o++)
    {
        regions[r].objs[o].msn = NULL;
        regions[r].objs[o].mb_deleted.clear();
        regions[r].objs[o].mb_shifts.clear();
    }
    camera.camUnSet();
}

void C_Editor::clearLand()
{
    for (tiles_t::Iterator it = tiles.getIterator(); !it.atEnd(); it++) delete it->getValue();
    tiles.clear();

    for(irr::u8 rc=0; rc<regions_count; rc++)
    {
        if (regions[rc].land)
        {
            regions[rc].land->remove();
            regions[rc].land_selector->drop();
        }
        regions[rc].land = NULL;
    }
}

bool C_Editor::testMapClick(irr::s32 x, irr::s32 z)
{
    irr::core::vector3df intersection;
    irr::core::triangle3df tri;
    const irr::scene::ISceneNode * nod;
    irr::scene::ISceneCollisionManager * cm = Device->getSceneManager()->getSceneCollisionManager();

    irr::core::line3d<irr::f32> ray = cm->getRayFromScreenCoordinates(irr::core::position2di(x, z), camera.csn);

    irr::core::vector3df cam_tar = camera.csn->getTarget();

    /*
        можно не перебирать все регионы а проверить координату луча где он пересекается с плоскостью XZ, те найти
        координаты икс зет в игрек равным нулю
    */
    for (irr::u8 rc=0; rc<regions_count; rc++)
        if ( cm->getCollisionPoint(ray, regions[rc].land_selector, intersection, tri, nod ))
        {
            regMapClick = rc;
            triObjClick = tri;
            posObjClick = const_cast<irr::scene::ISceneNode*>(nod);
            posMapClick = irr::core::vector3df(int(intersection.X), 0, int(intersection.Z));
            posMapClick_f = intersection;
            return true;
        }
    return false;
}//testMapClick

bool C_Editor::testObjClick(irr::s32 x, irr::s32 z)
{

    irr::core::vector3df intersection;
    irr::core::triangle3df tri;
    const irr::scene::ISceneNode * nod;
    irr::scene::ISceneCollisionManager * cm = Device->getSceneManager()->getSceneCollisionManager();

    irr::core::line3d<irr::f32> ray = cm->getRayFromScreenCoordinates(irr::core::position2di(x, z), camera.csn);

    //dyn
    if ( cm->getCollisionPoint(ray, buildings_selector, intersection, tri, nod ))
    {
        triObjClick = tri;
        posObjClick = const_cast<irr::scene::ISceneNode*>(nod);
        posMapClick_f = posMapClick = intersection;
        return true;
    }

    // static obj
    if ( cm->getCollisionPoint(ray, regions[regMapClick].buildings_selector, intersection, tri, nod ))
    {
        triObjClick = tri;
        posObjClick = const_cast<irr::scene::ISceneNode*>(nod);
        posMapClick_f = posMapClick = intersection;
        return true;
    }

    return false;
}//testObjClick

bool C_Editor::testCollide( irr::core::vector3df pos )
{
    irr::u8 reg_id = getRegionByPos(pos);

    irr::scene::ISceneCollisionManager * cm = Device->getSceneManager()->getSceneCollisionManager();
    irr::core::vector3df intersection;
    irr::core::triangle3df tri;
    const irr::scene::ISceneNode * nod;    irr::core::line3df ray;
    ray.start = camera.csn->getPosition();
    ray.end = ray.start + (pos - ray.start).normalize() * 1000.0f;

    // static obj
    if ( cm->getCollisionPoint(ray, regions[reg_id].buildings_selector, intersection, tri, nod ))
    {
        regMapClick = reg_id;
        triObjClick = tri;
        posObjClick = const_cast<irr::scene::ISceneNode*>(nod);
        posMapClick_f = posMapClick = intersection;
        return true;
    }

    // dynamic obj
    if ( cm->getCollisionPoint(ray, buildings_selector, intersection, tri, nod ))
    {
        regMapClick = reg_id;
        triObjClick = tri;
        posObjClick = const_cast<irr::scene::ISceneNode*>(nod);
        posMapClick_f = posMapClick = intersection;
        return true;
    }

    return false;
}

irr::u8 C_Editor::getRegionByPos( irr::core::vector3df pos )
{
    irr::u16
        r_wdh = worldWidth/regions_per_x,
        r_hgt = worldHeight/regions_per_z,
        reg_id = regions_per_x*int(pos.Z/r_hgt) + int((pos.X<0?-pos.X:pos.X)/r_wdh);
    return reg_id;
}

void C_Editor::makeScreenShot()
{
    irr::io::path scr_path(map_file);
    irr::core::cutFilenameExtension(scr_path, map_file);
    scr_path.append(".jpg");
    Device->getVideoDriver()->writeImageToFile(Device->getVideoDriver()->createScreenShot(), scr_path);
}

bool C_Editor::saveMap()
{
    //irr::io::IWriteFile *fil = Device->getFileSystem()->createAndWriteFile(map_file);
    irr::io::IXMLWriter* xml = Device->getFileSystem()->createXMLWriter(map_file);
    if (xml)
    {
        irr::core::array< irr::core::stringw > attrs, vals;

        xml->writeXMLHeader();
        xml->writeElement(L"map"); xml->writeLineBreak();

        xml->writeElement(L"config"); xml->writeLineBreak();
            xml->writeElement(L"param1", true,
                L"worldSize", irr::core::stringw(worldSize).c_str(),
                L"worldType", irr::core::stringw(worldType).c_str()
            ); xml->writeLineBreak();
        xml->writeClosingTag(L"config"); xml->writeLineBreak();

        xml->writeElement(L"tilemap"); xml->writeLineBreak();
            attrs.clear();
            attrs.push_back(L"x");
            attrs.push_back(L"z");
            attrs.push_back(L"tex");
            attrs.push_back(L"p");
            attrs.push_back(L"t");
            attrs.push_back(L"b");
            for(irr::u16 z=0, zmax=worldHeight; z<zmax; z++)
            for(irr::u16 x=0, xmax=worldWidth;  x<xmax; x++)
            {
                vals.clear();
                vals.push_back( irr::core::stringw(x) );
                vals.push_back( irr::core::stringw(z) );
                xz_key tkey(x,z);
                vals.push_back( irr::core::stringw( ((tile_t*)tiles[tkey])->tex_id) );
                vals.push_back( irr::core::stringw( ((tile_t*)tiles[tkey])->isPassable) );
                vals.push_back( irr::core::stringw( ((tile_t*)tiles[tkey])->isTransparent) );
                vals.push_back( irr::core::stringw( ((tile_t*)tiles[tkey])->isBuildable) );
                xml->writeElement(L"t", true, attrs, vals); xml->writeLineBreak();
            }
        xml->writeClosingTag(L"tilemap"); xml->writeLineBreak();

        xml->writeElement(L"static_objs"); xml->writeLineBreak();
            attrs.clear();
            attrs.push_back(L"x");
            attrs.push_back(L"z");
            attrs.push_back(L"t"); //tip
            for(irr::u16 r=0; r<9; r++)
            if (regions[r].land)
            {
                for(irr::u16 o=0, omax=SB_COUNT; o<omax; o++)
                {
                    irr::core::map< xz_key, xz_key >::Iterator myIt = regions[r].objs[o].mb_shifts.getIterator();
                    while ( !myIt.atEnd() )
                    if ( !regions[r].objs[o].mb_deleted.find(myIt->getKey()) )
                    {
                        xz_key koo = myIt->getKey();
                        vals.clear();
                        vals.push_back( irr::core::stringw(koo.X) );
                        vals.push_back( irr::core::stringw(koo.Y) );
                        vals.push_back( irr::core::stringw(o) );
                        xml->writeElement(L"o", true, attrs, vals); xml->writeLineBreak();
                        myIt++;
                    }

                }
            }
        xml->writeClosingTag(L"static_objs"); xml->writeLineBreak();

        xml->writeElement(L"dynamic_objs"); xml->writeLineBreak();
            attrs.clear();
            attrs.push_back(L"x");
            attrs.push_back(L"z");
            attrs.push_back(L"r"); //rotation
            attrs.push_back(L"t"); //tip
            attrs.push_back(L"ex"); //entrance x
            attrs.push_back(L"ez"); //entrance z
            for(irr::u16 b=0, bmax=buildings.size(); b<bmax; b++)
            {
                vals.clear();
                vals.push_back( irr::core::stringw(buildings[b]->msn->getPosition().X) );
                vals.push_back( irr::core::stringw(buildings[b]->msn->getPosition().Z) );
                vals.push_back( irr::core::stringw(buildings[b]->msn->getRotation().Y) );
                vals.push_back( irr::core::stringw(buildings[b]->ab) );
                vals.push_back( irr::core::stringw(buildings[b]->entrance.X) );
                vals.push_back( irr::core::stringw(buildings[b]->entrance.Y) );
                xml->writeElement(L"obj", false, attrs, vals); xml->writeLineBreak();
                for(irr::u16 t=0, tmax=buildings[b]->tiles.size(); t<tmax; t++)
                {
                    xml->writeElement(L"t", true,
                        L"x", irr::core::stringw(buildings[b]->tiles[t].X).c_str(),
                        L"z", irr::core::stringw(buildings[b]->tiles[t].Y).c_str()
                    );
                }
                xml->writeClosingTag(L"obj"); xml->writeLineBreak();
            }
        xml->writeClosingTag(L"dynamic_objs"); xml->writeLineBreak();

        xml->writeClosingTag(L"map");

        xml->drop();
        return true;
    }
    return false;
}//saveMap

bool C_Editor::loadMap(const irr::c8 *fname, bool genCollisionData)
{
    #if _DEBUG
    printf("========= load map %s ==========\n", fname);
    #endif

    map_file = "./res/maps/";
    map_file.append(fname);

    clearScene();

    irr::io::IXMLReader* xml = Device->getFileSystem()->createXMLReader(map_file);
    if (xml)
    {
        while (xml->read())
        {
            switch (xml->getNodeType())
            {
                case irr::io::EXN_ELEMENT:
                    if ( irr::core::stringc("config") == xml->getNodeName() )
                    {
                        xml->read();
                        xml->read();
                        if ( irr::io::EXN_ELEMENT == xml->getNodeType() )
                        {
                            worldSize = xml->getAttributeValueAsInt(L"worldSize");
                            worldType = xml->getAttributeValueAsInt(L"worldType");
                            createMap(fname, worldSize, worldType);
                            #if _DEBUG
                            printf("mapSize: %d, mapType: %d\n", worldSize, worldType);
                            #endif
                        }
                    }
                    else if ( irr::core::stringc("tilemap") == xml->getNodeName() )
                    {
                        while(xml->read() && xml->getNodeType() != irr::io::EXN_ELEMENT_END )
                        if ( irr::io::EXN_ELEMENT == xml->getNodeType() )
                        {
                            irr::u16 x = xml->getAttributeValueAsInt(L"x");
                            irr::u16 z = xml->getAttributeValueAsInt(L"z");
                            xz_key tkey(x,z);
                            tile_t* tile = ((tile_t*)tiles[tkey]);
                            tile->tex_id = xml->getAttributeValueAsInt(L"tex");
                            tile->isPassable = xml->getAttributeValueAsInt(L"p")==1 ? true : false;
                            tile->isTransparent = xml->getAttributeValueAsInt(L"t")==1 ? true : false;
                            //tiles[z][x].isBuildable = xml->getAttributeValueAsInt(L"b")==1 ? true : false;
                            setTileTex( xz_key(x,z), ((tile_t*)tiles[tkey])->tex_id/16, ((tile_t*)tiles[tkey])->tex_id%16);
                            #if _DEBUG
                            printf("tile - x:%d y:%d tex:%d p:%d t:%d b:%d\n", x,z,tile->tex_id, tile->isPassable, tile->isTransparent, tile->isBuildable );
                            #endif
                        }
                    }
                    else if ( irr::core::stringc("static_objs") == xml->getNodeName() )
                    {
                        while(xml->read() && xml->getNodeType() != irr::io::EXN_ELEMENT_END )
                        if ( irr::io::EXN_ELEMENT == xml->getNodeType() )
                        {
                            irr::u16 x = xml->getAttributeValueAsInt(L"x");
                            irr::u16 z = xml->getAttributeValueAsInt(L"z");
                            irr::u16 t = xml->getAttributeValueAsInt(L"t");

                            setDecor( (static_building_t)t, 0, x,z);
                            #if _DEBUG
                            printf("static - x:%d y:%d tip:%d\n", x,z,t );
                            #endif
                        }
                    }
                    else if ( irr::core::stringc("dynamic_objs") == xml->getNodeName() )
                    {
                        while(xml->read() && xml->getNodeType() != irr::io::EXN_ELEMENT_END )
                        if ( irr::io::EXN_ELEMENT == xml->getNodeType() )
                        {
                            irr::s16 x = xml->getAttributeValueAsInt(L"x");
                            irr::s16 z = xml->getAttributeValueAsInt(L"z");
                            irr::u16 r = xml->getAttributeValueAsInt(L"r");
                            irr::u16 t = xml->getAttributeValueAsInt(L"t");
                            irr::u16 ex = xml->getAttributeValueAsInt(L"ex");
                            irr::u16 ez = xml->getAttributeValueAsInt(L"ez");

                            irr::u16 bid = buildings.size();
                            buildings.push_back( new diobj_t );
                            buildings[bid]->ab = (animate_building_t)t;
                            buildings[bid]->msn = addAnimatedModel( (animate_building_t)t, irr::core::vector3df(x,0,z), irr::core::vector3df(0,r,0) );
                            buildings[bid]->entrance = xz_key(ex,ez);

                            while(xml->read() && xml->getNodeType() != irr::io::EXN_ELEMENT_END )
                            if ( irr::io::EXN_ELEMENT == xml->getNodeType() )
                            {
                                xz_key tile ( xml->getAttributeValueAsInt(L"x"), xml->getAttributeValueAsInt(L"z") );
                                buildings[bid]->tiles.push_back( tile );
                                ((tile_t*)tiles[tile])->isBuildable = false;
                            }
                            #if _DEBUG
                            printf("dynamic - x:%d y:%d tip:%d\n", x,z,t );
                            #endif
                        }
                    }

                break;
            }
        }

        xml->drop();

        if ( genCollisionData ) genBuildingsMetaTriSelectors();

        return true;
    }
    return false;
}

bool C_Editor::deleteMap(const irr::c8 *fname)
{
    int (*p_remove)(const char*) = &remove;
    if (p_remove != 0)
    {
        irr::io::path pth("./res/maps/");
        pth.append(fname);

        irr::io::path pic;
        irr::core::cutFilenameExtension(pic,pth);
        pic.append(".jpg");
        remove(pic.c_str());


        return remove(pth.c_str())==0 ? true : false;
    }

    return false;
}
