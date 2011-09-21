#ifndef ST_LEVELSELECT_H_INCLUDED
#define ST_LEVELSELECT_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

#include "Game/editor.h";

// id gui элементов для обработчика событий
enum
{
        LEVELSELECT_IDBUTTON_MAINMENU = 1,
        LEVELSELECT_IDBUTTON_CREATE,
        LEVELSELECT_IDBUTTON_LOAD,
        LEVELSELECT_IDBUTTON_DELETE,
        LEVELSELECT_IDDELETEDIALOG,
};

class ST_LevelSelect : public CState
{
    public:
        ST_LevelSelect(irr::IrrlichtDevice* device) : CState(device)
        {
            irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

            tabcontrol = guienv->addTabControl(irr::core::rect< irr::s32 >(10,10, 210, 310), 0, true, true);

                tabload = tabcontrol->addTab(L"Новая игра");
                    st_maplist = guienv->addStaticText(L"Выберите карту:", irr::core::rect< irr::s32 >(10,10,150,30), false, false, tabload, -1, false);
                    lb_maplist = guienv->addListBox( irr::core::rect<irr::s32>(10, 30, 190, 235), tabload, -1, false);

                    btn_load = guienv->addButton( irr::core::rect<irr::s32>(10, 240, 93, 265),  tabload, LEVELSELECT_IDBUTTON_LOAD, L"Начать");

                    preview = guienv->addImage( irr::core::rect<irr::s32>(250, 100, 250+512, 100+384) );
                    preview_img = Device->getVideoDriver()->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<u32>(512,384));


            but_back = guienv->addButton( irr::core::rect<irr::s32>(30, 320, 150, 350),  0, LEVELSELECT_IDBUTTON_MAINMENU, L"В главное меню");

            // по умолчанию самодеактивируемся
            deactivate();
        }

        ~ST_LevelSelect()
        {
            //dtor
        }

        // обработчик событий
        virtual bool OnEvent(const irr::SEvent& event)
        {
            if ( event.EventType == irr::EET_GUI_EVENT )
            {
                // генерим событие для передачи основному обработчику (менеджеру состояний)
                irr::SEvent se;
                se.EventType = irr::EET_USER_EVENT;
                se.UserEvent.UserData1 = game::EVENT_SWITCH_STATE; // тип события - переключить состояние

                irr::s32 id = event.GUIEvent.Caller->getID();

                if ( event.GUIEvent.EventType == irr::gui::EGET_LISTBOX_CHANGED || event.GUIEvent.EventType == irr::gui::EGET_LISTBOX_SELECTED_AGAIN)
                {
                    irr::io::path pth("./res/maps/");
                    pth.append( lb_maplist->getListItem(lb_maplist->getSelected())  );
                    irr::core::cutFilenameExtension(pth, pth);
                    pth.append(".jpg");
                    irr::video::IImage* img = Device->getVideoDriver()->createImageFromFile(pth);
                    img->copyToScaling( preview_img );
                    preview->setImage( Device->getVideoDriver()->addTexture("preview_tex", preview_img) );
                }
                else if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
                {

                    switch(id)
                    {
                        case LEVELSELECT_IDBUTTON_MAINMENU:
                            se.UserEvent.UserData2 = game::STATE_MAINMENU;
                            return Device->getEventReceiver()->OnEvent(se);
                        break;

                        case LEVELSELECT_IDBUTTON_LOAD:
                            if (lb_maplist->getSelected() == -1)
                            {
                                Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Ничего не выбрано");
                                return true;
                            }
                            if (Editor->loadMap( irr::core::stringc(lb_maplist->getListItem(lb_maplist->getSelected())).c_str(), true ) )
                            {
                                //Editor->hideMiniMap();
                                se.UserEvent.UserData2 = game::STATE_GAME;
                                return Device->getEventReceiver()->OnEvent(se);
                            }
                            Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Не удалось загрузить карту.");
                        break;

                    }
                }
            }

            return false;
        }//OnEvent

        virtual void activate()
        {
            tabcontrol->setVisible(true);
            but_back->setVisible(true);
            preview->setVisible(true);
            preview->setImage( 0 );

            //список карт
            irr::io::IFileSystem * fs = Device->getFileSystem();
            irr::io::path old_path = fs->getWorkingDirectory();
            fs->changeWorkingDirectoryTo("./res/maps");
            irr::io::IFileList * fl = fs->createFileList();

            irr::u16 map_cnt = fl->getFileCount();

            #ifdef _IRR_WINDOWS_
                irr::u8 def_dir = 2; // def dirs . & ..
            #else
                irr::u8 def_dir = 1; // def dirs only ..
            #endif

            if (map_cnt>def_dir)
            {
                lb_maplist->clear();
                for (irr::u32 i=def_dir; i < map_cnt; i++)
                {
                    irr::io::path fname = fl->getFileName(i);
                    if ( irr::core::hasFileExtension(fname, "xml") )
                        lb_maplist->addItem( irr::core::stringw (fname).c_str() );
                }
            }

            fs->changeWorkingDirectoryTo(old_path);
            fl->drop();
        }

        virtual void deactivate()
        {
            tabcontrol->setVisible(false);
            but_back->setVisible(false);
            preview->setVisible(false);
        }

    private:
        irr::gui::IGUITabControl* tabcontrol;

        irr::gui::IGUITab* tabload;
        irr::gui::IGUIStaticText* st_maplist;
        irr::gui::IGUIListBox* lb_maplist;
        irr::gui::IGUIButton* btn_load;
        irr::gui::IGUIImage* preview;
        irr::video::IImage* preview_img;

        irr::gui::IGUIButton* but_back;

        irr::io::path map_pth;
};

#endif // ST_LEVELSELECT_H_INCLUDED
