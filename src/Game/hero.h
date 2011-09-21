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

    bool OnEvent(const irr::SEvent& event);

    void init(C_GameMap* gm);
    void updateHeroAnim(irr::f32 timediff);

private:
    irr::IrrlichtDevice* Device;

    C_GameMap* gamemap;

    struct SCamKeyMap
    {
        SCamKeyMap() {};
        SCamKeyMap(irr::s32 a, irr::EKEY_CODE k) : action(a), keycode(k) {}

        irr::s32 action;
        irr::EKEY_CODE keycode;
    };
    irr::core::array<SCamKeyMap> KeyMap;
    bool CursorKeys[6];
    bool MouseKeys[3];

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

    float heroTowerTheta;
    float heroChassisTheta;

    irr::f32 MoveSpeed;
    irr::f32 RotateSpeed;

    bool firstHeroUpdate;
    irr::s32 lastHeroAnimationTime;

    bool isMove, isAttack;

    void setupHeroModel();
};

extern C_Hero* hero;

#endif // HERO_H_INCLUDED
