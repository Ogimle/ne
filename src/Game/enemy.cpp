#include "enemy.h"
#include "../Utils/fx_creator.h"

C_Enemy::C_Enemy(irr::IrrlichtDevice* device)
{
    Device = device;
}

C_Enemy::~C_Enemy()
{
    for(irr::u16 i, imax=enemies.size(); i<imax; i++) delete enemies[i];
    enemies.clear();
}

void C_Enemy::init(C_GameMap* gm)
{
    gamemap = gm;
    Editor->getSpawnPoints( &spawnPoints );
    currentSP = 0;
    for(irr::u16 i=0, imax=spawnPoints.size(); i<imax; i++) addEnemy();
}

irr::s16 C_Enemy::testCollide( irr::core::vector3df pos, irr::f32 tolerance)
{

    for(irr::u16 i=0, imax=enemies.size(); i<imax; i++)
    {
        if ( pos.equals( enemies[i]->node->getPosition(), tolerance) )
            return i;
    }
    return -1;
}

bool C_Enemy::testCollideEx( irr::core::vector3df pos, irr::core::array<irr::u16>* ret, irr::f32 tolerance)
{
    for(irr::u16 i=0, imax=enemies.size(); i<imax; i++)
    {
        if ( pos.equals( enemies[i]->node->getPosition(), tolerance) )
            ret->push_back(i);
    }
    return ret->size()>0?true:false;
}

irr::u32 C_Enemy::getNextSpawnPoint()
{
    currentSP++;
    if (currentSP==spawnPoints.size()) currentSP=0;
    return currentSP;
}

void C_Enemy::hit(irr::u16 id, irr::f32 dmg)
{
    enemies[id]->hitPoints -= dmg;
}

void C_Enemy::update(irr::f32 timediff, irr::core::vector3df hero_pos)
{
    for(irr::u16 i=0, imax=enemies.size(); i<imax; i++)
        updEnemy(i, timediff, hero_pos);
}

void C_Enemy::addEnemy(irr::f32 rate)
{
    S_Enemy* e = new S_Enemy();
    e->hitPoints = 10*rate;
    e->speed = 0.9;
    e->melee_damage = 5*rate;
    e->rate = rate;
    e->node = setupEnemyModel();
    rand_reset();
    irr::s32 sp_id = getNextSpawnPoint();
    e->node->setPosition( irr::core::vector3df( -spawnPoints[sp_id].X-0.5, 0, spawnPoints[sp_id].Y+0.5 ) );

    e->isMove=false;
    e->isStartMove=false;

    e->alive=true;
    e->decay_time = 0;

    enemies.push_back(e);
}

void C_Enemy::delEnemy(irr::u32 id)
{
    irr::f32 r = enemies[id]->rate;
    enemies[id]->node->remove();
    delete enemies[id];
    enemies.erase(id);
    addEnemy(r+0.1f);
}

void C_Enemy::updEnemy(irr::u32 id, irr::f32 timeDiff, irr::core::vector3df hero_pos)
{
    S_Enemy* e = enemies[id];

    if ( e->alive && e->hitPoints <= 0 )
    {
        if (e->decay_time==0)
        {
            e->decay_time = timeDiff;
            irr::core::plane3df p(e->node->getPosition(), irr::core::vector3df(0,0,0));
            FX_Creator::addSmoke(e->node->getPosition(), irr::core::vector3df(0,0,0), 1.f, Device->getSceneManager(), p);
        }
        else if ( e->decay_time > 2 )
        {
            delEnemy(id);
        }
        else
        {
            e->decay_time += timeDiff;
        }
        return;
    }

    if ( !e->isMove
        && !e->node->getPosition().equals(hero_pos, 0.9)
        && gamemap->getPath(int(-e->node->getPosition().X), int(e->node->getPosition().Z), int(-hero_pos.X), int(hero_pos.Z))
    )
    {
        e->idxPathNode = 1;
        e->path = gamemap->path;
        e->isMove = true;
        e->isStartMove = true;

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
        e->node->setPosition(pos);

        if (e->isStartMove)
        {
            irr::core::vector3df rot = Direction.getHorizontalAngle();
            e->node->setRotation(rot);
            e->isStartMove = false;
        }

            //если герой рядом то останавливаемся и хлабучим
        if ( e->node->getPosition().equals(hero_pos, 0.9) )
        {
            e->isMove = false;
        }
        else if (fabs(ddx-nx)<0.001 && fabs(ddy-ny)<0.001) // пришли в пункт назначения
        {
            //идем к следующей точке
            e->idxPathNode++;
            if ( e->idxPathNode == e->path.size() )
                e->isMove = false;
            else
                e->isStartMove = true;

            /*
            irr::core::vector3df rot = Direction.getHorizontalAngle();
            sn_chassis_1->setRotation( rot );
            sn_tower->setRotation( rot );
            */
        }
    }
}

irr::scene::ISceneNode* C_Enemy::setupEnemyModel(bool isCannon, bool isPhazer, bool isRocket, bool isABomb)
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
