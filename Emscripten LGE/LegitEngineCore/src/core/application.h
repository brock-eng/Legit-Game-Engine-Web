#pragma once

#include "../../glfw3.h"

#include <iostream>
#include <stdio.h>

#include "emscripten.h"
#include "../../src/utils/ImGui/imgui.h"
#include "../../src/utils/ImGui/imgui_impl_glfw.h"
#include "../../src/utils/ImGui/imgui_impl_opengl3.h"

#include "../Components/mathlib.h"


#if 0 // for debugging
#define INCLUDE_ALL_GRAPHICS_COMPONENTS
#endif

#include "../graphics/graphics_components.h"

#include "../input/input.h"

#define MAX_KEYS  1024
#define MAX_BUTTONS 32

namespace legit_engine {

   class Application
   {
   protected:
      graphics::Window* m_Window;
      renderables::BatchRenderer2D* m_Renderer;
      shaders::Shader* m_Shader;

      bool m_Active;

      bool m_KeysNewState[MAX_KEYS] = { 0 };
      bool m_KeysOldState[MAX_KEYS] = { 0 };
      bool m_MouseOldState[MAX_BUTTONS] = { 0 };
      bool m_MouseNewState[MAX_BUTTONS] = { 0 };

   public:
      // This is main keystate container.   
      // m_Keys and m_Mouse can be probed for their current state
      // by using m_Keys[EL_KEY]
      // Refer to input.h for the complete key list
      struct sKeyStates
      {
         bool bPressed;
         bool bReleased;
         bool bHeld;
      } m_Keys[MAX_KEYS], m_Mouse[MAX_BUTTONS];

      // This is intended to be probed for the pointers x and y coords
      struct mouseCoords
      {
         float x;
         float y;
      } m_MousePosition;

      struct scrollState
      {
         bool up;
         bool down;
         bool right;
         bool left;
      } m_MouseScroll;

      float m_ScreenWidth, m_ScreenHeight;


   public:
      Application(const char* name, unsigned int screenWidth, unsigned int screenHeight);
      ~Application();

      virtual bool OnUserCreate() = 0;
      virtual bool OnUserUpdate() = 0;

      void Start();


   private:
      void ApplicationThread();

   public:
      void getScreenSize(int& width, int& height);
      components::Vec2 getScreenSize();
      void getMousePosition(float& mouseX, float& mouseY);
      components::Vec2 getMousePosition();
      void getMousePositionNormalized(float& mouseX, float& mouseY);
      components::Vec2 getMousePositionNormalized();
      inline const void setFullScreen() const { m_Window->setFullscreen(); }
      void getScrollWheel();
   };


   static void dispatch_main(void* fp)
   {
      std::function<void()>* func = (std::function<void()>*)fp;
      (*func)();
   }

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
      m_Shader->setUniform1i("texture_0", 1);
      m_Shader->setUniform1i("texture_1", 2);
      m_Shader->setUniform1i("texture_2", 3);
      m_Shader->setUniform1i("texture_3", 4);
      m_Shader->setUniform1i("texture_4", 5);
      m_Shader->setUniform1i("texture_5", 6);
      m_Shader->setUniform1i("texture_6", 7);
      m_Shader->setUniform1i("texture_7", 8);

      // ImGui Init
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(m_Window->getWindowPointer(), true);
      ImGui_ImplOpenGL3_Init();
      ImGui::GetStyle().ScaleAllSizes(1.0f);

   }

   Application::~Application()
   {
      delete m_Renderer;
      delete m_Shader;
      delete m_Window;

      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
   }

   void Application::Start()
   {
      m_Window->getWindowSize(m_ScreenWidth, m_ScreenHeight);
      std::cout << "[Legit Engine Web]: Window created with dimensions " << m_ScreenWidth << " by " << m_ScreenHeight << std::endl;
      std::cout << "[Legit Engine Web]: Successfully loaded shader program 'mandelbrot.shader'" << std::endl;
      std::cout << "[Legit Engine Web]: ImGui initialization success" << std::endl;
      std::cout << "[Legit Engine Web]: Running application" << std::endl;


      m_Window->getMousePositionNormalized(m_MousePosition.x, m_MousePosition.y);

      ApplicationThread();
      std::cout << "[Legit Engine Web]: Stopping." << std::endl;
   }

   void Application::ApplicationThread()
   {

      OnUserCreate();

      std::function<void()> mainLoop = [&]() {

         float fOldTime = glfwGetTime();

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

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();

         getScrollWheel();

         m_Renderer->begin();

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
      };

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
      m_MouseScroll.up = (yOffset == 1);
      m_MouseScroll.down = (yOffset == -1);
      m_MouseScroll.right = (xOffset == 1);
      m_MouseScroll.left = (xOffset == -1);
   }
}