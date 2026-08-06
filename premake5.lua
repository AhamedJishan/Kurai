workspace "Kurai"
    configurations {"Debug", "Release"}
    architecture "x86_64"
    
    startproject "DawnEditor"

    GameLibName = "KuraiCore"
    GameLibIncludeDir = "%{wks.location}/KuraiCore/src"

    include "Dawn"
    include "KuraiCore"
    include "DawnEditor"
    include "DawnPlayer"