workspace "AsciiJuke"
do
    configurations { "Debug", "Release" }
    location "./"

    platforms { "x64", "x32" }

    --flags "FatalWarnings"
    warnings "Extra"
end

project "AsciiJuke"
do
    location "./project"
    kind "ConsoleApp"
    language "C"
    targetdir "./bin/%{cfg.buildcfg}"
    toolset "clang"
    cdialect "C11"

    files { "./src/**.h", "./src/**.c" }

    externalincludedirs { "./libs/miniaudio" }

    filter "configurations:Debug"
    do
        defines { "DEBUG" }
        symbols "On"
    end
    filter "configurations:Release"
    do
        defines { "NDEBUG" }
        optimize "On"
    end
end
