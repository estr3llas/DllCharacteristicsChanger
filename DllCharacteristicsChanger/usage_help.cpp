#pragma once

#include <stdio.h>

void helpTxt() {
	puts("[i] DllCharacteristicsChanger v0.0.1");
	puts("[i]This is a tool to manipulate DllCharacteristics field of a binary.\n");
	puts("[i] Options:");
	puts("\t-sA,    Set ASLR <IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040> flag");
	puts("\t-rA,    Remove ASLR <IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040> flag");
	puts("\t-sN,    Set DEP <IMAGE_DLLCHARACTERISTICS_NX_COMPAT 0x0100> flag");
	puts("\t-rN,    Remove DEP <IMAGE_DLLCHARACTERISTICS_NX_COMPAT 0x0100> flag");
	puts("\t-sI,    Set Authenticode Check <IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY 0x0080> flag");
	puts("\t-rI,    Remove Authenticode Check <IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY 0x0080> flag");
}
void usageTxt() {

	puts("[*] DllCharacteristicsChanger v0.0.1");
	puts("[*] This is a tool to enable/disable ASLR <IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE>, DEP <IMAGE_DLLCHARACTERISTICS_NX_COMPAT> and Authenticode Signing <IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY> on your binary!");
	puts("\n\t[i] Usage: DllCharacteristicsChanger.exe <file.exe> --help/-h");
	puts("\n[!] Created by: @CryptDeriveKey.");
}
