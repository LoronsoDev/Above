--PREMAKE FOR ABOVE ENGINE

workspace "Above"
	architecture "x64"
	startproject "Sandbox"
	configurations
	{
		"Debug",
		"Release",	--Faster version of Debug
		"Dist"		--Everything stripped out, max performance
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Above/vendor/GLFW/include"

include "Above/vendor/GLFW"

	------------ENGINE PROJECT---------------

	project "Above"
		location "Above"
		kind "SharedLib"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "abpch.h"
		pchsource "%{prj.name}/src/abpch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.hxx",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}"
		}

		links
		{
			"GLFW",
			"opengl32.lib"
		}
		
		--Windows projects

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"AB_PLATFORM_WINDOWS",
				"AB_BUILD_DLL"
			}

			postbuildcommands
			{
				("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
			}

		filter "configurations:Debug"
			defines "AB_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "AB_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "AB_DIST"
			buildoptions "/MD"
			optimize "On"


	------------SANDBOX PROJECT---------------

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.hxx",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"Above/vendor/spdlog/include",
			"Above/src"
		}

		links
		{
			"ABOVE"
		}

		--Windows projects

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"AB_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "AB_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "AB_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "AB_DIST"
			buildoptions "/MD"
			optimize "On"
