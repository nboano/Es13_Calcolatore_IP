// Se sono in ambiente console (e non WEB) devo includere le librerie standard del C.
#ifndef var

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#endif

#define MAX_STR 128
#define MAX_SPLIT 16

/// @brief Rappresenta un'indirizzo IPv4 (32 bit, 4 x 8).
typedef struct t_IpAddress {
    unsigned char byte4;
    unsigned char byte3;
    unsigned char byte2;
    unsigned char byte1;
} IpAddress;

/// @brief Rappresenta ogni possibile classe di IP (A, B, C, D, E).
typedef enum t_IpClass {
    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E'
} IpClass;

/// @brief Funzione split (UTILITY).
int split(const char* s, char separ, char v[MAX_SPLIT][MAX_STR]) {
	int i = 0, j = 0, is = 0;
	do
	{
		if (s[i] == separ) {
			v[j][is] = 0;
			j++;
			is = 0;
		}
		else {
			v[j][is] = s[i];
			is++;
		}
		i++;
	} while (s[i] != 0 && j < MAX_SPLIT);
	return j;
}

/// @brief Ottiene l'indirizzo di rete da un'IP di rete e la subnet mask.
IpAddress OttieniIndirizzoRete(IpAddress ipLocale, IpAddress subnetMask) {
    IpAddress netaddr;

    netaddr.byte1 = ipLocale.byte1 & subnetMask.byte1;
    netaddr.byte2 = ipLocale.byte2 & subnetMask.byte2;
    netaddr.byte3 = ipLocale.byte3 & subnetMask.byte3;
    netaddr.byte4 = ipLocale.byte4 & subnetMask.byte4;

    return netaddr;

    // IN ALTERNATIVA
    // unsigned int netaddr_i = *((unsigned int*)&ipLocale) & *((unsigned int*)&subnetMask);
    // return *((IpAddress*)&netaddr_i);
}

/// @brief Ottiene l'indirizzo di broadcast da un'IP di rete e la wildcard mask.
IpAddress OttieniIndirizzoBroadcast(IpAddress ipLocale, IpAddress wildcardMask) {
    IpAddress netaddr;

    netaddr.byte1 = ipLocale.byte1 | wildcardMask.byte1;
    netaddr.byte2 = ipLocale.byte2 | wildcardMask.byte2;
    netaddr.byte3 = ipLocale.byte3 | wildcardMask.byte3;
    netaddr.byte4 = ipLocale.byte4 | wildcardMask.byte4;

    return netaddr;
}

/// @brief Ottiene la wildcard mask dalla subnet mask.
IpAddress OttieniWildcardMask(IpAddress subnetmask) {
    IpAddress wildcard;

    wildcard.byte1 = ~subnetmask.byte1;
    wildcard.byte2 = ~subnetmask.byte2;
    wildcard.byte3 = ~subnetmask.byte3;
    wildcard.byte4 = ~subnetmask.byte4;

    return wildcard;
}

/// @brief Ottiene la classe di un'IP in input.
IpClass OttieniClasseIP(IpAddress ip) {
    IpClass cls;

    if(ip.byte1 >= 0 && ip.byte1 <= 127)        cls = A;
    else if(ip.byte1 >= 128 && ip.byte1 <= 191) cls = B;
    else if(ip.byte1 >= 192 && ip.byte1 <= 223) cls = C;
    else if(ip.byte1 >= 224 && ip.byte1 <= 239) cls = D;
    else if(ip.byte1 >= 240 && ip.byte1 <= 255) cls = E;

    return cls;
}

/// @brief Converte un'IP in stringa. (XXX.XXX.XXX.XXX)
const char* IpToString(IpAddress ip, char ip_bf[]) {

    strcpy(ip_bf, "");
    sprintf(ip_bf, "%i.%i.%i.%i", (int)ip.byte1, (int)ip.byte2, (int)ip.byte3, (int)ip.byte4);

    return ip_bf;
}

void normalize_bin_number(char sn[8 + 1]) {
    char tmp[8 + 1] = ""; strcpy(tmp, sn);

    strcpy(sn, "00000000");

    char* ptr = sn + 8;
    ptr -= strlen(tmp);
    strcpy(ptr,tmp);
}

/// @brief Converte un'IP in stringa binaria. (XXXXXXXX.XXXXXXXX.XXXXXXXX.XXXXXXXX)
const char* IpToBinaryString(IpAddress ip, char ip_bf[]) {
    strcpy(ip_bf, "");

    char byte1_bf[8 + 1]; itoa(ip.byte1, byte1_bf, 2); normalize_bin_number(byte1_bf);
    char byte2_bf[8 + 1]; itoa(ip.byte2, byte2_bf, 2); normalize_bin_number(byte2_bf);
    char byte3_bf[8 + 1]; itoa(ip.byte3, byte3_bf, 2); normalize_bin_number(byte3_bf);
    char byte4_bf[8 + 1]; itoa(ip.byte4, byte4_bf, 2); normalize_bin_number(byte4_bf);

    sprintf(ip_bf, "%s.%s.%s.%s", byte1_bf, byte2_bf, byte3_bf, byte4_bf);

    return ip_bf;
}

/// @brief Ottiene un'IP da una stringa (ATTENZIONE: Prima controllare che l'IP sia valido con IPValid).
IpAddress IpFromString(const char* str) {
    IpAddress addr;

    char spl[MAX_SPLIT][MAX_STR];
    split(str, '.', spl);

    addr.byte1 = atoi(spl[0]);
    addr.byte2 = atoi(spl[1]);
    addr.byte3 = atoi(spl[2]);
    addr.byte4 = atoi(spl[3]);

    return addr;
}

/// @brief Ottiene la lunghezza del prefisso subnet di una rete, data la sua maschera di sottorete.
int LunghPrefissoSottorete(IpAddress subnetmask) {
    char bf[50];
    IpToBinaryString(subnetmask, bf);

    int ultimoUno = 32;
    char* ptr = bf + strlen(bf) - 1; // MI PORTO A FINE STRINGA

    while(*ptr-- != '1' && ultimoUno > 0) if(*ptr != '.') ultimoUno--;

    return ultimoUno;
}

/// @brief Ottiene il massimo numero di host in una rete, data la wildcard mask.
int MassimoNumHost(IpAddress wildcardmask) {
    return ((wildcardmask.byte1 | 1) * (wildcardmask.byte2 | 1) * (wildcardmask.byte3 | 1) * (wildcardmask.byte4 | 1)) - 2;
}

/// @brief Controlla se un'IP è valido.
bool IpValid(const char* str) {

    int contapunti = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if(str[i] >= '0' && str[i] <= '9')
            continue;

        if(str[i] == '.')
            contapunti++;
        else return false;
    }

    if(contapunti != 3) return false;

    char spl[MAX_SPLIT][MAX_STR];
    split(str, '.', spl);

    if(
        atoi(spl[0]) > 255 || atoi(spl[0]) < 0 ||
        atoi(spl[1]) > 255 || atoi(spl[1]) < 0 ||
        atoi(spl[2]) > 255 || atoi(spl[2]) < 0 ||
        atoi(spl[3]) > 255 || atoi(spl[3]) < 0
    ) return false; 
    else return true;
}