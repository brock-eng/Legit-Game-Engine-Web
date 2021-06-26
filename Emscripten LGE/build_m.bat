em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 "mandelbrot.cpp" "LegitEngineCore/src/utils/ImGui/imgui.cpp" "LegitEngineCore/src/utils/ImGui/imgui_draw.cpp" "LegitEngineCore/src/utils/ImGui/imgui_impl_glfw.cpp" "LegitEngineCore/src/utils/ImGui/imgui_impl_opengl3.cpp" "LegitEngineCore/src/utils/ImGui/imgui_tables.cpp" "LegitEngineCore/src/utils/ImGui/imgui_widgets.cpp"  -o output.html  -s LLD_REPORT_UNDEFINED -s USE_GLFW=3 -s FULL_ES3=1 --emrun
pause