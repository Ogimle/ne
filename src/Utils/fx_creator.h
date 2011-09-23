#ifndef FX_CREATOR_H_INCLUDED
#define FX_CREATOR_H_INCLUDED

#include "../ParticleEngine/Irrlicht/IParticleSystem.h"
#include "../ParticleEngine/Irrlicht/Particle.h"

class ColorAffector : public irr::scene::particle::IParticleAffector
{
public:
    ColorAffector(const irr::video::SColor& targetColor)
    {
        Target = targetColor;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        irr::f32 lifeTime = particle->LifeTime;//particle->DestroyTimeMs-particle->CreateTimeMs;
        irr::f32 livedTime = particle->TimeLived;//timeMs-particle->CreateTimeMs;

        particle->Color = Target.getInterpolated(particle->StartColor, livedTime/lifeTime);
    }
    irr::video::SColor Target;

};


class ColorAffectorQ : public irr::scene::particle::IParticleAffector
{
public:
    ColorAffectorQ(const irr::video::SColor& targetColor0, const irr::video::SColor& targetColor1)
    {
        Target0 = targetColor0;
        Target1 = targetColor1;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        irr::f32 lifeTime = particle->LifeTime;//particle->DestroyTimeMs-particle->CreateTimeMs;
        irr::f32 livedTime = particle->TimeLived;//timeMs-particle->CreateTimeMs;


        particle->Color = particle->StartColor.getInterpolated_quadratic(Target0, Target1, livedTime/lifeTime);
    }
    irr::video::SColor Target0;
    irr::video::SColor Target1;

};

class GravityAffector : public irr::scene::particle::IParticleAffector
{
public:
    GravityAffector(const irr::core::vector3df& gravity)
    {
        Gravity = gravity;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        particle->Speed += Gravity*diff;
    }
    irr::core::vector3df Gravity;

};

class PlaneCollisionAffector : public irr::scene::particle::IParticleAffector
{
public:
    PlaneCollisionAffector(const irr::core::plane3df& plane)
    {
        Plane = plane;
    }
    void affect(irr::scene::particle::Particle* particle, irr::u32 timeMs, irr::f32 diff)
    {
        irr::core::vector3df collision;
        if (Plane.getIntersectionWithLimitedLine(particle->Position, particle->Position+particle->Speed*diff, collision))
        {
            irr::core::vector3df r = particle->Speed-(2*particle->Speed.dotProduct(Plane.Normal))*Plane.Normal;
            particle->Speed = r*0.5f;
        }
    }
    irr::core::plane3df Plane;
};

