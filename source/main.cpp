#include <stdio.h>
#include <stdlib.h>
#include <fat.h>
#include <string.h>

#include "WiiSys.h"
#include "FileReader.h"

//created by Ernani da S. Costa

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------
	
	WiiSys wiiSystems;

	wiiSystems.Init();
	
	FileReader fileReader;
	fileReader.OpenFile("sd:/apps/TXTAdvReader/Adventure.txt");
	fileReader.PrepareStream();

	bool stayInGame = true;
	while(stayInGame)
	{
		if (!fileReader.RunStream(wiiSystems.CheckControllInput()))
		{
			stayInGame = false;
		}
	}

	exit(0);
	
	return 0;
}
