#pragma once

namespace legit_engine {
   namespace buffers {

      class IndexBuffer
      {
      private:
         GLuint m_BufferID;
         GLuint m_Count;

      public:
         // ** max indices is 65535, for 4e10 indices use GLuint for array

         IndexBuffer(GLushort* data, GLsizei count);
         IndexBuffer(GLuint* data, GLsizei count);
         ~IndexBuffer();

         void bind() const;
         void unbind() const;

         GLuint getCount() const;
      };


      IndexBuffer::IndexBuffer(GLushort* data, GLsizei count)
      {
         m_Count = count;

         glGenBuffers(1, &m_BufferID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLushort), data, GL_STATIC_DRAW);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }

      IndexBuffer::IndexBuffer(GLuint* data, GLsizei count)
      {
         m_Count = count;

         glGenBuffers(1, &m_BufferID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }

      IndexBuffer::~IndexBuffer()
      {
         glDeleteBuffers(1, &m_BufferID);
      }

      void IndexBuffer::bind() const
      {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
      }
      void IndexBuffer::unbind() const
      {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      }

      GLuint IndexBuffer::getCount() const { return m_Count; }
   }
}