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
IncludeDir["GLFW"]	= "Above/vendor/GLFW/include"
IncludeDir["Glad"]	= "Above/vendor/Glad/include"
IncludeDir["ImGui"] = "Above/vendor/imgui"
IncludeDir["glm"] = "Above/vendor/glm"
IncludeDir["stb_image"] = "Above/vendor/stb_image"

group "Dependencies"
	include "Above/vendor/Glad"
	include "Above/vendor/glfw"
	include "Above/vendor/imgui"

group ""

	------------ENGINE PROJECT---------------

	project "Above"
		location "Above"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off" 

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		pchheader "abpch.h"
		pchsource "%{prj.name}/src/abpch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.hpp",
			"%{prj.name}/src/**.hxx",
			"%{prj.name}/src/**.cpp",
			"%{prj.name}/vendor/stb_image/**.cpp",
			"%{prj.name}/vendor/stb_image/**.h",
			"%{prj.name}/vendor/glm/glm/**.inl",
			"%{prj.name}/vendor/glm/glm/**.hpp"
		}

		defines
		{
			"AB_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stb_image}"
		}

		links
		{
			"GLFW",
			"Glad",
			"ImGui",
			"opengl32.lib"
		}
		
		--Windows projects

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"AB_PLATFORM_WINDOWS",
				"AB_BUILD_DLL",
				"GLFW_INCLUDE_NONE"
			}

		filter "configurations:Debug"
			defines "AB_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "AB_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "AB_DIST"
			runtime "Release"
			optimize "on"


	------------SANDBOX PROJECT---------------

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		links
		{
			"Above"
		}
		
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
			"Above/src",
			"Above/vendor",
			"%{IncludeDir.glm}"
		}

		--Windows projects

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"AB_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "AB_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "AB_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "AB_DIST"
			runtime "Release"
			optimize "on"
