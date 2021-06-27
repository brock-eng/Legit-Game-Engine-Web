#pragma once

#include "../../../glfw3.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <emscripten.h>
#include "../../Components/mathlib.h"

namespace legit_engine {
   namespace shaders {
      
      struct ShaderSources;

      class Shader
      {
      private:
         const char* m_filepath;
         unsigned int m_ShaderID;
      public:
         Shader(const char* filepath);
         ~Shader();

         void setUniform1i(const GLchar* name, int value);
         void setUniform1f(const GLchar* name, float value);
         void setUniform2f(const GLchar* name, const components::Vec2& matrix);
         void setUniform3f(const GLchar* name, const components::Vec3& matrix);
         void setUniform4f(const GLchar* name, const components::Vec4& matrix);
         void setUniformMat4(const GLchar* name, const components::mat4& matrix);

         inline const unsigned int getRendererID() const { return m_ShaderID; }

         void Enable() const;
         void Disable() const;
      private:
         ShaderSources ParseShader();
         unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
         unsigned int CompileShader(unsigned int type, const std::string& source);

         GLint GetUniformLocation(const GLchar* name);
      };

      struct ShaderSources
      {
         std::string VertexSource;
         std::string FragmentSource;
      };

      Shader::Shader(const char* filepath)
      {
         m_filepath = filepath;
         ShaderSources filePaths = ParseShader();
         m_ShaderID = CreateShader(filePaths.VertexSource, filePaths.FragmentSource);
      }

      Shader::~Shader()
      {
         glDeleteProgram(m_ShaderID);
      }

      void Shader::Enable() const
      {
         glUseProgram(m_ShaderID);
      }

      void Shader::Disable() const
      {
         glUseProgram(0);
      }

      GLint Shader::GetUniformLocation(const GLchar* name)
      {
         return glGetUniformLocation(m_ShaderID, name);
      }

      void Shader::setUniform1f(const GLchar* name, float value)
      {
         glUniform1f(GetUniformLocation(name), value);
      }

      void Shader::setUniform1i(const GLchar* name, int value)
      {
         glUniform1i(GetUniformLocation(name), value);
      }

      void Shader::setUniform2f(const GLchar* name, const components::Vec2& vector2D)
      {
         glUniform2f(GetUniformLocation(name), vector2D.x, vector2D.y);
      }

      void Shader::setUniform3f(const GLchar* name, const components::Vec3& vector3D)
      {
         glUniform3f(GetUniformLocation(name), vector3D.x, vector3D.y, vector3D.z);
      }

      void Shader::setUniform4f(const GLchar* name, const components::Vec4& vector4D)
      {
         glUniform4f(GetUniformLocation(name), vector4D.x, vector4D.y, vector4D.z, vector4D.w);
      }

      void Shader::setUniformMat4(const GLchar* name, const components::mat4& matrix4X4)
      {
         glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix4X4.elements);
      }

