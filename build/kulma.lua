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

        files {
            KULMA_DIR .. "src/**.h",
            KULMA_DIR .. "src/**.cpp"
        }
end
