function kulma_project(_name, _kind)
    project(_name)
        kind(_kind)

        includedirs {
            path.join(KULMA_DIR, "include")
        }

        files {
            path.join(KULMA_DIR, "src", "**.h"),
            path.join(KULMA_DIR, "src", "**.cpp")
        }

        configuration { "linux-*" }
            links {
                "X11",
                "GL",
        }
        
        configuration {}
end

function kulma_example_project(_name)

    project("example-" .. _name)
        kind "ConsoleApp"

    configuration {}

    includedirs {
        path.join(KULMA_DIR, "include")
    }

    files {
        path.join(KULMA_DIR, "examples", _name, "**.cpp"),
        path.join(KULMA_DIR, "examples", _name, "**.h")
    }

    links {
        "kulma"
    }

    configuration {}
end
