#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include <irrlicht.h>

#include "../ParticleEngine/Irrlicht/IParticleSystem.h"
#include "../Utils/decalscenenode.h"
#include "../Utils/wavefindway.h"

class C_Hero
{
public:
    C_Hero(irr::IrrlichtDevice* device);
    ~C_Hero();

    void init(C_WaveFindWay* gm);
    void updateHeroAnim(irr::f32 timediff);
    irr::scene::ISceneNode* getModelRoot() { return sn_chassis_1; }

    void setPath( std::vector<Point> p );
    irr::core::vector3df getPosition() { return sn_chassis_1->getAbsolutePosition(); }
    irr::core::vector3df getRocket() { return sn_rocket->getAbsolutePosition(); }

void testpath()
{
    if (path.size()==0) return;
    irr::video::SMaterial material;
    material.setTexture(0, 0);
    material.Lighting = false;
    material.NormalizeNormals = true;
    material.setFlag(irr::video::EMF_BACK_FACE_CULLING, true);

    for (int i=0, imax=path.size(); i<imax; i++)
    {
        int x=path[i].X, y=path[i].Y;
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

    C_WaveFindWay* gamemap;

    std::vector<Point> path;
    irr::s32 idxPathNode;
    irr::s32 idxLockNode;

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
    void fxMove(bool doit);
};

//extern C_Hero* hero;

#endif // HERO_H_INCLUDED
