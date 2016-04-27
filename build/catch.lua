project "unit_test"
    kind("ConsoleApp")

    removeflags {
        "NoExceptions",
        "NoRTTI",
    }
    
    files {
        path.join(KULMA_DIR, "tests", "**.cpp")
    }

    links {
        "kulma"
    }

    includedirs {
        path.join(KULMA_DIR, "include"),
        KULMA_THIRDPARTY_DIR
    }
    