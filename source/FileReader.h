#pragma once

#include <stdio.h>
#include <vector>

class FileReader
{
public:
	FileReader();
	~FileReader();

	bool OpenFile(const char* fileName);
	void PrepareStream();
	bool RunStream(int playerChoice);

private:
	void AssignPagePositions();
	void LoadChoices();
	void PrintTxt();
	bool SelectChoice(int playerChoice);

	FILE* m_pStream;
	std::vector<fpos_t> m_streamPosVect;
	std::vector<fpos_t> m_optionVect;
	fpos_t m_posStreamEnd;

	bool m_bWaitingForChoice;
	bool m_bChoicesAvailable;

};

