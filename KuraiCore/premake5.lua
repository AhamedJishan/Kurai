-- Kurai Core aka the Game

project "KuraiCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    targetdir   "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    objdir      "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"
    
    -- Relative paths for Assets
    debugdir "%{cfg.targetdir}"

    includedirs {
        "src",
        "%{wks.location}/Dawn/src",
        "%{wks.location}/Dawn/src/Vendor/",
        "%{wks.location}/Dawn/vendor/include"
    }

    --libdirs { }

    links {
        "Dawn"
    }

    files {
        "src/**.h",
        "src/**.cpp"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"