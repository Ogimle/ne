#ifndef ST_MAINMENU_H_INCLUDED
#define ST_MAINMENU_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

// id gui элементов для обработчика событий
enum
{
        MM_IDBUTTON_GAME = 1,
        MM_IDBUTTON_LEVELMANAGE,
        MM_IDBUTTON_OPTIONS,
        MM_IDBUTTON_EXIT,
};

class ST_MainMenu : public CState
{
    public:
        ST_MainMenu(irr::IrrlichtDevice* device) : CState(device)
        {
            irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

            irr::s32 hpos = 25, ht = 20;
            but_game =      guienv->addButton( irr::core::rect<irr::s32>(10, hpos, 150, hpos+ht),  0, MM_IDBUTTON_GAME, L"ИГРАТЬ");

            hpos += ht+5;
            but_editor =    guienv->addButton( irr::core::rect<irr::s32>(10, hpos, 150, hpos+ht),  0, MM_IDBUTTON_LEVELMANAGE, L"РЕДАКТОР");

            hpos += ht+5;
            but_option =    guienv->addButton( irr::core::rect<irr::s32>(10, hpos, 150, hpos+ht),  0, MM_IDBUTTON_OPTIONS, L"НАСТРОЙКИ");

            hpos += ht+5;
            but_exit =      guienv->addButton( irr::core::rect<irr::s32>(10, hpos, 150, hpos+ht),  0, MM_IDBUTTON_EXIT, L"ВЫХОД");

            // по умолчанию самодеактивируемся
            deactivate();

        }

        ~ST_MainMenu()
        {
            //dtor
        }

        // обработчик событий
        virtual bool OnEvent(const irr::SEvent& event)
        {
            if ( event.EventType == irr::EET_GUI_EVENT )
            {
                if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
                {
                    irr::s32 id = event.GUIEvent.Caller->getID();

                    // генерим событие для передачи основному обработчику (менеджеру состояний)
                    irr::SEvent se;
                    se.EventType = irr::EET_USER_EVENT;
                    se.UserEvent.UserData1 = game::EVENT_SWITCH_STATE; // тип события - переключить состояние

                    //
                    switch(id)
                    {
                        case MM_IDBUTTON_GAME:
                            se.UserEvent.UserData2 = game::STATE_LEVELSELECT; // событие - переключится в игру
                            Device->getEventReceiver()->OnEvent(se);
                        break;

                        case MM_IDBUTTON_LEVELMANAGE:
                            se.UserEvent.UserData2 = game::STATE_LEVELMANAGE;  // событие - переключится в редактор
                            Device->getEventReceiver()->OnEvent(se);
                        break;

                        case MM_IDBUTTON_OPTIONS:
                            se.UserEvent.UserData2 = game::STATE_OPTIONS;  // событие - переключится в настройки
                            Device->getEventReceiver()->OnEvent(se);
                        break;

                        case MM_IDBUTTON_EXIT:
                            se.UserEvent.UserData2 = game::STATE_NONE; // событие - выход из игры
                            Device->getEventReceiver()->OnEvent(se);
                        break;
                    }
                }
            }

            return false;
        }//OnEvent

        virtual void activate()
        {
            but_game->setVisible(true);
            but_editor->setVisible(true);
            but_option->setVisible(true);
            but_exit->setVisible(true);
        }

        virtual void deactivate()
        {
            but_game->setVisible(false);
            but_editor->setVisible(false);
            but_option->setVisible(false);
            but_exit->setVisible(false);
        }

    private:
        irr::gui::IGUIButton* but_game;
        irr::gui::IGUIButton* but_editor;
        irr::gui::IGUIButton* but_option;
        irr::gui::IGUIButton* but_exit;
};

#endif // ST_MAINMENU_H_INCLUDED
