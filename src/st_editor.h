#ifndef ST_EDITOR_H_INCLUDED
#define ST_EDITOR_H_INCLUDED

#include "StateManager/sm_conf.h"
#include "StateManager/CState.h"

#include "Game/editor.h";
#include "Game/conf_game.h";

// id gui элементов для обработчика событий
enum
{
        EDITOR_IDBUTTON_EXIT = 1,
        EDITOR_IDBUTTON_SAVE,
        EDITOR_EXITWITHOUTSAVE,

        EDITOR_IDBUTTON_BRUSH_0,
        EDITOR_IDBUTTON_BRUSH_1,
        EDITOR_IDBUTTON_BRUSH_2,
        EDITOR_IDBUTTON_BRUSH_3,
        EDITOR_IDBUTTON_BRUSH_4,

        EDITOR_IDBUTTON_DEL,

        EDITOR_IDBUTTON_B_BASE,
        EDITOR_IDBUTTON_B_CHASSIS,
        EDITOR_IDBUTTON_B_ELECTRONICS,
        EDITOR_IDBUTTON_B_NUCLEAR,
        EDITOR_IDBUTTON_B_PHASERS,
        EDITOR_IDBUTTON_B_CANNON,
        EDITOR_IDBUTTON_B_MISSILES,
        EDITOR_IDBUTTON_B_WALL_1,
        EDITOR_IDBUTTON_B_WALL_2,
        EDITOR_IDBUTTON_B_WALL_3,
        EDITOR_IDBUTTON_B_WALL_4,
        EDITOR_IDBUTTON_B_TREE_1,

};

