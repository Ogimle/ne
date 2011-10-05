#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <irrlicht.h>
#include <vector>
#include "../Utils/wavefindway.h"

struct S_Enemy
{
    std::vector<waypoint_t> path;
    irr::s32 idxPathNode;
    irr::s32 idxLockNode;
    irr::s16 hitPoints;
    irr::f32 speed;
    irr::u16 melee_damage;
    irr::f32 rate;
    irr::scene::ISceneNode* node;

    bool isMove;
    bool isStartMove;
    irr::f32 stay_time;

    bool alive;
    irr::f32 decay_time;
};

class C_Enemy
{
public:
    C_Enemy(irr::IrrlichtDevice* device);
    ~C_Enemy();

    void init(C_WaveFindWay* gm);

    void setTarget(irr::scene::ISceneNode* node) { target = node; }

    irr::s16 testCollide( irr::core::vector3df pos, irr::f32 tolerance = 0.4);
    bool testCollideEx( irr::core::vector3df pos, irr::core::array<irr::u16>* ret, irr::f32 tolerance = 0.4);
    void hit(irr::u16 id, irr::f32 dmg);

    void update(irr::f32 timediff, irr::core::vector3df hero_pos);

void testpath()
{
    if (enemies[0]->path.size()==0) return;
    irr::video::SMaterial material;
    material.setTexture(0, 0);
    material.Lighting = false;
    material.NormalizeNormals = true;
    material.setFlag(irr::video::EMF_BACK_FACE_CULLING, true);

    for (int i=0, imax=enemies[0]->path.size(); i<imax; i++)
    {
        int x=enemies[0]->path[i].X, y=enemies[0]->path[i].Y;
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
        Device->getVideoDriver()->draw3DTriangle(t, irr::video::SColor(0,255,0,0));
        Device->getVideoDriver()->draw3DTriangle(t2, irr::video::SColor(0,255,0,0));
    }
    //gamemap->testlocked(Device);
}


private:
    irr::IrrlichtDevice* Device;
    irr::core::array<S_Enemy*> enemies;
    irr::scene::ISceneNode* target;
    irr::f32 spawn_time;
    irr::f32 spawn_timeout;

    C_WaveFindWay* gamemap;
    irr::core::array< irr::core::vector2di > spawnPoints;
    irr::u32 getNextSpawnPoint();
    irr::u32 currentSP;

    irr::s32 rand_seed;
    void rand_reset(irr::s32 d=0) { rand_seed = Device->getTimer()->getTime()+d; }
    irr::s32 rand_gen(irr::s32 range) { srand(Device->getTimer()->getTime()); return rand() % (range+1); }

    void addEnemy(irr::f32 rate=1);
    void delEnemy(irr::u32 id);
    void updEnemy(irr::u32 id, irr::f32 timeDiff, irr::core::vector3df hero_pos);
    irr::scene::ISceneNode* setupEnemyModel(bool isCannon=false, bool isPhazer=false, bool isRocket=false, bool isABomb=false);
};


#endif // ENEMY_H_INCLUDED
