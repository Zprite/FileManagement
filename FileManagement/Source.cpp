#define _CRT_SECURE_NO_DEPRECATE
#define min_ascii 32
#define max_ascii 254 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>


void sel();
void help();
void fread(char*file_name);
void fwrite(char*file_name);
int fcopy(char*file_name, char* copy_name);
int fdel(char*file_name, int delete_line);
int freplace(char* file_name, int replace_line);
int finsert(char* file_name, int replace_line);
int fmerge(char*file_name, char* file2_name, char* mergefile_name);
int fencrypt(char*file_name, char *encryptfile_name);
int fdecrypt(char*decrypt_file, char* outfile);

int main()
{
	printf("----Welcome to file manager!----\n Write 'help' for a list of available commands!\n\n");
	sel();
	printf("\n\nPress any key to exit. . .");
	_getch();
}
void help()
{
	printf("\n\n- 'open'    - Input name of an already existing file. Opened file is passed to the other functions.\n"
		"              IMPORTANT: Extension is needed! (example: test.txt)\n\n");
	printf("- 'make'    - Creates a new file with selected name. File is automatically opened.\n"
		"              IMPORTANT: Extension is needed! (example: newfile.txt)\n\n");
	printf("- 'read'    - Print the contents of opened file.\n\n");
	printf("- 'write'   - Overwrite file with user input.\n\n");
	printf("- 'delete'  - Delete line in opened file at select line.\n\n");
	printf("- 'insert'  - Insert user input at select line. \n\n");
	printf("- 'replace' - Replace line with user input at select line.\n\n");
	printf("- 'merge'   - Merges the contents of opened file and select file, to select output file.\n\n");
	printf("- 'encrypt' - Encrypts opened file with outputed key. Use key in 'decrypt' to decrypt.\n\n");
	printf("- 'decrypt' - Decrypts opened file with select key. Outputs decrypted file to select file.\n\n");
	printf("- 'exit'    - Exit program.\n\n");
	printf("-----------------------------------------------------------------------------------------------\n\n");
}
void sel()
{
	char keyword[10];
	do {
		char filename[20];
		fgets(keyword, sizeof keyword, stdin);
		char *pos;
		if ((pos = strchr(keyword, '\n')) != NULL)
			*pos = '\0';

		if (!strcmp(keyword, "open")) {
			FILE * fp;
			printf("Enter file name: ");
			fgets(filename, sizeof filename, stdin);
			char *pos;
			if ((pos = strchr(filename, '\n')) != NULL)
				*pos = '\0';
			fp = fopen(filename, "r");
			if (fp == NULL) { printf("Error! File cannot be opened or does not exist\n"); }
			else
			{
				printf("%s is loaded!\n\n", filename);
				fclose(fp);
			}
		}
		else if (!strcmp(keyword,"make"))
		{
			FILE * fp;
			printf("Create new file with name: ");
			scanf("%s", filename);
			getchar(); // Flush stdin to remover trailing newline.
			fp = fopen(filename, "w");
			if (fp == NULL) { printf("Error! File cannot be created\n"); }
			else 
			{
				printf("Successfully created '%s'!\n\n", filename);
				fclose(fp);
			}
		}
		else if (!strcmp(keyword,"read"))
		{
			fread(filename);
		}
		else if (!strcmp(keyword,"write")) {
			char proceed = 'n';
			printf("WARNING: This will overwrite content of file, do you wish to proceed? (y/n)");
			scanf("%c", &proceed);
			getchar(); // Flush stdin
			if (proceed == 'y') 
			{
				fwrite(filename);
				printf("\nWrite successfull!\n\n");
			}
			else printf("Exiting write function...\n\n");
		}
		else if (!strcmp(keyword,"delete")) {
			int delete_line;
			printf("Select line to delete: ");
			scanf("%d", &delete_line);
			if (!fdel(filename, delete_line)) {
				printf("Sucessfully deleted line %d in %s!\n\n", delete_line, filename);
			} else printf(" Failed to delete line %d in %s!\n\n", delete_line, filename);
			getchar(); // FLush STDIN
		}
		else if (!strcmp(keyword,"insert")) {
			int insert_line;
			printf("Select line to insert to: ");
			scanf("%d", &insert_line);
			if(!finsert(filename, insert_line))
			printf("Sucessfully inserted to line %d in %s!\n\n", insert_line, filename);
			else printf("Failed to insert to line %d in %s!\n\n", insert_line, filename);
		}
		else if (!strcmp(keyword,"replace")) {
			int replace_line;
			printf("select line to replace: ");
			scanf("%d", &replace_line);
			if(!freplace(filename, replace_line))
			printf("Sucessfully replaced line %d in %s!\n\n", replace_line, filename);
			else("Failed to replace line %d in %s!\n\n", replace_line, filename);
		}
		else if (!strcmp(keyword,"merge")) {
			char filename2[20];
			char newfile[20];
			printf("Merge with file: ");
			scanf("%s", filename2);
			printf("Enter name for output file: ");
			scanf("%s", newfile);
			getchar(); //Flush stdin to remove trailing newline.
			if (!fmerge(filename, filename2, newfile))
				printf("Files successfully merged.\n");
			else
				printf("Failed to merge files!\n");
		}
		else if (!strcmp(keyword,"encrypt"))
		{
			char encryptfile[20];
			printf("Enter name for encrypted outfile: ");
			scanf("%s", encryptfile);
			getchar(); // Flush stdin to remover trailing newline.
			fencrypt(filename, encryptfile);
		}
		else if (!strcmp(keyword,"decrypt")) {
			char decryptfile[20];
			printf("Enter name for outputted decryptfile: ");
			scanf("%s", decryptfile);
			fdecrypt(filename, decryptfile);
			getchar();
		}
		else if (!strcmp(keyword,"help")) {
			help();
		} 
		else if (!strcmp(keyword, "exit")) {
			printf("\n Exiting...");
			break;
		}
		else printf("'%s' is not a valid command! Type 'help' for a list of commands.\n\n", keyword);

	} while (1);
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
	if (nChars > 0) nWords += 1;
	printf("\n\nNumber of lines in file: %d\n", nLines+1);
	printf("Number of characters in file: %d\n", nChars); // counts all characters except \n and space
	printf("Number of words in file: %d\n\n", nWords);
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
			printf("Input to line %d:", replace_line);
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
	char codein[9];
	getchar(); // Flush stdin
	printf("\n\nInput decryption code (example: '11023398') : ");
	fgets(codein, sizeof codein, stdin);
	if (strlen(codein) > 8) 
	{
		printf("Error! Only accept 8 digit input!!\n");
		return -1;
	}
	int counter = 0;
	for (int i = 0; i < 8; i++) // Parse numbers from string and put into decrypt_code array.
	{
		if (i % 2 == 0) // Check if number from string is odd or even, this will determine if it should be multiplied by 10 due to its position.
		{
			decrypt_code[counter] = ((int)codein[i] - 48) * 10; // Converts numbers from ASCII to integer.
		}
		else
		{
			decrypt_code[counter] += ((int)codein[i] - 48); // Converts numbers from ASCII to integer.
			counter++;
		}
	}

	FILE *finp, *foutp;
	finp = fopen(decrypt_file,"r");
	foutp = fopen(outfile, "w");
	char readinput = fgetc(finp);
	int code_counter = 0;
	while (readinput != EOF)
	{
		fputc(readinput - decrypt_code[code_counter], foutp);
		readinput = fgetc(finp);
		code_counter++;
		code_counter = code_counter % 4;
	}
	fclose(finp);
	fclose(foutp);
	printf("Successfully decrypted file!\n");
	return 0;
}

