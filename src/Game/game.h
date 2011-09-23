#ifndef CGAME_H
#define CGAME_H

#include <irrlicht.h>
#include "conf_game.h"
#include "gamemap.h"
#include "hero.h"
#include "enemy.h"

class C_Game
{
    public:
        C_Hero* hero;
        C_Enemy* enemy;

        C_Game(irr::IrrlichtDevice* device);
        virtual ~C_Game();

        bool OnEvent(const irr::SEvent& event);

        void start();
        void finish();

        void update(irr::f32 timediff);

    private:
        irr::IrrlichtDevice* Device;
        C_GameMap gamemap;

        irr::u32 score;
        irr::u32 level;
};

extern C_Game* Game;

#endif // CGAME_H
