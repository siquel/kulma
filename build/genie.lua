KULMA_DIR = path.getabsolute("..") .. "/"
local KULMA_BUILD_DIR = KULMA_DIR .. ".build/"
local KULMA_THIRDPARTY_DIR = KULMA_DIR .. "3rdparty/"

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
