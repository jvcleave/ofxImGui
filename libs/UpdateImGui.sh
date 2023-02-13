#!/bin/sh
# Run this script to easily update ImGui
# Copyright 2023 Daan de Lange

# Clone fresh repo
if [[ -d dir ]]; then
	echo "Cloning ocornut/imgui..."
	git clone https://github.com/ocornut/imgui.git ./imgui_git
	cd ./imgui_git

	git checkout -b docking

	# Sha of latest version release (in docking branch!)
	git checkout d822c65 # 1.89.2

	cd ../
fi;

# Copy the files we need
# Backend
echo "Copying backend files..."
mkdir -p ./imgui_git/backends
mv ./imgui_git/backends/imgui_impl_glfw.cpp		./imgui/backends/imgui_impl_glfw.cpp
mv ./imgui_git/backends/imgui_impl_glfw.h		./imgui/backends/imgui_impl_glfw.h
mv ./imgui_git/backends/imgui_impl_opengl2.cpp	./imgui/backends/imgui_impl_opengl2.cpp
mv ./imgui_git/backends/imgui_impl_opengl2.h	./imgui/backends/imgui_impl_opengl2.h
mv ./imgui_git/backends/imgui_impl_opengl3.cpp	./imgui/backends/imgui_impl_opengl3.cpp
mv ./imgui_git/backends/imgui_impl_opengl3.h	./imgui/backends/imgui_impl_opengl3.h
#mv ./imgui_git/backends/imgui_impl_vulkan.cpp	./imgui/backends/imgui_impl_vulkan.cpp
#mv ./imgui_git/backends/imgui_impl_vulkan.h		./imgui/backends/imgui_impl_vulkan.h

# Source files
echo "Copying source files..."
mkdir -p ./imgui_git/src
mv ./imgui_git/imgui_demo.cpp		./imgui/src/imgui_demo.cpp
mv ./imgui_git/imgui_draw.cpp		./imgui/src/imgui_draw.cpp
mv ./imgui_git/imgui_internal.h		./imgui/src/imgui_internal.h
mv ./imgui_git/imgui_tables.cpp		./imgui/src/imgui_tables.cpp
mv ./imgui_git/imgui_widgets.cpp	./imgui/src/imgui_widgets.cpp
mv ./imgui_git/imgui.cpp			./imgui/src/imgui.cpp
mv ./imgui_git/imgui.h				./imgui/src/imgui.h
mv ./imgui_git/imstb_rectpack.h		./imgui/src/imstb_rectpack.h
mv ./imgui_git/imstb_textedit.h		./imgui/src/imstb_textedit.h
mv ./imgui_git/imstb_truetype.h		./imgui/src/imstb_truetype.h
mv ./imgui_git/LICENSE.txt			./imgui/src/LICENSE.txt

# Copy some docs
echo "Copying some docs..."
mkdir -p ./imgui_git/docs
#mv ./imgui_git/docs/ ./imgui/docs
mv ./imgui_git/docs/BACKENDS.md		./imgui/docs/BACKENDS.md
mv ./imgui_git/docs/CHANGELOG.txt	./imgui/docs/CHANGELOG.txt
mv ./imgui_git/docs/README.md		./imgui/docs/README.md

# Remove git repo // cleanup
echo "Cleaning up git repo..."
rm -rf ./imgui_git

# Done !
echo "Done !"
echo "You ImGui has been updates, don't forget to check the breaking changes !"
