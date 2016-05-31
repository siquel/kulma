function kulma_project(_name, _kind)
    project(_name)
        kind(_kind)
        
        flags {
            "FatalWarnings"
        }

        includedirs {
            path.join(KULMA_DIR, "include")
        }

        files {
            path.join(KULMA_DIR, "src", "**.h"),
            path.join(KULMA_DIR, "src", "**.cpp")
        }

        configuration { "linux-*" }
            links {
                "pthread",
                "rt",
                "X11",
                "Xrandr",
                "GL",
            }
            buildoptions {
                "-std=c++11"
            }
        
        configuration {}
end

function kulma_example_project(_name)

    project("example-" .. _name)
        kind "ConsoleApp"

    configuration {}
    
    flags {
        "FatalWarnings"
    }

    includedirs {
        path.join(KULMA_DIR, "include")
    }

    files {
        path.join(KULMA_DIR, "examples", _name, "**.cpp"),
        path.join(KULMA_DIR, "examples", _name, "**.h")
    }
    configuration { "linux-*" }
            links {
                "rt",
                "pthread",
                "X11",
                "Xrandr",
                "GL",
            }
            buildoptions {
                "-std=c++11"
            }

    configuration {}
            
    links {
        "kulma"
    }

    configuration {}
end