class ST_Editor : public CState
{
    public:
        ST_Editor(irr::IrrlichtDevice* device) : CState(device)
        {
            irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

            toolinfo = guienv->addStaticText(L"", irr::core::rect< s32 >(10,35,150,55), true, false );

            tbar = guienv->addToolBar();
            irr::u16 sx=10, wdh=32;
            btn_brush_0 = tbar->addButton(EDITOR_IDBUTTON_BRUSH_0, L"Sel.");
            btn_brush_0->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_brush_0->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5; wdh=55;
            btn_brush_1 = tbar->addButton(EDITOR_IDBUTTON_BRUSH_1, L"Земля");
            btn_brush_1->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_brush_1->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_brush_2 = tbar->addButton(EDITOR_IDBUTTON_BRUSH_2, L"Вода");
            btn_brush_2->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_brush_2->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_brush_3 = tbar->addButton(EDITOR_IDBUTTON_BRUSH_3, L"Камни");
            btn_brush_3->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_brush_3->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_brush_4 = tbar->addButton(EDITOR_IDBUTTON_BRUSH_4, L"Трава");
            btn_brush_4->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_brush_4->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5; wdh=65;
            btn_del = tbar->addButton(EDITOR_IDBUTTON_DEL, L"Удалить");
            btn_del->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_del->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5; wdh=30;
            btn_bld_base = tbar->addButton(EDITOR_IDBUTTON_B_BASE, L"Ba");
            btn_bld_base->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_base->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_chassis = tbar->addButton(EDITOR_IDBUTTON_B_CHASSIS, L"Za");//Ch
            btn_bld_chassis->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_chassis->setRelativePosition( irr::core::position2di(sx,2) );
/*
            sx+=wdh+5;
            btn_bld_electronics = tbar->addButton(EDITOR_IDBUTTON_B_ELECTRONICS, L"El");
            btn_bld_electronics->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_electronics->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_nuclear = tbar->addButton(EDITOR_IDBUTTON_B_NUCLEAR, L"Nu");
            btn_bld_nuclear->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_nuclear->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_phasers = tbar->addButton(EDITOR_IDBUTTON_B_PHASERS, L"Ph");
            btn_bld_phasers->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_phasers->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_cannon = tbar->addButton(EDITOR_IDBUTTON_B_CANNON, L"Ca");
            btn_bld_cannon->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_cannon->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_missiles = tbar->addButton(EDITOR_IDBUTTON_B_MISSILES, L"Mi");
            btn_bld_missiles->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_missiles->setRelativePosition( irr::core::position2di(sx,2) );
*/
//----
            sx+=wdh+5;
            btn_bld_chassis = tbar->addButton(EDITOR_IDBUTTON_B_TREE_1, L"T1");//Ch
            btn_bld_chassis->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_chassis->setRelativePosition( irr::core::position2di(sx,2) );
//----
            sx+=wdh+5;
            btn_bld_wall_1 = tbar->addButton(EDITOR_IDBUTTON_B_WALL_1, L"W1");
            btn_bld_wall_1->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_wall_1->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_wall_2 = tbar->addButton(EDITOR_IDBUTTON_B_WALL_2, L"W2");
            btn_bld_wall_2->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_wall_2->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_wall_3 = tbar->addButton(EDITOR_IDBUTTON_B_WALL_3, L"W3");
            btn_bld_wall_3->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_wall_3->setRelativePosition( irr::core::position2di(sx,2) );

            sx+=wdh+5;
            btn_bld_wall_4 = tbar->addButton(EDITOR_IDBUTTON_B_WALL_4, L"W4");
            btn_bld_wall_4->setMinSize( irr::core::dimension2du(wdh,25) );
            btn_bld_wall_4->setRelativePosition( irr::core::position2di(sx,2) );

            btn_save = tbar->addButton(EDITOR_IDBUTTON_SAVE, L"Сохр.");
            btn_save->setMinSize( irr::core::dimension2du(55,25) );
            btn_save->setRelativePosition( irr::core::position2di(1024-120,2) );

            btn_exit = tbar->addButton(EDITOR_IDBUTTON_EXIT, L"Выход");
            btn_exit->setMinSize( irr::core::dimension2du(55,25) );
            btn_exit->setRelativePosition( irr::core::position2di(1024-60,2) );

            current_brush = -1;
            current_tile = xz_key(-1,-1);
            cursor = NULL;

            arrow = Device->getSceneManager()->addAnimatedMeshSceneNode(
                Device->getSceneManager()->addArrowMesh("arrow", irr::video::SColor(255, 255, 0, 0), irr::video::SColor(255, 0, 255, 0)), NULL);
            arrow->setMaterialFlag(video::EMF_LIGHTING, false);
            arrow->setScale(irr::core::vector3df(1,2,1));
            arrow->setRotation(irr::core::vector3df(0,0,180));

            // по умолчанию самодеактивируемся
            deactivate();
        }

        ~ST_Editor()
        {
            //dtor
        }

