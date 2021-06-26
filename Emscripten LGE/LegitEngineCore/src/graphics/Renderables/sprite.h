#pragma once

#include "../Renderables/renderable2d.h"
#include "../Renderables/texture.h"

namespace legit_engine {
   namespace renderables {

      class Sprite : public renderables:: Renderable2D
      {
      private:
         shaders::Shader* m_Shader;
      public:

         Sprite();

         Sprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader);

         Sprite(float x, float y, float width, float height, renderables::Texture* texture, shaders::Shader* shader);

         void setUV(float x, float y, float width, float height);

         inline shaders::Shader& getShader() const { return *m_Shader; }
         inline const components::Vec2& getSize() const { return m_Size; }
         inline const components::Vec3& getPosition() const { return m_Position; }
         inline const components::Vec4& getColor() const { return m_Color; }
         inline const std::vector<components::Vec2>& getUV() const { return m_UV; }
         inline components::Vec3& getPositionM() { return m_Position; }
         inline void setPosition(float x, float y, float z)
         {
            m_Position.x = x;
            m_Position.y = y;
            m_Position.z = z;
         }
      };



      Sprite::Sprite()
         : Renderable2D(components::Vec3(0, 0, 0), components::Vec2(0, 0), 0, 0)
      {}

      Sprite::Sprite(float x, float y, float width, float height, const components::Vec4& color, shaders::Shader* shader)
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), color, 0), m_Shader(shader)
      {}

      Sprite::Sprite(float x, float y, float width, float height, renderables::Texture* texture, shaders::Shader* shader)
         : Renderable2D(components::Vec3(x, y, 0), components::Vec2(width, height), texture), m_Shader(shader)
      {}


      // Sets the Renderable2D UV values used for selecting the area of a texture to be rendered.
      // Useful for texture maps with multiple sprites/textures per single image file.
      // Coordinates are relative and the bottom left corner of a texture is treated as (0, 0) 
      // with the top right being (1, 1)
      void Sprite::setUV(float x, float y, float width, float height)
      {
         m_UV[0].x = x;
         m_UV[0].y = y;
         m_UV[1].x = x;
         m_UV[1].y = y + height;
         m_UV[2].x = x + width;
         m_UV[2].y = y + height;
         m_UV[3].x = x + width;
         m_UV[3].y = y;
      }

} }