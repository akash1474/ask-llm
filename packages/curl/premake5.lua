project "cURL"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"lib/**.c",
		"include/**.h"
	}
	
	includedirs{"include","lib"}
	defines { "BUILDING_LIBCURL","CURL_STATICLIB","HTTP_ONLY" }

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		defines { "_CRT_SECURE_NO_WARNINGS", "USE_WINDOWS_SSPI" }

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

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
