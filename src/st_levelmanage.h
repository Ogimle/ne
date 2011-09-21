#ifndef ST_LEVELMANAGE_H_INCLUDED
#define ST_LEVELMANAGE_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

#include "Game/editor.h";

// id gui элементов для обработчика событий
enum
{
        LEVELMANAGE_IDBUTTON_MAINMENU = 1,
        LEVELMANAGE_IDBUTTON_CREATE,
        LEVELMANAGE_IDBUTTON_LOAD,
        LEVELMANAGE_IDBUTTON_DELETE,
        LEVELMANAGE_IDDELETEDIALOG,
};

class ST_LevelManage : public CState
{
    public:
        ST_LevelManage(irr::IrrlichtDevice* device) : CState(device)
        {
            irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

            tabcontrol = guienv->addTabControl(irr::core::rect< irr::s32 >(10,10, 210, 310), 0, true, true);

                tabcreate = tabcontrol->addTab(L"Создать");

                    st_mapname = guienv->addStaticText(L"Имя файла (англ.яз.)", irr::core::rect< irr::s32 >(10,10,86,30), false, false, tabcreate, -1, false);
                    eb_mapname = guienv->addEditBox(L"", irr::core::rect< irr::s32 >(88,10,188,30), true, tabcreate);

                    st_worldtype = guienv->addStaticText(L"Ландшафт", irr::core::rect< irr::s32 >(10,40,86,60), false, false, tabcreate, -1, false);
                    cb_worldtype = guienv->addComboBox(irr::core::rect< irr::s32 >(88,40,188,60), tabcreate);
                        cb_worldtype->addItem(L"Земля", 0);
                        cb_worldtype->addItem(L"Космос", 1);
                        cb_worldtype->setSelected(0);

                    st_size = guienv->addStaticText(L"Размер", irr::core::rect< irr::s32 >(10,70,86,90), false, false, tabcreate, -1, false);
                    cb_size = guienv->addComboBox(irr::core::rect< irr::s32 >(88,70,188,90), tabcreate);
                        cb_size->addItem(L"8x256", 0);
                        cb_size->addItem(L"32x128", 1);
                        cb_size->addItem(L"64x64", 2);
                        cb_size->addItem(L"96x96", 3);
                        cb_size->setSelected(0);

                    btn_create = guienv->addButton( irr::core::rect<irr::s32>(88, 110, 188, 130),  tabcreate, LEVELMANAGE_IDBUTTON_CREATE, L"Создать");

                tabload = tabcontrol->addTab(L"Загрузить");
                    st_maplist = guienv->addStaticText(L"Список карт", irr::core::rect< irr::s32 >(10,10,150,30), false, false, tabload, -1, false);
                    lb_maplist = guienv->addListBox( irr::core::rect<irr::s32>(10, 30, 190, 235), tabload, -1, false);

                    btn_load = guienv->addButton( irr::core::rect<irr::s32>(10, 240, 93, 265),  tabload, LEVELMANAGE_IDBUTTON_LOAD, L"Загрузить");
                    btn_del =  guienv->addButton( irr::core::rect<irr::s32>(100, 240, 168, 265),  tabload, LEVELMANAGE_IDBUTTON_DELETE, L"Удалить");

                    preview = guienv->addImage( irr::core::rect<irr::s32>(250, 100, 250+512, 100+384) );
                    preview_img = Device->getVideoDriver()->createImage(irr::video::ECF_R8G8B8, irr::core::dimension2d<u32>(512,384));


            but_back = guienv->addButton( irr::core::rect<irr::s32>(30, 320, 150, 350),  0, LEVELMANAGE_IDBUTTON_MAINMENU, L"В главное меню");

            // по умолчанию самодеактивируемся
            deactivate();
        }

        ~ST_LevelManage()
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

                if (event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_OK && id == LEVELMANAGE_IDDELETEDIALOG)
                {
                    if (Editor->deleteMap( irr::core::stringc(lb_maplist->getListItem(lb_maplist->getSelected())).c_str() ))
                        lb_maplist->removeItem( lb_maplist->getSelected() );
                    else
                        Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Не удалось удалить карту.");
                    return true;
                }
                else if ( event.GUIEvent.EventType == irr::gui::EGET_LISTBOX_CHANGED || event.GUIEvent.EventType == irr::gui::EGET_LISTBOX_SELECTED_AGAIN)
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
                        case LEVELMANAGE_IDBUTTON_MAINMENU:
                            se.UserEvent.UserData2 = game::STATE_MAINMENU;
                            return Device->getEventReceiver()->OnEvent(se);
                        break;

                        case LEVELMANAGE_IDBUTTON_CREATE:
                            map_pth = irr::io::path("./res/maps/");
                            map_pth.append( eb_mapname->getText() );
                            if ( irr::core::stringw(eb_mapname->getText()).size()==0)
                            {
                                Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Укажите имя карты! (прим. map.xml)");
                            }
                            else if ( Device->getFileSystem()->existFile( map_pth ) )
                            {
                                Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Карта с таким именем существует, перейдите в раздел редактирование.");
                            }
                            else if (Editor->createMap(irr::core::stringc(eb_mapname->getText()).c_str(), cb_size->getSelected(), cb_worldtype->getSelected()))
                            {
                                se.UserEvent.UserData2 = game::STATE_EDITOR;
                                return Device->getEventReceiver()->OnEvent(se);
                            }
                            else
                            {
                                Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Не удалось создать карту.");
                            }
                        break;

                        case LEVELMANAGE_IDBUTTON_LOAD:
                            if (lb_maplist->getSelected() == -1)
                            {
                                Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Ничего не выбрано");
                                return true;
                            }
                            if (Editor->loadMap( irr::core::stringc(lb_maplist->getListItem(lb_maplist->getSelected())).c_str() ) )
{
                                se.UserEvent.UserData2 = game::STATE_EDITOR;
                                return Device->getEventReceiver()->OnEvent(se);
                            }
                            Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Не удалось загрузить карту.");
                        break;

                        case LEVELMANAGE_IDBUTTON_DELETE:
                            if (lb_maplist->getSelected() == -1)
                            {
                                Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Ничего не выбрано");
                                return true;
                            }
                            Device->getGUIEnvironment()->addMessageBox(L"Внимание!", L"Продолжить удаление?", false, irr::gui::EMBF_OK | irr::gui::EMBF_CANCEL, 0, LEVELMANAGE_IDDELETEDIALOG);
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


        irr::gui::IGUITab* tabcreate;
        irr::gui::IGUIStaticText* st_mapname;
        irr::gui::IGUIEditBox* eb_mapname;
        irr::gui::IGUIStaticText* st_worldtype;
        irr::gui::IGUIComboBox* cb_worldtype;
        irr::gui::IGUIStaticText* st_size;
        irr::gui::IGUIComboBox* cb_size;
        irr::gui::IGUIButton* btn_create;

        irr::gui::IGUITab* tabload;
        irr::gui::IGUIStaticText* st_maplist;
        irr::gui::IGUIListBox* lb_maplist;
        irr::gui::IGUIButton* btn_load;
        irr::gui::IGUIButton* btn_del;
        irr::gui::IGUIImage* preview;
        irr::video::IImage* preview_img;

        irr::gui::IGUIButton* but_back;

        irr::io::path map_pth;
};

#endif // ST_LEVELMANAGE_H_INCLUDED
