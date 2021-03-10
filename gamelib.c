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
const char* stato_giocatore[]= {"astronauta", "impostore", "assassinato", "defenestrato"};
const char* tipo_stanza[]= {"vuota", "quest_semplice", "quest_complicata", "botola"};
static void prima_stanza();
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

  giocatori = (struct Giocatore*) malloc(sizeof(struct Giocatore)*scelta); //creo nell'heap l'array struct Giocatore* giocatori
  stanza_inizio= (struct Stanza*) malloc(sizeof(struct Stanza));
  n_giocatori=scelta;
  for (int i = 0; i < scelta; i++)
    {

      struct Giocatore *giocatore_corrente= malloc(sizeof(struct Giocatore));
      giocatore_corrente->posizione_stanza= NULL;
      giocatore_corrente->nome= rand()%scelta; //genera i giocatori randomicamente
      giocatore_corrente->stato= astronauta;

      giocatori[i]= *giocatore_corrente;

    }

  printf("Digitare il numero delle quest da eseguire per vincere il gioco: ");
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
    free(stanza_inizio);
}

void menu() // Definizione della funzione menu
{
   printf("Menù di scelta:\n 1) Imposta gioco\n 2) Gioca\n 3) Termina gioco\n" );
}

void prima_stanza() // in questa funzione definisco il tipo della stanza iniziale
{
  stanza_inizio->avanti= NULL;
  stanza_inizio->sinistra=NULL;
  stanza_inizio->destra=NULL;
  stanza_inizio->stanza_precedente=NULL;
   int random= rand()%100;
   if (random<=24)

     printf("La stanza iniziale è di tipo %s\n", tipo_stanza[stanza_inizio->tipo] );

   else

     if (random>=25 && random<=39)
     {
       printf("La stanza iniziale è di tipo %s\n", tipo_stanza[stanza_inizio->tipo] );
     }
      else

        if (random>=40 && random<=69)
        {
          printf("La stanza iniziale è di tipo %s\n", tipo_stanza[stanza_inizio->tipo] );
        }
        else

          if (random>=70 && random<=99)
          {
            printf("La stanza iniziale è di tipo %s\n", tipo_stanza[stanza_inizio->tipo] );
          }

  }
void stampa_giocatori() //stampa le informazioni relative ad ogni giocatore
{
  for (int i = 0; i < n_giocatori; i++)
  {

    switch (giocatori[i].nome)
    {
      case 0: printf("rosso\n");
      break;
      case 1: printf("blu\n");
      break;
      case 2: printf("verde\n");
      break;
      case 3: printf("giallo\n");
      break;
      case 4: printf("bianco\n");
      break;
      case 5: printf("nero\n");
      break;
      case 6: printf("marrone\n");
      break;
      case 7: printf("arancione\n");
      break;
      case 8: printf("viola\n");
      break;
      case 9: printf("fucsia\n");
      break;
    }
  }
  prima_stanza(); //tutti i giocatori iniziano il gioco in questa stanza iniziale
}
void inizia_gioco(){}
