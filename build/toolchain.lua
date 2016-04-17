

function toolchain(build_dir, lib_dir)

	-- support clang
	newoption
	{
		trigger = "compiler",
		value   = "COMPILER",
		description = "Define compiler",
		allowed = 
		{
			{ "linux-gcc", "Linux GCC compiler"},
			{ "linux-clang", "Linux Clang compiler"}
		}
	}

	if (_ACTION == nil) then return end

	if _ACTION == "clean" then
		os.rmdir(BUILD_DIR)
	end

	if _ACTION == "gmake" then
		if nil == _OPTIONS["compiler"] then
			print("Choose a compiler!")
			os.exit(1)
		end

		if "linux-gcc" == _OPTIONS["compiler"] then
		elseif "linux-clang" == _OPTIONS["compiler"] then
			premake.gcc.cc = "clang"
			premake.gcc.cxx = "clang++"
		end
	end

	flags {
		"StaticRuntime",
		"Unicode",
		"NoPCH",
		"NoRTTI",
		"NoExceptions",
		"Symbols",
		"FatalWarnings"
	}

	configuration { "debug", "x86" }
		targetsuffix "-debug-86"
	configuration { "debug", "x64" }
		targetsuffix "-debug-64"

	configuration { "release", "x86" }
		targetsuffix "-release-86"
	configuration { "release", "x64" }
		targetsuffix "-release-64"

	configuration { "x86", "linux-*", "debug"}
		targetdir (build_dir .. "x86/debug")
		objdir (build_dir .. "obj/x86/debug")

	configuration { "x86", "linux-*", "release"}
		targetdir (build_dir .. "x86/release")
		objdir (build_dir .. "obj/x86/release")

	configuration { "x64", "linux-*", "debug"}
		targetdir (build_dir .. "x64/debug")
		objdir (build_dir .. "obj/x64/debug")

	configuration { "x64", "linux-*", "release"}
		targetdir (build_dir .. "x64/release")
		objdir (build_dir .. "obj/x64/release")

	configuration { "linux-*" }
		buildoptions {
			"-Wall",
			"-Wextra"
		}
end