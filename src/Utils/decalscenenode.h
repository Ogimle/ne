#include <irrlicht.h>
#include <time.h>

class DecalSceneNode : public irr::scene::ISceneNode
{
private:
    irr::scene::ISceneNode* _Parent;

    irr::core::aabbox3d<irr::f32> Box;
    irr::core::array< irr::video::S3DVertex > Vertices;
    irr::core::array< irr::u16 > Indices;
    irr::video::SMaterial Material;
    irr::core::vector3df Scale;
    irr::core::vector3df Pos;
    irr::core::vector3df Rot;

    irr::u32 LifeTime;
    irr::s32 Speed;
    irr::u16 Count;
    irr::u16 MaxCount;
    irr::u32 LastAdd;

    void add(irr::core::vector3df pos, irr::core::vector3df rot);
    void hide();

    void prnvert(irr::u16 i);

public:
    DecalSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, irr::video::ITexture* image, irr::core::vector3df scale = irr::core::vector3df(1,1,1), irr::u32 lifetime=5000, irr::u32 speed=500, irr::u32 maxcount=50);

    void unlink() { _Parent = SceneManager->getRootSceneNode(); Speed = -Speed; Count--; }

    void setLifeTime(irr::u32 seconds) { LifeTime = seconds; }
    void setSpeed(irr::u32 seconds) { Speed = seconds; }

    virtual void OnAnimate( irr::u32 timeMs );
    virtual void OnRegisterSceneNode();
    void render();

    const irr::core::aabbox3d<irr::f32>& getBoundingBox() const { return Box; }
};
