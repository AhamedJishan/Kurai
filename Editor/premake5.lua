-- Editor

project "Editor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    systemversion "latest"

    targetdir   "%{wks.location}/bin/%{cfg.buildcfg}/%{prj.name}"
    objdir      "%{wks.location}/bin-int/%{cfg.buildcfg}/%{prj.name}"

    -- Relative paths for Assets
    debugdir "%{cfg.targetdir}"

    includedirs {
        "src/",
        "../Engine/src",
        "../Engine/src/Vendor",
        "../Engine/vendor/include",
    }

    -- libdirs { } Not needed right now

    links {
        "Engine"
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"