#include <math.h>

#include "../Emscripten LGE/LegitEngineCore/legit_engine.h"

using namespace legit_engine;
using namespace renderables;
using namespace components;
using namespace std;
float debugX, debugY;


class Fractal : public Application
{

public:
   Fractal(const char* name, unsigned int screenWidth, unsigned int screenHeight)
      : Application(name, screenWidth, screenHeight) {}

   // simulation constants
   Sprite canvas;
   float scale = 1.0f;
   float mZoom = 1.0f;
   float mRatio;
   Vec2 mousePan, offset, prevOffset = { 0.0f, 0.0f };
   int numIterations = 100;
   float mousePanFactor = 1.2f;


   // returns the world coordinates of a given screen coordinate input
   void ScreenToWorld(float& X, float& Y)
   {
      X = (X - m_ScreenWidth / 2.0f) / scale + offset.x;
      Y = (Y - m_ScreenHeight / 2.0f) / scale + offset.y;
   }

   // simulation start up
   bool OnUserCreate() override
   {
      // legit engine at the time of this code production 
      // has a built in shader program, this overrides that shader
      // with our mandelbrot set program
      delete m_Shader;
      m_Shader = new shaders::Shader("res/shaders/mandelbrot.shader");
      m_Shader->Enable();

      // OpenGL background color set to black
      glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

      // allocating a 'canvas' sprite that covers the entire screen
      canvas = Sprite(-m_ScreenWidth, -m_ScreenHeight, m_ScreenWidth, m_ScreenHeight, Vec4(1.0f, 1.0f, 1.0f, 0), m_Shader);

      // the screen aspect ratio
      mRatio = m_ScreenWidth / m_ScreenHeight;


      // setting simulation defaults
      scale = 0.38;
      offset.x = 450;
      offset.y = -25;
      prevOffset = offset;

      // initializing number of iterations to 100
      m_Shader->setUniform1i("numIterations", numIterations);

      return true;
   }

   // main simulation loop
   bool OnUserUpdate() override
   {

      if (!ImGui::GetIO().WantCaptureMouse)
      {
         // start mouse panning
         if (m_Mouse[BUTTON_1].bPressed)
         {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            mousePan = { (m_MousePosition.x - prevOffset.x * scale), (m_MousePosition.y - prevOffset.y * scale) };
         }
         // update offset while dragging
         if (m_Mouse[BUTTON_1].bHeld)
         {
            offset = { (m_MousePosition.x - mousePan.x) / scale, (m_MousePosition.y - mousePan.y) / scale };
         }
         // drag end, store previous offset
         else if (m_Mouse[BUTTON_1].bReleased)
         {
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            prevOffset = offset;
         }
         // zoom in or out focused on the mouse location
         if (m_MouseScroll.up || m_MouseScroll.down || m_Keys[KEY_W].bHeld || m_Keys[KEY_S].bHeld)
         {
            float mouseXBefore = m_MousePosition.x;
            float mouseYBefore = m_MousePosition.y;

            ScreenToWorld(mouseXBefore, mouseYBefore);

            // zoom in
            if (m_MouseScroll.up || m_Keys[KEY_W].bHeld)
            {
               scale *= 1.02f;
            }
            // zoom out
            else if (m_MouseScroll.down || m_Keys[KEY_S].bHeld)
            {
               scale *= 0.98f;
            }

            float mouseXAfter = m_MousePosition.x;
            float mouseYAfter = m_MousePosition.y;

            ScreenToWorld(mouseXAfter, mouseYAfter);

            offset.x -= (mouseXBefore - mouseXAfter);
            offset.y -= (mouseYBefore - mouseYAfter);

            prevOffset = offset;
         }
         // reset view
         if (m_Keys[KEY_R].bPressed)
         {
            scale = 0.38;
            offset.x = 450;
            offset.y = -25;
            prevOffset = offset;
         }

         if (m_Keys[KEY_UP].bPressed)
         {
            numIterations += 10;
         }
         else if (m_Keys[KEY_DOWN].bPressed)
         {
            numIterations -= 10;
         }
      }

      // creating our gpu transformation mat4
      float mvp[16] = {
            1.0f / (scale), 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / (scale * mRatio), 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -offset.x * mRatio * mousePanFactor / m_ScreenWidth, -offset.y * mousePanFactor / m_ScreenHeight, 0.0f, 1.0f };

      // converting from an array of floats to a mat4
      mat4 mvpMatrix(mvp);

      /*
         Here we update our canvas sprite based on the current screen size.
         We then set our shader uniforms, including both the number of simulation
         iterations and transform matrix.
       */
      canvas = Sprite(0, 0, m_ScreenWidth, m_ScreenHeight, Vec4(1.0f, 1.0f, 1.0f, 0), m_Shader);
      mat4 projection = mat4::orthographic(0, m_ScreenWidth, 0, m_ScreenHeight, -1, 1);
      m_Shader->setUniformMat4("pr_matrix", projection);
      m_Shader->setUniform1i("numIterations", numIterations);
      m_Shader->setUniformMat4("uMVPMatrix", mvpMatrix);
      m_Renderer->submitSprite(&canvas);

      // setting a minimum number of iterations
      // bad things happen if we set it to zero 
      if (numIterations < 5 || numIterations == NULL)
      {
         numIterations = 5;
      }

      Menu();

      return true;
   }

   void Menu()
   {
      ImGui::Begin("Mandelbrot Viewer", 0, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("Authored 2021 Zach Brock");
      ImGui::Text("Press 'R' to reset view, 'W' to zoom in, 'S' to zoom out");
      ImGui::Text("Press 'UP/DOWN' to increase/decrease interations by 10");
      ImGui::Text("Current FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Scale: %.3f", scale);
      ImGui::Text("Ratio: %0.3f", mRatio);
      ImGui::Text("OffsetX: %.3f", offset.x);
      ImGui::Text("OffsetY: %.3f ", offset.y);
      ImGui::SliderInt("Iterations: ", &numIterations, 5, 1500);
      ImGui::SetWindowFontScale(1.25f);
      ImGui::End();
   }

};


int main()
{
   const char* title = "Mandelbrot Viewer";
   unsigned int screenWidth = 1000, screenHeight = 600;
   Fractal mandelbrotSet(title, screenWidth, screenHeight);

   mandelbrotSet.Start();
}