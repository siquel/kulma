KULMA_DIR = path.getabsolute("..") .. "/"
KULMA_BUILD_DIR = KULMA_DIR .. ".build/"
KULMA_THIRDPARTY_DIR = KULMA_DIR .. "3rdparty/"

solution "kulma"
	configurations {
		"debug",
		"release"
	}

	platforms {
		"x32",
		"x64"
	}

	language "C++"

	configuration {}

dofile("toolchain.lua")

toolchain(KULMA_BUILD_DIR, KULMA_THIRDPARTY_DIR)
dofile("kulma.lua")

group "engine"
kulma_project("kulma", "StaticLib")

group "examples"
kulma_example_project("00-helloworld")

group "unit_test"
    project "unit_test"
    kind("ConsoleApp")
    files {
        path.join(KULMA_DIR, "tests", "**.cpp")
    }
    links {
        "kulma"
    }
    includedirs {
        KULMA_THIRDPARTY_DIR
    }
    
