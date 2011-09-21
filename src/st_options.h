#ifndef ST_OPTIONS_H_INCLUDED
#define ST_OPTIONS_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

// id gui элементов для обработчика событий
enum
{
        OPTIONS_IDBUTTON_MAINMENU = 1
};

class ST_Options : public CState
{
    public:
        ST_Options(irr::IrrlichtDevice* device) : CState(device)
        {
            irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

            irr::s32 hpos = 25, ht = 20;
            but_back =      guienv->addButton( irr::core::rect<irr::s32>(10, hpos, 100, hpos+ht),  0, MM_IDBUTTON_GAME, L"BACK TO MAIN");

            // по умолчанию самодеактивируемся
            deactivate();
        }

        ~ST_Options()
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

                    switch(id)
                    {
                        case OPTIONS_IDBUTTON_MAINMENU:
                            se.UserEvent.UserData2 = game::STATE_MAINMENU; // событие - переключится в игру
                            Device->getEventReceiver()->OnEvent(se);
                        break;
                    }
                }
            }

            return false;
        }//OnEvent

        virtual void activate()
        {
            but_back->setVisible(true);
        }

        virtual void deactivate()
        {
            but_back->setVisible(false);
        }

    private:
        irr::gui::IGUIButton* but_back;
};

#endif // ST_OPTIONS_H_INCLUDED
