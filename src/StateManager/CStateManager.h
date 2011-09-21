#ifndef CSTATEMANAGER_H
#define CSTATEMANAGER_H

//#include <irrlicht.h>

#include "sm_conf.h"
#include "CState.h"

typedef irr::core::map< game::STATES , CState* > t_States;
typedef irr::core::map< game::STATES , CState* >::Iterator t_States_Iterator;

class CStateManager : public irr::IEventReceiver
{
    public:
        CStateManager(irr::IrrlichtDevice* device)
        {
            Device = device;
            MainState = NULL;
            NextState = game::STATE_NONE;
            LastTime = 0;
        }

        ~CStateManager()
        {
            // удаляем зарегистрированные состояния
            for (t_States_Iterator it = States.getIterator(); !it.atEnd(); it++) delete it->getValue();
            States.clear();
        }

        // обработчик событий
        virtual bool OnEvent(const irr::SEvent& event)
        {
            // если событие пользовательское и запрошено переключение состояния
            if ( event.EventType == irr::EET_USER_EVENT)
            {
                // переключаемся на запрошенное состояние
                // и выходим с прерыванием дальнейшей обработки событий
                if (event.UserEvent.UserData1 == game::EVENT_SWITCH_STATE)
                {
                    switchState( (game::STATES)event.UserEvent.UserData2 );
                    return true;
                }

                // переключаемся на состояние которое указанно как следующее
                // и выходим с прерыванием дальнейшей обработки событий
                if (event.UserEvent.UserData1 == game::EVENT_SWITCH_NEXTSTATE)
                {
                    switchState( NextState );
                    return true;
                }
            }

            //не обрабатываем события если курсор за пределами вьювпорта
            if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
            {
                irr::core::dimension2d<irr::s32> viewArea;
                viewArea.Width  = Device->getVideoDriver()->getViewPort().getWidth();
                viewArea.Height = Device->getVideoDriver()->getViewPort().getHeight();
                if ( event.MouseInput.X < 0 || event.MouseInput.X > viewArea.Width ) return true;
                if ( event.MouseInput.Y < 0 || event.MouseInput.Y > viewArea.Height ) return true;
            }            // иначе вызываем обработчик текущего состояния MainState

            // иначе вызываем обработчик текущего состояния MainState
            if (MainState) return MainState->OnEvent(event);
            return false;
        }

        // регистрация состояния в массиве состояний States
        void registerState(game::STATES n, CState* s)
        {
            // если такое состояние уже есть, то грохаем его предварительно
            // в принципе можно и не проверять, просто внимательнее регистрировать
            if (States.find(n))
            {
                delete States[n];
                States.remove(n);
            }
            States[n] = s; // добавляем состояние
        }//registerState

        // переключаем состояние
        void switchState(game::STATES n)
        {
            #ifdef _DEBUG
            Device->getLogger()->log("StateManager: switch to state ", state2str(n) );
            #endif
            if (MainState) MainState->deactivate(); // если текущее состояние установлено - деактивируем его

            if (n == game::STATE_NONE) // запрошен сброc текущего состояния, т.е. у меня равносильно EXIT
            {
                MainState = NULL;
            }
            else //иначе записываем в текущее состояние запрошенное из пула и активируем его
            {
                MainState = States[n];
                MainState->activate();
            }
        }//switchState

        // устанавливает следующее за текущим состояние
        // например делаем загрузчик основным состоянием, а главное меню следующим
        // отработав загрузчик сгенерит game::EVENT_SWITCH_NEXTSTATE не "задумываясь" на что надо переключатся
        void setNextState(game::STATES n)
        {
            NextState = n;
        }

        // вызывается из главного цикла каждый фрейм
        // если существует текущее состояние, то вызывает его update и возвращает true
        // инача возвращает false и главный цикл завершает свою работу - выход из приложения
        bool update()
        {
            if (!MainState) return false;

            irr::u32 TimeDiff = Device->getTimer()->getTime() - LastTime;
            MainState->update(TimeDiff/1000.f);
            LastTime = Device->getTimer()->getTime();

            return true;
        }//update

    protected:
        t_States States; // пул состояний

        CState* MainState; // главное состояние
        game::STATES NextState; // следующее за главным состояние

    private:
        irr::IrrlichtDevice* Device; // обратная связь с движком
        irr::u32 LastTime; // для подсчета дельты

};

#endif // CSTATEMANAGER_H
