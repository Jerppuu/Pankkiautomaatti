//
//  main.c
//  Pankkiautomaatti
//
//  Created by Jere Pesälä on 28/10/2019.
//  Copyright © 2019 Jere Pesälä. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Funktioiden määritys
int valikko (void);
void nosto (void);
void talletus (void);
void saldo(void);
void kortinpaivitys(void);
int syotteentarkistus(void);

int tilinsaldo=0;
int tunnusluku=0;
char tilinumero[20];




/*Pääalgorytmi lukee tilitiedoston sen jälkeen, kun käyttäjä on syöttänyt tilinumeron ja pin koodin oikein.
  Sen jälkeen se kopioi saldon tilinsaldo funktioon ja käynnistää valikko funktion.*/
int main (void){
    int syotto, yritys=3;
    FILE *tili;


       do {
           printf("Syota tilinumero\n\n");
           scanf("%s", tilinumero);
           strcat(tilinumero, ".tili.txt");
           tili = fopen(tilinumero, "r");
           printf("\n");
       } while (tili == NULL);
       
       fscanf(tili, "%d", &tunnusluku);
       fscanf(tili, "%d", &tilinsaldo);
       fclose(tili);
    
       do {
           printf("Syotä tunnusluku\n\n");
           scanf("%d", &syotto);
           while(fgetc(stdin) != '\n'){
           ;}
           if (tunnusluku != syotto && yritys > 0){
               printf("\n\nVäärä tunnusluku yritä uudelleen\nYrityksiä jäljellä %d\n\n", (yritys-1));
               yritys -= 1;}
           if (yritys == 0){
               printf("\nLiian monta väärä yritystä\n");
               return 0;}
           } while(tunnusluku != syotto);
    
    printf("\n");
    valikko();
    return 0;
}




/*Valikko funktio lukee syötteen ja jos syöte on oikein, valitsee sitä vastaavan funktion.
  Muuten ilmoittaa virheellisestä valinnasta ja palaa alkuun.*/

int valikko(){
char valinta;
    
    do {
        printf("----------------------------------------------\n"  );
        printf("| Tervetuloa automaatille, valitset toiminto |\n"  );
        printf("|           N          Nosto                 |\n"  );
        printf("|           T          Talletus              |\n"  );
        printf("|           S          Saldo                 |\n"  );
        printf("|           Q          Lopeta                |\n"  );
        printf("----------------------------------------------\n\n");
        
        scanf("%sh", &valinta);
        
        if (strcmp(&valinta, "N") == 0){
            printf("\n");
            nosto();}
        
        else if (strcmp(&valinta, "T") == 0){
            printf("\n");
            talletus();}
        
        else if (strcmp(&valinta, "S") == 0){
            printf("\n");
            saldo();}
        
        else if (strcmp(&valinta, "Q")  == 0){
            printf("\nKiitos Käynnistä!\n\n\n");}
            
        else
            printf("\nVirheellinen valinta\n\n");
        
    }while (strcmp(&valinta, "Q") != 0);
    exit (0);
}




/*Nosto funktio tarkistaa ensin, onko tilillä rahaa vähintään 20€, joka on minimi nosto.
  Jos ei, palauttaa se käyttäjän alkuvalikkoono. Muuten funktio kysyy nostettavaa summaa.
  Nostettavien arvojen sisälle sopivan sumnman saatuaan funktio vähentää summan tililtä,
  muokkaa sen kortinpäivitys funktiolla tilitiedostoon ja laskee yksinkertaisella while rakenteella,
  miten setelit saadaan mahdollisimman isoina seteleinä annettua. Tämän jälkeen se tulostaa annettavat
  setelien määrät ja palaa valikkoon.*/

void nosto(){
    int syote=0, seteli20=0, seteli50=0;
    if (tilinsaldo < 20){
        printf("Virhe, tilillä ei ole tarpeeksi saldoa\n\n");
        valikko();}
    
    printf("Syötä nostettava summa, vähintään 20€, enintään 1000€\nSyötä Q palataksesi päävalikkooon\n\n");
    syote = syotteentarkistus();
    while ((syote % 10) != 0 || syote < 20 || syote == 30 || syote > 1000){
        printf("\nSummaa ei voi nostaa, yritä eri summaa\n\n");
        syote = syotteentarkistus();}
    
    if (tilinsaldo < syote){
        printf("\nTilin saldo ei riitä nostoon\n\n");
        valikko();}
    
    tilinsaldo -= syote;
    kortinpaivitys();
    
    while ((syote % 50) !=0){
        syote -= 20;
        seteli20 += 1;
    }
    seteli50 = syote / 50;
    
    printf("\nSaat:\n");
    if (seteli20 > 0){
        printf("%d kpl 20e seteleitä\n", seteli20);}
    if (seteli50 > 0){
        printf("%d kpl 50e seteleitä\n", seteli50);}
    printf("\nRahat tulossa, Kiitos käynnistä!\n\n");
    }




/*Talletus tallettaa saldo muuttujalle syötetyn summan, ja muokkaa sen kortinpäivitys funktiolla tilitiedostoon.
  Lisäksi se kettuilee käyttäjälle, jos hän yrittää syöttää miinuslukuja talletussummaksi.*/

void talletus(){
    int syote=0;
    printf("Syötä talletettava määrä\nSyötä Q palataksesi päävalikkooon.\n\n");
    syote = syotteentarkistus();
    if (syote < 0) {
        printf("\nNostot tapahtuu noston kautta ;)\n\n");
        valikko();
    }
    tilinsaldo += syote;
    kortinpaivitys();
    printf("\nRahat talletettu\n\n");
}




/*Saldo lukee tilin saldon saldo muuttuujasta, ja tulostaa sen käyttäjälle.*/

void saldo (){
    printf("Saldosi on %d euroa.\n\n", tilinsaldo);
}




/*Syöttentarkistus tarkistaa syötteen kokonaisluvuksi ennen kuin syöttää sen funktioon.
  Lisäksi se palauttaa käyttäjän päävalikkoon, jos käyttäjä syöttää Q:n. Hyväksytyn luvun
  se palauttaa int paluuarvona.*/

int syotteentarkistus(){
    char syote[10];
    char tarkistus[10];
    int luku = 0;
    
    do {
        scanf("%s", syote);
        sscanf(syote, "%d", &luku);
        while(fgetc(stdin) != '\n'){;}
        
        if (strcmp(syote, "Q") == 0){
            printf("\nPalataan valikkoon\n\n");
            return valikko();}
        
        sprintf(tarkistus, "%d", luku);

        if(luku == 0 || (strcmp(syote, tarkistus) != 0)){
        printf("\nVirheellinen summa, yritä uudelleen\n\n");
        }
    } while(luku == 0 || (strcmp(syote, tarkistus) != 0));
    return luku;
}




/*Kortinpäivitys päivittää saldon muutoksen tilitiedostoon.
  Kutsutaan aina kun saldon määrä muuttuu nostossa tai talletuksessa.*/

void kortinpaivitys(){
    FILE *tili;
    
    tili = fopen(tilinumero, "w");
        
    fprintf(tili, "%d\n", tunnusluku);
    fprintf(tili, "%d", tilinsaldo);
    fclose(tili);
}
