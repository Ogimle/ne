#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include <irrlicht.h>
#include "gamemap.h"

#include "../ParticleEngine/Irrlicht/IParticleSystem.h"
#include "../Utils/decalscenenode.h"

class C_Hero
{
public:
    C_Hero(irr::IrrlichtDevice* device);
    ~C_Hero();

    void init(C_GameMap* gm);
    irr::core::vector3df getPosition() { return sn_chassis_1->getPosition(); }
    void updateHeroAnim(irr::f32 timediff);
    irr::scene::ISceneNode* getModelRoot() { return sn_chassis_1; }

    void setPath( std::vector<void*> p, irr::u32 idx=1 ) { path = p; idxPathNode=idx; isStartMove=true; }
    irr::core::vector3df getRocket() { return sn_rocket->getAbsolutePosition(); }


private:
    irr::IrrlichtDevice* Device;

    C_GameMap* gamemap;

    std::vector<void*> path;
    irr::u32 idxPathNode;

    irr::scene::IAnimatedMeshSceneNode* sn_hero;
    irr::scene::IAnimatedMeshSceneNode* sn_chassis_1;
    irr::scene::IAnimatedMeshSceneNode* sn_chassis_2;
    irr::scene::IMeshSceneNode* sn_tower;
    irr::scene::IMeshSceneNode* sn_rocket;
    irr::scene::IMeshSceneNode* sn_cannon;
    irr::scene::IMeshSceneNode* sn_fazer;
    irr::scene::IMeshSceneNode* sn_abomb;

    irr::scene::IParticleSystem* ps_gaz;
    DecalSceneNode* ps_traks;

    irr::f32 MoveSpeed;
    irr::f32 RotateSpeed;

    bool firstHeroUpdate;
    irr::s32 lastHeroAnimationTime;

    bool isMove, isStartMove, isAttack;

    void setupHeroModel();
};

//extern C_Hero* hero;

#endif // HERO_H_INCLUDED
