#include "Configuration.h"

void SingleResults()
{
#if _DEBUG
	printf("[Custom Results Music] Round Clear will only play\n");
#endif
	WRITE_MEMORY(0xCFD47F, uint8_t, 0xE9, 0x95, 0x00, 0x00, 0x00, 0xC3, 0x90, 0x90, 0x90);
}

void ResultsDuration(float duration)
{
#if _DEBUG
	printf("[Custom Results Music] Set duration: %f\n", duration);
#endif
	// Special thanks to brianuuu for finding the address needed for adjusting the duration!
	WRITE_MEMORY(0x17046C0, double, duration);
}

void OnlySRankResult()
{
	WRITE_MEMORY(0xCFD4CA, uint8_t, 0xF8, 0x38);
	WRITE_MEMORY(0xCFD4E8, uint8_t, 0xF8, 0x38);
}

void NoSRankResult()
{
	WRITE_MEMORY(0xCFD4CA, uint8_t, 0x00, 0x39);
	WRITE_MEMORY(0xCFD4E8, uint8_t, 0x00, 0x39);
}

extern "C" __declspec(dllexport) void Init()
{
	Configuration::load("mod.ini");

#if _DEBUG
	printf("[Custom Results Music] Current directory: %s\n", Configuration::songChoice.c_str());
#endif

	if (Configuration::songChoice == "sound_Custom")
	{
		if (Configuration::customSingle)
			SingleResults();
		else
			ResultsDuration(Configuration::customDuration);
	}
	else if (Configuration::songChoice == "sound_sonic-2006")
		ResultsDuration(7.381f);
	else if (Configuration::songChoice == "sound_sonic-Unleashed")
		ResultsDuration(6.021f);
	else if (Configuration::songChoice == "sound_sonic-Colors")
		ResultsDuration(8.01f);
	else if (Configuration::songChoice == "sound_sonic-LostWorld")
		ResultsDuration(8.182f);
	else if (Configuration::songChoice == "sound_sonic-BlackKnight")
		ResultsDuration(11.0f);
	else if (Configuration::songChoice != "")
		SingleResults();
}

extern "C" __declspec(dllexport) void PostInit()
{
	if (Configuration::onlySRank)
		OnlySRankResult();
	else if (Configuration::noSRank)
		NoSRankResult();

}