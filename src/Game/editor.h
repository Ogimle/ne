#ifndef C_EDITOR_H_INCLUDED
#define C_EDITOR_H_INCLUDED

#include <irrlicht.h>

#include "../Utils/minimap.h"

#include "conf_game.h"
#include "camera.h"

typedef irr::core::vector2di xz_key;

struct tile_t
{
    tile_t(irr::u16 tex=15, bool p=true, bool t=true, bool b=true) { tex_id = tex; isPassable=p; isTransparent=t; isBuildable=b;}
    bool isPassable, isTransparent, isBuildable;
    irr::u16 tex_id;
};

typedef irr::core::map< xz_key, tile_t* > tiles_t;

struct diobj_t
{
    animate_building_t ab;
    irr::scene::IAnimatedMeshSceneNode* msn;
    irr::core::array< xz_key > tiles;
    xz_key entrance;
};

struct t_stobj
{
    irr::scene::IMeshSceneNode* msn;
    irr::core::map< xz_key, xz_key > mb_shifts;
    irr::core::map< xz_key, xz_key > mb_deleted;
};

struct t_region
{
    irr::scene::IMeshSceneNode* land;
    irr::scene::ITriangleSelector* land_selector;
    irr::scene::IMetaTriangleSelector* buildings_selector;
    t_stobj objs[SB_COUNT];
};



class C_Editor
{
    public:
        C_Camera camera;
        irr::core::vector3df posMapClick;
        irr::core::vector3df posMapClick_f;
        irr::core::triangle3df triObjClick;
        irr::scene::ISceneNode* posObjClick;

        irr::u8 regMapClick;

        C_Editor(irr::IrrlichtDevice* device);
        ~C_Editor();

        bool createMap(const irr::c8 *fname, irr::u8 size, irr::u8 worlType);
        bool loadMap(const irr::c8 *fname, bool genCollisionData=false);
        void fillCostMap(irr::u16 mc[256][256]);
        bool saveMap();
        void makeScreenShot();
        bool deleteMap(const irr::c8 *fname);
        void clearScene();

        void fillMiniMap();

        bool testMapClick(irr::s32 x, irr::s32 z);
        bool testObjClick(irr::s32 x, irr::s32 z);
        bool testCollide( irr::core::vector3df pos );

        void set2dTile(xz_key t_, int b_, bool isEarth,  bool buildCheck = true);
        bool deleteDynObj(irr::s32 x, irr::s32 y);
        void deleteStatObj();

        bool setBase(irr::scene::IAnimatedMeshSceneNode* node);
        bool setPlant(animate_building_t tip, irr::scene::IAnimatedMeshSceneNode* node);
        bool setDecor(static_building_t tip, irr::scene::IAnimatedMeshSceneNode* node=0, irr::u16 x=-1, irr::u16 z=-1);

        void hideMiniMap() { minimap->setVisible(false); }

        irr::u16 getWordlWidth() { return worldWidth; }
        irr::u16 getWordlHeigh() { return worldHeight; }

        const tile_t* getTile(irr::u32 x, irr::u32 y) { return (tile_t*)tiles[ xz_key(x,y) ]; }

        void getSpawnPoints( irr::core::array< irr::core::vector2di >* pnt)
        {
            pnt->clear();
            for(irr::u16 i=0, imax=buildings.size(); i<imax; i++)
                pnt->push_back( buildings[i]->entrance );
        }

    protected:

    private:
        irr::core::stringc map_file;

        irr::IrrlichtDevice* Device;
        irr::scene::ISceneNode* root_sn; //корневая нода для ландшафта и статики
        irr::scene::ISceneNode* root_bsn; //корневая нода для зданий

        irr::u8 worldSize; // размер карты
        irr::u8 worldType; // тип тайловой поверхности
        irr::u16 worldWidth; // ширина карты
        irr::u16 worldHeight; // высота карты
        irr::u8 regions_count; // число регионов карты
        irr::u8 regions_per_x; // число регионов в "строке"
        irr::u8 regions_per_z; // число регионов в "столбце"

        irr::scene::ISceneNode* Skydome;

        C_MiniMap* minimap;

        // контейнеры для игрового поля
        tiles_t tiles; // тайловая карта
        t_region regions[9]; // хранилище статических  объектов
        irr::core::array< diobj_t* > buildings; // хранилище динамических объектов
        irr::scene::IMetaTriangleSelector* buildings_selector;


        xz_key appendModelBuffer( irr::scene::IMeshSceneNode* node, irr::scene::IMesh* mesh, irr::core::vector3df pos = irr::core::vector3df(0,0,0) );
        void insertModelBuffer( irr::scene::IMeshSceneNode* node, irr::scene::IMesh* mesh, xz_key shift, irr::core::vector3df pos);
        void deleteModelBuffer( irr::scene::IMeshSceneNode* node, irr::scene::IMesh* mesh, xz_key shift );

        irr::scene::IMeshSceneNode* createEmptyMeshSceneNode();
        void createLand(irr::u8 worldType=1);
        void clearLand();

        irr::scene::IAnimatedMeshSceneNode* addAnimatedModel(animate_building_t tip, irr::core::vector3df pos, irr::core::vector3df rot);
        bool fillTileTmpTable( xz_key t_,  bool buildCheck = true);
        void prepareTileTmpTable(int b, bool isEarth);
        void placeTileTmpTable( xz_key t_);
        void setTileTex( xz_key t_, int texType, int texNum);
        void setTileFlag(xz_key t_, bool passable, bool transparent, bool buildable);
        void genBuildingsMetaTriSelectors();
        irr::u8 getRegionByPos( irr::core::vector3df pos );
};

extern C_Editor* Editor;

#endif // C_EDITOR_H_INCLUDED
