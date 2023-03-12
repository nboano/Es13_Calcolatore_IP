#include "lib/KinderC/kinderc.hpp"
#include "../IPAddress.c"

exported void calcola();

int main() {
    calcola();
}

#define txtIndirizzo HTMLInputElement($("#txtIndirizzo"))
#define txtSubnet HTMLInputElement($("#txtSubnet"))

void calcola() {

    // Pulisco tutti i risultati
    $$(".param").setAttribute("value", "");
    $$(".result").innerHTML = "";
    

    const char* locale = txtIndirizzo.value;
    const char* subnet = txtSubnet.value;

    if(!IpValid(locale)) {
        txtIndirizzo.style["color"] = "red";
        return;
    }

    if(!IpValid(subnet)) {
        txtSubnet.style["color"] = "red";
        return;
    }

    txtIndirizzo.style["color"] = "black";
    txtSubnet.style["color"] = "black";

    IpAddress iplocale = IpFromString(locale);
    IpAddress subnetmask = IpFromString(subnet);

    free((void*)locale);
    free((void*)subnet);

    if(!SubnetMaskValid(subnetmask)) {
        txtSubnet.style["color"] = "red";
        return;
    }

    IpAddress indirizzo_rete = OttieniIndirizzoRete(iplocale, subnetmask);
    IpAddress wildcardmask = OttieniWildcardMask(subnetmask);
    IpAddress indirizzo_broadcast = OttieniIndirizzoBroadcast(iplocale, wildcardmask);

    IpAddress first = IntToIp(IpToInt(indirizzo_rete) + 1);
    IpAddress last = IntToIp(IpToInt(indirizzo_broadcast) - 1);
    
    int pref_sottorete = LunghPrefissoSottorete(subnetmask);
    int max_n_host = MassimoNumHost(wildcardmask);

    IpClass classe_ip = OttieniClasseIP(iplocale);

    char buffer_ip[50];

    $("#indirizzo-ip").setAttribute("value", IpToString(iplocale, buffer_ip));
    $("#indirizzo-ip-bin").innerText = IpToBinaryString(iplocale, buffer_ip);

    $("#subnet-mask").setAttribute("value", 
        String::Format("%s/%i", IpToString(subnetmask, buffer_ip), pref_sottorete)
    );
    $("#subnet-mask-bin").innerText = IpToBinaryString(subnetmask, buffer_ip);


    $("#indirizzo-rete").setAttribute("value", IpToString(indirizzo_rete, buffer_ip));
    $("#indirizzo-rete-bin").innerText = IpToBinaryString(indirizzo_rete, buffer_ip);

    $("#indirizzo-broadcast").setAttribute("value", IpToString(indirizzo_broadcast, buffer_ip));
    $("#indirizzo-broadcast-bin").innerText = IpToBinaryString(indirizzo_broadcast, buffer_ip);

    $("#wildcard-mask").setAttribute("value", IpToString(wildcardmask, buffer_ip));
    $("#wildcard-mask-bin").innerText = IpToBinaryString(wildcardmask, buffer_ip);

    $("#max-n-host").setAttribute("value", String::Format("%i", max_n_host));
    $("#classe-ip").setAttribute("value", String::Format("%c", classe_ip));

    if(max_n_host > 0) {
        $("#ip-start").innerText = IpToString(first, buffer_ip);
        $("#ip-end").innerText = IpToString(last, buffer_ip);
    } else {
        $("#ip-start").innerText = "N/D";
        $("#ip-end").innerText = "N/D";
    }

    $("#status-ip").setAttribute("value", IpStatus(iplocale));
}