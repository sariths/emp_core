premake.modules.lua = {}
local m = premake.modules.lua

local p = premake

newaction {
	trigger = "intelTBB",
	description = "Builds the Intel TBB library",

	onStart = function()
		print("About to compile, move and clean the Intel TBB library")
	end,

	execute = function()
        
        -- Compile tbb in Release and Debug format. Will create the 
        --tbb_release and tbb_debug directories in ./build
        os.execute("cd 3rdparty/IntelTBB;make tbb_build_prefix='tbb' tbb")

        mv = "mv"
        if is_windows then
            mv = "move"
        end
        -- Move results to Lib
        os.execute(mv.." 3rdparty/IntelTBB/build/tbb_release libs/RELEASE/tbb")
        os.execute(mv.." 3rdparty/IntelTBB/build/tbb_debug libs/DEBUG/tbb") 
    
           
          
	end,

	onEnd = function()
		print("Finished compiling Intel TBB")
	end
}

return m