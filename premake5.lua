workspace "OpenGL"
configurations { "Debug", "Release" }
platforms { "x64" }
startproject "OpenGL"

project "OpenGL"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
staticruntime "Off"
systemversion "latest"

targetdir "%{wks.location}/bin"
objdir "%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.platform}/%{prj.name}"
debugdir "%{wks.location}/bin"

files {
    "OpenGL/src/**.h",
    "OpenGL/src/**.hpp",
    "OpenGL/src/**.c",
    "OpenGL/src/**.cpp",
    "OpenGL/src/shaders/**",
    "vendor/glad/include/**.h",
    "vendor/glad/src/glad.c"
}

includedirs {
    "OpenGL/src",
    "vendor/glad/include",
    "vendor/SDL3/include",
    "vendor/stbimage"
}

links {
    "SDL3"
}

filter "system:windows"
defines { "_CONSOLE" }
links { "opengl32.lib" }

filter "platforms:x64"
architecture "x86_64"
libdirs { "vendor/SDL3/lib/x64" }
postbuildcommands {
    '{COPY} "%{wks.location}/vendor/SDL3/lib/x64/SDL3.dll" "%{cfg.targetdir}"',
    '{COPYDIR} "%{wks.location}/OpenGL/src/shaders" "%{cfg.targetdir}/shaders"',
    '{COPYDIR} "%{wks.location}/OpenGL/src/shaders" "%{wks.location}/shaders"'
}

filter "configurations:Debug"
defines { "_DEBUG" }
symbols "On"

filter "configurations:Release"
defines { "NDEBUG" }
optimize "On"
