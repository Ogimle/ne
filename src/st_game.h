#ifndef ST_GAME_H_INCLUDED
#define ST_GAME_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

#include "Game/game.h";
#include "Game/editor.h";
#include "Game/conf_game.h";

// id gui элементов для обработчика событий
enum
{
        GAME_IDBUTTON_MAINMENU = 1
};

class ST_Game : public CState
{
    public:
        ST_Game(irr::IrrlichtDevice* device) : CState(device)
        {
            isExitDialog = false;
        }

        ~ST_Game()
        {
            //dtor
        }

        // обработчик событий
        virtual bool OnEvent(const irr::SEvent& event)
        {
            if ( event.EventType == irr::EET_GUI_EVENT )
            {
                irr::s32 id = event.GUIEvent.Caller->getID();

                if (event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_OK && id == GAME_IDBUTTON_MAINMENU)
                {
                    // генерим событие для передачи основному обработчику (менеджеру состояний)
                    irr::SEvent se;
                    se.EventType = irr::EET_USER_EVENT;
                    se.UserEvent.UserData1 = game::EVENT_SWITCH_STATE; // тип события - переключить состояние
                    se.UserEvent.UserData2 = game::STATE_MAINMENU;
                    return Device->getEventReceiver()->OnEvent(se);
                }

                if (event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_CANCEL && id == GAME_IDBUTTON_MAINMENU)
                {
                    isExitDialog = false;
                }
            }
            else if ( event.EventType == irr::EET_KEY_INPUT_EVENT)
            {
                if ( !event.KeyInput.PressedDown )
                {
                    if ( event.KeyInput.Key == irr::KEY_ESCAPE && !isExitDialog)
                    {
                        Device->getGUIEnvironment()->addMessageBox(L"Внимание!", L"Выйти из игры?", true, irr::gui::EMBF_OK | irr::gui::EMBF_CANCEL, 0, GAME_IDBUTTON_MAINMENU);
                        isExitDialog = true;
                    }
                    else if ( event.KeyInput.Key == irr::KEY_F12 )
                    {
                        Device->getCursorControl()->setVisible( !Device->getCursorControl()->isVisible() );
                    }
                }

            }

            bool testEvent = Game->OnEvent(event);
            if (!testEvent) Editor->camera.OnEvent( event );

        }//OnEvent

        virtual void activate()
        {
            Editor->fillMiniMap();
            Game->start();
        }

        virtual void deactivate()
        {
            Game->finish();
            Editor->clearScene();
        }

        virtual void update(irr::f32 timediff)
        {
            Game->update(timediff);
        }

    private:
        bool isExitDialog;
};

#endif // ST_GAME_H_INCLUDED
