-- premake5.lua
workspace "trantor"
   configurations { "Debug", "Release" }
   architecture "x64"
   startproject "server"

   -- Set C++ standard
   cppdialect "C++17"

   -- Output directories
   targetdir ("bin/%{cfg.buildcfg}")
   objdir ("bin-int/%{cfg.buildcfg}")

   -- Options from CMake
   newoption {
      trigger = "build-c-ares",
      description = "Build C-ARES"
   }

   newoption {
      trigger = "use-spdlog",
      description = "Allow using the spdlog logging library"
   }

project "trantor"
   kind "StaticLib"  -- Change to "SharedLib" if BUILD_SHARED_LIBS is enabled
   language "C++"

   -- Files and directories
   files {
      "src/**.h",
      "src/**.cpp",
      "trantor/**.h",
      "trantor/**.cpp"
   }

   includedirs {
      "%{prj.location}",
      "%{prj.location}/trantor/utils",
      "%{prj.location}/trantor/net",
      "%{prj.location}/trantor/net/inner",
      "%{prj.location}/third_party/wepoll"
   }

   -- Compiler options
   filter "system:windows"
      defines { "_WIN32_WINNT=0x0601" }
      buildoptions { "/wd4251", "/wd4275" }  -- MSVC warnings suppression
      defines { "MSVC_COMPILER" }
   filter "system:linux or macosx"
      buildoptions { "-I /usr/local/include -g -Wall -Wextra -Werror -Wno-unused-parameter" }
   filter "system:haiku"
      links { "network" }

   -- Export header generation (Placeholder, requires custom implementation in Premake)

   -- Generate Doxygen (Manual step, as Premake doesn't directly support Doxygen)

-- Server application
project "server"
   kind "ConsoleApp"
   language "C++"
   files { "src/TcpServerTest.cc" }
   targetdir ("bin/%{cfg.buildcfg}")
   links { "trantor", "SQLiteCpp", "sqlite3", "pthread", "dl" }
   includedirs { "%{wks.location}/third_party/SQLiteCpp/include" }

-- Client application
project "client"
   kind "ConsoleApp"
   language "C++"
   files { "src/TcpClientTest.cc" }
   targetdir ("bin/%{cfg.buildcfg}")
   links { "trantor" }