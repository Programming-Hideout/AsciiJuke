if not os.isdir("./libs") then
    os.mkdir("./libs")
end

-- PORTAUDIO
if not os.isfile("./libs/libportaudio.a") then
    local result_str, response_code = http.download(
        "https://github.com/PortAudio/portaudio/archive/refs/tags/v19.7.0.zip", "./libs/portaudio.zip")

    if result_str == "OK" then
        print("PortAudio downloaded\n")
        zip.extract("./libs/portaudio.zip", "./libs/")
        os.rename("./libs/portaudio-19.7.0", "./libs/portaudio")
        os.chdir("./libs/portaudio")
        os.execute("./configure && make")
        os.copyfile("./lib/.libs/libportaudio.a", "../libportaudio.a")
        os.chdir("..")
        os.rmdir("portaudio-19.7.0")
        os.remove("portaudio.zip")
        os.chdir("..")
        
    end
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
