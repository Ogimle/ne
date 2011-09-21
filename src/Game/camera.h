#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <irrlicht.h>

class C_Camera
{
    public:
        irr::scene::ICameraSceneNode* csn;

        C_Camera(irr::IrrlichtDevice* device);

        virtual bool OnEvent(const irr::SEvent& event);

        void camSet(int mw, int mh);
        void camUnSet();
        void camReset();
        void camUpdate();
        void camZoom(float z);
        void camRotate(float a);
        void camElevate(float a);
        void camShift(int sx, int sy);

        void setRadius(float r);
        void setTheta(float t);
        void setPhi(float p);

        float getRadius() { return Radius; }
        float getTheta() { return Theta; }
        float getPhi() { return Phi; }

    private:
        irr::IrrlichtDevice* Device;

        float maxZoom;
        float minZoom;

        float maxPitch;
        float minPitch;

        float Radius;
        float Theta; // degrees
        float Phi; // degrees

        float LinearVelocity;
        float AngularVelocity;

        // event recivers var
        int omx;
        int omy;

};//C_Camera

#endif // CAMERA_H_INCLUDED
