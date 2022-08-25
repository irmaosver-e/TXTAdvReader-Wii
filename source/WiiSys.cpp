#include "WiiSys.h"

#include <gccore.h>
#include <wiiuse/wpad.h>



WiiSys::WiiSys(){}

bool WiiSys::Init()
{
	static GXRModeObj* rmode = NULL;
	static void* xfb = NULL;

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

	return true;
}

bool WiiSys::CheckForQuit()
{
	// Call WPAD_ScanPads each loop, this reads the latest controller states
	WPAD_ScanPads();

	// WPAD_ButtonsDown tells us which buttons were pressed in this loop
	// this is a "one shot" state which will not fire again until the button has been released
	u32 pressed = WPAD_ButtonsDown(0);

	// We return to the launcher application via exit
	if (pressed & WPAD_BUTTON_HOME)
		return 0;

	// Wait for the next frame
	VIDEO_WaitVSync();
	return true;
}

int WiiSys::CheckControllInput()
{
	VIDEO_WaitVSync();
	WPAD_ScanPads();
	u32 pressed = WPAD_ButtonsDown(0);

	if (pressed & WPAD_BUTTON_A)
		return 1;
	else if (pressed & WPAD_BUTTON_B)
		return 2;
	else if (pressed & WPAD_BUTTON_MINUS)
		return 3;
	else if (pressed & WPAD_BUTTON_PLUS)
		return 4;
	else if (pressed & WPAD_BUTTON_1)
		return 5;
	else if (pressed & WPAD_BUTTON_2)
		return 6;
	else if (pressed & WPAD_BUTTON_HOME)
		return 7;

	//VIDEO_WaitVSync();
	return 0;
}
