#include "camera.h"

C_Camera::C_Camera(irr::IrrlichtDevice* device)
{
    Device = device;
    csn = NULL;

    IsCamShift=true;
    IsCamRotate=true;
    IsCamElevate=true;
    IsCamZoom=true;

    maxZoom = 64.f;
    minZoom = 5.f;

    maxPitch = 65.f;
    minPitch = 10.f;

    Radius = 20.f;

    Theta  = 0.f; // degrees
    Phi    = 45.f; // degrees

    LinearVelocity = 1.f;
    AngularVelocity = 1.f;

    camSet(0,0);

    omx = 0;
    omy = 0;
}//C_Camera

bool C_Camera::OnEvent(const irr::SEvent& event)
{

    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        switch (event.KeyInput.Key)
        {
            case irr::KEY_NUMPAD5:
                camReset();
            break;

            case irr::KEY_NUMPAD8:
                if (IsCamElevate) camElevate(1.f);
            break;

            case irr::KEY_NUMPAD2:
                if (IsCamElevate) camElevate(-1.f);
            break;

            case irr::KEY_NUMPAD4:
                if (IsCamRotate) camRotate(1.f);
            break;

            case irr::KEY_NUMPAD6:
                if (IsCamRotate) camRotate(-1.f);
            break;
        }
    }

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        switch(event.MouseInput.Event)
        {
            //колесо
            case irr::EMIE_MOUSE_WHEEL:
                if (IsCamZoom) camZoom(event.MouseInput.Wheel);
            break;

            //движение
            case irr::EMIE_MOUSE_MOVED:
                if (IsCamShift && event.MouseInput.isRightPressed() && !event.MouseInput.isMiddlePressed() && !event.MouseInput.isLeftPressed()) //прижата только правая
                {
                    camShift(event.MouseInput.X-omx, event.MouseInput.Y-omy);
                }
                else if (IsCamElevate && IsCamRotate && event.MouseInput.isMiddlePressed() && !event.MouseInput.isRightPressed() && !event.MouseInput.isLeftPressed()) //прижата только средняя
                {
                    camRotate( (event.MouseInput.X-omx) );
                    camElevate( (event.MouseInput.Y-omy) );
                }
                omx = event.MouseInput.X;
                omy = event.MouseInput.Y;
            break;
        }
    }

    return false;
}

void C_Camera::camSet(int mw, int mh)
{
    if ( !csn)
    {
        csn = Device->getSceneManager()->addCameraSceneNode(0, irr::core::vector3df(-mw/2, 16 , mh+16), irr::core::vector3df(-mw/2, 0, mh/2));
        csn->setFOV(95);
        csn->setFarValue(128);
    }
    csn->setTarget( irr::core::vector3df(-mw/2, 0, mh/2) );

    camUpdate();
}

void C_Camera::camUnSet()
{
    if (csn)
    {
        csn->remove();
        csn = NULL;
    }
}

void C_Camera::setRadius(float r)
{
    Radius = r;
    if (Radius < minZoom) { Radius = minZoom; return; }
    if (Radius > maxZoom) { Radius = maxZoom; return; }
}

void C_Camera::setTheta(float t)
{
    Theta = t;
}

void C_Camera::setPhi(float p)
{
    Phi = p;
    if (Phi < minPitch) { Phi = minPitch; return; }
    if (Phi > maxPitch) { Phi = maxPitch; return; }
}

void C_Camera::camUpdate()
{
     float sinOfPhi = sinf(Phi * irr::core::DEGTORAD);
     float cosOfPhi = cosf(Phi * irr::core::DEGTORAD);

     float sinOfTheta = sinf(Theta * irr::core::DEGTORAD);
     float cosOfTheta = cosf(Theta * irr::core::DEGTORAD);

     irr::core::vector3df
        tar = csn->getTarget(),
        pos = csn->getPosition(),
        offset;

     // these are switched around a little from what i posted above
     offset.X = Radius * sinOfTheta * sinOfPhi;
     offset.Y = Radius * cosOfPhi;
     offset.Z = Radius * cosOfTheta * sinOfPhi;

     csn->setPosition(tar+offset);
     csn->setTarget( tar );
     csn->updateAbsolutePosition();
}//camUpdate

void C_Camera::camReset()
{
    Theta  = 0.f; // degrees
    Phi    = 45.f; // degrees
    camUpdate();
}

void C_Camera::camZoom(float z)
{
    Radius += z;
    if (Radius < minZoom) { Radius = minZoom; return; }
    if (Radius > maxZoom) { Radius = maxZoom; return; }
    camUpdate();
}//camZoom

void C_Camera::camRotate(float a)
{
    Theta += a*AngularVelocity;
    camUpdate();
}//camRotate

void C_Camera::camElevate(float a)
{
    Phi += a*AngularVelocity;
    if (Phi < minPitch) { Phi = minPitch; return; }
    if (Phi > maxPitch) { Phi = maxPitch; return; }
    camUpdate();
}//camRotate

void C_Camera::camShift(int sx, int sy)
{
    irr::core::vector3df
        cam_target = csn->getTarget(),
        cam_position = csn->getPosition(),
        cam_z_dir = cam_position - cam_target,
        cam_x_dir = cam_position - cam_target, tmp;

    //z shift
    cam_z_dir.Y = 0;
    cam_z_dir.normalize();

    cam_position -= cam_z_dir * sy * 0.1;
    cam_target -= cam_z_dir * sy * 0.1;

    //x shift
    cam_x_dir.normalize();
    tmp = cam_x_dir.crossProduct(irr::core::vector3df(0,1,0));
    cam_x_dir = tmp.normalize();

    cam_position -= cam_x_dir * sx * 0.1;
    cam_target -= cam_x_dir * sx * 0.1;

    //setup
    csn->setPosition(cam_position);
    csn->setTarget(cam_target);
}
