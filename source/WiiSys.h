#pragma once

class WiiSys
{
public:
	WiiSys();

	bool Init();
	
	bool CheckForQuit();

	int CheckControllInput();

};

