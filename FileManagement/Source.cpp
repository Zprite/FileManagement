#define _CRT_SECURE_NO_DEPRECATE
#define min_ascii 32
#define max_ascii 254 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <time.h>

void fread(char*file_name);
void fwrite(char*file_name);
int fcopy(char*file_name, char* copy_name);
int fdel(char*file_name, int delete_line);
int freplace(char* file_name, int replace_line);
int finsert(char* file_name, int replace_line);
int fmerge(char*file_name, char* file2_name, char* mergefile_name);
int fencrypt(char*file_name, char *encryptfile_name);

int main()
{
	char file_name[20];
	char encrypt_name[20];
	printf("Input source file: ");
	scanf("%s", file_name);
	printf("Input encrypted file name: ");
	scanf("%s", encrypt_name);
	fencrypt(file_name, encrypt_name);
	fread(file_name);
	fread(encrypt_name);
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
	printf("  1  "); // UI start
	while (readinput != EOF)
	{
		printf("%c", readinput);
		if (readinput == '\n') 
		{
			nLines++;
			nWords++;
			// **UI**
			if (nLines+1 > 99){printf( "%d  ", nLines + 1); } 
			else if (nLines + 1 > 9 && nLines + 1 < 100) { printf(" %d  ", nLines + 1); }
			else if (nLines < 9) { printf("  %d  ", nLines + 1); }
			// **UI**
		}
		else if (readinput == ' ') nWords++;
		else nChars++;
		i++;
		readinput = fgetc(fp);
	}
	fclose(fp);
	printf("\nNumber of lines in file: %d\n", nLines+1);
	printf("Number of characters in file: %d\n", nChars); // counts all characters except \n and space
	printf("Number of words in file: %d\n", nWords);
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
	int nLine = 1;
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
	int nLine = 1;
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
	char input_buffer[100];
	int nLine = 1;
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
			printf("Input to line %d:", replace_line + 1);
			fgets(input_buffer, sizeof input_buffer, stdin);
			fputs(input_buffer, fp2);
			nLine++;
			fputc(readinput, fp2);
		}
		if (readinput == '\n') { nLine++; }
		readinput = fgetc(fp);
	}
	if (replace_line >= nLine)
	{
		if (nLine + 100 < replace_line) // Exit with error code if a large number of newlines is required. This is to prevent program from making dangerous trash files :=)
		{
			printf("Error! Maximum newlines to add is 100.\n");
			remove("copy.txt");
			return -1;
		}
		for (nLine; nLine < replace_line; nLine++)
		{
			fputc('\n', fp2);
		}
		printf("Input to line %d:", replace_line + 1);
		getchar(); // Getchar to flush the newline from scanf
		fgets(input_buffer, sizeof input_buffer, stdin);
		fputs(input_buffer, fp2);
	}
	fclose(fp);
	fclose(fp2);
	remove(file_name);
	rename("copy.txt", file_name);
	return 0;
}

int fcopy(char*file_name, char* copy_name)
{
	FILE *fp, *fp2;
	fp = fopen(file_name, "r");
	fp2 = fopen(copy_name, "w");
	char readinput = getc(fp);
	while (readinput != EOF)
	{
		fputc(readinput, fp2);
		readinput = getc(fp);
	}
	fclose(fp);
	fclose(fp2);
	printf("Successfully copied contents of '%s' to '%s'.\n", file_name, copy_name);
	return 0;
}

int fmerge(char*file_name, char* file2_name, char* mergefile_name)
{
	FILE *fp, *fp2, *fmp;
	fp = fopen(file_name, "r");
	fp2 = fopen(file2_name, "r");
	fmp = fopen(mergefile_name, "w");
	if (fp == NULL || fp2 == NULL || fmp == NULL)
	{
		printf("Error opening file..!!\n");
		fclose(fp);
		fclose(fp2);
		fclose(fmp);
		return-1;
	}
	char readinput = fgetc(fp);
	while (readinput != EOF)
	{
		putc(readinput, fmp);
		readinput = fgetc(fp);
	}
	fclose(fp);
	putc('\n', fmp);
	readinput = fgetc(fp2);
	while (readinput != EOF)
	{
		putc(readinput, fmp);
		readinput = fgetc(fp2);
	}
	fclose(fp2);
	fclose(fmp);
	printf("Files successfully merged.\n");
	return 0;
}

int fencrypt(char*file_name, char *encryptfile_name)
{
	FILE *fp, *efp;
	int decrypt_code[4];
	fp = fopen(file_name, "r");
	efp = fopen(encryptfile_name, "w");
	if (fp == NULL || efp == NULL)
	{
		printf("Error opening file..!");
		fclose(fp); 
		fclose(efp);
		remove(encryptfile_name);
		return -1;
	}
	srand(time(NULL));
	printf("IMPORTANT!! DECRYPTION CODE FOR '%s' is: ", encryptfile_name);
	for (int i = 0; i < 4; i++) // GENERATE ENCRYPTION CODE
	{
		decrypt_code[i] = (rand() % 99);
		if (decrypt_code[i] < 10)
			printf("0%d", decrypt_code[i]);
		else
		printf("%d", decrypt_code[i]);
	}
	printf("\n");
	int counter = 0;
	char readinput = fgetc(fp);
	while (readinput != EOF)
	{
		printf("Shifting by %d\n", decrypt_code[counter]);
		fputc(readinput + decrypt_code[counter], efp);
		readinput = fgetc(fp);
		counter++;
		counter = counter % 4;
	}
	printf("Successfully encrypted file!\n");
	fclose(fp);
	fclose(efp);
	return 0;
}

int fdecrypt(char*decrypt_file, char* outfile)
{
	int decrypt_code[4];
	int codein[8];
	printf("Input decryption code (example: '11023398') : ");
}

