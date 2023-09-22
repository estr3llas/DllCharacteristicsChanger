#pragma once

#include <Windows.h>

BOOL setASLR(PIMAGE_OPTIONAL_HEADER Optional) {

	if (!Optional) {
		perror("\n[-]Unable to set ASLR\n[-]Invalid Optional Header");
		return FALSE;
	}

	Optional->DllCharacteristics |= IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
	return TRUE;
}

BOOL removeASLR(PIMAGE_OPTIONAL_HEADER Optional) {

	if (!Optional) {
		perror("\n[-]Unable to remove ASLR\n[-]Invalid Optional Header");
		return FALSE;
	}

	Optional->DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
	return TRUE;
}

BOOL setDEP(PIMAGE_OPTIONAL_HEADER Optional) {

	if (!Optional) {
		perror("\n[-]Unable to set DEP\n[-]Invalid Optional Header");
		return FALSE;
	}

	Optional->DllCharacteristics |= IMAGE_DLLCHARACTERISTICS_NX_COMPAT;
	return TRUE;
}

BOOL removeDEP(PIMAGE_OPTIONAL_HEADER Optional) {

	if (!Optional) {
		perror("\n[-]Unable to remove DEP\n[-]Invalid Optional Header");
		return FALSE;
	}

	Optional->DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_NX_COMPAT;
	return TRUE;
}

BOOL setFINTEGRITY(PIMAGE_OPTIONAL_HEADER Optional) {

	if (!Optional) {
		perror("\n[-]Unable to set Authenticode check\n[-]Invalid Optional Header");
		return FALSE;
	}

	Optional->DllCharacteristics |= IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY;
	return TRUE;
}

BOOL removeFINTEGRITY(PIMAGE_OPTIONAL_HEADER Optional) {

	if (!Optional) {
		perror("\n[-]Unable to remove Authenticode check\n[-]Invalid Optional Header");
		return FALSE;
	}

	Optional->DllCharacteristics &= ~IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY;
	return TRUE;
}