class FX_Creator
{
    public:
        static irr::scene::IParticleSystem* exhaust(irr::scene::ISceneNode* sn, irr::scene::ISceneManager* smgr)
        {
            irr::scene::IParticleSystem* ps = irr::scene::createParticleSystem(sn, smgr, 6);
            ps->setScale(irr::core::vector3df(1,1,1));
            ps->updateAbsolutePosition();

            irr::scene::particle::IParticleDrawer* drawer = ps->addTrailParticleDrawer();
            irr::scene::particle::SParticleUV uv;
            irr::scene::particle::IParticleEmitter* emitter;
            //irr::scene::particle::IParticleAffector* affector;

            ///Smoke Trails
            drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.0f), irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f)));
            drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f), irr::core::vector2df(0.25f,0.5f), irr::core::vector2df(0.5f,0.5f)));
            drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.5f,0.0f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f)));
            drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.25f,0.5f)));

            emitter = drawer->addStandardEmitter(
                irr::core::vector3df(0,0,0),        //rotation
                irr::core::vector3df(0,0,0),        //rotationspeed
                irr::core::vector3df(0,10,0),       //speed
                irr::core::vector3di(0, 0, 10),  //maxAngle
                40,                                  //minPpS
                50,                                //maxPpS
                0,                                 //maxEmitt
                250,                                //minlifetime
                400,                               //maxlifetime
                irr::core::vector2df(0.25,1.0),     //minsize
                irr::core::vector2df(0.5,2.0),     //maxsize
                irr::core::vector2df(2.f),          //sizespeed
                irr::video::SColor(200,128,128,128), //minColor
                irr::video::SColor(200,200,200,200)  //maxColor
                );

            irr::video::SMaterial overrideMaterial;
            overrideMaterial.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
            overrideMaterial.setTexture(0, smgr->getVideoDriver()->getTexture("./res/tex/fig7_alpha.png"));
            overrideMaterial.setFlag(irr::video::EMF_LIGHTING, false);
            overrideMaterial.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
            overrideMaterial.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
            overrideMaterial.MaterialTypeParam = irr::video::pack_texureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
            ps->setOverrideMaterial(overrideMaterial);
            return ps;
    }

    static void addExplosion(const irr::core::vector3df& pos, const irr::core::vector3df& rot, const irr::f32& scale, irr::scene::ISceneManager* smgr, const irr::core::plane3df& p)
    {
        irr::scene::IParticleSystem* system = irr::scene::createParticleSystem(smgr->getRootSceneNode(), smgr, 6);
        system->setPosition(pos);
        system->setRotation(rot);
        system->setScale(irr::core::vector3df(scale));
        system->updateAbsolutePosition();

        ///Spark Trails
        irr::scene::particle::IParticleDrawer* drawer = system->addTrailParticleDrawer();

        irr::scene::particle::SParticleUV uv(irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(1.0f,0.75f), irr::core::vector2df(0.75f,1.0f), irr::core::vector2df(1.0f,1.0f));
        uv.turnRight();
        drawer->addUVCoords(uv);

        irr::scene::particle::IParticleEmitter* emitter =
            drawer->addStandardEmitter(
                irr::core::vector3df(0,0,0),
                irr::core::vector3df(0,0,0),
                irr::core::vector3df(0,15,0),
                irr::core::vector3di(180, 0, 180),
                0, 500, 30, 500, 1200,
                irr::core::vector2df(1.0),
                irr::core::vector2df(1.5),
                irr::core::vector2df(0,0),
                irr::video::SColor(255,255,255,0),
                irr::video::SColor(255,255,255,0));

        irr::scene::particle::IParticleAffector* affector = new ColorAffector(irr::video::SColor(0,100,50,0));
        drawer->addAffector(affector);
        affector->drop();

        ///Smoke Trails
        drawer = system->addTrailParticleDrawer();

        uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.5f, 0.5f), irr::core::vector2df(0.0f,0.5f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f));
        uv.turnRight();
        drawer->addUVCoords(uv);
        uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f+0.125f), irr::core::vector2df(0.5f, 0.5f+0.125f), irr::core::vector2df(0.0f,0.5f+0.125f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f+0.125f));
        uv.turnRight();
        drawer->addUVCoords(uv);
        uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f+0.125f+0.125f), irr::core::vector2df(0.5f, 0.5f+0.125f+0.125f), irr::core::vector2df(0.0f,0.5f+0.125f+0.125f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f+0.125f+0.125f));
        uv.turnRight();
        drawer->addUVCoords(uv);

        emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,10,0), irr::core::vector3di(180, 0, 180), 0, 500, 10, 400, 600, irr::core::vector2df(0.25,1.0), irr::core::vector2df(0.25,1.0), irr::core::vector2df(2.f), irr::video::SColor(255,255,128,50), irr::video::SColor(255,255,128,50));

        affector = new ColorAffectorQ(irr::video::SColor(128,128,128,50), irr::video::SColor(0,0,0,0));
        drawer->addAffector(affector);
        affector->drop();

        ///Debris
        drawer = system->addParticleDrawer();

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.5f), irr::core::vector2df(0.5f+0.0833f,0.5f), irr::core::vector2df(0.5f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f+0.0833f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f+0.0833f+0.0833f,0.5f), irr::core::vector2df(0.5f+0.0833f+0.0833f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f+0.0833f+0.0833f,0.5f+0.0833f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.5f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f), irr::core::vector2df(0.5f,0.5f+0.0833f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f+0.0833f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.5f+0.0833f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f+0.0833f), irr::core::vector2df(0.5f,0.5f+0.0833f+0.0833f+0.0833f), irr::core::vector2df(0.5f+0.0833f,0.5f+0.0833f+0.0833f+0.0833f)));

        emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(1,0,0), irr::core::vector3df(0,12,0), irr::core::vector3di(45, 0, 45), 0, 400, 10, 2000, 3000, irr::core::vector2df(0.2,0.2), irr::core::vector2df(0.5,0.5), irr::core::vector2df(0,0), irr::video::SColor(255,255,100,0), irr::video::SColor(255,255,100,0));

        affector = new ColorAffectorQ(irr::video::SColor(255,50,50,50), irr::video::SColor(255,50,50,50));
        drawer->addAffector(affector);
        affector->drop();
        affector = new GravityAffector(irr::core::vector3df(0.f,-15.0f,0.f));
        drawer->addAffector(affector);
        affector->drop();
        affector = new PlaneCollisionAffector(p);
        drawer->addAffector(affector);
        affector->drop();

        ///Wave Emitter
        drawer = system->addOrientedParticleDrawer();
        drawer->getMaterial().BackfaceCulling = false;

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.75f,0.5f), irr::core::vector2df(1.0f,0.5f), irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(1.0f,0.75f)));

        emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3di(0), 20, 50, 1, 500, 500, irr::core::vector2df(2.5,2.5), irr::core::vector2df(2.5,2.5), irr::core::vector2df(15), irr::video::SColor(255,200,100,0), irr::video::SColor(255,200,128,0));

        affector = new ColorAffector(irr::video::SColor(0,100,10,0));
        drawer->addAffector(affector);
        affector->drop();

        ///Fire/Smoke
        drawer = system->addParticleDrawer();

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.0f), irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f), irr::core::vector2df(0.25f,0.5f), irr::core::vector2df(0.5f,0.5f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.5f,0.0f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.25f,0.5f)));

        emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(1,0,0), irr::core::vector3df(2,0,0), irr::core::vector3di(0, 180, 0), 200, 400, 50, 1000, 1200, irr::core::vector2df(1,1), irr::core::vector2df(2,2), irr::core::vector2df(2), irr::video::SColor(255,200,180,0), irr::video::SColor(255,200,180,0));

        affector = new ColorAffectorQ(irr::video::SColor(255, 100, 0, 0), irr::video::SColor(0,0,0,0));
        drawer->addAffector(affector);
        affector->drop();
        affector = new GravityAffector(irr::core::vector3df(0.f,10.0f,0.f));
        drawer->addAffector(affector);
        affector->drop();

        ///Spark Emitter
        drawer = system->addParticleDrawer();

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5f,0.75f), irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(0.5f,1.0f), irr::core::vector2df(0.75f,1.0f)));

        emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,6,0), irr::core::vector3di(90, 0, 90), 20, 50, 10, 1000, 1200, irr::core::vector2df(1.5, 1.5), irr::core::vector2df(2.0, 2.0), irr::core::vector2df(1,1), irr::video::SColor(255,255,128,0), irr::video::SColor(255,255,128,0));

        affector = new GravityAffector(irr::core::vector3df(0.f,0.8f,0.f));
        drawer->addAffector(affector);
        affector->drop();
        affector = new ColorAffector(irr::video::SColor(0,255,128,0));
        drawer->addAffector(affector);
        affector->drop();

        ///Flash Emitter
        drawer = system->addParticleDrawer();

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5,0), irr::core::vector2df(0.75,0), irr::core::vector2df(0.5,0.25), irr::core::vector2df(0.75f,0.25f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5+0.25f,0), irr::core::vector2df(0.75+0.25f,0), irr::core::vector2df(0.5+0.25f,0.25), irr::core::vector2df(0.75f+0.25f,0.25f)));

        emitter = drawer->addStandardEmitter(irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0), irr::core::vector3di(0), 20, 50, 5, 200, 300, irr::core::vector2df(1,1), irr::core::vector2df(1,1), irr::core::vector2df(3), irr::video::SColor(255,255,255,100), irr::video::SColor(255,255, 255,100));

        affector = new ColorAffector(irr::video::SColor(0,255, 255,100));
        drawer->addAffector(affector);
        affector->drop();

        irr::video::SMaterial overrideMaterial;
        overrideMaterial.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        overrideMaterial.setTexture(0, smgr->getVideoDriver()->getTexture("./res/tex/fig7_alpha.png"));
        overrideMaterial.setFlag(irr::video::EMF_LIGHTING, false);
        overrideMaterial.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        overrideMaterial.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
        overrideMaterial.MaterialTypeParam = irr::video::pack_texureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
        system->setOverrideMaterial(overrideMaterial);

        irr::scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(3000);
        system->addAnimator(anim);
        anim->drop();

        system->drop();
    }

    static irr::scene::IParticleSystem* addCommet(const irr::core::vector3df& pos, const irr::core::vector3df& rot, const irr::f32& scale, irr::scene::ISceneManager* smgr)
    {
        irr::scene::IParticleSystem* system = irr::scene::createParticleSystem(smgr->getRootSceneNode(), smgr, 6);
        system->setPosition(pos);
        system->setRotation(rot);
        system->setScale(irr::core::vector3df(scale));
        system->updateAbsolutePosition();

        irr::scene::particle::IParticleDrawer* drawer;
        irr::scene::particle::IParticleEmitter* emitter;
        irr::scene::particle::IParticleAffector* affector;

        ///Smoke Trails
        drawer = system->addParticleDrawer();

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.0f), irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f), irr::core::vector2df(0.25f,0.5f), irr::core::vector2df(0.5f,0.5f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.25f,0.0f), irr::core::vector2df(0.5f,0.0f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.5f,0.25f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.25f), irr::core::vector2df(0.25f,0.25f), irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.25f,0.5f)));

        emitter = drawer->addStandardEmitter(
            irr::core::vector3df(0,0,0),        //rotation
            irr::core::vector3df(0,0,0),        //rotationspeed
            irr::core::vector3df(0,0,0),       //speed
            irr::core::vector3di(0, 0, 0),  //maxAngle
            10,                                  //minPpS
            20,                                //maxPpS
            0,                                 //maxEmitt
            500,                                //minlifetime
            700,                               //maxlifetime
            irr::core::vector2df(0.1,0.1),     //minsize
            irr::core::vector2df(0.25,0.25),     //maxsize
            irr::core::vector2df(1.f),          //sizespeed
            irr::video::SColor(200,128,128,128), //minColor
            irr::video::SColor(200,200,200,200));  //maxColor

        ///Flash Emitter
        drawer = system->addParticleDrawer();

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5,0), irr::core::vector2df(0.75,0), irr::core::vector2df(0.5,0.25), irr::core::vector2df(0.75f,0.25f)));
        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.5+0.25f,0), irr::core::vector2df(0.75+0.25f,0), irr::core::vector2df(0.5+0.25f,0.25), irr::core::vector2df(0.75f+0.25f,0.25f)));

        emitter = drawer->addStandardEmitter(
            irr::core::vector3df(0,0,0),
            irr::core::vector3df(0,0,0),
            irr::core::vector3df(0,0,0),
            irr::core::vector3di(0),
            20, 50, 0, 100, 100,
            irr::core::vector2df(0.7,0.7),
            irr::core::vector2df(0.7,0.7),
            irr::core::vector2df(3),
            irr::video::SColor(255,255,255,100),
            irr::video::SColor(255,255,255,100));

        affector = new ColorAffector(irr::video::SColor(0,255, 0,0));
        drawer->addAffector(affector);
        affector->drop();

        irr::video::SMaterial overrideMaterial;
        overrideMaterial.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        overrideMaterial.setTexture(0, smgr->getVideoDriver()->getTexture("./res/tex/fig7_alpha.png"));
        overrideMaterial.setFlag(irr::video::EMF_LIGHTING, false);
        overrideMaterial.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        overrideMaterial.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
        overrideMaterial.MaterialTypeParam = irr::video::pack_texureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
        system->setOverrideMaterial(overrideMaterial);

        irr::scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(3000);
        system->addAnimator(anim);
        anim->drop();

        return system;
    }

    static void addShotHit(const irr::core::vector3df& pos, const irr::core::vector3df& rot, const irr::f32& scale, irr::scene::ISceneManager* smgr, const irr::core::plane3df& p)
    {
        irr::scene::IParticleSystem* system = irr::scene::createParticleSystem(smgr->getRootSceneNode(), smgr, 6);
        system->setPosition(pos);
        system->setRotation(rot);
        system->setScale(irr::core::vector3df(scale));
        system->updateAbsolutePosition();

        ///Spark Trails
        irr::scene::particle::IParticleDrawer* drawer = system->addTrailParticleDrawer();

        irr::scene::particle::SParticleUV uv(irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(1.0f,0.75f), irr::core::vector2df(0.75f,1.0f), irr::core::vector2df(1.0f,1.0f));
        uv.turnRight();
        drawer->addUVCoords(uv);

        irr::scene::particle::IParticleEmitter* emitter =
            drawer->addStandardEmitter(
                irr::core::vector3df(0,0,0),
                irr::core::vector3df(0,0,0),
                irr::core::vector3df(0,1,0),
                irr::core::vector3di(90, 0, 90),
                0, 500, 15, 250, 500,
                irr::core::vector2df(1.0),
                irr::core::vector2df(1.5),
                irr::core::vector2df(0,0),
                irr::video::SColor(255,255,255,0),
                irr::video::SColor(255,255,255,0));

        irr::scene::particle::IParticleAffector* affector = new ColorAffector(irr::video::SColor(0,100,50,0));
        drawer->addAffector(affector);
        affector->drop();

        ///Smoke Trails
        drawer = system->addTrailParticleDrawer();

        uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f), irr::core::vector2df(0.5f, 0.5f), irr::core::vector2df(0.0f,0.5f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f));
        uv.turnRight();
        drawer->addUVCoords(uv);
        uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f+0.125f), irr::core::vector2df(0.5f, 0.5f+0.125f), irr::core::vector2df(0.0f,0.5f+0.125f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f+0.125f));
        uv.turnRight();
        drawer->addUVCoords(uv);
        uv = irr::scene::particle::SParticleUV(irr::core::vector2df(0.0f,0.5f+0.125f+0.125f), irr::core::vector2df(0.5f, 0.5f+0.125f+0.125f), irr::core::vector2df(0.0f,0.5f+0.125f+0.125f+0.125f), irr::core::vector2df(0.5f,0.5f+0.125f+0.125f+0.125f));
        uv.turnRight();
        drawer->addUVCoords(uv);

        emitter = drawer->addStandardEmitter(
            irr::core::vector3df(0,0,0),
            irr::core::vector3df(0,0,0),
            irr::core::vector3df(0,1,0),
            irr::core::vector3di(90, 0, 90),
            0, 500, 10, 400, 600,
            irr::core::vector2df(0.25,0.5),
            irr::core::vector2df(0.25,0.5),
            irr::core::vector2df(1.f),
            irr::video::SColor(255,255,255,255),
            irr::video::SColor(255,255,255,255));

        affector = new ColorAffectorQ(irr::video::SColor(128,50,50,50), irr::video::SColor(0,0,0,0));
        drawer->addAffector(affector);
        affector->drop();

        ///Wave Emitter
        drawer = system->addOrientedParticleDrawer();
        drawer->getMaterial().BackfaceCulling = false;

        drawer->addUVCoords(irr::scene::particle::SParticleUV(irr::core::vector2df(0.75f,0.5f), irr::core::vector2df(1.0f,0.5f), irr::core::vector2df(0.75f,0.75f), irr::core::vector2df(1.0f,0.75f)));

        emitter = drawer->addStandardEmitter(
            irr::core::vector3df(0,0,0),
            irr::core::vector3df(0,0,0),
            irr::core::vector3df(0,0,0),
            irr::core::vector3di(0),
            20, 50, 1, 250, 250,
            irr::core::vector2df(0.5,0.5),
            irr::core::vector2df(0.5,0.5),
            irr::core::vector2df(2),
            irr::video::SColor(255,150,150,150),
            irr::video::SColor(255,50,50,50));

        affector = new ColorAffector(irr::video::SColor(0,100,100,100));
        drawer->addAffector(affector);
        affector->drop();

        irr::video::SMaterial overrideMaterial;
        overrideMaterial.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        overrideMaterial.setTexture(0, smgr->getVideoDriver()->getTexture("./res/tex/fig7_alpha.png"));
        overrideMaterial.setFlag(irr::video::EMF_LIGHTING, false);
        overrideMaterial.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
        overrideMaterial.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
        overrideMaterial.MaterialTypeParam = irr::video::pack_texureBlendFunc (irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA, irr::video::EMFN_MODULATE_1X, irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
        system->setOverrideMaterial(overrideMaterial);

        irr::scene::ISceneNodeAnimator* anim = smgr->createDeleteAnimator(3000);
        system->addAnimator(anim);
        anim->drop();

        system->drop();
    }

};//end of fx_creator
#endif // FX_CREATOR_H_INCLUDED
