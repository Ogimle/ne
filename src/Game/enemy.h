#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <irrlicht.h>
#include <vector>
#include "gamemap.h"

struct S_Enemy
{
    std::vector<void*> path;
    irr::u32 idxPathNode;
    irr::s16 hitPoints;
    irr::f32 speed;
    irr::u16 melee_damage;
    irr::f32 rate;
    irr::scene::ISceneNode* node;

    bool isMove;
    bool isStartMove;

    bool alive;
    irr::f32 decay_time;
};

class C_Enemy
{
public:
    C_Enemy(irr::IrrlichtDevice* device);
    ~C_Enemy();

    void init(C_GameMap* gm);

    void setTarget(irr::scene::ISceneNode* node) { target = node; }

    irr::s16 testCollide( irr::core::vector3df pos, irr::f32 tolerance = 0.4);
    bool testCollideEx( irr::core::vector3df pos, irr::core::array<irr::u16>* ret, irr::f32 tolerance = 0.4);
    void hit(irr::u16 id, irr::f32 dmg);

    void update(irr::f32 timediff, irr::core::vector3df hero_pos);

private:
    irr::IrrlichtDevice* Device;
    irr::core::array<S_Enemy*> enemies;
    irr::scene::ISceneNode* target;

    C_GameMap* gamemap;
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
