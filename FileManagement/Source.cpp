#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <Windows.h>

void fread(char*file_name);
void fwrite(char*file_name);
int fdel(char*file_name, int delete_line);
int freplace(char* file_name, int replace_line);
int finsert(char* file_name, int replace_line);
int main()
{
	char file_name[20];
	int replace_line;
	printf("Input file name (.txt extension needed): ");
	scanf("%s", file_name);
	printf("\n\n");
	fwrite(file_name);
	fread(file_name);
	printf("\n\n");
	do{
		printf("Insert to line: ");
		scanf("%d", &replace_line);
	} while (finsert(file_name, replace_line-1) == -1); // -1 because first line is line 1 instead of line 0 
	fread(file_name);
	printf("\n\nPress any key to exit. . .");
	_getch();
}

void fread(char*file_name)
{
	FILE *fp;
	int i, nLines, nChars, nWords;
	i = nLines = nChars = nWords = 0;

	printf("\n\nShowing contents of '%s':\n", file_name);
	fp = fopen(file_name, "r");
	char readinput = fgetc(fp);
	while (readinput != EOF)
	{
		if (readinput == '\n'){
			nLines++;
			nWords++;
		}
		else if (readinput == ' ') nWords++;
		else nChars++;
		printf("%c", readinput);
		i++;
		readinput = fgetc(fp);
	}
	fclose(fp);
	printf("\nNumber of lines in file: %d\n", nLines);
	printf("Number of characters in file: %d\n", nChars); // Displays all characters except \n and space
	printf("Number of words in file: %d\n", nWords-1); // -1 to exclude newline at the start
}

void fwrite(char*file_name)
{
	FILE *fp;
	char input_buffer[100];
	int nLines;
	printf("Enter ammount of lines to input:");
	scanf("%d", &nLines);
	getchar(); // Getchar to flush the newline from scanf
	printf("\n --Input Lines--\n");
	fp = fopen(file_name, "w");
	for (int i = 0; i < nLines; i++) {
		fgets(input_buffer, sizeof input_buffer, stdin);
		fputs(input_buffer, fp);
	}
	printf("\nWriting to file...\n");
	fclose(fp);
}

int fdel(char*file_name, int delete_line)
{
	FILE *fp, *fp2;
	int nLine = 0;
	fp = fopen(file_name, "r");
	fp2 = fopen("copy.txt", "w");
	char readinput = fgetc(fp);
	while (readinput != EOF)
	{
		if (nLine != delete_line)
		{
			fputc(readinput, fp2);
		}
		if (readinput == '\n') nLine++;
		readinput = fgetc(fp);
	}
	fclose(fp);
	fclose(fp2);
	if (delete_line > nLine)
	{
		printf("ERROR, LINE %d does not exist !!\n", delete_line);
		remove("copy.txt");
		return -1;
	}
	remove(file_name);
	rename("copy.txt", file_name);
	return 0;
}

int freplace(char* file_name, int replace_line)
{
	FILE *fp, *fp2;
	int nLine = 0;
	int has_replaced = 0;
	fp = fopen(file_name, "r");
	fp2 = fopen("copy.txt", "w");
	char readinput = fgetc(fp);
	while (readinput != EOF)
	{
		if (nLine != replace_line)
			fputc(readinput, fp2);
		else if (!has_replaced)
		{	
			getchar(); // Getchar to flush the newline from scanf
			char input_buffer[100];
			printf("Input to line %d:", replace_line+1);
			fgets(input_buffer, sizeof input_buffer, stdin);
			fputs(input_buffer,fp2);
			has_replaced = 1;
		}
		if (readinput == '\n') { nLine++; }
		readinput = fgetc(fp);
	}
	fclose(fp);
	fclose(fp2);
	if (replace_line > nLine)
	{
		printf("ERROR, LINE %d does not exist !!\n", replace_line);
		remove("copy.txt");
		return -1;
	}
	remove(file_name);
	rename("copy.txt", file_name);
	return 0;
}

int finsert(char* file_name, int replace_line)
{
	FILE *fp, *fp2;
	int nLine = 0;
	fp = fopen(file_name, "r");
	fp2 = fopen("copy.txt", "w");
	char readinput = fgetc(fp);
	while (readinput != EOF)
	{
		if (nLine != replace_line)
			fputc(readinput, fp2);
		else 
		{
			getchar(); // Getchar to flush the newline from scanf
			char input_buffer[100];
			printf("Input to line %d:", replace_line + 1);
			fgets(input_buffer, sizeof input_buffer, stdin);
			fputs(input_buffer, fp2);
			nLine++;
			fputc(readinput, fp2);
		}
		if (readinput == '\n') { nLine++; }
		readinput = fgetc(fp);
	}
	fclose(fp);
	fclose(fp2);
	if (replace_line > nLine)
	{
		printf("ERROR, LINE %d does not exist !!\n", replace_line);
		remove("copy.txt");
		return -1;
	}
	remove(file_name);
	rename("copy.txt", file_name);
	return 0;
}