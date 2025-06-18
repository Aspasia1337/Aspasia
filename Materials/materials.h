#pragma once

// This file is only for testing, not used.

static constexpr char szVMatBufferWhiteVisible[] =
    R"(    <!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
    format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
    {
    	shader = "csgo_effects.vfx"
     
        g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
        g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
        g_tMask1 = resource:"materials/default/default_mask_tga_344101f8.vtex"
        g_tMask2 = resource:"materials/default/default_mask_tga_344101f8.vtex"
        g_tMask3 = resource:"materials/default/default_mask_tga_344101f8.vtex"
        g_tRoughness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    	g_tMetalness = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
    	g_tAmbientOcclusion = resource:"materials/default/default_normal_tga_b3f4ec4c.vtex"
     
    	g_flColorBoost = 30
    	g_flOpacityScale = 245.55
        g_flFresnelExponent = 7.75
        g_flFresnelFalloff = 5
        g_flFresnelMax = 0.0
        g_flFresnelMin = 9
     
        F_ADDITIVE_BLEND = 1
        F_BLEND_MODE = 1
        F_TRANSLUCENT = 1
        F_IGNOREZ = 0
        F_DISABLE_Z_WRITE = 0
        F_DISABLE_Z_BUFFERING = 0
        F_RENDER_BACKFACES = 0
        g_vColorTint = [7.0, 7.0, 7.0, 0.37522]
    })";
