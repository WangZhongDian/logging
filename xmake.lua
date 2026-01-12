set_project("logging")
set_version("1.1.0")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})
set_defaultmode("debug")

add_includedirs("include",{public = true})

option("shared")
    set_default(false)
    set_showmenu(true)

target("logging")
    if has_config("shared") then
        set_kind("shared")
    else
        set_kind("static")
    end

    add_files("src/*.c", "src/filter/*.c", "src/handler/*.c", "src/utils/*.c")
    add_headerfiles("include/*.h", "include/(logging/*.h)")

includes("tests")