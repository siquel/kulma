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

KULMA_DIR = path.getabsolute("..") .. "/"
KULMA_BUILD_DIR = KULMA_DIR .. ".build/"
KULMA_THIRDPARTY_DIR = KULMA_DIR .. "3rdparty/"
KALUKETJU_DIR = path.join(KULMA_DIR, "kaluketju")

dofile(path.join(KALUKETJU_DIR, "scripts", "toolchain.lua"))

if not toolchain(KULMA_BUILD_DIR, KULMA_THIRDPARTY_DIR) then return end    
dofile("catch.lua")
dofile("kulma.lua")

group "engine"
kulma_project("kulma", "StaticLib")

group "examples"
kulma_example_project("00-helloworld")

