-- DawnPlayer

project "DawnPlayer"
    kind "ConsoleApp"
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
        "%{wks.location}/Dawn/src/Vendor",
        "%{wks.location}/Dawn/vendor/include",
        GameLibIncludeDir
    }

    -- libdirs {}

    links {
        "Dawn",
        GameLibName
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    postbuildcommands {
        -- Assets
        "{RMDIR} %{cfg.targetdir}/Assets",
        "{COPYDIR} %{wks.location}/Assets %{cfg.targetdir}/Assets",
        -- Assimp
        "{DELETE} %{cfg.targetdir}/assimp.dll",
        "{DELETE} %{cfg.targetdir}/assimp.pdb",
        "{COPY} %{wks.location}/Dawn/vendor/bin/assimp.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/Dawn/vendor/bin/assimp.pdb %{cfg.targetdir}",
        -- FMOD
        "{DELETE} %{cfg.targetdir}/fmod.dll",
        "{DELETE} %{cfg.targetdir}/fmodstudio.dll",
        "{COPY} %{wks.location}/Dawn/vendor/bin/fmod.dll %{cfg.targetdir}",
        "{COPY} %{wks.location}/Dawn/vendor/bin/fmodstudio.dll %{cfg.targetdir}"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "RELEASE"
        optimize "On"