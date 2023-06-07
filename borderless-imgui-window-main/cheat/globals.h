#pragma once
#include <cstdint>
#include <cstddef>
	
namespace globals
{
	inline std::uintptr_t clientAdress = 0;
	inline std::uintptr_t engineAdress = 0;

	inline bool glow = false;
	inline bool radar = false;
	inline bool aim = false;
	inline bool rageAim = false;
	inline bool bhop = false;

	inline float aimStength = 10.f;
	inline float aimSoftness = 3.f;

	inline float glowColor[] = { 1.f,0.f,0.f,1.f };
	inline float teamColor[] = { 0.f,0.f,1.f,0.f };
}

namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA98C;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x535AA08;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFFF7C;

	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
}

namespace aim
{
	// client
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA98C;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFFF7C;

	// engine
	constexpr ::std::ptrdiff_t dwClientState = 0x59F19C;
	constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
	constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;

	// entity
	constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
	constexpr ::std::ptrdiff_t m_bDormant = 0xED;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
	constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
	constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
	constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
	constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
}

namespace bhop
{
	constexpr auto localPlayer = 0xDEA98C;
	constexpr auto flags = 0x104;
	constexpr auto forceJump = 0x52BAC68;
	constexpr auto entityList = 0x4DFFF7C;
}
