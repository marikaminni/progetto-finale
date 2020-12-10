#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main(void)
{
  int scelta=0;
   do {
     menu();
     printf("Scegliere tra le opzioni elencate: \n" );
     scanf("%d",&scelta );

     if (scelta!=1 && scelta!=2 && scelta!=3)
     {
       printf("la sua scelta non è tra le opzioni elencate, riprovare\n" );
     }

  } while(scelta!=1 && scelta!=2 && scelta!=3);

   switch (scelta)
   {
     case 1: imposta_gioco();
     break;
     case 2: gioca();
     break;
     case 3: termina_gioco();
     break;

   }
  return 0;
}
