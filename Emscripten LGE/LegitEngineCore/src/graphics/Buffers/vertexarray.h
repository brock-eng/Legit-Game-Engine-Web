#pragma once

#include <vector>

#include "buffer.h"

namespace legit_engine {
   namespace buffers {

      class VertexArray
      {
      private:
         GLuint m_ArrayID;
         std::vector<Buffer*> m_Buffers;

      public:
         VertexArray();
         ~VertexArray();
         void AddBuffer(Buffer* buffer, GLuint index);
         void bind() const;
         void unbind() const;
      };

      VertexArray::VertexArray()
      {
         glGenVertexArrays(1, &m_ArrayID);
      }

      VertexArray::~VertexArray()
      {
         for (int i = 0; i < m_Buffers.size(); i++)
         {
            delete m_Buffers[i];
         }

         glDeleteVertexArrays(1, &m_ArrayID);
      }

      void VertexArray::AddBuffer(Buffer* buffer, GLuint index)
      {
         VertexArray::bind();
         buffer->bind();
         glEnableVertexAttribArray(index);
         glVertexAttribPointer(index, buffer->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
         buffer->unbind();
         unbind();
      }

      void VertexArray::bind() const
      {
         glBindVertexArray(m_ArrayID);
      }

      void VertexArray::unbind() const
      {
         glBindVertexArray(0);
      }
} }
