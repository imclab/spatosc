----------------------------------------------------------------------
-- Premake4 configuration script for spatosc
-- Adapted from ODE's build script by Jason Perkins.
-- For more information on Premake: http://industriousone.com/premake
----------------------------------------------------------------------

----------------------------------------------------------------------
-- Configuration options
----------------------------------------------------------------------

  -- always clean all of the optional components and toolsets
  if _ACTION == "clean" then
    for action in pairs(premake.actions) do
      os.rmdir(action)
    end
  end

----------------------------------------------------------------------
-- The solution, and solution-wide settings
----------------------------------------------------------------------

  solution "spatosc"

    language "C"
    location ( _OPTIONS["to"] or _ACTION )

    includedirs {
      "../include",
      "../src"
    }
      
    -- define all the possible build configurations
    configurations {
--      "DebugDLL", "ReleaseDLL", 
      "DebugLib", "ReleaseLib", 
    }
    
    configuration { "Debug*" }
      defines { "_DEBUG" }
      flags   { "Symbols" }
      
    configuration { "Release*" }
      flags   { "OptimizeSpeed", "NoFramePointer" }

    configuration { "Windows" }
      defines { "WIN32" }

    -- give each configuration a unique output directory
    for _, name in ipairs(configurations()) do
      configuration { name }
        targetdir ( "../lib/" .. name )
    end
      
    -- disable Visual Studio security warnings
    configuration { "vs*" }
      defines { "_CRT_SECURE_NO_DEPRECATE" }

    -- tell source to use config.h
    configuration { "vs*" }
      defines { "HAVE_CONFIG_H" }

    -- don't remember why we had to do this	(from ODE)
    configuration { "vs2002 or vs2003", "*Lib" }
      flags  { "StaticRuntime" }

----------------------------------------------------------------------
-- Write a custom <config.h> to .., based on the supplied flags
----------------------------------------------------------------------

-- First get the version number from "configure.ac" --

  io.input("../configure.ac")
  text = io.read("*all")
  io.close()
  text = string.sub(text,string.find(text, "AC_INIT.+"))
  version = string.sub(text,string.find(text, "%d+%.%d+.%d+"))

-- Replace it in "config.h" --

  io.input("config-msvc.h")
  local text = io.read("*all")

  text = string.gsub(text, '/%*VERSION%*/', '"'..version..'"')

  io.output("../config.h")
  io.write(text)
  io.close()


----------------------------------------------------------------------
-- The Spatosc library project
----------------------------------------------------------------------

  project "spatosc"

    kind     "StaticLib"
    location ( _OPTIONS["to"] or _ACTION )

    includedirs {
      "..",
    }

    files {
      "../src/*.cpp",
      "../include/*.h",
    }

    excludes {
      "../src/tests/*"
    }

    configuration { "windows" }
      links   { "liblo",
                "ws2_32",
              }
            
    configuration { "*Lib" }
      kind    "StaticLib"
      defines "LIBSPATOSC_LIB"
      
--    configuration { "*DLL" }
--      kind    "SharedLib"
--      defines "LIBSPATOSC_DLL"

    configuration { "Debug*" }
      targetname "libspatosc_d"
      
    configuration { "Release*" }
      targetname "libspatosc"

