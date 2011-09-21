#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include <irrlicht.h>

class C_MiniMap : public irr::gui::IGUIElement
{
	public:

		//! constructor
		C_MiniMap(irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle);

		//! destructor
		virtual ~C_MiniMap();

		virtual bool OnEvent(const irr::SEvent& event);
		virtual void draw();

		void setPixel(irr::u32 x, irr::u32 y, irr::video::SColor col, bool blend = true)
		{
            tex_p = (irr::u32*)Tex->lock();
            if (tex_p)
            {
                tex_p[ y*Tex->getSize().Width+x ] = col.color;
                Tex->unlock();
                Tex->regenerateMipMapLevels();
            }
    	}

		bool lockMapTex()
		{
            tex_p = (irr::u32*)Tex->lock();
            if (tex_p) return true;
            return false;
        }
		void setPixelEx(irr::u32 x, irr::u32 y, irr::video::SColor col, bool blend = true)
		{
            tex_p[ y*Tex->getSize().Width+x ] = col.color;
		}
		void unlockMapTex()
		{
            if (tex_p)
            {
                Tex->unlock();
                Tex->regenerateMipMapLevels();
                tex_p = NULL;
            }
		}

		void setMiniMapSize(irr::u32 w, irr::u32 h);

	private:
        irr::video::ITexture* Tex;
        irr::u32* tex_p;
        irr::scene::SMeshBuffer Mesh;
        irr::core::matrix4 Matrix;
};

#endif // MINIMAP_H_INCLUDED

