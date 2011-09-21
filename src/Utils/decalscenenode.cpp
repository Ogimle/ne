#include "decalscenenode.h"

DecalSceneNode::DecalSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, irr::video::ITexture* image, irr::core::vector3df scale, irr::u32 lifetime, irr::u32 speed, irr::u32 maxcount)
: ISceneNode(smgr->getRootSceneNode(), smgr, 0)
{
    _Parent = parent;

    Pos = irr::core::vector3df();
    Rot = irr::core::vector3df();
    Scale = scale;
    LifeTime = lifetime;
    Speed = speed;
    Count = 1;
    MaxCount = maxcount;
    LastAdd = 0;

    Material.setTexture(0, image);

    Material.Lighting = false;
    Material.BackfaceCulling = false;
    Material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
    /*
    Material.setFlag(irr::video::EMF_LIGHTING, false);
    Material.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
    Material.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    Material.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
    Material.MaterialTypeParam = irr::video::pack_texureBlendFunc (
        irr::video::EBF_SRC_ALPHA,
        irr::video::EBF_ONE_MINUS_SRC_ALPHA,
        irr::video::EMFN_MODULATE_1X,
        irr::video::EAS_VERTEX_COLOR | irr::video::EAS_TEXTURE );
    */

    Vertices.setAllocStrategy(irr::core::ALLOC_STRATEGY_SAFE);
    Indices.setAllocStrategy(irr::core::ALLOC_STRATEGY_SAFE);
}

void DecalSceneNode::add(irr::core::vector3df pos, irr::core::vector3df rot)
{
    irr::video::SColor poly_col(255,255,255,255);

    if (Vertices.size()<MaxCount*4)
    {
        Vertices.set_used(Count*4);
        Indices.set_used(Count*6);
    }

    if (Count > MaxCount)
    {
         Count = 1;
    }

    irr::core::vector3df a( -0.5, 0.001,  0.5);
    irr::core::vector3df b(  0.5, 0.001,  0.5);
    irr::core::vector3df c(  0.5, 0.001, -0.5);
    irr::core::vector3df d( -0.5, 0.001, -0.5);

    irr::core::matrix4 mat;
    mat.setScale(Scale);

    mat.transformVect(a);
    mat.transformVect(b);
    mat.transformVect(c);
    mat.transformVect(d);

    mat.makeIdentity();
    mat.setRotationDegrees(rot);
    mat.setTranslation(pos);

    mat.transformVect(a);
    mat.transformVect(b);
    mat.transformVect(c);
    mat.transformVect(d);

    irr::u16 vrt = (Count-1) * 4;

    Vertices[vrt]     = irr::video::S3DVertex( a.X, a.Y, a.Z,   0,1,0, poly_col, 0, 0);//00
    Vertices[vrt+1]   = irr::video::S3DVertex( b.X, b.Y, b.Z,   0,1,0, poly_col, 1, 0);//10
    Vertices[vrt+2]   = irr::video::S3DVertex( c.X, c.Y, c.Z,   0,1,0, poly_col, 1, 1);//11
    Vertices[vrt+3]   = irr::video::S3DVertex( d.X, d.Y, d.Z,   0,1,0, poly_col, 0, 1);//01

    Box.addInternalPoint(a);
    Box.addInternalPoint(b);
    Box.addInternalPoint(c);
    Box.addInternalPoint(d);

    irr::u16 idx = (Count-1) * 6;

    Indices[idx]    = vrt;
    Indices[idx+1]  = vrt+1;
    Indices[idx+2]  = vrt+2;
    Indices[idx+3]  = vrt;
    Indices[idx+4]  = vrt+2;
    Indices[idx+5]  = vrt+3;

    Count++; // прибавка  которую надо задекрементить на unlink чтобы не сбить с толку hide
}

void DecalSceneNode::hide()
{
    if ( Count*6 >= Indices.size()) Count = 1;

    irr::u16 idx = (Count-1) * 6;

    if ( Indices[idx] == 0 && Indices[idx+1]==0)
    {
        SceneManager->addToDeletionQueue(this);
        return;
    }

    Indices[idx]    = 0;
    Indices[idx+1]  = 0;
    Indices[idx+2]  = 0;
    Indices[idx+3]  = 0;
    Indices[idx+4]  = 0;
    Indices[idx+5]  = 0;

    Count++;
}

void DecalSceneNode::OnAnimate( irr::u32 timeMs )
{
    irr::s32 timeDiff = timeMs - LastAdd;
	if (timeDiff>abs(Speed))
	{
	    if (Speed > 0)
            add(_Parent->getAbsolutePosition(), _Parent->getAbsoluteTransformation().getRotationDegrees());
        else
            hide();

        LastAdd = timeMs;
	}
}


void DecalSceneNode::OnRegisterSceneNode()
{
    if (IsVisible) SceneManager->registerNodeForRendering(this);
    ISceneNode::OnRegisterSceneNode();
}


void DecalSceneNode::render()
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
    driver->setMaterial(Material);
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&Vertices[0], Vertices.size(), &Indices[0], Indices.size()/3, irr::video::EVT_STANDARD, irr::scene::EPT_TRIANGLES, irr::video::EIT_16BIT);
}
