#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

namespace game
{
    enum EVENTS
    {
        EVENT_SWITCH_STATE = 1,
        EVENT_SWITCH_NEXTSTATE
    };

    enum STATES
    {
        // обязательное состояние, требуется менеджеру чтобы определить завершение проложения
        STATE_NONE = 200,

        // игровые состояния
        STATE_LOADER,
        STATE_MAINMENU,
        //STATE_LEVELMENU,
        STATE_LEVELMANAGE,
        STATE_LEVELSELECT,
        STATE_GAME,
        STATE_EDITOR,
        STATE_OPTIONS
    };

    #ifdef _DEBUG
    irr::c8* state2str(STATES n)
    {
        switch(n)
        {
            case STATE_NONE: return "game::STATE_NONE";
            case STATE_LOADER: return "game::STATE_LOADER";
            case STATE_MAINMENU: return "game::STATE_MAINMENU";
            //case STATE_LEVELMENU: return "game::STATE_LEVELMENU";
            case STATE_LEVELMANAGE: return "game::STATE_LEVELMANAGE";
            case STATE_LEVELSELECT: return "game::STATE_LEVELSELECT";
            case STATE_GAME: return "game::STATE_GAME";
            case STATE_EDITOR: return "game::STATE_EDITOR";
            case STATE_OPTIONS: return "game::STATE_OPTIONS";
        }
    }
    #endif
};

#endif // CONST_H_INCLUDED
