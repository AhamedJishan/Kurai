-- GAME

project "Game"
    kind "ConsoleApp"
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

        "../Engine/vendor/include",

        "../Engine/src/Vendor/"
    }

    --libdirs { }

    links {
        "Engine"
    }

    files {
        "src/**.h",
        "src/**.cpp"
    }

    postbuildcommands {
        -- Assets
        "{RMDIR} %{cfg.targetdir}/Assets",
        "{COPYDIR} %{wks.location}/Assets %{cfg.targetdir}/Assets",
        -- Assimp
        "{DELETE} %{cfg.targetdir}/assimp.dll",
        "{DELETE} %{cfg.targetdir}/assimp.pdb",
        "{COPY} %{wks.location}/Engine/vendor/bin/assimp.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/Engine/vendor/bin/assimp.pdb %{cfg.targetdir}",
        -- FMOD
        "{DELETE} %{cfg.targetdir}/fmod.dll",
        "{DELETE} %{cfg.targetdir}/fmodstudio.dll",
        "{COPY} %{wks.location}/Engine/vendor/bin/fmod.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/Engine/vendor/bin/fmodstudio.dll %{cfg.targetdir}"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"