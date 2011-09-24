#include "game.h"
#include "bullets.h"

C_Game::C_Game(irr::IrrlichtDevice* device)
{
    Device = device;
    hero = NULL;
    enemy = NULL;

    KeyMap.push_back(SCamKeyMap(irr::EKA_MOVE_FORWARD, irr::KEY_KEY_W));
    KeyMap.push_back(SCamKeyMap(irr::EKA_MOVE_BACKWARD, irr::KEY_KEY_S));
    KeyMap.push_back(SCamKeyMap(irr::EKA_STRAFE_LEFT, irr::KEY_KEY_A));
    KeyMap.push_back(SCamKeyMap(irr::EKA_STRAFE_RIGHT, irr::KEY_KEY_D));
    KeyMap.push_back(SCamKeyMap(irr::EKA_JUMP_UP, irr::KEY_SPACE));

    for (irr::u32 i=0; i<6; ++i) CursorKeys[i] = false;
    for (irr::u32 i=0; i<3; ++i) MouseKeys[i] = false;
}

C_Game::~C_Game()
{
    //delete hero;
    printf("Game deleted\n");
}

bool C_Game::OnEvent(const irr::SEvent& event)
{
    if ( event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        for (irr::u32 i=0; i<KeyMap.size(); ++i)
        {
            if (KeyMap[i].keycode == event.KeyInput.Key)
            {
                CursorKeys[KeyMap[i].action] = event.KeyInput.PressedDown;
            }
        }
    }
    else if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
        {
        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
        {
        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
        {
            bool isMapClick = Editor->testMapClick( event.MouseInput.X, event.MouseInput.Y );

            if (isMapClick && enemy->testCollide( Editor->posMapClick_f)>-1 )
            {
                irr::core::vector3df start=hero->getRocket(), end = Editor->posMapClick_f;
                end.Y = start.Y = 0.3;
                addBullet_1(Device->getSceneManager(), start, end-start, 0.09f, Device->getTimer()->getTime()+1000);
            }
            else if (Editor->testObjClick(event.MouseInput.X, event.MouseInput.Y))
            {
                irr::core::vector3df start=hero->getRocket(), end = Editor->posMapClick_f;
                end.Y = start.Y = 0.3;
                addBullet_1(Device->getSceneManager(), start, end-start, 0.09f, Device->getTimer()->getTime()+1000);
            }
            else if (isMapClick)
            {
                if ( event.MouseInput.Shift )
                {
                    irr::core::vector3df start=hero->getRocket(), end = Editor->posMapClick_f;
                    end.Y = start.Y = 0.3;
                    addBullet_1(Device->getSceneManager(), start, end-start, 0.09f, Device->getTimer()->getTime()+1000);
                }
                else
                {
                    irr::core::vector3df t = Editor->camera.csn->getTarget();
                    gamemap.aStar->Reset();
                    if ( gamemap.getPath( -int(t.X), int(t.Z), -int(Editor->posMapClick.X), int(Editor->posMapClick.Z)) )
                    {
                        hero->setPath( gamemap.path );
                        #if _DEBUG
                        printf("t: %d %d\n", int(t.X), int(t.Z));
                        gamemap.PrintPath();
                        #endif
                    }
                }
            }
            MouseKeys[0] = false;
        }
        else if ( event.MouseInput.Event == irr::EMIE_RMOUSE_LEFT_UP )
        {
            if (Editor->testMapClick( event.MouseInput.X, event.MouseInput.Y ))
            {
                irr::core::vector3df start=hero->getRocket(), end = Editor->posMapClick_f;
                end.Y = start.Y = 0.3;
                addRocket(Device->getSceneManager(), start, end-start, 0.02f, Device->getTimer()->getTime()+3000);
            }
        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
        {
            MouseKeys[0] = true;
        }
    }

    return false;
}

void C_Game::start()
{
    Editor->camera.IsCamShift=false;

    hero = new C_Hero(Device);
    enemy = new C_Enemy(Device);

    hero->init(&gamemap);
    enemy->init(&gamemap);

    gamemap.init();

    for (irr::u32 i=0; i<6; ++i) CursorKeys[i] = false;
    for (irr::u32 i=0; i<3; ++i) MouseKeys[i] = false;
}

void C_Game::finish()
{
    Editor->camera.IsCamShift=true;

    delete hero; hero = NULL;
    delete enemy; enemy = NULL;
}

void C_Game::update(irr::f32 timediff)
{
    if ( hero ) hero->updateHeroAnim(timediff);
    if ( enemy ) enemy->update(timediff, hero->getPosition());
}
