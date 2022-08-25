#include "FileReader.h"
#include <string.h>
#include <fat.h>
#include <stdlib.h>


FileReader::FileReader()
{
	m_pStream = nullptr;

	m_bChoicesAvailable = false;
	m_bWaitingForChoice = false;

	//initialize filesys
	if (!fatInitDefault())
		printf("\nfatInitDefault() failed.\n");
	else
		printf("fileSys inited\n");

}

FileReader::~FileReader()
{
	m_streamPosVect.clear();
	m_optionVect.clear();
}

bool FileReader::OpenFile(const char* fileName)
{
	m_pStream = fopen(fileName, "r");

	if (!m_pStream)
	{
		printf("file not found\n");
		return false;
	}

	return true;
}

void FileReader::AssignPagePositions()
{
	char line[10];
	char scan[] = {"#-start-#"};

	fpos_t posStreamCurrent;
	int pageNumber = 0;
	int pageCount = 1;

	while (!feof(m_pStream))
	{
		fgets(line, 10, m_pStream);
		sscanf(line, "%s %d", scan, &pageNumber);

		//strcmp returns 0 if strings are equal
		if (strcmp(scan, "#page") == 0)
		{
			if (pageNumber == pageCount)
			{
				pageCount++;
				fgetpos(m_pStream, &posStreamCurrent);

				m_streamPosVect.push_back(posStreamCurrent);
			}
		}
		fgetpos(m_pStream, &m_posStreamEnd);
	}
	fsetpos(m_pStream, &m_streamPosVect[0]);
}

void FileReader::LoadChoices()
{
	char line[10];
	char scan[] = {"#-start-#"};
	bool storeChoices = false;

	fpos_t posStreamStart;
	fgetpos(m_pStream, &posStreamStart);

	while (strcmp(scan, "#endpage#") != 0)
	{
		fgets(line, 10, m_pStream);
		sscanf(line, "%s", scan);

		if (strcmp(scan, "#choices#") == 0)
		{
			storeChoices = true;
		}
		else
		{
			if (storeChoices && strcmp(scan, "#endpage#") != 0)
			{
				int choiceNumber;
				sscanf(line, "%d", &choiceNumber);
				m_optionVect.push_back(choiceNumber);
			}
		}		
	}
	fsetpos(m_pStream, &posStreamStart);
}

void FileReader::PrintTxt()
{
	char line[100];
	char scan[] = { "#-start-#" };

	printf("\x1b[3;0H");
	printf("Press HOME to exit\n");

	while (strcmp(scan, "#choices#") != 0)
	{
		fgets(line, 10, m_pStream);
		sscanf(line, "%s", scan);

		if (strcmp(scan, "#choices#") != 0)
		{
			printf("%s", line);
		}
	}	
}

bool FileReader::SelectChoice(int playerChoice)
{
	//if playerChoice == 0 player has not decided yet, skip code below
	if (playerChoice != 0 && (int)m_optionVect.size() > (playerChoice - 1))
	{
		//clear console
		printf("\x1b[2J");

		int optionArrayIndex = playerChoice - 1;
		int streamArrayIndex = m_optionVect[optionArrayIndex] - 1;
		fsetpos(m_pStream, &m_streamPosVect[streamArrayIndex]);

		m_optionVect.clear();

		return true;
	}
	else
	{
		//player pressed HOME button
		if (playerChoice == 7)
		{
			char line[10];
			fsetpos(m_pStream, &m_posStreamEnd);
			fgets(line, 10, m_pStream);
		}
	}
	return false;
}

void FileReader::PrepareStream()
{
	AssignPagePositions();
}

bool FileReader::RunStream(int playerChoice)
{
	if (!feof(m_pStream))
	{
		if (!m_bChoicesAvailable)
		{
			LoadChoices();
			m_bChoicesAvailable = true;
		}
		else if (!m_bWaitingForChoice)
		{
			PrintTxt();
			//printf("Select option\n");
			
			m_bWaitingForChoice = true;
		}
		else
		{
			if (SelectChoice(playerChoice))
			{
				m_bChoicesAvailable = false;
				m_bWaitingForChoice = false;
			}
		}
	}
	else
	{
		fclose(m_pStream);
		return false;
	}

	return true;
}
