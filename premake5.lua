-- DAWN

workspace "Kurai"
    configurations {"Debug", "Release"}
    architecture "x86_64"
    
    startproject "Game"

    include "Engine"
    include "Game"