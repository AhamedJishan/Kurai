-- DAWN

workspace "Dawn"
    configurations {"Debug", "Release"}
    architecture "x86_64"
    
    startproject "Game"

    include "Engine"
    include "Game"

    -- Engine for now, will be Application later