#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "gamelib.h"
extern _Bool debug;
int main(int argc,char**argv)
{
  
  if (argc==2) {
    if (strcmp(argv[1],"debug")==0) {
      debug=true;
    }
  }
  int scelta=0;
  if (debug==true) {
    imposta_gioco();
  }
     do {

       menu(); //richiama il menù principale definito in gamelib.c
       printf("Scegliere tra le opzioni elencate: ");

       scanf("%d",&scelta );
       while(getchar() !='\n');

       if (scelta!=1 && scelta!=2 && scelta!=3)
       {
         printf("la sua scelta non è tra le opzioni elencate, riprovare\n");
       }
       switch (scelta)
       {
         case 1: imposta_gioco();
         break;
         case 2: gioca();
         break;
         case 3: termina_gioco();
         break;

       }
    } while(scelta!=3);


  return 0;
}
