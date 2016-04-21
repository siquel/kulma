function kulma_project(_name, _kind)
    project(_name)
        kind(_kind)

        includedirs {
            KULMA_DIR .. "include"
        }

        configuration { "linux-*" }
            links {
                "X11",
                "GL"
            }

        flags {
            "NoRTTI",
            "NoExceptions",
            "FatalWarnings"
        }

        files {
            KULMA_DIR .. "src/**.h",
            KULMA_DIR .. "src/**.cpp"
        }
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

    flags {
        "FatalWarnings",
        "NoRTTI",
        "NoExceptions",
    }

    links {
        "kulma"
    }

end
