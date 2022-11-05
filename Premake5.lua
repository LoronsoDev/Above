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

group "Dependencies"
	include "Above/vendor/GLFW"
	include "Above/vendor/Glad"
	include "Above/vendor/imgui"

group ""

	------------ENGINE PROJECT---------------

	project "Above"
		location "Above"
		kind "SharedLib"
		language "C++"
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
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}"
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
			cppdialect "C++17"
			systemversion "latest"

			defines
			{
				"AB_PLATFORM_WINDOWS",
				"AB_BUILD_DLL",
				"GLFW_INCLUDE_NONE",
				"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
			}

			postbuildcommands
			{
				("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
			}

		filter "configurations:Debug"
			defines "AB_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "AB_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "AB_DIST"
			runtime "Release"
			optimize "On"


	------------SANDBOX PROJECT---------------

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		staticruntime "off"

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
			systemversion "latest"

			defines
			{
				"AB_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "AB_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "AB_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "AB_DIST"
			runtime "Release"
			optimize "On"