        // обработчик событий
        virtual bool OnEvent(const irr::SEvent& event)
        {
            if ( event.EventType == irr::EET_GUI_EVENT )
            {
                irr::s32 id = event.GUIEvent.Caller->getID();

                if (event.GUIEvent.EventType == irr::gui::EGET_MESSAGEBOX_OK && id == EDITOR_EXITWITHOUTSAVE)
                {
                    // генерим событие для передачи основному обработчику (менеджеру состояний)
                    irr::SEvent se;
                    se.EventType = irr::EET_USER_EVENT;
                    se.UserEvent.UserData1 = game::EVENT_SWITCH_STATE; // тип события - переключить состояние
                    se.UserEvent.UserData2 = game::STATE_LEVELMANAGE;
                    return Device->getEventReceiver()->OnEvent(se);
                }

                if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
                {
                    switch(id)
                    {
                        case EDITOR_IDBUTTON_BRUSH_0:
                            current_brush = -1;
                            toolinfo->setText(L" Выбор объекта");
                            unsetCursor();
                        break;

                        case EDITOR_IDBUTTON_BRUSH_1:
                            current_brush = 0;
                            isEarth = true;
                            toolinfo->setText(L" Земля");
                            unsetCursor();
                        break;

                        case EDITOR_IDBUTTON_BRUSH_2:
                            current_brush = 0;
                            isEarth = false;
                            toolinfo->setText(L" Вода");
                            unsetCursor();
                        break;

                        case EDITOR_IDBUTTON_BRUSH_3:
                            current_brush = 1;
                            isEarth = false;
                            toolinfo->setText(L" Камни");
                            unsetCursor();
                        break;

                        case EDITOR_IDBUTTON_BRUSH_4:
                            current_brush = 2;
                            isEarth = false;
                            toolinfo->setText(L" Трава");
                            unsetCursor();
                        break;

                        case EDITOR_IDBUTTON_DEL:
                            current_brush = 3;
                            toolinfo->setText(L" Удаление");
                            unsetCursor();
                        break;

                        case EDITOR_IDBUTTON_B_BASE:
                            current_brush = 4;
                            toolinfo->setText(L" База");
                            setCursor("./res/mdl/cursor_base.x", "./res/tex/base.png");
                        break;

                        case EDITOR_IDBUTTON_B_CHASSIS:
                            current_brush = 5;
                            toolinfo->setText(L" Завод шасси");
                            setCursor("./res/mdl/cursor_plant.x", "./res/tex/plant_chassis.png");
                        break;

                        case EDITOR_IDBUTTON_B_ELECTRONICS:
                            current_brush = 6;
                            toolinfo->setText(L" Завод электроники");
                            setCursor("./res/mdl/cursor_plant.x", "./res/tex/plant_chassis.png");
                        break;

                        case EDITOR_IDBUTTON_B_NUCLEAR:
                            current_brush = 7;
                            toolinfo->setText(L" Атомный завод");
                            setCursor("./res/mdl/cursor_plant.x", "./res/tex/plant_chassis.png");
                        break;

                        case EDITOR_IDBUTTON_B_PHASERS:
                            current_brush = 8;
                            toolinfo->setText(L" Завод фазеров");
                            setCursor("./res/mdl/cursor_plant.x", "./res/tex/plant_chassis.png");
                        break;

                        case EDITOR_IDBUTTON_B_CANNON:
                            current_brush = 9;
                            toolinfo->setText(L" Орудийный завод");
                            setCursor("./res/mdl/cursor_plant.x", "./res/tex/plant_chassis.png");
                        break;

                        case EDITOR_IDBUTTON_B_MISSILES:
                            current_brush = 10;
                            isEarth = false;
                            toolinfo->setText(L" Ракетный завод");
                            setCursor("./res/mdl/cursor_plant.x", "./res/tex/plant_chassis.png");
                        break;

                        case EDITOR_IDBUTTON_B_WALL_1:
                            current_brush = 11;
                            isEarth = false;
                            toolinfo->setText(L" Стена #1");
                            setCursor("./res/mdl/cursor_wall_1.x", "./res/tex/wall.png");
                        break;

                        case EDITOR_IDBUTTON_B_WALL_2:
                            current_brush = 12;
                            isEarth = false;
                            toolinfo->setText(L" Стена #2");
                            setCursor("./res/mdl/cursor_wall_1.x", "./res/tex/wall.png");
                        break;

                        case EDITOR_IDBUTTON_B_WALL_3:
                            current_brush = 13;
                            isEarth = false;
                            toolinfo->setText(L" Стена #3");
                            setCursor("./res/mdl/cursor_wall_2.x", "./res/tex/wall.png");
                        break;

                        case EDITOR_IDBUTTON_B_WALL_4:
                            current_brush = 14;
                            isEarth = false;
                            toolinfo->setText(L" Стена #4");
                            setCursor("./res/mdl/cursor_wall_2.x", "./res/tex/wall.png");
                        break;

                        case EDITOR_IDBUTTON_B_TREE_1:
                            current_brush = 15;
                            isEarth = false;
                            toolinfo->setText(L" Дерево #1");
                            setCursor("./res/mdl/tree_1.x", "./res/tex/blackwoo.png");
                        break;

                        case EDITOR_IDBUTTON_EXIT:
                            Device->getGUIEnvironment()->addMessageBox(L"Внимание!", L"Выйти без сохранения?", true, irr::gui::EMBF_OK | irr::gui::EMBF_CANCEL, 0, EDITOR_EXITWITHOUTSAVE);
                        break;

                        case EDITOR_IDBUTTON_SAVE:
                            hideGUI();
                            Editor->saveMap();
                            Editor->makeScreenShot();
                            showGUI();
                        break;

                    }
                }
            }

            irr::gui::IGUIElement * tst_id = Device->getGUIEnvironment()->getRootGUIElement()->getElementFromPoint( irr::core::vector2di(event.MouseInput.X, event.MouseInput.Y) );
            if ( !tst_id || (tst_id && 0 == tst_id->getID()) )
            {
                if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
                {
                    switch(event.MouseInput.Event)
                    {
                        case irr::EMIE_RMOUSE_LEFT_UP:
                            if (current_brush>=3 && !event.MouseInput.Shift)
                            {
                                current_brush = -1;
                                toolinfo->setText(L" Выбор объекта");
                                unsetCursor();
                            }
                        break;

                        case irr::EMIE_LMOUSE_LEFT_UP:
                            //printf("================== Draw Session End %d ms=======================\n", drawses);
                           if (cursor)
                           {
                               bool isBuildOK = false;
                               if      (current_brush==4) isBuildOK = Editor->setBase(cursor);
                               else if (current_brush==5) isBuildOK = Editor->setPlant(AB_CHASSIS, cursor);
                               else if (current_brush==6) isBuildOK = Editor->setPlant(AB_ELECTRONICS, cursor);
                               else if (current_brush==7) isBuildOK = Editor->setPlant(AB_NUCLEARS, cursor);
                               else if (current_brush==8) isBuildOK = Editor->setPlant(AB_PHASERS, cursor);
                               else if (current_brush==9) isBuildOK = Editor->setPlant(AB_CANNON, cursor);
                               else if (current_brush==10) isBuildOK = Editor->setPlant(AB_MISSILES, cursor);
                               else if (current_brush==11) isBuildOK = Editor->setDecor(SB_WALL_1, cursor);
                               else if (current_brush==12) isBuildOK = Editor->setDecor(SB_WALL_2, cursor);
                               else if (current_brush==13) isBuildOK = Editor->setDecor(SB_WALL_3, cursor);
                               else if (current_brush==14) isBuildOK = Editor->setDecor(SB_WALL_4, cursor);
                               else if (current_brush==15) isBuildOK = Editor->setDecor(SB_TREE_1, cursor);

                               if (!isBuildOK) Device->getGUIEnvironment()->addMessageBox(L"Ошибка", L"Площадка для строительства занята, расчистите предварительно область.");;
                           }
                           current_tile = xz_key(-1,-1);
                        break;

                        case irr::EMIE_LMOUSE_PRESSED_DOWN:
                            if (Editor->testMapClick( event.MouseInput.X, event.MouseInput.Y ))
                            {
                                xz_key tst(-Editor->posMapClick.X, Editor->posMapClick.Z);
                                if (tst != current_tile && current_brush>-1 && current_brush<3)
                                {
                                    Editor->set2dTile(tst, current_brush, isEarth);
                                    current_tile = tst;
                                }
                                else if (current_brush==3)
                                {
                                    if (!Editor->deleteDynObj(event.MouseInput.X, event.MouseInput.Y)) Editor->deleteStatObj();
                                }
                            }
                        break;

                        case irr::EMIE_MOUSE_MOVED:
                            Editor->testMapClick( event.MouseInput.X, event.MouseInput.Y );
                            if ( event.MouseInput.isLeftPressed() )
                            {
                                xz_key tst(-Editor->posMapClick.X, Editor->posMapClick.Z);
                                if (tst != current_tile)
                                {
                                    if ( current_brush>-1 && current_brush<3 )
                                    {
                                        Editor->set2dTile(tst, current_brush, isEarth);
                                    }
                                    /*
                                    else if (current_brush==11 )
                                    {
                                        Editor->setWall("", cursor);
                                    }
                                    */
                                    current_tile = tst;
                                }
                            }
                            setCursorPos( Editor->posMapClick );
                        break;

                        case irr::EMIE_MOUSE_WHEEL:
                            if (current_brush>=4 && current_brush<=10)
                            {
                                cursor->setRotation( cursor->getRotation() + irr::core::vector3df(0, event.MouseInput.Wheel * 90 ,0) );
                            }
                            else if (current_brush==11)
                            {
                                irr::core::vector3df rot = cursor->getRotation() + irr::core::vector3df(0, event.MouseInput.Wheel * 90 ,0);
                                if (rot.Y == 360 || rot.Y == -360) rot.Y = 0;
                                cursor->setRotation(rot);
                            }
                        break;
                    }

                    //if (current_brush>=-2 && current_brush<3)
                    Editor->camera.OnEvent( event );
                }
            }

            return false;
        }//OnEvent

