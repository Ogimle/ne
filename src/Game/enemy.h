#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "../Utils/fx_creator.h"

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
    bool alive;
};

class C_Enemy
{
public:
    C_Enemy(irr::IrrlichtDevice* device)
    {
        Device = device;
    }

    ~C_Enemy()
    {
        for(irr::u16 i, imax=enemies.size(); i<imax; i++) delete enemies[i];
        enemies.clear();
    }

    void init(C_GameMap* gm)
    {
        gamemap = gm;
        Editor->getSpawnPoints( &spawnPoints );
        addEnemy();
    }

    void setTarget(irr::scene::ISceneNode* node) { target = node; }

    irr::s16 testCollide( irr::core::vector3df pos, irr::f32 tolerance = 0.4)
    {

        for(irr::u16 i=0, imax=enemies.size(); i<imax; i++)
        {
            if ( pos.equals( enemies[i]->node->getPosition(), tolerance) )
                return i;
        }
        return -1;
    }

    void hit(irr::u16 id, irr::f32 dmg)
    {
        enemies[id]->hitPoints -= dmg;
        printf("%d\n", enemies[id]->hitPoints);
    }

    void update(irr::f32 timediff, irr::core::vector3df hero_pos)
    {
        for(irr::u16 i=0, imax=enemies.size(); i<imax; i++)
            updEnemy(i, timediff, hero_pos);
    }

private:
    irr::IrrlichtDevice* Device;
    irr::core::array<S_Enemy*> enemies;
    irr::scene::ISceneNode* target;

    C_GameMap* gamemap;
    irr::core::array< irr::core::vector2di > spawnPoints;

    irr::s32 rand_seed;
    irr::s32 rand_gen(irr::s32 range)
    {
        srand(Device->getTimer()->getTime());
        return rand() % (range+1);

        const irr::s32 m = 2147483399;	// a non-Mersenne prime
        const irr::s32 a = 40692;		// another spectral success story
        const irr::s32 q = m/a;
        const irr::s32 r = m%a;		// again less than q

        rand_seed = a * (rand_seed%q) - r * (rand_seed/q);
        if (rand_seed<0) rand_seed += m;
printf("%d / %d = %f\n", rand_seed, range, rand_seed % range);
        return rand_seed % range;
    }
    void rand_reset() { rand_seed = Device->getTimer()->getTime(); }

    void addEnemy(irr::f32 rate=1)
    {
        S_Enemy* e = new S_Enemy();
        e->hitPoints = 10*rate;
        e->speed = 0.9;
        e->melee_damage = 5*rate;
        e->rate = rate;
        e->node = setupEnemyModel();
        rand_reset();
        irr::s32 sp_id = rand_gen( spawnPoints.size()-1 ) ;
        e->node->setPosition( irr::core::vector3df( -spawnPoints[sp_id].X-0.5, 0, spawnPoints[sp_id].Y+0.5 ) );
        e->isMove=false;
        e->alive=true;
        enemies.push_back(e);
    }

    void delEnemy(irr::u32 id)
    {
        irr::f32 r = enemies[id]->rate;
        enemies[id]->node->remove();
        delete enemies[id];
        enemies.erase(id);
        addEnemy(r+0.1f);
    }

    void updEnemy(irr::u32 id, irr::f32 timeDiff, irr::core::vector3df hero_pos)
    {
        S_Enemy* e = enemies[id];

        if ( e->alive && e->hitPoints <= 0 )
        {
            delEnemy(id);
            return;
        }

        if ( !e->isMove && gamemap->getPath(int(-e->node->getPosition().X), int(e->node->getPosition().Z), int(-hero_pos.X), int(hero_pos.Z)) )
        {
            e->idxPathNode = 1;
            e->path = gamemap->path;
            e->isMove=true;

        }
        if ( e->isMove && e->idxPathNode < e->path.size() )
        {
            irr::core::vector3df pos = e->node->getPosition();
            irr::f32 nx=-pos.X, ny=pos.Z, ddx=0, ddy=0;
            irr::s32 dx, dy;
            gamemap->NodeToXY( e->path[e->idxPathNode] , &dx, &dy);
            ddx = dx+0.5;
            ddy = dy+0.5;


            irr::core::vector3df Direction = irr::core::vector3df( -ddx, 0, ddy)-irr::core::vector3df(-nx,0,ny);
            Direction.normalize();
            pos += Direction * timeDiff * e->speed;
            irr::core::vector3df rot = Direction.getHorizontalAngle();
            e->node->setPosition(pos);
            e->node->setRotation(rot);

            if (fabs(ddx-nx)<0.001 && fabs(ddy-ny)<0.001) // пришли в пункт назначения
            {
                //если герой рядо то хлабучим а путь сбрасываем иначе
                //проверям сменил ли герой мето положения
                // да - расчитываем новый путь
                // нет - идем к следующей точке
                e->idxPathNode++;
                if ( e->idxPathNode == e->path.size() ) e->isMove = false;
                /*
                irr::core::vector3df rot = Direction.getHorizontalAngle();
                sn_chassis_1->setRotation( rot );
                sn_tower->setRotation( rot );
                */
            }
        }
    }

    irr::scene::ISceneNode* setupEnemyModel(bool isCannon=false, bool isPhazer=false, bool isRocket=false, bool isABomb=false)
    {
        irr::scene::ISceneManager* smgr = Device->getSceneManager();

        irr::scene::IAnimatedMeshSceneNode* tmp1 = smgr->addAnimatedMeshSceneNode( smgr->getMesh("./res/mdl/hero/chassis_1.x") );
        tmp1->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/sha b.jpg") );
        tmp1->setMaterialFlag(irr::video::EMF_LIGHTING, false);

        irr::scene::IAnimatedMeshSceneNode* tmp2 = smgr->addAnimatedMeshSceneNode( smgr->getMesh("./res/mdl/hero/chassis_2a.x") , tmp1);
        tmp2->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/sha2.jpg") );
        tmp2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        tmp2->setFrameLoop(0,8);
        tmp2->setAnimationSpeed(4);

        tmp1->setScale( irr::core::vector3df(0.056, 0.056, 0.056) );

        irr::scene::IAnimatedMeshSceneNode* tmp3 = smgr->addAnimatedMeshSceneNode( smgr->getMesh("./res/mdl/hero/tower.x"), tmp1);
        tmp3->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/cor b.jpg") );
        tmp3->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        //tmp3->setDebugDataVisible(irr::scene::EDS_FULL);

        irr::scene::IMeshSceneNode* tmp4;

        if (isRocket)
        {
            tmp4 = smgr->addMeshSceneNode( smgr->getMesh("./res/mdl/hero/rocket.x") , tmp3 );
            tmp4->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/rak b.jpg") );
            tmp4->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        }

        if (isCannon)
        {
            tmp4 = smgr->addMeshSceneNode( smgr->getMesh("./res/mdl/hero/cannon.x") , tmp3 );
            tmp4->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/pus b.jpg") );
            tmp4->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        }

        if (isPhazer)
        {
            tmp4 = smgr->addMeshSceneNode( smgr->getMesh("./res/mdl/hero/fazer.x") , tmp3 );
            tmp4->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/faz b.jpg") );
            tmp4->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        }

        if (isABomb)
        {
            tmp4 = smgr->addMeshSceneNode( smgr->getMesh("./res/mdl/hero/abomb.x") , tmp3 );
            tmp4->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/bom.jpg") );
            tmp4->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        }

        return tmp1;
    }
};


#endif // ENEMY_H_INCLUDED
