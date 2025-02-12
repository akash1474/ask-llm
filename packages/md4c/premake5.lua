project "md4c"
	kind "StaticLib"
	language "C"
	warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.c"
	}
	includedirs{"src"}

	defines{"DMD4C_USE_UTF8"}

	filter "system:windows"
		systemversion "latest"
		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		optimize "off"
		staticruntime "On"
      	buildoptions { "/MP" }

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		staticruntime "On"
      	buildoptions { "/MP" }

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
		staticruntime "On"
        buildoptions { "/MP" }