        virtual void activate()
        {
            showGUI();
            Editor->fillMiniMap();
        }

        virtual void deactivate()
        {
            hideGUI();
            unsetCursor();
            Editor->clearScene();
        }

    private:
        irr::gui::IGUIStaticText* toolinfo;

        irr::gui::IGUIToolBar* tbar;
        irr::gui::IGUIButton* btn_brush_0;
        irr::gui::IGUIButton* btn_brush_1;
        irr::gui::IGUIButton* btn_brush_2;
        irr::gui::IGUIButton* btn_brush_3;
        irr::gui::IGUIButton* btn_brush_4;

        irr::gui::IGUIButton* btn_del;

        irr::gui::IGUIButton* btn_bld_base;
        irr::gui::IGUIButton* btn_bld_chassis;
        irr::gui::IGUIButton* btn_bld_electronics;
        irr::gui::IGUIButton* btn_bld_nuclear;
        irr::gui::IGUIButton* btn_bld_phasers;
        irr::gui::IGUIButton* btn_bld_cannon;
        irr::gui::IGUIButton* btn_bld_missiles;
        irr::gui::IGUIButton* btn_bld_wall_1;
        irr::gui::IGUIButton* btn_bld_wall_2;
        irr::gui::IGUIButton* btn_bld_wall_3;
        irr::gui::IGUIButton* btn_bld_wall_4;

