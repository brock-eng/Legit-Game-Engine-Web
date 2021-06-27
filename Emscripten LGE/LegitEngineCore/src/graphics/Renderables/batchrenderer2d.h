#pragma once

#include <vector>

#include <GLES3/gl3.h>

#include "sprite.h"
#include "renderer2d.h"
#include "../../game_assets/entity.h"

#include "../Buffers/vertexarray.h"
#include "../Shaders/shaders.h"


namespace legit_engine {
   namespace renderables {

#define RENDERER_MAX_SPRITES     60000
#define RENDERER_VERTEX_SIZE     sizeof(VertexData)
#define RENDERER_SPRITE_SIZE     RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE     RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE    RENDERER_MAX_SPRITES * 6
               
#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_COLOR_INDEX 2
#define SHADER_TEXTURE_INDEX 3

#define MAX_TEXTURES 8

      class BatchRenderer2D : public Renderer2D
      {
      private:
         components::Vec4 s_QuadVertices[4];

      protected:
         buffers::IndexBuffer* m_IBO;
         GLuint m_VBO;
         GLuint m_VAO;
         GLsizei m_IndexCount;
         float m_ScreenWidth, m_ScreenHeight;

         VertexData* m_Buffer;
         VertexData* m_BufferBase;

         std::vector<unsigned int> m_Textures;
      protected:


      public:
         BatchRenderer2D(float screenWidth, float screenHeight);
         ~BatchRenderer2D();
         void UpdateScreenSize(float screenWidth, float screenHeight);
         void begin();

         void submit(const Renderable2D* renderable) override;
         void submitSprite(const Sprite* sprite);
         void submitEntity(const Entity* entity);
         void submitEntity(float x, float y, float width, float height, float rotation, Texture* texture, const std::vector<components::Vec2> uv = Renderable2D::getDefaultUV());
         void submitLine(float x0, float y0, float x1, float y1, unsigned int color, float thickness);
         void paintPixel(int x, int y, components::Vec4 color, GLint shaderProgram, const char* uniformFloat);


         void end();
         void flush() override;

         inline const bool bufferHasData() { return m_Buffer != nullptr; };

      private: 
         void init();
      };

      BatchRenderer2D::BatchRenderer2D(float screenWidth, float screenHeight)
         : m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
      {
         init();
      }

      BatchRenderer2D::~BatchRenderer2D()
      {
         delete m_IBO;
         glDeleteBuffers(1, &m_VBO);
      }

      void BatchRenderer2D::UpdateScreenSize(float width, float height)

      {
         m_ScreenWidth = width;
         m_ScreenHeight = height;
      }

      void BatchRenderer2D::init()
      {
         glGenVertexArrays(1, &m_VAO);
         glGenBuffers(1, &m_VBO);

         glBindVertexArray(m_VAO);
         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);

         glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
         glEnableVertexAttribArray(SHADER_UV_INDEX);
         glEnableVertexAttribArray(SHADER_COLOR_INDEX);
         glEnableVertexAttribArray(SHADER_TEXTURE_INDEX);

         glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
         glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, texCoords));
         glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, color));
         glVertexAttribPointer(SHADER_TEXTURE_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)offsetof(VertexData, textureIndex));
         glBindBuffer(GL_ARRAY_BUFFER, 0);

         GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

         int offset = 0;
         for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
         {
            indices[i] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
         }

         m_IBO = new buffers::IndexBuffer(indices, RENDERER_INDICES_SIZE);

         s_QuadVertices[0] = { -0.5f, -0.5f, 0, 0 };
         s_QuadVertices[1] = { 0.5f, -0.5f, 0, 0 };
         s_QuadVertices[2] = { 0.5f,  0.5f, 0, 0 };
         s_QuadVertices[3] = { -0.5f,  0.5f, 0, 0 };

         m_BufferBase = new VertexData[RENDERER_MAX_SPRITES * 4];
      }

      void BatchRenderer2D::begin()
      {
         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         m_Buffer = m_BufferBase;
      }

      void BatchRenderer2D::submit(const renderables::Renderable2D* renderable)
      {
         const components::Vec2& size = renderable->getSize();
         const components::Vec3& position = renderable->getPosition();
         const components::Vec4& color = renderable->getColor();
         const std::vector<components::Vec2>& uv = renderable->getUV();
         const float tid = 0.0f;

         int r = color.x * 255.0f;
         int g = color.y * 255.0f;
         int b = color.z * 255.0f;
         int a = color.w * 255.0f;

         // this improves performance by nearly 20%
         unsigned int c = a << 24 | b << 16 | g << 8 | r;

         m_Buffer->vertex = position;
         m_Buffer->texCoords = uv[0];
         m_Buffer->color = c;
         m_Buffer->textureIndex = tid;
         m_Buffer++; // moves by sizeof (VertexData)

         m_Buffer->vertex = components::Vec3(position.x, position.y + size.y, position.z);
         m_Buffer->texCoords = uv[1];
         m_Buffer->color = c;
         m_Buffer->textureIndex = tid;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x + size.x, position.y + size.y, position.z);
         m_Buffer->texCoords = uv[2];
         m_Buffer->color = c;
         m_Buffer->textureIndex = tid;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x + size.x, position.y, position.z);
         m_Buffer->texCoords = uv[3];
         m_Buffer->color = c;
         m_Buffer->textureIndex = tid;
         m_Buffer++;

         m_IndexCount += 6;
      }

      void BatchRenderer2D::submitSprite(const Sprite* sprite)
      {
         const components::Vec2& size = sprite->getSize();
         const components::Vec3& position = sprite->getPosition();
         const components::Vec4& color = sprite->getColor();
         const std::vector<components::Vec2>& uv = sprite->getUV();
         float tid = sprite->getTextureID();
         int r = 0, g = 0, b = 0, a = 0, c = 0;

         if (m_Buffer == nullptr)
            return;

         float ts = 0.0f;
         if (tid > 0)
         {
            bool found = false;
            for (int i = 0; i < m_Textures.size(); i++)
               if (tid == m_Textures[i])
               {
                  ts = (float)(i + 1);
                  found = true;
                  break;
               }

            if (!found)
            {
               if (m_Textures.size() >= MAX_TEXTURES)
               {
                  end();
                  flush();
                  begin();
               }
               m_Textures.push_back(tid);
               ts = (float)m_Textures.size();
            }
         }
         else
         {
            r = color.x * 255.0f;
            g = color.y * 255.0f;
            b = color.z * 255.0f;
            a = color.w * 255.0f;
            c = a << 24 | b << 16 | g << 8 | r;
         }

         m_Buffer->vertex = position;
         m_Buffer->texCoords = uv[0];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x, position.y + size.y, position.z);
         m_Buffer->texCoords = uv[1];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x + size.x, position.y + size.y, position.z);
         m_Buffer->texCoords = uv[2];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = components::Vec3(position.x + size.x, position.y, position.z);
         m_Buffer->texCoords = uv[3];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_IndexCount += 6;
      }

      void BatchRenderer2D::submitEntity(const Entity* entity)
      {
         const Vec2& size = entity->getSize();
         const Vec3& position = entity->getPosition();
         const Vec4& color = entity->getColor();
         const std::vector<Vec2>& uv = entity->getUV();
         float tid = entity->getTextureID();
         float rotation = entity->getRotation();
         int r = 0, g = 0, b = 0, a = 0, c = 0;

         if (m_Buffer == nullptr)
         {
            std::cout << "Error: Renderer2D -> m_Buffer was nullptr." << std::endl;
            return;
         }

         float ts = 0.0f;
         if (tid > 0)
         {
            bool found = false;
            for (int i = 0; i < m_Textures.size(); i++)
               if (tid == m_Textures[i])
               {
                  ts = (float)(i + 1);
                  found = true;
                  break;
               }

            if (!found)
            {
               if (m_Textures.size() >= MAX_TEXTURES)
               {
                  end();
                  flush();
                  begin();
               }
               m_Textures.push_back(tid);
               ts = (float)m_Textures.size();
            }
         }
         else
         {
            r = color.x * 255.0f;
            g = color.y * 255.0f;
            b = color.z * 255.0f;
            a = color.w * 255.0f;
            c = a << 24 | b << 16 | g << 8 | r;
         }

         Vec4 quadVertices[4];
         if (rotation)
         {
            rotation *= PI / 180.0f;

            float SIN = sin(rotation);
            float COS = cos(rotation);


            quadVertices[0] = { position.x + (COS * -size.x / 2.0f - SIN * -size.y / 2.0f),  position.y + (SIN * -size.x / 2.0f + COS * -size.y / 2.0f), 0, 0 };
            quadVertices[1] = { position.x + (COS * -size.x / 2.0f - SIN * size.y / 2.0f),   position.y + (SIN * -size.x / 2.0f + COS * size.y / 2.0f), 0, 0 };
            quadVertices[2] = { position.x + (COS * size.x / 2.0f - SIN * size.y / 2.0f),    position.y + (SIN * size.x / 2.0f + COS * size.y / 2.0f), 0, 0 };
            quadVertices[3] = { position.x + (COS * size.x / 2.0f - SIN * -size.y / 2.0f),   position.y + (SIN * size.x / 2.0f + COS * -size.y / 2.0f), 0, 0 };
         }
         else
         {
            quadVertices[0] = { position.x - size.x, position.y - size.y, 0, 0 };
            quadVertices[1] = { position.x - size.x, position.y + size.y, 0, 0 };
            quadVertices[2] = { position.x + size.x, position.y + size.y, 0, 0 };
            quadVertices[3] = { position.x + size.x, position.y - size.y, 0, 0 };
         }


         m_Buffer->vertex = { quadVertices[0].x, quadVertices[0].y, quadVertices[0].z };
         m_Buffer->texCoords = uv[0];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = { quadVertices[1].x, quadVertices[1].y, quadVertices[1].z };
         m_Buffer->texCoords = uv[1];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = { quadVertices[2].x, quadVertices[2].y, quadVertices[2].z };
         m_Buffer->texCoords = uv[2];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = { quadVertices[3].x, quadVertices[3].y, quadVertices[3].z };
         m_Buffer->texCoords = uv[3];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_IndexCount += 6;
      }

      void BatchRenderer2D::submitEntity(float x, float y, float width, float height, float rotation, Texture* texture, const std::vector<Vec2> uv)
      {
         const Vec2& size = { width, height };
         const Vec3& position = { x, y, 0.0f };
         float tid = texture->getID();
         unsigned int c = 0;

         if (m_Buffer == NULL)
         {
            std::cout << "Error: Renderer2D -> m_Buffer was nullptr." << std::endl;
            return;
         }

         float ts = 0.0f;
         if (tid > 0)
         {
            bool found = false;
            for (int i = 0; i < m_Textures.size(); i++)
               if (tid == m_Textures[i])
               {
                  ts = (float)(i + 1);
                  found = true;
                  break;
               }

            if (!found)
            {
               if (m_Textures.size() >= MAX_TEXTURES)
               {
                  end();
                  flush();
                  begin();
               }
               m_Textures.push_back(tid);
               ts = (float)m_Textures.size();
            }
         }


         Vec4 quadVertices[4];
         if (rotation)
         {
            rotation *= PI / 180.0f;

            float SIN = sin(rotation);
            float COS = cos(rotation);


            quadVertices[0] = { position.x + (COS * -size.x / 2.0f - SIN * -size.y / 2.0f),  position.y + (SIN * -size.x / 2.0f + COS * -size.y / 2.0f), 0, 0 };
            quadVertices[1] = { position.x + (COS * -size.x / 2.0f - SIN * size.y / 2.0f),   position.y + (SIN * -size.x / 2.0f + COS * size.y / 2.0f), 0, 0 };
            quadVertices[2] = { position.x + (COS * size.x / 2.0f - SIN * size.y / 2.0f),    position.y + (SIN * size.x / 2.0f + COS * size.y / 2.0f), 0, 0 };
            quadVertices[3] = { position.x + (COS * size.x / 2.0f - SIN * -size.y / 2.0f),   position.y + (SIN * size.x / 2.0f + COS * -size.y / 2.0f), 0, 0 };
         }
         else
         {
            quadVertices[0] = { position.x - size.x, position.y - size.y, 0, 0 };
            quadVertices[1] = { position.x - size.x, position.y + size.y, 0, 0 };
            quadVertices[2] = { position.x + size.x, position.y + size.y, 0, 0 };
            quadVertices[3] = { position.x + size.x, position.y - size.y, 0, 0 };
         }

         m_Buffer->vertex = { quadVertices[0].x, quadVertices[0].y, quadVertices[0].z };
         m_Buffer->texCoords = uv[0];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = { quadVertices[1].x, quadVertices[1].y, quadVertices[1].z };
         m_Buffer->texCoords = uv[1];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = { quadVertices[2].x, quadVertices[2].y, quadVertices[2].z };
         m_Buffer->texCoords = uv[2];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_Buffer->vertex = { quadVertices[3].x, quadVertices[3].y, quadVertices[3].z };
         m_Buffer->texCoords = uv[3];
         m_Buffer->color = c;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_IndexCount += 6;
      }

      void BatchRenderer2D::submitLine(float x0, float y0, float x1, float y1, unsigned int color, float thickness)
      {
         const std::vector<Vec2>& uv = { {0, 0}, {0, 1}, {1, 1}, {1, 0} };
         float ts = 0.0f;
         float mid = 0;
         float ms = 0.0f;

         Vec2 normal = Vec2(y1 - y0, -(x1 - x0)).normalise() * thickness;

         Vec3 vertex = Vec3(x0 + normal.x, y0 + normal.y, 0.0f);
         m_Buffer->vertex = vertex;
         m_Buffer->texCoords = uv[0];
         m_Buffer->color = color;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         vertex = Vec3(x1 + normal.x, y1 + normal.y, 0.0f);
         m_Buffer->vertex = vertex;
         m_Buffer->texCoords = uv[1];
         m_Buffer->color = color;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         vertex = Vec3(x1 - normal.x, y1 - normal.y, 0.0f);
         m_Buffer->vertex = vertex;
         m_Buffer->texCoords = uv[2];
         m_Buffer->color = color;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         vertex = Vec3(x0 - normal.x, y0 - normal.y, 0.0f);
         m_Buffer->vertex = vertex;
         m_Buffer->texCoords = uv[3];
         m_Buffer->color = color;
         m_Buffer->textureIndex = ts;
         m_Buffer++;

         m_IndexCount += 6;
      }


      void BatchRenderer2D::end()
      {
         glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
         glBufferSubData(GL_ARRAY_BUFFER, 0, (m_Buffer - m_BufferBase) * RENDERER_VERTEX_SIZE, m_BufferBase);
         m_Buffer = m_BufferBase;
      }

      void BatchRenderer2D::flush()
      {

         for (int i = 0; i < m_Textures.size(); i++)
         {
            glActiveTexture(GL_TEXTURE0 + i + 1);
            glBindTexture(GL_TEXTURE_2D, m_Textures[i]);
         }

         glBindVertexArray(m_VAO);
         m_IBO->bind();

         glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

         m_IBO->unbind();
         glBindVertexArray(0);

         m_IndexCount = 0;
      }
   }
}
