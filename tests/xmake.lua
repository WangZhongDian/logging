add_deps("logging")

set_default(false)

target("test-simple")
    add_files("test-simple.c")
    add_tests("defualt")

target("test-map")
    add_files("test-map.c")
    add_includedirs("../src")
    add_tests("defualt")

target("test-logs")
    add_files("test-logs.c")
    add_tests("defualt")

target("test-log-file")
    add_files("test-log-file.c")
    add_tests("defualt")

target("test-filter")
    add_files("test-filter.c")
    add_tests("defualt")