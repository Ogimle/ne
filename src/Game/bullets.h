#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED

#include "../Utils/fx_creator.h"
#include "../Utils/ISceneNodeAnimatorFinishing.h"
#include "editor.h"

class CRocketAnimator : public irr::scene::ISceneNodeAnimatorFinishing
{
public:

    CRocketAnimator(
        irr::scene::ISceneManager* manager,
        irr::core::vector3df& dir, //направление выстрела
        irr::f32 speed, //скорость
        irr::u32 timeout //дата самоуничножения CurrentTime+TimeOut, если не было столкновений
    ) : ISceneNodeAnimatorFinishing(timeout)
    {
        SceneManager = manager;
        Dir = dir;
        Dir.normalize();
        Speed = speed;
        TimeOut = timeout;
    }

    virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs)
    {
        if (!node) return;
        if (HasFinished) return;

        irr::u32 t = (TimeOut-timeMs);

        irr::core::vector3df pos = node->getPosition();

        if (t < 0)
        {
            HasFinished = true;
            SceneManager->addToDeletionQueue(node);
            return;
        }

        if ( Editor->testCollide(pos) )
        {
            irr::core::vector3df n = Editor->triObjClick.getNormal().normalize();
            irr::core::plane3df p(pos, n);
            FX_Creator::addExplosion(pos+(n*0.3f), n.getHorizontalAngle()/*+irr::core::vector3df(90,0,0)*/, 0.3f, SceneManager, p);
            HasFinished = true;
            SceneManager->addToDeletionQueue(node);
            return;
        }

        irr::core::vector3df rel = Dir * Speed;
        node->setPosition( node->getPosition()+rel );
    }

    virtual irr::scene::ESCENE_NODE_ANIMATOR_TYPE getType() const { return irr::scene::ESNAT_UNKNOWN; }

    ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager)
    {
        CRocketAnimator * newAnimator = new CRocketAnimator(SceneManager, Dir, Speed, TimeOut);
        return newAnimator;
    }

private:
    irr::scene::ISceneManager* SceneManager;

    irr::core::vector3df Dir;
    irr::f32 Speed;
    irr::u32 TimeOut;

};

void addRocket(irr::scene::ISceneManager* smgr, irr::core::vector3df start, irr::core::vector3df dir, irr::f32 speed, irr::u32 timeout)
{
    irr::scene::IParticleSystem* commet = FX_Creator::addCommet(start, irr::core::vector3df(), 0.2, smgr);

    irr::scene::ISceneNodeAnimator* anim =  new CRocketAnimator(smgr, dir, speed, timeout);
    commet->addAnimator(anim);
    anim->drop();
}


#endif // BULLETS_H_INCLUDED
