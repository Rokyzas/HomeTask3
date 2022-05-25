// Rokas Čebatorius, Programų sistemos 1 kursas, 2 pogrupis
// rokas.cebatorius@mif.stud.vu.lt
// 2021-11-05
// 
// Parašyti funkciją, kuri iš duoto teksto išmeta žodžius, kurie prasideda ir 
// baigiasi ta pačia raide(tarpų skaičius turi likti nepakitęs).
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFileIntoBuffer(char **inputBuffer, int* lineCount,int* newBufferAddress, int argc, char* argv[]);
void changeBuffer(char** inputBuffer, int* lineCount);
void writeBufferIntoFile(char** inputBuffer, int* lineCount, int argc,char* argv[]);

#define BUFFER_SIZE 256

int main(int argc, char* argv[])
{
	char **inputBuffer = malloc(sizeof(char*) * 1);
	int newBufferAddress;
	int lineCount = 0;

	printf("Program removes words from text that begin and end with the same symbol, preserving all whitespaces.\n");
	printf("Program outputs results into chosen name file, if not chosen then to the screen.\n");

	readFileIntoBuffer(inputBuffer, &lineCount, &newBufferAddress, argc, argv);
	inputBuffer = newBufferAddress;

	changeBuffer(inputBuffer, &lineCount);
	
	writeBufferIntoFile(inputBuffer, &lineCount, argc, argv);

	free(inputBuffer);

	return 0;
}


void readFileIntoBuffer(char** inputBuffer, int* lineCount, int* newBufferAddress, int argc, char* argv[])
{
	char dataFile[20];
	FILE* fp;

	// Validation of file name
	if (argc < 2)
	{
		printf("Couldn't find data file\nEnter valid data file name(up to 20 symbols):\n");
		scanf("%s", dataFile);
	}
	else
	{
		strcpy(dataFile, argv[1]);
	}
	
	fp = fopen(dataFile, "r");
	while(!fp)
	{
		printf("Couldn't find data file\nEnter valid data file name(up to 20 symbols):\n");
		scanf("%s", dataFile);
		fp = fopen(dataFile, "r");
	}
	

	// Reading from file
	char temp[BUFFER_SIZE];
	char dump[9999];

	while (!feof(fp))
	{
		fgets(temp, BUFFER_SIZE, fp);
		if (strlen(temp) == (BUFFER_SIZE-1))
		{
			fgets(dump, 9999, fp); // clearing whats after 255 symbols
			if (temp[(BUFFER_SIZE - 1)] != '\0')
			{
				temp[(BUFFER_SIZE - 1)] = '\n';
			}
		}
		++(*lineCount);

		inputBuffer = realloc(inputBuffer, sizeof(char*) * (*lineCount)); // reallocating new line

		inputBuffer[(*lineCount) - 1] = malloc(sizeof(char) * (strlen(temp) + 1)); // allocating space for line (length of read string + 1)

		strcpy(inputBuffer[(*lineCount) - 1], temp); // copying temp to buffer
	}

	*newBufferAddress = inputBuffer; // saving new buffer address

	fclose(fp);
}

void changeBuffer(char** inputBuffer, int* lineCount)
{
	char word[BUFFER_SIZE];
	int offset, offsetSum;
	int flag = 0; // flag for cheking if its first word in line
	for (int i = 0; i < (*lineCount); ++i)
	{
		offsetSum = 0;
		while (sscanf(inputBuffer[i], " %s%n", word, &offset) == 1) // using offset to move word by word
		{
			flag = 0;
			if (word[0] == word[strlen(word) - 1]) // cheking if word start and ends with the same symbol
			{
				for (int j = 0; j < strlen(inputBuffer[i]); ++j) // moving symbols one by one to delete the word that matched criteria
				{
					
					if ((inputBuffer[i][0] != ' ') || flag == 1)
					{
						inputBuffer[i][j] = inputBuffer[i][j + offset];
						flag = 1;
					}
					else // if its not the first word, we want to preserve all whitespaces
					{
						inputBuffer[i][j + 1] = inputBuffer[i][j + offset];
					}
					
				}
			}
			else
			{
				offsetSum += offset;
				inputBuffer[i] += offset; // changin position of buffer using offset
			}
		} 
		inputBuffer[i] -= offsetSum; // reseting adress back to starting point
	}
}

void writeBufferIntoFile(char** inputBuffer, int* lineCount, int argc, char* argv[])
{
	FILE* fp = NULL;
	int fileCreated = 0;
	if (argc > 2) // cheking if there is 3 argument for result file
	{
		if (fp = fopen(argv[2], "w"))
		{
			fileCreated = 1;
		}
	}

	// writing to file or screen
	for (int i = 0; i < (*lineCount); ++i)
	{
		if (fileCreated)
		{
			fputs(inputBuffer[i], fp);
		}
		else
		{
			fputs(inputBuffer[i], stdout);	
		}
	}

	if (fileCreated)
	{
		fclose(fp);
	}
}
