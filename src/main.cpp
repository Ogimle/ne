#include <irrlicht.h>

using namespace irr;

#include "SkinSystem/CGUIProgressBar.h"
#include "SkinSystem/CImageGUISkin.h"

//подключаем менеджер состояний
#include "StateManager/CStateManager.h"
CStateManager* sm;

// подключаем состояния
#include "st_loader.h"
#include "st_mainmenu.h"
#include "st_game.h"
#include "st_editor.h"
#include "st_levelmanage.h"
#include "st_levelselect.h"
#include "st_options.h"

#include "Game/editor.h";
C_Editor* Editor;

#include "Game/game.h";
C_Game* Game;

int main()
{
        irr::IrrlichtDevice *device = irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1024, 768), 16, false, false, false, 0);

        if (!device) return 1;

        device->setWindowCaption(L"Nether Earth");

        irr::video::IVideoDriver* driver = device->getVideoDriver();
        irr::scene::ISceneManager* smgr = device->getSceneManager();
        irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
        irr::io::IFileSystem* filesys = device->getFileSystem();

        Editor = new C_Editor(device);
        Game = new C_Game(device);

        // создаем менеджер состояний и регистрируем в нем состояния
        sm = new CStateManager(device);
        sm->registerState( game::STATE_LOADER, new ST_Loader(device) );
        sm->registerState( game::STATE_MAINMENU, new ST_MainMenu(device) );
        sm->registerState( game::STATE_GAME, new ST_Game(device) );
        sm->registerState( game::STATE_EDITOR, new ST_Editor(device) );
        sm->registerState( game::STATE_LEVELMANAGE, new ST_LevelManage(device) );
        sm->registerState( game::STATE_LEVELSELECT, new ST_LevelSelect(device) );
        sm->registerState( game::STATE_OPTIONS, new ST_Options(device) );
        sm->switchState( game::STATE_LOADER ); // переключаемся на загрузчик
        sm->setNextState( game::STATE_MAINMENU ); // следующее за загрузчиком будет главное меню
        //sm->setNextState( game::STATE_LEVELMANAGE );

        device->setEventReceiver(sm);

        // грузим скин
        gui::SImageGUISkinConfig guicfg = LoadGUISkinFromFile(filesys, driver, "res/ui/guiskin.cfg");
        gui::CImageGUISkin* skin = new gui::CImageGUISkin(driver, guienv->getSkin());
        skin->loadConfig(guicfg);
        gui::IGUIFont* font = guienv->getFont("res/ui/font_16.xml");
        if (font != 0)
        {
            skin->setFont(font, gui::EGDF_DEFAULT);
            skin->setFont(font, gui::EGDF_WINDOW);
        }
        guienv->setSkin(skin);
        skin->drop();

        // главный цикл
        int lastFPS = -1;
/*
irr::video::SMaterial material;
        material.setTexture(0, 0);
        material.Lighting = false;
        material.NormalizeNormals = true;
*/
        while(device->run())
        {
            if (device->isWindowActive())
            {

                driver->beginScene(true, true, irr::video::SColor(255,100,101,140));
                smgr->drawAll();

                /*if(Editor->posObjClick)
                {
                        // мы должны сбросить трансформации перед отрисовкой.
                        driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
                        driver->setMaterial(material);
                        driver->draw3DTriangle(Editor->triObjClick, video::SColor(0,255,0,0));
                }*/

                guienv->drawAll();
                driver->endScene();

                if (!sm->update()) break;

                int fps = driver->getFPS();
                if (lastFPS != fps)
                {
                        core::stringw str = L"Nether Earth [";
                        str += driver->getName();
                        str += "] FPS:";
                        str += fps;//driver->getPrimitiveCountDrawn();

                        device->setWindowCaption(str.c_str());
                        lastFPS = fps;
                }
            }
            else
                device->yield();
        }

        delete Editor;
        delete sm;
        device->drop();

        return 0;
}
