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
static int max_num_giocatori=10;
static void prima_stanza();
static enum Tipo_stanza randomStanza()
{
  int randStanza=((rand()%100)+1);
  if (randStanza<=25)
      return botola;
  else if (randStanza<=40)
        return quest_semplice;
  else if (randStanza<=70)
        return quest_complicata;
  else
      return vuota;
}
static enum Stato_giocatore randStato(int n_giocatori){
  int randStato=(rand()%100)+1;
  int middle=50;
  if (n_giocatori==4) {
    middle=25;
  }
  else if (n_giocatori>4 && n_giocatori<7) {
    middle=33;
  }
  else if (n_giocatori>=7) {
    middle=30;
  }
  if (randStato>=middle) {
    return astronauta;
  }
  else return impostore;
}

static void turni_giocatori(){
  int turni;
}

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

  giocatori = (struct Giocatore*) malloc(sizeof(struct Giocatore*)*scelta); //creo nell'heap l'array struct Giocatore* giocatori
  stanza_inizio= (struct Stanza*) malloc(sizeof(struct Stanza)); //creo nell'heap la stanza iniziale

  enum Nome_giocatore temp_nomi[10]; // utilizzo una variabile temporanea per evitare di avere ripetizioni nei nomi dei giocatori

  for (int i = 0; i < max_num_giocatori; i++)
  {
    temp_nomi[i] = (enum Nome_giocatore)i;
  }
  for (int i = 0; i < max_num_giocatori; i++) {    // shuffle array
    enum Nome_giocatore temp = temp_nomi[i];
    int randomNomi = rand() % max_num_giocatori;
    temp_nomi[i]= temp_nomi[randomNomi];
    temp_nomi[randomNomi] = temp;
}
    int num_impostori=0;
    n_giocatori=scelta;
    for (int i = 0; i < scelta; i++)
      {
        struct Giocatore* giocatore_corrente= malloc(sizeof(struct Giocatore));
        giocatori[i]= *giocatore_corrente;
      }
    while (num_impostori==0) {
      for (int i = 0; i < scelta; i++)
        {
          struct Giocatore* giocatore_corrente= &giocatori[i];
          giocatore_corrente->posizione_stanza= NULL;
          giocatore_corrente->nome= temp_nomi[i];
          giocatore_corrente->stato=randStato(n_giocatori);
          if (giocatore_corrente->stato==impostore) {
          num_impostori++;

          }

        }
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
    //printf("\033[0;32m");
    printf("\n\t\t----------------------------------------------------------\n");
    printf(" \t\t                         START              \n");
    printf("\t\t----------------------------------------------------------\n");
  }

}

void termina_gioco()
{
    //free(giocatori);
    free(stanza_inizio);
    stanza_inizio=NULL;
    printf("\n\t\t----------------------------------------------------------\n");
    printf(" \t\t                         GAME OVER             \n");
    printf("\t\t----------------------------------------------------------\n");
}

void menu() // Definizione della funzione menu
{
   printf("Menù di scelta:\n 1) Imposta gioco\n 2) Gioca\n 3) Termina gioco\n" );
}

void prima_stanza(enum Tipo_stanza choice) // in questa funzione definisco il tipo della stanza iniziale
{
  stanza_inizio->avanti= NULL;
  stanza_inizio->sinistra=NULL;
  stanza_inizio->destra=NULL;
  stanza_inizio->stanza_precedente=NULL;
  switch (choice)
  {
    case botola: printf("la stanza iniziale è di tipo botola\n");
    break;
    case quest_semplice: printf("la stanza inziale è di tipo quest_semplice\n");
    break;
    case quest_complicata: printf("la stanza iniziale è di tipo quest_complicata\n");
    break;
    case vuota: printf("la stanza iniziale è di tipo vuota\n");
    break;
  }


  }
void stampa_giocatori() //stampa le informazioni relative ad ogni giocatore
{
  for (int i = 0; i < n_giocatori; i++)
  {

    switch (giocatori[i].nome)
    {
      case 0: printf("il giocatore rosso");
      break;
      case 1: printf("il giocatore blu");
      break;
      case 2: printf("il giocatore verde");
      break;
      case 3: printf("il giocatore giallo");
      break;
      case 4: printf("il giocatore bianco");
      break;
      case 5: printf("il giocatore nero");
      break;
      case 6: printf("il giocatore marrone");
      break;
      case 7: printf("il giocatore arancione");
      break;
      case 8: printf("il giocatore viola");
      break;
      case 9: printf("il giocatore fucsia");
      break;
    }
    switch (giocatori[i].stato) {
      case 0: printf(" è un astronauta\n" );
      break;
      case 1: printf(" è un impostore\n");
      break;
    }
  }
  int counters[4] = {0,0,0,0};
    for (int i=0; i< 1; ++i)
    {
      enum Tipo_stanza choice = randomStanza();
      prima_stanza(choice);
      counters[choice]++;
    }
}
void inizia_gioco(){
  gioco_impostato= true;
  gioca();
}
