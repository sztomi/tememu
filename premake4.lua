-- this is a premake4 configuration file
-- see http://industriousone.com/premake

solution "tememu"
    configurations { "debug", "release" }

    configuration { "debug" }
        targetdir "build/debug"

    configuration { "release" }
        targetdir "build/release"

    project "test"
        language "C++"
        kind     "ConsoleApp"
        files    { "./src/**.h", "./src/**.cpp", "./test/main.cpp" }

        configuration { "debug" }
            defines { "_DEBUG" }
            flags   { "Symbols" }

        configuration { "release" }
            defines { "NDEBUG", "RELEASE" }
            flags   { "Optimize" }

    project "libtememu"
        language "C++"
        kind     "StaticLib"
        files    { "./src/**.h", "./src/**.cpp" }

        configuration { "debug" }
            defines { "_DEBUG" }
            flags   { "Symbols" }

        configuration { "release" }
            defines { "NDEBUG", "RELEASE" }
            flags   { "Optimize" }
        
