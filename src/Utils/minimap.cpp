#include "minimap.h"


//! constructor
C_MiniMap::C_MiniMap(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle)
: irr::gui::IGUIElement(irr::gui::EGUIET_ELEMENT, environment, parent, id, rectangle)
{
	#ifdef _DEBUG
	setDebugName("C_MiniMap");
	#endif
      Mesh.Vertices.set_used(4);
      Mesh.Indices .set_used(6);

      irr::video::SColor white(255, 255, 255, 255);

      Mesh.Vertices[0] = irr::video::S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
      Mesh.Vertices[1] = irr::video::S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
      Mesh.Vertices[2] = irr::video::S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
      Mesh.Vertices[3] = irr::video::S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

      Mesh.Indices[0] = 0;
      Mesh.Indices[1] = 1;
      Mesh.Indices[2] = 2;
      Mesh.Indices[3] = 2;
      Mesh.Indices[4] = 3;
      Mesh.Indices[5] = 0;

      Mesh.getMaterial().Lighting = false;
      Mesh.getMaterial().BackfaceCulling = false;
      Mesh.getMaterial().ZBuffer = false;
      //Mesh.getMaterial().Wireframe = true;
      //Mesh.getMaterial().MaterialType = irr::video::EMT_TRANSPARENT_ADD_COLOR;
      Mesh.setHardwareMappingHint(irr::scene::EHM_STATIC, irr::scene::EBT_VERTEX_AND_INDEX);

      Matrix.makeIdentity();

      Tex = NULL;
      tex_p = NULL;
}

//! destructor
C_MiniMap::~C_MiniMap()
{
    if (Tex) Tex->drop();
}

void C_MiniMap::setMiniMapSize(irr::u32 w, irr::u32 h)
{
    irr::video::IImage* Image = Environment->getVideoDriver()->createImage(irr::video::ECF_A8R8G8B8, irr::core::dimension2d<irr::u32>(w,h) );
    if (Tex) Tex->drop();
    Tex = Environment->getVideoDriver()->addTexture( "minimap", Image);
    Image->drop();
    Mesh.getMaterial().setTexture(0, Tex);
}

bool C_MiniMap::OnEvent(const irr::SEvent& event)
{
	return IGUIElement::OnEvent(event);
}

void C_MiniMap::draw()
{
      irr::video::IVideoDriver* driver = Environment->getVideoDriver();
      if (! (driver && IsVisible))
         return;

	// draw the frame

	irr::core::position2di pos = getRelativePosition().UpperLeftCorner;
	irr::core::rect<irr::s32> frameRect(AbsoluteRect.UpperLeftCorner.X-2, AbsoluteRect.UpperLeftCorner.Y-2, AbsoluteRect.LowerRightCorner.X+2, AbsoluteRect.LowerRightCorner.Y+2);
	Environment->getSkin()->draw2DRectangle(this, Environment->getSkin()->getColor(irr::gui::EGDC_3D_HIGH_LIGHT), frameRect);

	// draw map

      irr::core::rect<irr::s32> oldViewPort = driver->getViewPort();
      driver->setViewPort(getAbsolutePosition());

      // clear the projection matrix
      irr::core::matrix4 oldProjMat = driver->getTransform(irr::video::ETS_PROJECTION);
      driver->setTransform(irr::video::ETS_PROJECTION, irr::core::matrix4());

      // clear the view matrix
      irr::core::matrix4 oldViewMat = driver->getTransform(irr::video::ETS_VIEW);
      driver->setTransform(irr::video::ETS_VIEW, irr::core::matrix4());

      driver->setTransform(irr::video::ETS_WORLD, Matrix);

      driver->setMaterial(Mesh.Material);

      driver->drawMeshBuffer(&Mesh);

      // restore view matrix
      driver->setTransform(irr::video::ETS_VIEW, oldViewMat);

      // restore projection matrix
      driver->setTransform(irr::video::ETS_PROJECTION, oldProjMat);

      // restore the view area
      driver->setViewPort(oldViewPort);
}
