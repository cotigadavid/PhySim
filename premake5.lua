 workspace "PhySim"
	architecture "x64"
	startproject "PhySim"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "PhySim/vendor/GLFW/include"
IncludeDir["Glad"] = "PhySim/vendor/Glad/include"
IncludeDir["glm"] = "PhySim/vendor/glm"
IncludeDir["stb_image"] = "PhySim/vendor/stb_image"
IncludeDir["yaml_cpp"] = "PhySim/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "PhySim/vendor/ImGuizmo"
IncludeDir["Box2D"] = "PhySim/vendor/Box2D/include"

include "PhySim/vendor/GLFW"
include "PhySim/vendor/Glad"
include "PhySim/vendor/yaml-cpp"
--include "PhySim/vendor/ImGuizmo"
include "PhySim/vendor/Box2D"


project "PhySim"
	location "PhySim"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pspch.h"
	pchsource "PhySim/src/pspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		
	}

	links
	{
		"Box2D",
		"GLFW",
		"opengl32.lib",
		"Glad",
		"yaml-cpp"
		
	}

	filter "files:PhySim/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
	           systemversion "latest"
            	staticruntime "On"

		defines
		{
			"PS_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "PS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "PS_DIST"
		runtime "Release"
		optimize "on"