        irr::gui::IGUIButton* btn_save;
        irr::gui::IGUIButton* btn_exit;

        irr::scene::IAnimatedMeshSceneNode * cursor;
        irr::scene::IAnimatedMeshSceneNode * arrow;

        xz_key current_tile;
        xz_key tmp_tile;
        irr::s8 current_brush;
        bool isEarth;

        void setCursorPos( irr::core::vector3df pos )
        {
            if (cursor) cursor->setPosition(pos);
            arrow->setPosition(pos+irr::core::vector3df(0,2,0));
        }//setCursorPos

        void setCursor(const irr::c8* name, const irr::c8* tex)
        {
            irr::scene::IAnimatedMesh* msh = Device->getSceneManager()->getMesh(name);
            irr::video::ITexture* Tex = Device->getVideoDriver()->getTexture(tex);
            if (msh && tex)
            {
                if (cursor) cursor->remove();
                cursor = Device->getSceneManager()->addAnimatedMeshSceneNode(msh);
                cursor->setMaterialTexture(0, Tex);
                cursor->setMaterialFlag(irr::video::EMF_LIGHTING, false);
                //cursor->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
                cursor->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
            }
            else Device->getLogger()->log("st_editor::setCursor", "mesh or tex not found.");
        }

        void showGUI()
        {
            tbar->setVisible(true);
            toolinfo->setVisible(true);
            arrow->setVisible(true);
        }

        void hideGUI()
        {
            tbar->setVisible(false);
            toolinfo->setVisible(false);
            arrow->setVisible(false);
        }

        void unsetCursor()
        {
            if (cursor)
            {
                cursor->remove();
                cursor = NULL;
            }
        }//cl_gui_cursor_clear
};

#endif // ST_EDITOR_H_INCLUDED
