#pragma once

#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include <fileapi.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include ".\Options.h"
#include ".\usage_help.h"
#include ".\selectedoptions.h"

HANDLE OpenFile(char filename[50]);
BOOL checkMZ(HANDLE hFile);
char* readPEData(HANDLE hFile, DWORD fileSize);
BOOL overwriteNewOptionalHeader(HANDLE hFile, char* PEfileData, PIMAGE_OPTIONAL_HEADER OptionalHeader, PIMAGE_DOS_HEADER Dos, DWORD fileSize);
void extractPEHeaders(char* PEfileData, PIMAGE_DOS_HEADER* Dos, PIMAGE_NT_HEADERS* Nt, PIMAGE_FILE_HEADER* FileHeader, PIMAGE_OPTIONAL_HEADER* Optional);

#define AUTHOR "spyw4re"
#define MZ "MZ"

int main(int argc, char* argv[]) {

	if (argc < 2) {
		usageTxt();
		return EXIT_FAILURE;
	}

	SelectedOptions selectedOptions = processOptions(argc, argv);

	if (selectedOptions.showHelp) {
		helpTxt();
		return EXIT_SUCCESS;
	}

	/* Open the provided file */
	HANDLE hFile = OpenFile(argv[1]);

	if (hFile == INVALID_HANDLE_VALUE) {
		return EXIT_FAILURE;
	}

	printf("[i] DllCharacteristicsChanger v0.0.1\n");

	/* Check for MZ magic number*/
	BOOL isPE = checkMZ(hFile);

	if (isPE == FALSE) {
		perror("File is not a PE. Exiting...");
		CloseHandle(hFile);
		return EXIT_FAILURE;
	}

	/* get file size and set the pointer to begin */
	DWORD fileSize = GetFileSize(hFile, NULL);

	if (fileSize == INVALID_FILE_SIZE) {
		printf("\nError getting file size. Error code: %" PRId32, GetLastError());
		CloseHandle(hFile);
		return EXIT_FAILURE;
	}

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	/* Read the file data and put into PEfileData variable */
	char* PEfileData = readPEData(hFile, fileSize);

	/* Walk the PE Headers, looking for Optional Header */
	PIMAGE_DOS_HEADER Dos;
	PIMAGE_NT_HEADERS Nt;
	PIMAGE_FILE_HEADER FileHeader;
	PIMAGE_OPTIONAL_HEADER Optional;

	extractPEHeaders(PEfileData, &Dos, &Nt, &FileHeader, &Optional);

	/* handle tool options */
	if (selectedOptions.setASLR) {
		puts("\n\t[*] Setting ASLR...");
		setASLR(Optional);
		puts("\t[+] ASLR setted!\n");
	}

	if (selectedOptions.removeASLR) {
		puts("\n\t[*] Removing ASLR...");
		removeASLR(Optional);
		puts("\t[+] ASLR removed!\n");
	}

	if (selectedOptions.setDEP) {
		puts("\n\t[*] Setting DEP...");
		setDEP(Optional);
		puts("\t[+] DEP setted!\n");
	}

	if (selectedOptions.removeDEP) {
		puts("\n\t[*] Removing DEP...");
		removeDEP(Optional);
		puts("\t[+] DEP removed!\n");
	}

	if (selectedOptions.setFINTEGRIY) {
		puts("\n\t[*] Setting Authenticode check...");
		setFINTEGRITY(Optional);
		puts("\t[+] Authenticode check setted!\n");
	}

	if (selectedOptions.removeFINTEGRITY) {
		puts("\n\t[*] Removing Authenticode check...");
		removeFINTEGRITY(Optional);
		puts("\t[+] Authenticode check removed!\n");
	}

	/* Overwrite the provided file with the new DllCharacteristics */
	BOOL overwriteSuccess = overwriteNewOptionalHeader(hFile, PEfileData, Optional, Dos, fileSize);

	if (overwriteSuccess == FALSE) {
		perror("Unable to overwrite the file Optional Header.");
		free(PEfileData);
		CloseHandle(hFile);
		return EXIT_FAILURE;
	}

	printf("[+] DllCharacteristics value: 0x%" PRIX32, Optional->DllCharacteristics);

	/* FINISHED! */
	free(PEfileData);
	CloseHandle(hFile);

	return EXIT_SUCCESS;
}

HANDLE OpenFile(char filename[50]) {

	HANDLE hFile = CreateFileA(
		filename,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD errorCode = GetLastError();
		if (errorCode == ERROR_FILE_NOT_FOUND) {
			printf("\nThe file %s does not exist.\n", filename);
		}
		else if (errorCode == ERROR_ACCESS_DENIED) {
			printf("\nAccess to %s is denied.\n", filename);
		}
		else {
			printf("\nFailed to open the file %s. Error code: %" PRId32 "\n", filename, errorCode);
		}
	}

	return hFile;
}

BOOL checkMZ(HANDLE hFile) {
	char magicNumber[2] = { 0 };
	DWORD bytesRead = 0;

	if (!ReadFile(
		hFile,
		magicNumber,
		sizeof(magicNumber),
		&bytesRead,
		NULL
	)) {
		printf("\nUnable to read the file. Error code: %" PRId32, GetLastError());
		return FALSE;
	}

	if (bytesRead == 2 && memcmp(magicNumber, MZ, 2) == 0) {
		return TRUE;
	}

	return FALSE;
}

char* readPEData(HANDLE hFile, DWORD fileSize) {
	char* PEfileData = (char*)malloc(fileSize);

	if (PEfileData == NULL) {
		printf("Failed to allocate memory for the file content.\n");
		CloseHandle(hFile);
		return NULL;
	}

	DWORD bytesRead = 0;

	if (!ReadFile(
		hFile,
		PEfileData,
		fileSize,
		&bytesRead,
		NULL
	)) {
		printf("\nError reading the file data. Error code: %l" PRId32 "n", GetLastError());
		CloseHandle(hFile);
		free(PEfileData);
		return NULL;
	}

	return PEfileData;
}

BOOL overwriteNewOptionalHeader(HANDLE hFile, char* PEfileData, PIMAGE_OPTIONAL_HEADER OptionalHeader, PIMAGE_DOS_HEADER Dos, DWORD fileSize) {
	memcpy(PEfileData + Dos->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), OptionalHeader, sizeof(IMAGE_OPTIONAL_HEADER));

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

	DWORD bytesRead = 0;

	if (!WriteFile(
		hFile,
		PEfileData,
		fileSize,
		&bytesRead,
		NULL
	)) {
		printf("\nError writing the modified data back to the file. Error code: %" PRId32 "\n", GetLastError());
		CloseHandle(hFile);
		free(PEfileData);
		return FALSE;
	}

	return TRUE;
}

void extractPEHeaders(char* PEfileData, PIMAGE_DOS_HEADER* Dos, PIMAGE_NT_HEADERS* Nt, PIMAGE_FILE_HEADER* FileHeader, PIMAGE_OPTIONAL_HEADER* Optional) {
	*Dos = (PIMAGE_DOS_HEADER)PEfileData;
	*Nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)*Dos + (*Dos)->e_lfanew);
	*FileHeader = &((*Nt)->FileHeader);
	*Optional = &((*Nt)->OptionalHeader);
}