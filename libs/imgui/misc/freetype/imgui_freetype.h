// dear imgui: FreeType font builder (used as a replacement for the stb_truetype builder)
// (headers)

#pragma once
#include "imgui.h" // IMGUI_API
#ifndef IMGUI_DISABLE

// Usage:
// - Add '#define IMGUI_ENABLE_FREETYPE' in your imconfig to automatically enable support
// for imgui_freetype in imgui. It is equivalent to selecting the default loader with:
// io.Fonts->SetFontLoader(ImGuiFreeType::GetFontLoader())

// Optional support for OpenType SVG fonts:
// - Add '#define IMGUI_ENABLE_FREETYPE_PLUTOSVG' to use plutosvg (not provided). See #7927.
// - Add '#define IMGUI_ENABLE_FREETYPE_LUNASVG' to use lunasvg (not provided). See #6591.

// Forward declarations
struct ImFontAtlas;
struct ImFontLoader;

// Hinting greatly impacts visuals (and glyph sizes).
// - By default, hinting is enabled and the font's native hinter is preferred over the auto-hinter.
// - When disabled, FreeType generates blurrier glyphs, more or less matches the stb_truetype.h
// - The Default hinting mode usually looks good, but may distort glyphs in an unusual way.
// - The Light hinting mode generates fuzzier glyphs but better matches Microsoft's rasterizer.
// You can set those flags globally in ImFontAtlas::FontLoaderFlags
// You can set those flags on a per font basis in ImFontConfig::FontLoaderFlags
typedef unsigned int ImGuiFreeTypeLoaderFlags;
enum ImGuiFreeTypeLoaderFlags_
{
    ImGuiFreeTypeLoaderFlags_NoHinting     = 1 << 0,
    ImGuiFreeTypeLoaderFlags_NoAutoHint    = 1 << 1,
    ImGuiFreeTypeLoaderFlags_ForceAutoHint = 1 << 2,
    ImGuiFreeTypeLoaderFlags_LightHinting  = 1 << 3,
    ImGuiFreeTypeLoaderFlags_MonoHinting   = 1 << 4,
    ImGuiFreeTypeLoaderFlags_Bold          = 1 << 5,
    ImGuiFreeTypeLoaderFlags_Oblique       = 1 << 6,
    ImGuiFreeTypeLoaderFlags_Monochrome    = 1 << 7,
    ImGuiFreeTypeLoaderFlags_LoadColor     = 1 << 8,
    ImGuiFreeTypeLoaderFlags_Bitmap        = 1 << 9,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    ImGuiFreeTypeBuilderFlags_NoHinting     = ImGuiFreeTypeLoaderFlags_NoHinting,
    ImGuiFreeTypeBuilderFlags_NoAutoHint    = ImGuiFreeTypeLoaderFlags_NoAutoHint,
    ImGuiFreeTypeBuilderFlags_ForceAutoHint = ImGuiFreeTypeLoaderFlags_ForceAutoHint,
    ImGuiFreeTypeBuilderFlags_LightHinting  = ImGuiFreeTypeLoaderFlags_LightHinting,
    ImGuiFreeTypeBuilderFlags_MonoHinting   = ImGuiFreeTypeLoaderFlags_MonoHinting,
    ImGuiFreeTypeBuilderFlags_Bold          = ImGuiFreeTypeLoaderFlags_Bold,
    ImGuiFreeTypeBuilderFlags_Oblique       = ImGuiFreeTypeLoaderFlags_Oblique,
    ImGuiFreeTypeBuilderFlags_Monochrome    = ImGuiFreeTypeLoaderFlags_Monochrome,
    ImGuiFreeTypeBuilderFlags_LoadColor     = ImGuiFreeTypeLoaderFlags_LoadColor,
    ImGuiFreeTypeBuilderFlags_Bitmap        = ImGuiFreeTypeLoaderFlags_Bitmap,
#endif
};

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
typedef ImGuiFreeTypeLoaderFlags_ ImGuiFreeTypeBuilderFlags_;
#endif

namespace ImGuiFreeType
{
    IMGUI_API const ImFontLoader* GetFontLoader();
    IMGUI_API void SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = nullptr);
    IMGUI_API bool DebugEditFontLoaderFlags(ImGuiFreeTypeLoaderFlags* p_font_loader_flags);
}

#endif // #ifndef IMGUI_DISABLE
