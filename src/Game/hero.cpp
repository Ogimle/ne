#include "hero.h"
#include "enemy.h"
#include "editor.h"
#include "../Utils/fx_creator.h"

C_Hero::C_Hero(irr::IrrlichtDevice* device)
{
    Device = device;
}

C_Hero::~C_Hero()
{
}

void C_Hero::init(C_WaveFindWay* gm)
{
    gamemap = gm;

    setupHeroModel();
    irr::core::vector3df pos( -int(Editor->getWordlWidth()/2)+0.5, 0, int(Editor->getWordlHeigh()/2)+0.5 );
    gamemap->setCost(-pos.X, pos.Z, 777);
    sn_chassis_1->setPosition(pos);
    sn_tower->setPosition(pos);
	updateHeroAnim(0);
}

void C_Hero::setupHeroModel()
{
    //sn_hero = Device->getSceneManager()->addAnimatedMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero.x") );

    sn_chassis_1 = Device->getSceneManager()->addAnimatedMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/chassis_1.x") );
    sn_chassis_1->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/sha r.jpg") );
    sn_chassis_1->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    sn_chassis_2 = Device->getSceneManager()->addAnimatedMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/chassis_2a.x") , sn_chassis_1);
    sn_chassis_2->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/sha2.jpg") );
    sn_chassis_2->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    sn_chassis_2->setFrameLoop(4,8);
    sn_chassis_2->setAnimationSpeed(4);

    sn_chassis_1->setScale( irr::core::vector3df(0.056, 0.056, 0.056) );

    sn_tower = Device->getSceneManager()->addMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/tower.x") );
    sn_tower->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/cor r.jpg") );
    sn_tower->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    sn_rocket = Device->getSceneManager()->addMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/rocket.x") , sn_tower );
    sn_rocket->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/rak r.jpg") );
    sn_rocket->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    sn_cannon = Device->getSceneManager()->addMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/cannon.x") , sn_tower );
    sn_cannon->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/pus r.jpg") );
    sn_cannon->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    sn_fazer = Device->getSceneManager()->addMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/fazer.x") , sn_tower );
    sn_fazer->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/faz r.jpg") );
    sn_fazer->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    sn_abomb = Device->getSceneManager()->addMeshSceneNode( Device->getSceneManager()->getMesh("./res/mdl/hero/abomb.x") , sn_tower );
    sn_abomb->setMaterialTexture( 0, Device->getVideoDriver()->getTexture("./res/tex/tank/bom.jpg") );
    sn_abomb->setMaterialFlag(irr::video::EMF_LIGHTING, false);

    sn_tower->setScale( irr::core::vector3df(0.056, 0.056, 0.056) );

    ps_gaz = NULL;

    firstHeroUpdate = true;
    isStartMove = false;
    MoveSpeed = 1.5f;
	RotateSpeed = 0.5f;
    idxPathNode = -1;
    idxLockNode = -1;
}

void C_Hero::fxMove(bool doit)
{
    if (doit)
    {
        ps_gaz = FX_Creator::exhaust(sn_chassis_1->getJointNode("Ar_tube"), Device->getSceneManager());
        ps_traks = new DecalSceneNode(sn_chassis_1->getJointNode("Ar_traks"), Device->getSceneManager(), Device->getVideoDriver()->getTexture("./res/tex/tank/traks.png"), irr::core::vector3df(0.65, 1, 0.3),5000,200,50);
    }
    else
    {
        if (ps_gaz)
        {
            ps_gaz->remove();
            ps_gaz = NULL;
        }

        if (ps_traks)
        {
            ps_traks->unlink();
            ps_traks = NULL;
        }
    }
}

void C_Hero::setPath( std::vector<Point> p )
{
    //разлочиваем ноду на которую шли
    if (isMove) gamemap->setCost( path[idxPathNode].X, path[idxPathNode].Y, 0 );
    path.clear();
    path = p;
    idxLockNode=0; // лочим начальную точку, на ней стоим
    //gamemap->setCost( path[idxLockNode].X, path[idxLockNode].Y, 777 );
    idxPathNode=1; // заказываем следующую

    if (!isMove) fxMove(true);
    isStartMove=isMove=true;

}

