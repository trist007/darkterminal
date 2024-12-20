-- premake5.lua
require "generate"

workspace "trantor"
   configurations { "Debug", "Release" }
   architecture "x86_64"

   -- Set C++ standard
   cppdialect "C++20"

   GenerateExportHeader( "trantor/exports.h" )

   -- Output directories
   targetdir ("bin/%{cfg.buildcfg}")
   objdir ("bin-int/%{cfg.buildcfg}")

--   -- Options from CMake
--   newoption {
--      trigger = "build-c-ares",
--      description = "Build C-ARES"
--   }
--
--   newoption {
--      trigger = "use-spdlog",
--      description = "Allow using the spdlog logging library"
--   }

project "trantor"
    kind "StaticLib"  -- Change to "SharedLib" if BUILD_SHARED_LIBS is enabled
    defines { "BYTE_ORDER=LITTLE_ENDIAN" }
    language "C++"

   -- Files and directories
   files {
--      "trantor/**.h",
--      "trantor/**.cc",
--      "third_party/**.h",
--      "third_party/**.c"
        "trantor/utils/AsyncFileLogger.cc",
        "trantor/utils/ConcurrentTaskQueue.cc",
        "trantor/utils/Date.cc",
        "trantor/utils/LogStream.cc",
        "trantor/utils/Logger.cc",
        "trantor/utils/MsgBuffer.cc",
        "trantor/utils/SerialTaskQueue.cc",
        "trantor/utils/TimingWheel.cc",
        "trantor/utils/Utilities.cc",
        --"trantor/utils/crypto/blake2.cc",
        --"trantor/utils/crypto/sha1.cc",
        --"trantor/utils/crypto/sha3.cc",
        --"trantor/utils/crypto/sha256.cc",
        --"trantor/utils/crypto/md5.cc",
        --"trantor/utils/crypto/botan.cc",
        "trantor/net/inner/tlsprovider/OpenSSLProvider.cc",
        "trantor/utils/crypto/openssl.cc",
        "trantor/net/inner/AresResolver.cc",
        "trantor/net/EventLoop.cc",
        "trantor/net/EventLoopThread.cc",
        "trantor/net/EventLoopThreadPool.cc",
        "trantor/net/InetAddress.cc",
        "trantor/net/TcpClient.cc",
        "trantor/net/TcpServer.cc",
        "trantor/net/Channel.cc",
        "trantor/net/inner/Acceptor.cc",
        "trantor/net/inner/Connector.cc",
        "trantor/net/inner/Poller.cc",
        "trantor/net/inner/Socket.cc",
        "trantor/net/inner/MemBufferNode.cc",
        "trantor/net/inner/StreamBufferNode.cc",
        "trantor/net/inner/AsyncStreamBufferNode.cc",
        "trantor/net/inner/FileBufferNodeUnix.cc",
        "trantor/net/inner/TcpConnectionImpl.cc",
        "trantor/net/inner/Timer.cc",
        "trantor/net/inner/TimerQueue.cc",
        "trantor/net/inner/poller/EpollPoller.cc",
        "trantor/net/inner/poller/KQueue.cc",
        "trantor/net/inner/poller/PollPoller.cc",
        "trantor/net/inner/Acceptor.h",
        "trantor/net/inner/Connector.h",
        "trantor/net/inner/Poller.h",
        "trantor/net/inner/Socket.h",
        "trantor/net/inner/TcpConnectionImpl.h",
        "trantor/net/inner/Timer.h",
        "trantor/net/inner/TimerQueue.h",
        "trantor/net/inner/poller/EpollPoller.h",
        "trantor/net/inner/poller/KQueue.h",
        "trantor/net/inner/poller/PollPoller.h",
        "trantor/net/inner/BufferNode.h",
        "trantor/utils/AsyncFileLogger.h",
        "trantor/utils/ConcurrentTaskQueue.h",
        "trantor/utils/Date.h",
        "trantor/utils/Funcs.h",
        "trantor/utils/LockFreeQueue.h",
        "trantor/utils/LogStream.h",
        "trantor/utils/Logger.h",
        "trantor/utils/MsgBuffer.h",
        "trantor/utils/NonCopyable.h",
        "trantor/utils/ObjectPool.h",
        "trantor/utils/SerialTaskQueue.h",
        "trantor/utils/TaskQueue.h",
        "trantor/utils/TimingWheel.h",
        --"trantor/utils/crypto/blake2.h",
        --"trantor/utils/crypto/md5.h",
        --"trantor/utils/crypto/sha256.h",
        --"trantor/utils/crypto/sha1.h",
        --"trantor/utils/crypto/sha3.h",
        "trantor/utils/Utilities.h"
   }

    includedirs {
       "%{prj.location}",
       "%{prj.location}/trantor",
       "%{prj.location}/trantor/utils",
       "%{prj.location}/trantor/net",
       "%{prj.location}/trantor/net/inner",
       "%{prj.location}/third_party/wepoll"
       --"/usr/local/Cellar/botan/3.6.1/include/botan-3"
    }

   -- Compiler options
   filter "system:windows"
      defines { "_WIN32_WINNT=0x0601" }
      buildoptions { "/wd4251", "/wd4275" }  -- MSVC warnings suppression
      defines { "MSVC_COMPILER" }
      defines { "WIN32" }
   filter "system:linux or macosx"
      buildoptions { "-I /usr/local/include -g -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter -Wno-deprecated-pragma" }
   filter "system:haiku"
      links { "network" }

   -- Export header generation (Placeholder, requires custom implementation in Premake)

   -- Generate Doxygen (Manual step, as Premake doesn't directly support Doxygen)

-- Server application
project "server"
    kind "ConsoleApp"
    defines { "BYTE_ORDER=LITTLE_ENDIAN" }
    language "C++"
    files { "src/TcpServerTest.cc" }
    targetdir ("bin/%{cfg.buildcfg}")
    libdirs { "bin/Debug" }
    links { "trantor", "SQLiteCpp", "sqlite3", "ssl", "crypto", "cares", "pthread", "dl" }
    includedirs { "%{prj.location}/third_party/SQLiteCpp/include", "%{prj.location}" }
 
-- -- Client application
project "client"
    kind "ConsoleApp"
    defines { "BYTE_ORDER=LITTLE_ENDIAN" }
    language "C++"
    files { "src/TcpClientTest.cc" }
    targetdir ("bin/%{cfg.buildcfg}")
    links { "trantor" }
    includedirs { "%{prj.location}", "trantor/utils/Utilities.h" }
