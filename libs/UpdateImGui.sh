#!/bin/sh
#!/usr/bin/gsed -f

# Run this script to easily update ImGui
# Copyright 2023 Daan de Lange

# ------------------
# CONFIG
# Sha of latest version release (in docking branch!)
# How to find it : Omar publishes a version commit, for example "Version 1.89.3" which is the non-docking version
# Right after that, in the docking branch, there's a commit "Merge branch 'master' into docking ".
# That's the one you wish. The docs/CHANGELOG.txt should NOT state "VERSION 1.89.3 (In Progress)"
#VERSION_SHA=d822c65 # 1.89.2
VERSION_SHA=1921967 # 1.89.3
# ------------------

echo "\nFinding ImGui..."

# Clone fresh repo
if [[ ! -d ./imgui_git ]]; then
	echo "Cloning ocornut/imgui..."
	git clone https://github.com/ocornut/imgui.git ./imgui_git -b docking
else
	echo "Found ImGui repo in /libs/imgui_git"
fi;

# Pick the right
echo "Checking out latest commit"
if [[ -d ./imgui_git ]]; then
	cd ./imgui_git
	git checkout -b docking
	# Alternative restoring all missing files AND reverings any change
	git reset --hard $VERSION_SHA --
	cd ../
else
	echo "Problem finding the imgui directory, exiting now."
	exit 1
fi;

# Copy the files we need
echo "\nCopying files..."
# Backend
echo "Copying backend files..."
mkdir -p ./imgui_git/backends
cp ./imgui_git/backends/imgui_impl_glfw.cpp         ./imgui/backends/imgui_impl_glfw.cpp
cp ./imgui_git/backends/imgui_impl_glfw.h           ./imgui/backends/imgui_impl_glfw.h
cp ./imgui_git/backends/imgui_impl_opengl2.cpp      ./imgui/backends/imgui_impl_opengl2.cpp
cp ./imgui_git/backends/imgui_impl_opengl2.h        ./imgui/backends/imgui_impl_opengl2.h
cp ./imgui_git/backends/imgui_impl_opengl3.cpp      ./imgui/backends/imgui_impl_opengl3.cpp
cp ./imgui_git/backends/imgui_impl_opengl3.h        ./imgui/backends/imgui_impl_opengl3.h
cp ./imgui_git/backends/imgui_impl_opengl3_loader.h ./imgui/backends/imgui_impl_opengl3_loader.h
#cp ./imgui_git/backends/imgui_impl_vulkan.cpp       ./imgui/backends/imgui_impl_vulkan.cpp
#cp ./imgui_git/backends/imgui_impl_vulkan.h         ./imgui/backends/imgui_impl_vulkan.h

# Source files
echo "Copying source files..."
mkdir -p ./imgui_git/src
cp ./imgui_git/imgui_demo.cpp		./imgui/src/imgui_demo.cpp
cp ./imgui_git/imgui_draw.cpp		./imgui/src/imgui_draw.cpp
cp ./imgui_git/imgui_internal.h		./imgui/src/imgui_internal.h
cp ./imgui_git/imgui_tables.cpp		./imgui/src/imgui_tables.cpp
cp ./imgui_git/imgui_widgets.cpp	./imgui/src/imgui_widgets.cpp
cp ./imgui_git/imgui.cpp			./imgui/src/imgui.cpp
cp ./imgui_git/imgui.h				./imgui/src/imgui.h
cp ./imgui_git/imstb_rectpack.h		./imgui/src/imstb_rectpack.h
cp ./imgui_git/imstb_textedit.h		./imgui/src/imstb_textedit.h
cp ./imgui_git/imstb_truetype.h		./imgui/src/imstb_truetype.h
cp ./imgui_git/LICENSE.txt			./imgui/src/LICENSE.txt

# Copy some docs
echo "Copying some docs..."
mkdir -p ./imgui_git/docs
#cp ./imgui_git/docs/ ./imgui/docs
mv ./imgui_git/docs/BACKENDS.md		./imgui/docs/BACKENDS.md
mv ./imgui_git/docs/CHANGELOG.txt	./imgui/docs/CHANGELOG.txt
mv ./imgui_git/docs/README.md		./imgui/docs/README.md

# Remove git repo // cleanup
echo "Cleaning up git repo..."
rm -rf ./imgui_git
fi;

# Apply patches
echo "\nApplying patches..."

# OSX sed is a pain in the ass. Use gnu-sed on osx.
if [ `uname -s` = "Darwin" ] ; then
	if ! [ -x "$(command -v gsed)" ] ; then
		echo "GSED is not installed, installing..."
		brew install gnu-sed
	fi;
fi;

# Option 1 : Patch file ?
#git apply --check Glfw_MultiContext_Support.diff
patch --dry-run -s ./imgui/backends/imgui_impl_glfw.cpp ./Glfw_MultiContext_Support.diff > /dev/null 2>&1
if [ $? -eq 0 ]; then
	#git apply Glfw_MultiContext_Support.diff
	echo "Applying a GIT diff as it seems compatible..."
	patch ./imgui/backends/imgui_impl_glfw.cpp ./Glfw_MultiContext_Support.diff

# option 2 : Sed search/replace
else
	echo "Git diff is not compatible, trying to apply a custom GNU sed patch that might be less vulnerable to breaking changes."
	# todo: On linux use sed instead of gsed ? Tested on OSX only.
	
	# Add include
	gsed -i '/^#include "imgui_impl_glfw\.h"$/a #include "backends\/imgui_impl_glfw_context_support.h" \/\/ CUSTOM OFXIMGUI ADDED LINE' ./imgui/backends/imgui_impl_glfw.cpp
	# Add scoped context on glfw callbacks
	gsed -i -e '/^.*void ImGui_ImplGlfw_.*Callback.*(GLFWwindow\* window.*)$/{n; s/$/\n\tImGui_ImplGlfw_ScopedContext sc\(window\)\; \/\/ CUSTOM OFXIMGUI ADDED LINE\n/}' ./imgui/backends/imgui_impl_glfw.cpp
	# Register Windows
	gsed -i '/^static void ImGui_ImplGlfw_CreateWindow(/,/^}$/s/^}$/\tImGui_ImplGlfw_RegisterWindowContext(vd->Window, ImGui::GetCurrentContext())\; \/\/ CUSTOM OFXIMGUI ADDED LINE\n}/' ./imgui/backends/imgui_impl_glfw.cpp
	#Forget windows
	gsed -i '/^static void ImGui_ImplGlfw_DestroyWindow(/,/^}$/s/vd->Window \= nullptr;$/ImGui_ImplGlfw_RemoveWindowContext(vd->Window)\; \/\/ CUSTOM OFXIMGUI ADDED LINE\n\t\tvd->Window \= nullptr\;/' ./imgui/backends/imgui_impl_glfw.cpp
fi;

# Done !
echo "\nDone !"
echo "ImGui has been updated, don't forget to check the breaking changes !"
