if not os.isdir("./libs") then
    os.mkdir("./libs")
end

-- PORTAUDIO
if not os.isfile("./libs/libportaudio.a") then
    os.chdir("./libs/portaudio")
    os.execute("./configure && make")
    os.copyfile("./lib/.libs/libportaudio.a", "../libportaudio.a")
    os.chdir("..")
    os.chdir("..")
end

-- CONFIG

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

    externalincludedirs { "./libs/portaudio/include" }

    libdirs { "./libs/" }
    links { ":libportaudio.a", "rt", "m", "asound" }

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
