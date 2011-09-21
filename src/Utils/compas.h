#ifndef COMPAS_H_INCLUDED
#define COMPAS_H_INCLUDED

class CGUICompass : public gui::IGUIElement
{
public:
   CGUICompass(core::rect<s32> rect, gui::IGUIEnvironment* env, gui::IGUIElement* parent)
      : gui::IGUIElement(gui::EGUIET_ELEMENT, env, parent, -1, rect)
   {
      Mesh.Vertices.set_used(4);
      Mesh.Indices .set_used(6);

      video::SColor white(255, 255, 255, 255);

      Mesh.Vertices[0] = video::S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
      Mesh.Vertices[1] = video::S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
      Mesh.Vertices[2] = video::S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
      Mesh.Vertices[3] = video::S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

      Mesh.Indices[0] = 0;
      Mesh.Indices[1] = 1;
      Mesh.Indices[2] = 2;
      Mesh.Indices[3] = 2;
      Mesh.Indices[4] = 3;
      Mesh.Indices[5] = 0;

      Mesh.getMaterial().Lighting = false;
      Mesh.getMaterial().BackfaceCulling = false;
      //Mesh.getMaterial().Wireframe = true;
      Mesh.getMaterial().MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
   }

   //
   void setCompassTexture(video::ITexture* texture)
   {
      Mesh.getMaterial().setTexture(0, texture);
   }

   void setCompassHeading(f32 deg)
   {
      Matrix.makeIdentity();
      Matrix.setRotationDegrees(core::vector3df(0, 0, deg));
   }

   //! render the compass
   virtual void draw()
   {
      video::IVideoDriver* driver = Environment->getVideoDriver();
      if (! (driver && IsVisible))
         return;

      core::rect<s32> oldViewPort = driver->getViewPort();
      driver->setViewPort(getAbsolutePosition());

      // clear the projection matrix
      core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);
      driver->setTransform(video::ETS_PROJECTION, core::matrix4());

      // clear the view matrix
      core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
      driver->setTransform(video::ETS_VIEW, core::matrix4());

      driver->setTransform(video::ETS_WORLD, Matrix);

      driver->setMaterial(Mesh.Material);

      driver->drawMeshBuffer(&Mesh);

      // restore view matrix
      driver->setTransform(video::ETS_VIEW, oldViewMat);

      // restore projection matrix
      driver->setTransform(video::ETS_PROJECTION, oldProjMat);

      // restore the view area
      driver->setViewPort(oldViewPort);
   }

private:
   scene::SMeshBuffer Mesh;
   core::matrix4 Matrix;
};

#endif // COMPAS_H_INCLUDED
