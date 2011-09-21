#include "hero.h"
#include "editor.h"
#include "bullets.h"
#include "../Utils/fx_creator.h"

C_Hero::C_Hero(irr::IrrlichtDevice* device)
{
    Device = device;

    KeyMap.push_back(SCamKeyMap(irr::EKA_MOVE_FORWARD, irr::KEY_KEY_W));
    KeyMap.push_back(SCamKeyMap(irr::EKA_MOVE_BACKWARD, irr::KEY_KEY_S));
    KeyMap.push_back(SCamKeyMap(irr::EKA_STRAFE_LEFT, irr::KEY_KEY_A));
    KeyMap.push_back(SCamKeyMap(irr::EKA_STRAFE_RIGHT, irr::KEY_KEY_D));
    KeyMap.push_back(SCamKeyMap(irr::EKA_JUMP_UP, irr::KEY_SPACE));

    for (irr::u32 i=0; i<6; ++i) CursorKeys[i] = false;
    for (irr::u32 i=0; i<3; ++i) MouseKeys[i] = false;
}

C_Hero::~C_Hero()
{
}

void C_Hero::init(C_GameMap* gm)
{
    gamemap = gm;
    setupHeroModel();
    for (irr::u32 i=0; i<6; ++i) CursorKeys[i] = false;
    for (irr::u32 i=0; i<3; ++i) MouseKeys[i] = false;
}

bool C_Hero::OnEvent(const irr::SEvent& event)
{
    if ( event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        for (irr::u32 i=0; i<KeyMap.size(); ++i)
        {
            if (KeyMap[i].keycode == event.KeyInput.Key)
            {
                CursorKeys[KeyMap[i].action] = event.KeyInput.PressedDown;
                return true;
            }
        }
    }
    else if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
        {
            return true;
        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
        {
            if (Editor->testMapClick( event.MouseInput.X, event.MouseInput.Y ))
            {
                if ( event.MouseInput.Shift )
                {
                    irr::core::vector3df start=sn_rocket->getAbsolutePosition(), end = Editor->posMapClick_f;
                    end.Y = start.Y = 0.3;
                    addRocket(Device->getSceneManager(), start, end-start, 0.02f, 3000);
                }
                else
                {
                    irr::core::vector3df t = Editor->camera.csn->getTarget();
                    //gamemap->aStar->Reset();
                    if ( gamemap->getPath( -int(t.X), int(t.Z), -int(Editor->posMapClick.X), int(Editor->posMapClick.Z)) )
                    {
                        idxPathNode = 1;
                        #if _DEBUG
                        printf("t: %d %d\n", int(t.X), int(t.Z));
                        gamemap->PrintPath();
                        #endif
                    }
                }
            }
            MouseKeys[0] = false;
            return true;
        }
        else if ( event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP )
        {
            if (Editor->testMapClick( event.MouseInput.X, event.MouseInput.Y ))
            {
                irr::core::vector3df start=sn_rocket->getAbsolutePosition(), end = Editor->posMapClick_f;
                end.Y = start.Y = 0.3;
                addRocket(Device->getSceneManager(), start, end-start, 0.02f, 9000);
            }
        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
        {
            MouseKeys[0] = true;
            return true;
        }
    }

    return false;
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
    sn_chassis_2->setFrameLoop(0,8);
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
    MoveSpeed = .9f;
	RotateSpeed = 0.5f;
	updateHeroAnim(0);
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

	irr::core::vector3df target = Editor->camera.csn->getTarget();

    //!Update move

    if ( idxPathNode>0 && idxPathNode < gamemap->path.size() )
    {
        irr::f32 nx=-target.X, ny=target.Z, ddx=0, ddy=0;
        irr::s32 dx, dy;
        gamemap->NodeToXY( gamemap->path[idxPathNode] , &dx, &dy);
        ddx = dx+0.5;
        ddy = dy+0.5;


        irr::core::vector3df Direction = irr::core::vector3df( -ddx, 0, ddy)-irr::core::vector3df(-nx,0,ny);
        Direction.normalize();
        target += Direction * timeDiff * MoveSpeed;

        if (fabs(ddx-nx)<0.001 && fabs(ddy-ny)<0.001) // пришли в пункт назначения
        {
            idxPathNode++;
        }
        else
        {
            irr::core::vector3df rot = Direction.getHorizontalAngle();
            sn_chassis_1->setRotation( rot );
            sn_tower->setRotation( rot );
        }
        //sn_chassis_2->animateJoints();
    }

    //!update animation

        bool testAttack = MouseKeys[0];

        // move

        if ( !isMove && idxPathNode>0 )
        {
            ps_gaz = FX_Creator::exhaust(sn_chassis_1->getJointNode("Ar_tube"), Device->getSceneManager());
            ps_traks = new DecalSceneNode(sn_chassis_1->getJointNode("Ar_traks"), Device->getSceneManager(), Device->getVideoDriver()->getTexture("./res/tex/tank/traks.png"), irr::core::vector3df(0.65, 1, 0.3),5000,200,50);
            isMove = true;
        }
        else if ( isMove && idxPathNode == gamemap->path.size() )
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

            isMove = false;
        }

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

        Editor->camera.csn->setPosition(target+offset);
        Editor->camera.csn->setTarget( target );

        sn_tower->setPosition( target );
        sn_chassis_1->setPosition( target );

        Editor->camera.csn->updateAbsolutePosition();
    }
}
