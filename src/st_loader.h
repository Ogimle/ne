#ifndef ST_LOADER_H_INCLUDED
#define ST_LOADER_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

#include "SkinSystem/SkinLoader.h"
#include "SkinSystem/CGUIProgressBar.h"

class ST_Loader : public CState
{
    public:
        ST_Loader(irr::IrrlichtDevice* device) : CState(device)
        {
            irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

            mdl.clear();
            mdl.push_back("./res/mdl/cursor_base.x");
            mdl.push_back("./res/mdl/cursor_plant.x");
            mdl.push_back("./res/mdl/cursor_wall_1.x");
            mdl.push_back("./res/mdl/cursor_wall_2.x");
            mdl.push_back("./res/mdl/base.x");
            mdl.push_back("./res/mdl/plant_chassis.x");
            mdl.push_back("./res/mdl/wall_1.x");
            mdl.push_back("./res/mdl/wall_2.x");
            mdl.push_back("./res/mdl/wall_3.x");
            mdl.push_back("./res/mdl/wall_4.x");

            tex.clear();
            tex.push_back("./res/tex/ground.jpg");
            tex.push_back("./res/tex/plant.png");
            tex.push_back("./res/tex/skydome_cosmo.jpg");
            tex.push_back("./res/tex/skydome_earth.jpg");

            tex.push_back("./res/ui/button.png");
            tex.push_back("./res/ui/font_160.png");
            tex.push_back("./res/ui/fontlucida.png");
            tex.push_back("./res/ui/gauge.png");
            tex.push_back("./res/ui/sankenpane.png");
            tex.push_back("./res/ui/windowbg.png");

            progres_mdl = mdl.size()-1;
            progres_tex = tex.size()-1;
            progres_stp = 1 / (progres_mdl+progres_tex+2);

            bar = new irr::gui::CGUIProgressBar(guienv->getRootGUIElement(), guienv, irr::core::rect<irr::s32>(10,10, 1014, 40) );
            bar->setAutomaticText(L"Загрузка...");
            bar->setProgress( 0 );

            isDone = false;

            // по умолчанию самодеактивируемся
            deactivate();
        }

        ~ST_Loader()
        {
            //dtor
        }

        // обработчик событий
        virtual bool OnEvent(const irr::SEvent& event)
        {
            // ну какие в загрузчики события???
            return false;
        }//OnEvent

        virtual void activate()
        {
            bar->setVisible(true);
        }

        virtual void deactivate()
        {
            bar->setVisible(false);
        }

        virtual void update(irr::f32 timediff)
        {
            if (isDone)
            {
                // генерим событие для передачи основному обработчику (менеджеру состояний)
                irr::SEvent se;
                se.EventType = irr::EET_USER_EVENT;
                se.UserEvent.UserData1 = game::EVENT_SWITCH_NEXTSTATE; // событие которое указано следующим
                Device->getEventReceiver()->OnEvent(se);
                return;
            }

            /**
                вот в этом замечательном месте можно очень даже замечательно
                вызвать загрузку следующего по списку игрового ресурса
            */
            if ( progres_mdl>0 )
            {
                Device->getSceneManager()->getMesh( mdl[ progres_mdl-- ] );
            }
            else if ( progres_tex > 0 )
            {
                Device->getVideoDriver()->getTexture( tex[ progres_tex-- ] );
            }
            else
            {
                isDone = true;
            }
            bar->setProgress( bar->getProgress() + progres_stp );
        }

    private:
        irr::u32 progres_mdl;
        irr::u32 progres_tex;
        irr::u32 progres_stp;

        bool isDone;

        irr::gui::CGUIProgressBar* bar;

        irr::core::array< irr::core::stringc > mdl;
        irr::core::array< irr::core::stringc > tex;
};

#endif // ST_LOADER_H_INCLUDED
