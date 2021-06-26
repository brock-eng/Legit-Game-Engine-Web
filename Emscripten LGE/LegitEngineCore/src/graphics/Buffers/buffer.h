#pragma once


#include "../../../glfw3.h"
// #include <GLES3/gl3.h>

namespace legit_engine {
   namespace buffers {

      class Buffer
      {
      private:
         GLuint m_BufferID;
         GLuint m_ComponentCount;

      public:
         Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
         ~Buffer();

         void bind() const;
         void unbind() const;

         GLuint GetComponentCount() const;
      };


      Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount)
      {
         m_ComponentCount = componentCount;

         glGenBuffers(1, &m_BufferID);
         glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
         glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
         glBindBuffer(GL_ARRAY_BUFFER, 0);
      }

      Buffer::~Buffer()
      {
         glDeleteBuffers(GL_ARRAY_BUFFER, &m_BufferID);
      }

      void Buffer::bind() const
      {
         glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
      }
      void Buffer::unbind() const
      {
         glBindBuffer(GL_ARRAY_BUFFER, 0);
      }

      GLuint Buffer::GetComponentCount() const { return m_ComponentCount; }
} }



