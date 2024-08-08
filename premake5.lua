-- To avoid auto-formatting with neovim:
-- :noautocmd w
local libs = {
    {
        name="portaudio",
        extracted_name="portaudio-19.7.0",
        url="https://github.com/PortAudio/portaudio/archive/refs/tags/v19.7.0.zip",
    }
}

if not os.isdir("./libs") then
    os.mkdir("./libs")
end

for i, lib in ipairs(libs) do
    local file_name = "./libs/" .. lib.name .. ".zip";
    local result_str, response_code = http.download(lib.url, file_name)

    if result_str == "OK" then
        print(lib.name .. " downloaded\n")
        zip.extract(file_name, "./libs/")
        os.rename("./libs/" .. lib.extracted_name, "./libs/" .. lib.name)
    end
end


workspace "AsciiJuke"
do
    configurations { "Debug", "Release" }
    location "./"

    platforms { "x64", "x32" }

    flags "FatalWarnings"
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
