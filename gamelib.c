#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"

struct Giocatore* giocatori;
struct Stanza* lista_stanze;
struct Stanza* stanza_inizio;
unsigned short quest_da_finire;

void imposta_gioco()
{
  int scelta=0;
  do
  {
    printf("Digitare un numero da 4 a 10 per impostare il numero dei giocatori:");
    scanf("%d",&scelta );
    if (scelta < 4 || scelta >10)
    {
      printf("si deve inserire un numero compreso fra 4 e 10,riprovare\n");
    }
  }while (scelta < 4 || scelta >10);

}












void menu() // Definizione della funzione
{
   printf("Menù di scelta:\n 1) Imposta gioco\n 2) Gioca\n 3) Termina gioco\n" );
}
