#pragma once

#include "../../utils/StbImage/stb_image_include.h"

#include <string>

#include "../../../glfw3.h"
#include <emscripten.h>

namespace legit_engine {
   namespace renderables {


      class Texture
      {
      public:
         static unsigned int m_GlobalTextureCount;
      private:

         std::string m_Path;
         unsigned char* m_LocalBuffer;
         int m_Width, m_Height, m_BPP;

         unsigned int m_ID;
      public: 
         Texture(const std::string& path);
         ~Texture();
         void bind(unsigned int slot = 0);
         void unbind() const;

         static void setGlobalTextureCount(unsigned int count) { m_GlobalTextureCount = count; }
         inline const unsigned int getID() const { return m_ID; }
         inline const unsigned int getWidth() const { return m_Width; }
         inline const unsigned int getHeight() const { return m_Height; }
      private:
         GLuint load();


      };

      unsigned int Texture::m_GlobalTextureCount{ 0 };

      Texture::Texture(const std::string& path)
         : m_Path(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
      {
         stbi_set_flip_vertically_on_load(1);
         m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 3);
         glGenTextures(1, &m_ID);
         glBindTexture(GL_TEXTURE_2D, m_ID);

         std::cout << m_ID << std::endl;

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
         glBindTexture(GL_TEXTURE_2D, 0);

         if (m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);
         else
         {
            std::string reason = stbi_failure_reason();
            std::cout << "Error: Shader could not load texture @ '" << path << "'. Reason: " << reason << std::endl;
         }
      }

      Texture::~Texture()
      {
         glDeleteTextures(1, &m_ID);
         delete m_LocalBuffer;
      }

      void Texture::bind(unsigned int slot)
      {
         glActiveTexture(GL_TEXTURE0 + slot);
         glBindTexture(GL_TEXTURE_2D, m_ID);
      }

      void Texture::unbind() const
      {
         glBindTexture(GL_TEXTURE_2D, 0);
      }
} }