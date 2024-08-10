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

    cdialect "C17"

    files { "./src/**.h", "./src/**.c" }

    externalincludedirs { "./libs/miniaudio" }

    filter { "system:linux" }
    do
        links { "m", "pthread", "dl" }
    end

    filter { "system:bsd" }
    do
        links { "m", "pthread" }
    end

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
