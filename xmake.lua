add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

add_requires("levilamina 26.20.*", {configs = {target_type = get_config("target_type")}})

add_requires("levibuildscript")
add_requires("preloader 1.15.7")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("tests")
    set_default(false)
    set_showmenu(true)
    set_description("Enable tests")

option("target_type")
    set_default("server")
    set_showmenu(true)
    set_values("server", "client")
option_end()

target("more-dimensions")
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")
     if is_plat("windows") then
        add_defines("NOMINMAX", "UNICODE", "MORE_DIMENSIONS_EXPORTS")
        set_exceptions("none") -- To avoid conflicts with /EHa.
        add_cxflags( "/EHa", "/utf-8", "/W4", "/w44265", "/w44289", "/w44296", "/w45263", "/w44738", "/w45204")
        add_cxflags(
            "/EHs",
            "-Wno-microsoft-cast",
            "-Wno-invalid-offsetof",
            "-Wno-c++2b-extensions",
            "-Wno-microsoft-include",
            "-Wno-overloaded-virtual",
            "-Wno-ignored-qualifiers",
            "-Wno-missing-field-initializers",
            "-Wno-potentially-evaluated-expression",
            "-Wno-pragma-system-header-outside-header",
            {tools = {"clang_cl"}}
        )
        set_toolchains("clang-cl")
    end
    add_packages("levilamina", "snappy", "preloader")
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")
    add_files(
        "src/more_dimensions/**.cpp"
    )
    add_includedirs(
        "src"
    )
    add_headerfiles(
        "src/(more_dimensions/api/**.h)",
        "src/(more_dimensions/core/Macros.h)"
    )
    if is_config("tests", true) then
        add_files("src/test/TestCustomDimension.cpp",
                "src/test/generator/flat-gen-village/**.cpp",
                "src/test/generator/generator-terrain/**.cpp")
    end