void C_Hero::updateHeroAnim(irr::f32 timediff)
{
	bool isChanged = false;

    if (firstHeroUpdate)
	{
		lastHeroAnimationTime = timediff;

		firstHeroUpdate = false;
		isChanged = true;
		isMove = isAttack = false;
	}

	// get time
	irr::f32 timeDiff = (irr::f32) ( timediff - lastHeroAnimationTime );
	lastHeroAnimationTime = timediff;

	//!init

	irr::core::vector3df target = sn_chassis_1->getPosition();

    //!Update move

    if (isStartMove)
    {
        // проверяем не встал ли кто на пути, если да ищем новый путь
        if ( gamemap->getCost(path[idxPathNode].X, path[idxPathNode].Y)==9999 )
        {
            idxPathNode = -1;
            isStartMove = isMove = false;
            fxMove(false);
            return;
        }

        // лочим клетку на которую идем
        gamemap->setCost( path[idxPathNode].X, path[idxPathNode].Y, 9999 );
        // разблокируем ту с которой уходим
        gamemap->setCost( path[idxLockNode].X, path[idxLockNode].Y, 0 );
        idxLockNode = idxPathNode;

        irr::f32 nx=-target.X, ny=target.Z, ddx=0, ddy=0;
        irr::u32 dx=path[idxPathNode].X, dy=path[idxPathNode].Y;
        ddx = dx+0.5;
        ddy = dy+0.5;

        irr::core::vector3df Direction = irr::core::vector3df( -ddx, 0, ddy)-irr::core::vector3df(-nx,0,ny);
        Direction.normalize();
        irr::core::vector3df rot = Direction.getHorizontalAngle();
        sn_chassis_1->setRotation( rot );
        sn_tower->setRotation( rot );
        isStartMove = false;
    }

    if ( idxPathNode < path.size() )
    {
        irr::f32 nx=-target.X, ny=target.Z, ddx=0, ddy=0;
        irr::u32 dx=path[idxPathNode].X, dy=path[idxPathNode].Y;
        ddx = dx+0.5;
        ddy = dy+0.5;


        irr::core::vector3df Direction = irr::core::vector3df( -ddx, 0, ddy)-irr::core::vector3df(-nx,0,ny);
        Direction.normalize();
        target += Direction * timeDiff * MoveSpeed;

        if (fabs(ddx-nx)<0.001 && fabs(ddy-ny)<0.001) // пришли в пункт назначения
        {
            idxPathNode++;
            if (idxPathNode<path.size())
                isStartMove = true;
            else
            {
                isMove = false;
                fxMove(false);
            }
        }
    }


    //!update animation
    //!update camera

    if (isChanged || isMove)
    {
        float sinOfPhi = sinf(Editor->camera.getPhi() * irr::core::DEGTORAD);
        float cosOfPhi = cosf(Editor->camera.getPhi() * irr::core::DEGTORAD);

        float sinOfTheta = sinf(Editor->camera.getTheta() * irr::core::DEGTORAD);
        float cosOfTheta = cosf(Editor->camera.getTheta() * irr::core::DEGTORAD);

        irr::core::vector3df offset;

        // these are switched around a little from what i posted above
        offset.X = Editor->camera.getRadius() * sinOfTheta * sinOfPhi;
        offset.Y = Editor->camera.getRadius() * cosOfPhi;
        offset.Z = Editor->camera.getRadius() * cosOfTheta * sinOfPhi;

        sn_tower->setPosition( target );
        sn_chassis_1->setPosition( target );

        Editor->camera.csn->setPosition(target+offset);
        Editor->camera.csn->setTarget( target );

        Editor->camera.csn->updateAbsolutePosition();
    }
}
