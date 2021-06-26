#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#define GLFW_INCLUDE_ES3

#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <emscripten.h>

#include <functional>
#include "application.h"


static void dispatch_main(void* fp)
{
   std::function<void()> func = (std::function<void()>*)fp;
   (*func)();

}
namespace legit_engine {

   using namespace graphics;
   using namespace components;
   using namespace shaders;
   using namespace buffers;
   using namespace renderables;

   Application::Application(const char* name, unsigned int screenWidth, unsigned int screenHeight)
   {
      m_Window = new graphics::Window(name, screenWidth, screenHeight);
      m_Renderer = new renderables::BatchRenderer2D(screenWidth, screenHeight);
      m_Shader = new shaders::Shader("LegitEngineCore/res/Shaders/textured.shader");

      // Basic shader setup.  Legit engine currently does not support multiple shader files for 
      // a single application
      m_Shader->Enable();
      mat4 ortho = mat4::orthographic(0, screenWidth, 0, screenHeight, -1.0f, 1.0f);
      m_Shader->setUniformMat4("pr_matrix", ortho);
      auto loc = glGetUniformLocation(m_Shader->getRendererID(), "textures");
      int textures[MAX_TEXTURES];
      // allocating texture I.D. array
      for (int i = 0; i < MAX_TEXTURES; i++)
         textures[i] = i;
      glUniform1iv(loc, MAX_TEXTURES, textures);
   }

   Application::~Application()
   {
      delete m_Renderer;
      delete m_Shader;
      delete m_Window;

      // report memory leaks to visual studio debugger window
      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
      _CrtDumpMemoryLeaks();
   }

   void Application::Start()
   {
      std::cout << "[Legit Engine]: Starting" << std::endl;

      // ImGui setup procedure
      bootGui(m_Window);

      m_Window->getWindowSize(m_ScreenWidth, m_ScreenHeight);
      m_Window->getMousePositionNormalized(m_MousePosition.x, m_MousePosition.y);

      ApplicationThread();
      std::cout << "[Legit Engine]: Stopping." << std::endl;
   }
   std::function<void()> mainLoop = [&]() {
   void Application::ApplicationThread()
   {
      OnUserCreate();
      
   


      float fOldTime = glfwGetTime();

      std::function<void()> mainLoop = [&]() 
      {
         float xscroll, yscroll;


         float fNow = glfwGetTime();
         float fElapsedTime = (float)(fNow - fOldTime);
         fOldTime = fNow;

         for (int i = 0; i < MAX_KEYS; i++)
         {
            m_KeysNewState[i] = m_Window->KeyPressed(i);

            m_Keys[i].bPressed = false;
            m_Keys[i].bReleased = false;

            if (m_KeysNewState[i] != m_KeysOldState[i])
            {
               if (m_KeysNewState[i])
               {
                  m_Keys[i].bPressed = true;
                  m_Keys[i].bHeld = true;
               }
               else
               {
                  m_Keys[i].bReleased = true;
                  m_Keys[i].bHeld = false;
               }
            }

            m_KeysOldState[i] = m_KeysNewState[i];
         }

         for (int i = 0; i < MAX_BUTTONS; i++)
         {
            m_MouseNewState[i] = m_Window->MouseButtonsPressed(i);

            m_Mouse[i].bPressed = false;
            m_Mouse[i].bReleased = false;

            if (m_MouseNewState[i] != m_MouseOldState[i])
            {
               if (m_MouseNewState[i])
               {
                  m_Mouse[i].bPressed = true;
                  m_Mouse[i].bHeld = true;
               }
               else
               {
                  m_Mouse[i].bReleased = true;
                  m_Mouse[i].bHeld = false;
               }
            }

            m_MouseOldState[i] = m_MouseNewState[i];
         }
         m_Window->getMousePosition(m_MousePosition.x, m_MousePosition.y);
         m_Window->getWindowSize(m_ScreenWidth, m_ScreenHeight);
         m_Window->Clear();
         
         getScrollWheel();

         m_Renderer->begin();

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();
         

         if (!OnUserUpdate())
         {
            m_Active = false;
            std::cout << "[Legit Engine]: User ended application." << std::endl;
         }
         m_Renderer->end();
         m_Renderer->flush();

           
         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         
         m_Window->Update();

         if (m_Window->Closed())
            m_Active = false;
      }
      emscripten_set_main_loop_arg(dispatch_main, &mainLoop, 0, 1);
   }


   void Application::getScreenSize(int& width, int& height)
   {
      m_Window->getWindowSize(width, height);
   }

   components::Vec2 Application::getScreenSize()
   {
      return m_Window->getWindowSize();
   }

   void Application::getMousePosition(float& mouseX, float& mouseY)
   {
      return m_Window->getMousePosition(mouseX, mouseY);
   }

   components::Vec2 Application::getMousePosition()
   {
      return m_Window->getMousePosition();
   }

   void Application::getMousePositionNormalized(float& mouseX, float& mouseY)
   {
      m_Window->getMousePositionNormalized(mouseX, mouseY);
   }

   components::Vec2 Application::getMousePositionNormalized()
   {
      return m_Window->getMousePositionNormalized();
   }

   void Application::getScrollWheel()
   {
      float xOffset, yOffset;
      m_Window->getScrollWheel(xOffset, yOffset);
      m_MouseScroll.up =     (yOffset ==  1);
      m_MouseScroll.down =   (yOffset == -1);
      m_MouseScroll.right =  (xOffset ==  1);
      m_MouseScroll.left =   (xOffset == -1);
   }

   void Application::bootGui(legit_engine::graphics::Window* window)
   {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window->getWindowPointer(), true);
      ImGui_ImplOpenGL3_Init();
      ImGui::GetStyle().ScaleAllSizes(2.0f);
   }
}