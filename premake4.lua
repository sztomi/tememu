-- this is a premake4 script
-- see http://industriousone.com/premake

solution "tememu"
    language "C++"
    configurations { "debug", "release" }
    includedirs { "./thirdparty/include/" }
    libdirs { "./thirdparty/lib" }

    configuration { "debug" }
        targetdir "build/debug"
        defines { "DEBUG" }

    configuration { "release" }
        targetdir "build/release"

    project "test"
        kind     "ConsoleApp"
        files    { "./src/**.h", "./src/**.cpp", "./test/main.cpp" }
        links { "gtest", "gtest_main", "pthread" }

        configuration { "debug" }
            flags   { "Symbols" }

        configuration { "release" }
            defines { "NDEBUG", "RELEASE" }
            flags   { "Optimize" }

    project "tememu"
        kind     "StaticLib"
        files    { "./src/**.h", "./src/**.cpp" }

        configuration { "debug" }
            flags   { "Symbols" }

        configuration { "release" }
            defines { "NDEBUG", "RELEASE" }
            flags   { "Optimize" }
        
