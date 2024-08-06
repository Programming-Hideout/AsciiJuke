-- To avoid auto-formatting with neovim:
-- :noautocmd w

workspace "AsciiJuke"
    configurations {"Debug", "Release"}
    location "build"    
    
    platforms {"x64", "x32"}

    flags "FatalWarnings"
    warnings "Extra"

project "AsciiJuke"
    location "build/project"
    kind "ConsoleApp"
    language "C"
    targetdir "build/bin/%{cfg.buildcfg}"

    files { "./src/**.h", "./src/**.c" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    
    