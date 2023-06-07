#include "hacks.h"
#include "gui.h"
#include "globals.h"
#include "vector.h"

#include <thread>

constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

void hacks::VisualThread(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAdress + offsets::dwLocalPlayer);

		if (!localPlayer)
			continue;
		
		const auto glowManager = mem.Read<std::uintptr_t>(globals::clientAdress + offsets::dwGlowObjectManager);

		if (!glowManager)
			continue;

		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		for (auto i = 1; i <= 64; ++i)
		{
			const auto player = mem.Read<std::uintptr_t>(globals::clientAdress + offsets::dwEntityList + i * 0x10);

			if (!player)
				continue;

			const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);

			if (team == localTeam)
				continue;

			const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

			if (lifeState != 0)
				continue;

			if (globals::glow)
			{			
				const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

				mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::glowColor[0]); // red
				mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]); // green
				mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]); // blue
				mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]); // alpha

				mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
				mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
			}

			if (globals::radar)
				mem.Write(player + offsets::m_bSpotted, true);

			if (globals::aim)

				const auto memory = Memory{ "csgo.exe" };

				const auto client = mem.GetModuleAddress("client.dll");
				const auto engine = mem.GetModuleAddress("engine.dll");

				// infinite hack loop
				while (globals::aim)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));

					for (auto i = 1; i <= 64; ++i)
						{
							const auto player = mem.Read<std::uintptr_t>(globals::clientAdress + offsets::dwEntityList + i * 0x10);

							if (!player)
								continue;

							const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);

							if (team == localTeam)
								continue;

							const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

							if (lifeState != 0)
								continue;

							if (globals::glow)
							{
								const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

								mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::glowColor[0]); // red
								mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]); // green
								mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]); // blue
								mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]); // alpha

								mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
								mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
							}
					}

					// aimbot key
					if (!GetAsyncKeyState(0x48))
						continue;

					// get local player
					const auto localPlayer = mem.Read<std::uintptr_t>(client + aim::dwLocalPlayer);
					const auto localTeam = mem.Read<std::int32_t>(localPlayer + aim::m_iTeamNum);

					// eye position = origin + viewOffset
					const auto localEyePosition = mem.Read<Vector3>(localPlayer + aim::m_vecOrigin) + mem.Read<Vector3>(localPlayer + aim::m_vecViewOffset);

					const auto clientState = mem.Read<std::uintptr_t>(engine + aim::dwClientState);

					const auto localPlayerId =
						mem.Read<std::int32_t>(clientState + aim::dwClientState_GetLocalPlayer);

					const auto viewAngles = mem.Read<Vector3>(clientState + aim::dwClientState_ViewAngles);
					const auto aimPunch = mem.Read<Vector3>(localPlayer + aim::m_aimPunchAngle) * 2;

					// aimbot fov
					auto bestFov = globals::aimStength;
					auto bestAngle = Vector3{ };

					for (auto i = 1; i <= 64; ++i)
					{
						const auto player = mem.Read<std::uintptr_t>(client + aim::dwEntityList + i * 0x10);

						if (mem.Read<std::int32_t>(player + aim::m_iTeamNum) == localTeam)
							continue;

						if (mem.Read<bool>(player + aim::m_bDormant))
							continue;

						if (mem.Read<std::int32_t>(player + aim::m_lifeState))
							continue;

						if (mem.Read<std::int32_t>(player + aim::m_bSpottedByMask) & (1 << localPlayerId))
						{
							const auto boneMatrix = mem.Read<std::uintptr_t>(player + aim::m_dwBoneMatrix);

							// pos of player head in 3d space
							// 8 is the head bone index :)
							const auto playerHeadPosition = Vector3{
								mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
								mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
								mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
							};

							const auto angle = CalculateAngle(
								localEyePosition,
								playerHeadPosition,
								viewAngles + aimPunch
							);

							const auto fov = std::hypot(angle.x, angle.y);

							if (fov < bestFov)
							{
								bestFov = fov;
								bestAngle = angle;
							}
						}
					}

					// if we have a best angle, do aimbot
					if (!bestAngle.IsZero())
						mem.Write<Vector3>(clientState + aim::dwClientState_ViewAngles, viewAngles + bestAngle / globals::aimSoftness); // smoothing
				}
				if (globals::rageAim)

				const auto memory = Memory{ "csgo.exe" };

				// infinite hack loop
				while (globals::rageAim)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));

					globals::aim = false;

					for (auto i = 1; i <= 64; ++i)
					{
						const auto player = mem.Read<std::uintptr_t>(globals::clientAdress + offsets::dwEntityList + i * 0x10);

						if (!player)
							continue;

						const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);

						if (team == localTeam)
							continue;

						const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

						if (lifeState != 0)
							continue;

						if (globals::glow)
						{
							const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

							mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::glowColor[0]); // red
							mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]); // green
							mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]); // blue
							mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]); // alpha

							mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
							mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);
						}

						if (globals::radar)
							mem.Write(player + offsets::m_bSpotted, true);
					}

					// get local player
					const auto localPlayer = mem.Read<std::uintptr_t>(client + aim::dwLocalPlayer);
					const auto localTeam = mem.Read<std::int32_t>(localPlayer + aim::m_iTeamNum);

					// eye position = origin + viewOffset
					const auto localEyePosition = mem.Read<Vector3>(localPlayer + aim::m_vecOrigin) + mem.Read<Vector3>(localPlayer + aim::m_vecViewOffset);

					const auto clientState = mem.Read<std::uintptr_t>(engine + aim::dwClientState);

					const auto localPlayerId =
						mem.Read<std::int32_t>(clientState + aim::dwClientState_GetLocalPlayer);

					const auto viewAngles = mem.Read<Vector3>(clientState + aim::dwClientState_ViewAngles);
					const auto aimPunch = mem.Read<Vector3>(localPlayer + aim::m_aimPunchAngle) * 2;

					// aimbot fov
					auto bestFov = 360.f;
					auto bestAngle = Vector3{ };

					for (auto i = 1; i <= 64; ++i)
					{
						const auto player = mem.Read<std::uintptr_t>(client + aim::dwEntityList + i * 0x10);

						if (mem.Read<std::int32_t>(player + aim::m_iTeamNum) == localTeam)
							continue;

						if (mem.Read<bool>(player + aim::m_bDormant))
							continue;

						if (mem.Read<std::int32_t>(player + aim::m_lifeState))
							continue;

						if (mem.Read<std::int32_t>(player + aim::m_bSpottedByMask) & (1 << localPlayerId))
						{
							const auto boneMatrix = mem.Read<std::uintptr_t>(player + aim::m_dwBoneMatrix);

							// pos of player head in 3d space
							// 8 is the head bone index :)
							const auto playerHeadPosition = Vector3{
								mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
								mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
								mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
							};

							const auto angle = CalculateAngle(
								localEyePosition,
								playerHeadPosition,
								viewAngles + aimPunch
							);

							const auto fov = std::hypot(angle.x, angle.y);

							if (fov < bestFov)
							{
								bestFov = fov;
								bestAngle = angle;
							}
						}
					}

					// if we have a best angle, do aimbot
					if (!bestAngle.IsZero())
						mem.Write<Vector3>(clientState + aim::dwClientState_ViewAngles, viewAngles + bestAngle / 1.f); // smoothing
				}
		}
	}
}