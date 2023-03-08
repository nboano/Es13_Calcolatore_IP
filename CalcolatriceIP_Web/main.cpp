#include "lib/KinderC/kinderc.hpp"
#include "../IPAddress.c"

int main() {

}

#define txtIndirizzo HTMLInputElement($("#txtIndirizzo"))
#define txtSubnet HTMLInputElement($("#txtSubnet"))

exported void calcola() {

    const char* locale = txtIndirizzo.value;
    const char* subnet = txtSubnet.value;

    if(!IpValid(locale)) {
        alert("L'indirizzo di rete digitato non e' valido. Riprova.");
        return;
    }

    if(!IpValid(subnet)) {
        alert("La maschera di sottorete digitata non e' valida. Riprova.");
        return;
    }

    IpAddress iplocale = IpFromString(txtIndirizzo.value);
    IpAddress subnetmask = IpFromString(txtSubnet.value);

    free((void*)locale);
    free((void*)subnet);

    IpAddress indirizzo_rete = OttieniIndirizzoRete(iplocale, subnetmask);
    IpAddress wildcardmask = OttieniWildcardMask(subnetmask);
    IpAddress indirizzo_broadcast = OttieniIndirizzoBroadcast(iplocale, wildcardmask);

    int pref_sottorete = LunghPrefissoSottorete(subnetmask);
    int max_n_host = MassimoNumHost(wildcardmask);

    IpClass classe_ip = OttieniClasseIP(iplocale);

    char buffer_ip[50];

    printf("<br><b>INDIRIZZO IP:</b> %s<br>", IpToString(iplocale, buffer_ip));
    printf("<small><i>%s</i></small>", IpToBinaryString(iplocale, buffer_ip));

    printf("<br><b>SUBNET MASK:</b> %s<br>", IpToString(subnetmask, buffer_ip));
    printf("<small><i>%s</i></small>", IpToBinaryString(subnetmask, buffer_ip));

    printf("<br><b>LUNGHEZZA PREF. SOTTORETE: </b> %i<br>", pref_sottorete);
    printf("<br><b>MASSIMO NUMERO DI HOST: </b> %i<br>", max_n_host);

    printf("<br><b>INDIRIZZO DI RETE:</b> %s<br>", IpToString(indirizzo_rete, buffer_ip));

    printf("<br><b>WILDCARD MASK:</b> %s<br>", IpToString(wildcardmask, buffer_ip));

    printf("<br><b>INDIRIZZO BROADCAST:</b> %s<br>", IpToString(indirizzo_broadcast, buffer_ip));

    printf("<br><b>CLASSE IP:</b> %c<br>", (char)classe_ip);

    // unsigned int start = *((unsigned int*)&iplocale);
    // unsigned int end = *((unsigned int*)&indirizzo_broadcast);

    // for (unsigned int i = start; i < end; i++)
    // {
    //     printf("%s<br>", IpToString(*((IpAddress*)&i), buffer_ip));
    // }
    
}