      ShaderSources Shader::ParseShader()
      {
         const char* shaders[2];

         /*
         // vertex shader
         shaders[0] =
         {
            "attribute highp vec4 position;\n"
            "attribute highp vec2 texCoords;\n"
            "attribute highp vec4 color;\n"
            "attribute highp float textureID;\n"

            "uniform mat4 pr_matrix;\n"
            "uniform mat4 vw_matrix;\n"
            "uniform mat4 ml_matrix;\n"


            "varying highp vec4 vs_position;\n"
            "varying highp vec2 vs_texCoords;\n"
            "varying highp vec4 vs_color;\n"
            "varying highp float vs_textureID;\n"

            "void main()\n"
            "{\n"
               "vec4 calc_position = position * pr_matrix;\n"
               "gl_Position = calc_position;\n"
               "vs_position = calc_position;\n"
               "vs_texCoords = texCoords;\n"
               "vs_color = color;\n"
               "vs_textureID = textureID;\n"
            "}\n"
         };

         // fragment shader
         shaders[1] =
         {                                                                                                                         
            "precision highp float;\n"
            "\n"
            "varying highp vec4  vs_position;\n"
            "varying highp vec2  vs_texCoords;\n"
            "varying highp vec4  vs_color;\n"
            "varying highp float vs_textureID;\n"
            
            
            "uniform highp int numIterations;\n"
            
            "uniform highp mat4 uMVPMatrix;// = mat4(1.0);\n"
            
            "void main() {\n"
            
            "   vec2 p = (uMVPMatrix * vec4(vs_position.x, vs_position.y, 0, 1)).xy;\n"
            "   vec2 c = p;\n"
            "   vec3 color = vec3(0.0, 0.0, 0.0);\n"
            
            "   for (int i = 0; i < 1500; i++) {\n"
            "      p = vec2(p.x * p.x - p.y * p.y, 2.0 * p.x * p.y) + c;\n"
            "      if (dot(p, p) > 4.0) {\n"
            "         float colorRegulator = float(i - 1) - log(((log(dot(p, p))) / log(2.0))) / log(2.0);\n"
            "         color = vec3(0.95 + .012 * colorRegulator, 1.0, .2 + .4 * (1.0 + sin(.3 * colorRegulator)));\n"
            "         break;\n"
            "      }\n"
            "      if (i >= numIterations) break;\n"
            "   }\n"
            
            "   vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
            "   vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);\n"
            "   gl_FragColor.rgb = color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y);\n"
            
            "   gl_FragColor.a = 1.0;\n"
            " }\n"

         };
         */

         shaders[0] =
         {
            "attribute highp vec4 position;            \n"
            "attribute highp vec2 uv;                  \n"
            "attribute highp vec4 color;               \n"
            "attribute highp float tid;                \n"
            "                                          \n"
            "uniform highp mat4 pr_matrix;             \n"
            "uniform highp mat4 vw_matrix;             \n"
            "uniform highp mat4 ml_matrix;             \n"
            "                                          \n"
            "varying highp vec4 vs_position;           \n"
            "varying highp vec2 vs_uv;                 \n"
            "varying highp float vs_tid;               \n"
            "varying highp vec4 vs_color;              \n"
            "                                          \n"
            "void main()                               \n"
            "{                                         \n"
            "   gl_Position = position * pr_matrix;    \n"
            "   vs_position = position;                \n"
            "   vs_uv = uv;                            \n"
            "   vs_tid = tid;                          \n"
            "   vs_color = color;                      \n"
            "}                                         \n"
         };

         shaders[1] =
         {
            "precision highp float;                                           \n"
            "                                                                 \n"
            "uniform highp vec4 colour;                                       \n"
            "uniform highp vec2 light_pos;                                    \n"
            "                                                                 \n"
            "varying highp vec4 vs_position;                                  \n"
            "varying highp vec2 vs_uv;                                        \n"
            "varying highp vec4 vs_color;                                     \n"
            "varying highp float vs_tid;                                      \n"
            "                                                                 \n"
            "uniform sampler2D texture_0;                                     \n"
            "uniform sampler2D texture_1;                                     \n"
            "uniform sampler2D texture_2;                                     \n"
            "uniform sampler2D texture_3;                                     \n"
            "uniform sampler2D texture_4;                                     \n"
            "uniform sampler2D texture_5;                                     \n"
            "uniform sampler2D texture_6;                                     \n"
            "uniform sampler2D texture_7;                                     \n"
            "                                                                 \n"
            "void main()                                                      \n"
            "{                                                                \n"
            "   float intensity = 1.0 / length(vs_position.xy - light_pos);   \n"
            "   vec4 texColor = vs_color;                                     \n"
            "   vec4 nullColor = vec4(0.0, 0.0, 0.0, 0.0);                    \n"
            "   if (vs_tid > 0.0)                                             \n"
            "   {                                                             \n"
            "      int tid = int(1.0);                                        \n"
            "      if (tid == 1)                                              \n"
            "         texColor = texture2D(texture_0, vs_uv);                 \n"
            "      else if (tid == 2)                                         \n"
            "         texColor = texture2D(texture_1, vs_uv);                 \n"
            "      else if (tid == 3)                                         \n"
            "         texColor = texture2D(texture_2, vs_uv);                 \n"
            "      else if (tid == 4)                                         \n"
            "         texColor = texture2D(texture_3, vs_uv);                 \n"
            "      else if (tid == 5)                                         \n"
            "         texColor = texture2D(texture_4, vs_uv);                 \n"
            "      else if (tid == 6)                                         \n"
            "         texColor = texture2D(texture_5, vs_uv);                 \n"
            "      else if (tid == 7)                                         \n"
            "         texColor = texture2D(texture_6, vs_uv);                 \n"
            "      else if (tid == 8)                                         \n"
            "         texColor = texture2D(texture_7, vs_uv);                 \n"
            "                                                                 \n"
            "   }                                                             \n"
            "   if (texColor == Vec4(0.0f, 0.0f, 0.0f, 0.0f) discard;         \n"
            "   gl_FragColor = texColor;                                      \n"
            "}                                                                \n"
         };

         return { shaders[0], shaders[1] };
      }; 

      unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
      {
         unsigned int id = glCreateShader(type);
         const char* src = source.c_str();
         glShaderSource(id, 1, &src, nullptr);
         glCompileShader(id);

         int result;
         glGetShaderiv(id, GL_COMPILE_STATUS, &result);

         // error handling
         if (result == GL_FALSE)
         {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile shader: " << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
         }
         return id;
      }

      unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
      {
         unsigned int program = glCreateProgram();
         unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
         unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

         glAttachShader(program, vs);
         glAttachShader(program, fs);
         glLinkProgram(program);
         glValidateProgram(program);

         glDeleteShader(vs);
         glDeleteShader(fs);

         return program;
      }
} }