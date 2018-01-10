
project "emp"
    kind "ConsoleApp"
    language "C++"
    buildoptions { '-std=c++11' }

    files { 
        "../main.cpp",
        "../main.h",
        "../src/**",        
    }

    links {
        "Lua", "radiance","raycalls","rtrad"
    }

    includedirs{
        "../src/",
        third_party_dir,
        third_party_dir.."/intelTBB/include",
        lua_dir,
        third_party_dir.."/Radiance/src/common",        
    }
    
    -- Add the platform specific
    if is_windows then
        defines { "WIN" }    
        links {
            third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }
        includedirs {
            third_party_dir.."/SketchUp/WIN/headers", 
        }    
    elseif is_macos then
        defines { "MACOS" }    
        links {
            "tbb_debug",
            third_party_dir.."/SketchUp/MACOS/headers/SketchUpAPI.framework",
            "GoogleTest",
            "Lua",
            "radiance",
            "raycalls",
            "rtrad"                                     
        }  
        targetdir "../bin"
        buildoptions {"-F "..third_party_dir.."/SketchUp/MACOS/headers"}
        linkoptions {
            "-F "..third_party_dir.."/SketchUp/MACOS/headers", 
            "-L "..libs_dir.."/%{cfg.buildcfg}/tbb"
        }    
    elseif is_linux then
        defines { "LINUX", "AVOID_SKP" }    
        links {
            --third_party_dir.."/SketchUp/WIN/binaries/sketchup/x64/*",            
            third_party_dir.."/intelTBB/lib/intel64/vc14/*",            
        }

    end

    filter "configurations:Debug"
    files {
        third_party_dir.."/nvwa/nvwa/debug_new.cpp", 
    }
    includedirs{
        third_party_dir.."/nvwa/nvwa",     
    }

