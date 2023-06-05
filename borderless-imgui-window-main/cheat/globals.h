#pragma once
#include <cstdint>
#include <cstddef>
	
namespace globals
{
	inline std::uintptr_t clientAdress = 0;
	inline std::uintptr_t engineAdress = 0;

	inline bool glow = false;
	inline bool radar = false;

	inline float glowColor[] = { 1.f,0.f,0.f,1.f };
	inline float teamColor[] = { 0.f,0.f,1.f,0.f };
}

namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDE997C;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x5359988;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFEF0C;

	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
}