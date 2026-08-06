-- Dawn aka the Engine

project "Dawn"
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
        "src/Vendor",
        "vendor/include"
    }

    libdirs {
        "vendor/lib"
    }

    links {
        "glfw3",
        "opengl32",
        "assimp",
        "fmod_vc",
        "fmodstudio_vc"
        -- YAML is linked at the bottom
    }

    defines
    {
        "YAML_CPP_STATIC_DEFINE"
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

        links {
            "yaml-cppd"
        }
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"

        links {
            "yaml-cpp"
        }