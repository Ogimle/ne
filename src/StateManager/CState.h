#ifndef CSTATE_H
#define CSTATE_H

#include <irrlicht.h>

class CState : public irr::IEventReceiver
{
    public:
        CState(irr::IrrlichtDevice* device)
        {
            Device = device;
        }

        virtual void update(irr::f32 timediff){}
        virtual void activate(){}
        virtual void deactivate(){}

    protected:
        irr::IrrlichtDevice* Device;
};

#endif // CSTATE_H
