#include "game.h"

C_Game::C_Game(irr::IrrlichtDevice* device)
{
    Device = device;
    hero = NULL;
    enemy = NULL;
    printf("Game created\n");
}

C_Game::~C_Game()
{
    //delete hero;
    printf("Game deleted\n");
}

bool C_Game::OnEvent(const irr::SEvent& event)
{
    /*
    if ( event.EventType == irr::EET_KEY_INPUT_EVENT)
    {

    }
    else if ( event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        if ( event.MouseInput.Event == irr::EMIE_MOUSE_MOVED )
        {

        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP )
        {
        }
        else if ( event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN )
        {
        }
    }
    */
    return hero->OnEvent(event);
}

void C_Game::start()
{
    Editor->camera.IsCamShift=false;

    hero = new C_Hero(Device);
    enemy = new C_Enemy(Device);

    hero->init(&gamemap);
    enemy->init(&gamemap);

    gamemap.init();
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
