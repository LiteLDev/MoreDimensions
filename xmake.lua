add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

add_requires("levilamina")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

option("tests")
    set_default(false)
    set_showmenu(true)
    set_description("Enable tests")

target("more-dimensions") -- Change this to your plugin name.
    add_cxflags(
        "/EHa", -- To catch both structured (asynchronous) and standard C++ (synchronous) exceptions.
        "/utf-8" -- To enable UTF-8 source code.
    )
    add_defines(
        "_HAS_CXX23=1", -- To enable C++23 features
        "MORE_DIMENSIONS_EXPORTS", -- To export symbols in Windows.
        "NOMINMAX", -- To avoid conflicts with std::min and std::max.
        "UNICODE" -- To enable Unicode support in Windows API.
    )
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
    add_packages(
        "levilamina"
    )
    add_shflags(
        "/DELAYLOAD:bedrock_server.dll" -- To use forged symbols of SymbolProvider.
    )
    set_exceptions("none") -- To avoid conflicts with /EHa.
    set_kind("shared")
    set_symbols("debug")
    set_languages("cxx20")

    if has_config("tests") then
        add_files("src/test/TestCustomDimension.cpp")
    end

    after_build(function (target)
        local plugin_packer = import("scripts.after_build")

        local plugin_define = {
            pluginName = target:name(),
            pluginFile = path.filename(target:targetfile()),
        }
        
        plugin_packer.pack_plugin(target,plugin_define)
    end)
