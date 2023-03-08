#include <stdio.h>
#include <stdlib.h>
#include "lib/UTILITY_C/utilities.h"
#include "../IPAddress.c"

int main()
{
    Console.Clear();
    System.SetWindowTitle("CALCOLATORE IP");
    Console.PrintInterline();
    Color.Set(Color.Purple);
    Console.PrintCentered("CALCOLATORE IP\n");
    Color.Reset();
    Console.PrintInterline();

    printf("Inserisci indirizzo di rete: ");

    char input_ip[20]; gets(input_ip); fflush(stdin);
    if(!IpValid(input_ip)) {
        MsgBox.Error("Indirizzo di rete non valido!", "ERRORE INPUT");
        main();
    }

    printf("Inserisci la maschera di sottorete: ");
    char input_sm[20]; gets(input_sm); fflush(stdin);

    if(!IpValid(input_sm)) {
        MsgBox.Error("Maschera di sottorete non valida!", "ERRORE INPUT");
        main();
    }

    IpAddress iplocale = IpFromString(input_ip);
    IpAddress subnetmask = IpFromString(input_sm);

    IpAddress indirizzo_rete = OttieniIndirizzoRete(iplocale, subnetmask);
    IpAddress wildcardmask = OttieniWildcardMask(subnetmask);
    IpAddress indirizzo_broadcast = OttieniIndirizzoBroadcast(iplocale, wildcardmask);

    IpAddress primo_disp = indirizzo_rete; primo_disp.byte4++;
    IpAddress ultimo_disp = indirizzo_broadcast; ultimo_disp.byte4--;

    int pref_sottorete = LunghPrefissoSottorete(subnetmask);
    unsigned int max_n_host = MassimoNumHost(wildcardmask);

    IpClass classe_ip = OttieniClasseIP(iplocale);

    char buffer_ip[50];

    putchar('\n');
    printf("%-20s", "INDIRIZZO IP");
    Color.Set(Color.Red);
    printf("%-20s", IpToString(iplocale, buffer_ip));
    Color.Set(Color.Blue);
    printf("%s\n", IpToBinaryString(iplocale, buffer_ip));

    Color.Reset();
    printf("%-20s", "SUBNET MASK");
    Color.Set(Color.Green);
    printf("%-15s/%-4i", IpToString(subnetmask, buffer_ip), pref_sottorete);
    Color.Set(Color.Blue);
    printf("%s\n", IpToBinaryString(subnetmask, buffer_ip));

    Color.Reset();
    printf("%-20s", "WILDCARD MASK");
    Color.Set(Color.Yellow);
    printf("%-20s", IpToString(wildcardmask, buffer_ip));
    Color.Set(Color.Blue);
    printf("%s\n", IpToBinaryString(wildcardmask, buffer_ip));

    Color.Reset();
    printf("%-20s", "INDIRIZZO RETE");
    Color.Set(Color.Aqua);
    printf("%-20s", IpToString(indirizzo_rete, buffer_ip));
    Color.Set(Color.Blue);
    printf("%s\n", IpToBinaryString(indirizzo_rete, buffer_ip));

    Color.Reset();
    printf("%-20s", "BROADCAST");
    Color.Set(Color.Red);
    printf("%-20s", IpToString(indirizzo_broadcast, buffer_ip));
    Color.Set(Color.Blue);
    printf("%s\n", IpToBinaryString(indirizzo_broadcast, buffer_ip));

    Color.Reset();
    printf("%-20s", "N. HOST");
    Color.Set(Color.Purple);
    printf("%-20lu\n", max_n_host);

    Color.Reset();
    printf("%-20s", "CLASSE");
    Color.Set(Color.Purple);
    printf("%-20c\n", classe_ip);

    Color.Reset();
    printf("%-20s", "RANGE INDIRIZZI");
    Color.Set(Color.Green);
    printf("%s - ", IpToString(primo_disp, buffer_ip));
    printf("%s\n", IpToString(ultimo_disp, buffer_ip));


    Color.Reset();

    getch();
    main();
}
