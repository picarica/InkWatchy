#ifndef VAULT_UI_H
#define VAULT_UI_H

#include "../../defines/defines.h"

extern long long int key;

void initVault();
void loopVault();
void showVaultImage(String file);
void exitVault();

void hexStringToByteArray(const char* hexString, unsigned char* byteArray, size_t byteArraySize);

#endif
