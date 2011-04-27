-- this is a premake4 script
-- see http://industriousone.com/premake

solution "tememu"
    language "C++"
    configurations { "debug", "release" }
    includedirs { "./thirdparty/include/" }

    configuration { "debug" }
        targetdir "build/debug"

    configuration { "release" }
        targetdir "build/release"

    project "test"
        kind     "ConsoleApp"
        files    { "./src/**.h", "./src/**.cpp", "./test/main.cpp" }

        configuration { "debug" }
            defines { "_DEBUG" }
            flags   { "Symbols" }

        configuration { "release" }
            defines { "NDEBUG", "RELEASE" }
            flags   { "Optimize" }

    project "tememu"
        kind     "StaticLib"
        files    { "./src/**.h", "./src/**.cpp" }

        configuration { "debug" }
            defines { "_DEBUG" }
            flags   { "Symbols" }

        configuration { "release" }
            defines { "NDEBUG", "RELEASE" }
            flags   { "Optimize" }
        
