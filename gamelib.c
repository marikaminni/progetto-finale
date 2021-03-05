
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gamelib.h"

static struct Giocatore* giocatori;
struct Stanza* lista_stanze;
struct Stanza* stanza_inizio;
unsigned short quest_da_finire;
static void stampa_giocatori();
static void inizia_gioco();
static _Bool gioco_impostato=false;
static int n_giocatori=0;

void imposta_gioco()
{
  time_t t;
  srand((unsigned) time(&t));
  int scelta=0, n_quest=0;

  do
  {
    printf("Digitare un numero da 4 a 10 per impostare il numero dei giocatori: "); //richiedo il numero dei giocatori

    scanf("%d",&scelta );
    while(getchar() !='\n');

    if (scelta < 4 || scelta >10)
    {
      printf("si deve inserire un numero compreso fra 4 e 10,riprovare\n");
    }
  }while (scelta < 4 || scelta >10);

  giocatori = (struct Giocatore*) malloc(sizeof(struct Giocatore)*scelta);
  n_giocatori=scelta;
  for (size_t i = 0; i < scelta; i++)
  {
    struct Giocatore *giocatore_corrente= malloc(sizeof(struct Giocatore));
    giocatore_corrente->posizione_stanza= NULL;
    giocatore_corrente->stato= astronauta;
    giocatore_corrente->nome= rosso;

    giocatori[i]= *giocatore_corrente;
  }

  printf("Digitare il numero delle quest da eseguire per vincere il gioco: ");  // richiedo il numero delle quest necessarie per vincere il gioco

  scanf("%d", &n_quest);
  while(getchar() !='\n');


  printf("Decidere se:\n 1) Stampare i giocatori \n 2) Iniziare il gioco\n");
  scanf("%d", &scelta );
  while(getchar() !='\n');

  do {

      switch (scelta)
      {
        case 1: stampa_giocatori();
        break;
        case 2: inizia_gioco();
        break;
        default: printf("scelta non valida, riprovare\n");
      }

    } while(scelta!=1 && scelta!=2);
  gioco_impostato= true;
}



void gioca()
{
  if (gioco_impostato==false) //non si può giocare se non si è impostato il gico
  {
    printf("Prima devi impostare il gioco \n");
  }
 else
  {
   printf("inizio gioco \n");
  }
}

void termina_gioco()
{

    free(giocatori);

}


void menu() // Definizione della funzione menu
{
   printf("Menù di scelta:\n 1) Imposta gioco\n 2) Gioca\n 3) Termina gioco\n" );
}

void stampa_giocatori() //stampa le informazioni relative ad ogni giocatore
{
  for (size_t i = 0; i < n_giocatori; i++)
  {
    printf(" nome= %d \n", giocatori[i].nome );
  }
}
void inizia_gioco(){}
