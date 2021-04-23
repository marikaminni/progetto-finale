#include "gamelib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static struct Giocatore *giocatori;
struct Stanza *lista_stanze;
struct Stanza *stanza_inizio;
unsigned short quest_da_finire, n_quest;
static void stampa_giocatori();
static _Bool gioco_impostato = false;
static int n_giocatori = 0;
static int max_num_giocatori = 10;
_Bool debug = false;
const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
void stampa_stato(enum Stato_giocatore stato);
void stampa_nome(enum Nome_giocatore nome);
void stampa_stanza(enum Tipo_stanza choice);

static enum Tipo_stanza randomStanza() {
  int randStanza = ((rand() % 100) + 1);
  if (randStanza <= 25)
    return botola;
  else if (randStanza <= 40)
    return quest_semplice;
  else if (randStanza <= 70)
    return quest_complicata;
  else
    return vuota;
}
static enum Stato_giocatore randStato(int n_giocatori) {
  int randStato = (rand() % 100) + 1;
  int middle = 50;
  if (n_giocatori == 4) {
    middle = 25;
  } else if (n_giocatori > 4 && n_giocatori < 7) {
    middle = 33;
  } else if (n_giocatori >= 7) {
    middle = 30;
  }
  if (randStato >= middle) {
    return astronauta;
  } else
    return impostore;
}

static void avanza(struct Giocatore *giocatore_corrente) {
  int direzione_presa = 0;
  do {
    printf("Scegliere in quale direzione procedere :\n");
    int scelta_direzione;
    printf(" -1 avanti\n");
    printf(" -2 destra\n");
    printf(" -3 sinistra\n");
    printf(" -4 resta fermo\n");
    scanf("%d", &scelta_direzione);
    while (getchar() != '\n')
      ;
    if (scelta_direzione < 1 || scelta_direzione > 4) {
      printf("si deve inserire un numero compreso fra 1 e 4,riprovare\n");
    } else
      direzione_presa = scelta_direzione;
  } while (direzione_presa == 0);

  switch (direzione_presa) {
  case 1:
    if (giocatore_corrente->posizione_stanza->avanti == NULL) {
      struct Stanza *nuova_stanza =
          (struct Stanza *)malloc(sizeof(struct Stanza));
      nuova_stanza->descrizione = randomStanza();
      nuova_stanza->emergenza_chiamata = non_effettuata;
      giocatore_corrente->posizione_stanza->avanti = nuova_stanza;
      nuova_stanza->stanza_precedente = lista_stanze;
      lista_stanze = nuova_stanza;
    }
    giocatore_corrente->posizione_stanza =
        giocatore_corrente->posizione_stanza->avanti;
    break;
  case 2:
    if (giocatore_corrente->posizione_stanza->destra == NULL) {
      struct Stanza *nuova_stanza =
          (struct Stanza *)malloc(sizeof(struct Stanza));
      nuova_stanza->descrizione = randomStanza();
      nuova_stanza->emergenza_chiamata = non_effettuata;
      giocatore_corrente->posizione_stanza->destra = nuova_stanza;
      nuova_stanza->stanza_precedente = lista_stanze;
      lista_stanze = nuova_stanza;
    }
    giocatore_corrente->posizione_stanza =
        giocatore_corrente->posizione_stanza->destra;
    break;
  case 3:
    if (giocatore_corrente->posizione_stanza->sinistra == NULL) {
      struct Stanza *nuova_stanza =
          (struct Stanza *)malloc(sizeof(struct Stanza));
      nuova_stanza->descrizione = randomStanza();
      nuova_stanza->emergenza_chiamata = non_effettuata;
      giocatore_corrente->posizione_stanza->sinistra = nuova_stanza;
      nuova_stanza->stanza_precedente = lista_stanze;
      lista_stanze = nuova_stanza;
    }
    giocatore_corrente->posizione_stanza =
        giocatore_corrente->posizione_stanza->sinistra;
    break;
  case 4:
    printf("il giocatore rimane nella stanza \n");
    break;
  }
};

static void esegui_quest(struct Giocatore *giocatore_corrente) {
  if (giocatore_corrente->posizione_stanza->descrizione == quest_semplice) {
    quest_da_finire++;
    giocatore_corrente->posizione_stanza->descrizione = vuota;
  } else if (giocatore_corrente->posizione_stanza->descrizione ==
             quest_complicata) {
    quest_da_finire += 2;
    giocatore_corrente->posizione_stanza->descrizione = vuota;
  }

  else if (giocatore_corrente->posizione_stanza->descrizione == vuota ||
           giocatore_corrente->posizione_stanza->descrizione == botola) {
    printf("Per poter eseguire la quest, il tipo della stanza deve essere: "
           "semplice o complicata\n");
  }
};
static void chiamata_emergenza(struct Giocatore *altrogiocatore) {
  if (altrogiocatore->posizione_stanza->emergenza_chiamata == non_effettuata) {

    struct Giocatore *giocatore_corrente;
    int n_assassinati = 0;
    for (int i = 0; i < n_giocatori; i++) {
      giocatore_corrente = &giocatori[i];
      if (giocatore_corrente->posizione_stanza ==
          altrogiocatore->posizione_stanza) {
        if (giocatore_corrente->stato == assassinato) {
          n_assassinati++;
        }
      }
    }
    if (n_assassinati == 0) {
      printf(
          "Non si può effettuare la chiamata perchè non ci sono assassinati\n");
      return;
    }
    altrogiocatore->posizione_stanza->emergenza_chiamata = effettuata;
    int n_defenestrati = 0;
    for (int i = 0; i < n_giocatori; i++) {
      giocatore_corrente = &giocatori[i];
      int soglia = 30;
      if (giocatore_corrente->stato != assassinato ||
          giocatore_corrente->stato == defenestrato) {
        struct Giocatore *giocatore_stanza;
        for (int i = 0; i < n_giocatori; i++) {
          giocatore_stanza = &giocatori[i];
          if (giocatore_stanza != giocatore_corrente &&
              giocatore_stanza->posizione_stanza ==
                  giocatore_corrente->posizione_stanza) {
            if (giocatore_corrente->stato == astronauta) {
              switch (giocatore_stanza->stato) {
              case astronauta:
                soglia -= 30;
                break;
              case impostore:
                soglia += 20;
                break;
              case assassinato:
                break;
              case defenestrato:
                break;
              }
            } else if (giocatore_corrente->stato == impostore) {
              switch (giocatore_stanza->stato) {
              case impostore:
                soglia -= 30;
                break;
              case astronauta:
                soglia += 20;
                break;
              case assassinato:
                break;
              case defenestrato:
                break;
              }
            }
          }
        }
        if ((rand() % 100) + 1 >= soglia) {
          giocatore_corrente->stato = defenestrato;
          stampa_nome(giocatore_corrente->nome);
          printf(" è stato defenestrato\n");
          n_defenestrati++;
          break;
        }
      }
    }
    if (n_defenestrati == 0) {
      printf("Nessuno è stato defenestrato\n");
    }
  } else {
    printf("In questa stanza è già stata eseguita la chiamata\n");
  }
};
static void uccidi_astronauta(struct Giocatore *giocatore_corrente){};
/*struct Giocatore *altrogiocatore;
struct Giocatore *giocatore_stanza;
for (int i = 0; i < n_giocatori; i++) {
  altrogiocatore = &giocatori[i];
  if (giocatore_corrente->posizione_stanza ==
      altrogiocatore->posizione_stanza) {
    if (altrogiocatore->stato == astronauta) {
      altrogiocatore->stato = assassinato;
      stampa_nome(altrogiocatore->nome);
      printf(" è stato assassinato\n");
      break;
    }
  }
   int probabilita = 0;
    if (giocatore_corrente->posizione_stanza ==
        altrogiocatore->posizione_stanza) {
      if (giocatore_stanza->stato == astronauta) {
        probabilita += 50;
      }

    } else if () {

  }
  }
}
}
if ((rand() % 100) + 1 >= probabilita) {
  giocatore_corrente->stato = defenestrato;
  stampa_nome(giocatore_corrente->nome);
  printf(" è stato defenestrato\n");
  n_defenestrati++;
  break;
}
};*/
static void usa_botola(struct Giocatore *giocatore_corrente) {
  if (giocatore_corrente->posizione_stanza->descrizione == botola) {

  } else
    printf("Per poter utilizzare la funzione usa botola, la stanza deve "
           "essere "
           "di tipo botola\n");
};
static void sabotaggio(struct Giocatore *giocatore_corrente) {
  if (giocatore_corrente->posizione_stanza->descrizione == quest_semplice ||
      giocatore_corrente->posizione_stanza->descrizione == quest_complicata) {

    giocatore_corrente->posizione_stanza->descrizione = vuota;

  } else if (giocatore_corrente->posizione_stanza->descrizione == vuota ||
             giocatore_corrente->posizione_stanza->descrizione == botola) {
    printf("Per poter effettuare il sabotaggio, il tipo della stanza deve "
           "essere: "
           "semplice o complicata\n");
  }
};

void imposta_gioco() {
  time_t t;
  srand((unsigned)time(&t));
  int scelta = 0;
  n_quest = 0;
  quest_da_finire = 0;
  if (debug == true) {
    scelta = 3;
  } else {
    do {
      printf("Digitare un numero da 4 a 10 per impostare il numero dei "
             "giocatori: "); // richiedo il numero dei giocatori
      scanf("%d", &scelta);
      while (getchar() != '\n')
        ;

      if (scelta < 4 || scelta > 10) {
        printf("si deve inserire un numero compreso fra 4 e 10,riprovare\n");
      }
    } while (scelta < 4 || scelta > 10);
  }

  giocatori = (struct Giocatore *)malloc(
      sizeof(struct Giocatore) *
      scelta); // creo nell'heap l'array struct Giocatore* giocatori
  stanza_inizio = (struct Stanza *)malloc(
      sizeof(struct Stanza)); // creo nell'heap la stanza iniziale
  lista_stanze = stanza_inizio;
  stanza_inizio->emergenza_chiamata = non_effettuata;
  enum Nome_giocatore
      temp_nomi[10]; // utilizzo una variabile temporanea per evitare di
                     // avere ripetizioni nei nomi dei giocatori

  for (int i = 0; i < max_num_giocatori; i++) {
    temp_nomi[i] = (enum Nome_giocatore)i;
  }
  for (int i = 0; i < max_num_giocatori; i++) { // shuffle array
    enum Nome_giocatore temp = temp_nomi[i];
    int randomNomi = rand() % max_num_giocatori;
    temp_nomi[i] = temp_nomi[randomNomi];
    temp_nomi[randomNomi] = temp;
  }
  int num_impostori = 0;
  n_giocatori = scelta;
  for (int i = 0; i < scelta; i++) {
    struct Giocatore *giocatore_corrente = malloc(sizeof(struct Giocatore));
    giocatori[i] = *giocatore_corrente;
  }
  enum Tipo_stanza stanza = randomStanza();

  switch (stanza) {
  case quest_semplice:
    n_quest++;
    break;
  case quest_complicata:
    n_quest += 2;
    break;
  case botola:
    break;
  case vuota:
    break;
  }
  stanza_inizio->avanti = NULL;
  stanza_inizio->sinistra = NULL;
  stanza_inizio->destra = NULL;
  stanza_inizio->stanza_precedente = NULL;
  stanza_inizio->descrizione = stanza;
  while (num_impostori == 0) {
    for (int i = 0; i < scelta; i++) {
      struct Giocatore *giocatore_corrente = &giocatori[i];
      giocatore_corrente->posizione_stanza = stanza_inizio;
      giocatore_corrente->nome = temp_nomi[i];
      giocatore_corrente->stato = randStato(n_giocatori);
      if (giocatore_corrente->stato == impostore) {
        if (num_impostori < 3) {
          num_impostori++;
        } else
          giocatore_corrente->stato = astronauta;
      }
    }
  }

  if (debug == true) {
    n_quest = 2;
    gioco_impostato = true;
    stampa_giocatori();
  } else {
    printf("Digitare il numero delle quest da eseguire per "
           "vincere il "
           "gioco: ");
    scanf("%hd", &n_quest);
    while (getchar() != '\n')
      ;

    printf("Decidere se:\n 1) Stampare i giocatori \n 2) "
           "Iniziare il "
           "gioco\n");
    scanf("%d", &scelta);
    while (getchar() != '\n')
      ;
    gioco_impostato = true;
    do {

      switch (scelta) {
      case 1:
        stampa_giocatori();
        break;
      case 2:
        gioca();
        break;
      default:
        printf("scelta non valida, riprovare\n");
      }

    } while (scelta != 1 && scelta != 2);
  }
}

void gioca(int n_quest) {
  if (gioco_impostato ==
      false) // non si può giocare se non si è impostato il gico
  {
    printf("Prima devi impostare il gioco \n");
  } else {
    // printf("\033[0;32m");
    printf("\n\t\t-------------------------------------------------"
           "------"
           "---\n");
    printf(" \t\t                         START              \n");
    printf("\t\t---------------------------------------------------"
           "------"
           "-\n");
  }
  int ordine_giocatori[n_giocatori];
  for (int i = 0; i < n_giocatori; i++) {
    ordine_giocatori[i] = i;
  }
  int turno = 1;
  // int turn_counter = 0;
  do {
    struct Giocatore *giocatore_corrente;
    for (int i = 0; i < n_giocatori; i++) { // shuffle array
      int temp = ordine_giocatori[i];
      int randTurni = rand() % n_giocatori;
      ordine_giocatori[i] = ordine_giocatori[randTurni];
      ordine_giocatori[randTurni] = temp;
    }
    for (int i = 0; i < n_giocatori; i++) {
      giocatore_corrente = &giocatori[ordine_giocatori[i]];
      if (giocatore_corrente->stato == assassinato ||
          giocatore_corrente->stato == defenestrato) {
        continue;
      }
      printf("%s", CLEAR_SCREEN_ANSI);
      printf("---------------------------------------\n");
      printf("è il turno %d, gioca ", turno);
      stampa_nome(giocatore_corrente->nome);
      stampa_stato(giocatore_corrente->stato);
      stampa_stanza(giocatore_corrente->posizione_stanza->descrizione);
      printf("altri giocatori nella stanza:\n");
      for (int i = 0; i < n_giocatori; i++) {

        struct Giocatore *altrogiocatore = &giocatori[i];
        if (altrogiocatore->posizione_stanza ==
                giocatore_corrente->posizione_stanza &&
            altrogiocatore->nome != giocatore_corrente->nome) {
          printf("\t- ");
          stampa_nome(altrogiocatore->nome);
          printf("\n");
        }
      }
      int scelta_corretta = 0;
      do {
        printf("Scegli una delle seguenti azioni:\n");
        int scelta;

        switch (giocatore_corrente->stato) {
        case astronauta:
          printf(" -1: avanza\n");
          printf(" -2: esegui quest\n");
          printf(" -3: chiamata emergenza\n");
          break;
        case impostore:
          printf(" -1: avanza\n");
          printf(" -2: uccidi astronauta\n");
          printf(" -3: chiamata emergenza\n");
          printf(" -4: usa botola\n");
          printf(" -5: sabotaggio\n");
          break;

        case defenestrato:
          scelta_corretta = -1;
          break;
        case assassinato:
          scelta_corretta = -1;
          break;
        }
        printf("azione:");
        scanf("%d", &scelta);
        while (getchar() != '\n')
          ;
        switch (giocatore_corrente->stato) {
        case astronauta:
          if (scelta >= 1 && scelta <= 3) {
            scelta_corretta = scelta;
          }

          break;
        case impostore:
          if (scelta >= 1 && scelta <= 5) {
            scelta_corretta = scelta;
          }

          break;
        case defenestrato:
          break;
        case assassinato:
          break;
        }
      } while (scelta_corretta == 0);

      switch (giocatore_corrente->stato) {
      case astronauta:
        if (scelta_corretta == 1) {
          avanza(giocatore_corrente);
        } else if (scelta_corretta == 2) {
          esegui_quest(giocatore_corrente);
        } else if (scelta_corretta == 3) {
          chiamata_emergenza(giocatore_corrente);
        }

        break;
      case impostore:
        if (scelta_corretta == 1) {
          avanza(giocatore_corrente);
        } else if (scelta_corretta == 2) {
          uccidi_astronauta(giocatore_corrente);
        } else if (scelta_corretta == 3) {
          chiamata_emergenza(giocatore_corrente);
        } /*else if(scelta_corretta==4){
           usa_botola(giocatore_corrente);
         }*/
        else if (scelta_corretta == 5) {
          sabotaggio(giocatore_corrente);
        }

        break;
      case defenestrato:
        break;
      case assassinato:
        break;
      }
      if (quest_da_finire == n_quest) {
        printf("\t\tGli astronauti vincono!\n");
        termina_gioco();
      }
      sleep(2);
    }
    turno++;
    // break;
  } while (true);
}

void termina_gioco() {
  struct Stanza *stanza_corrente = lista_stanze;
  while (stanza_corrente != NULL) {
    printf("%p %p %p\n", stanza_corrente, lista_stanze,
           lista_stanze->stanza_precedente);
    lista_stanze = lista_stanze->stanza_precedente;
    free(stanza_corrente);
    stanza_corrente = lista_stanze;
  }

  /*struct Giocatore *giocatore_corrente;
  for (int i = 0; i < n_giocatori; i++) {

    giocatore_corrente = &giocatori[i];
    giocatore_corrente->posizione_stanza = NULL;
    printf("%p \n", giocatore_corrente);
    free(giocatore_corrente);
  }*/
  free(giocatori);

  printf("\n\t\t---------------------------------------------------"
         "------"
         "-\n");
  printf(" \t\t                         GAME OVER             \n");
  printf("\t\t-----------------------------------------------------"
         "-----\n");
}

void menu() // Definizione della funzione menu
{
  printf("Menù di scelta:\n 1) Imposta gioco\n 2) Gioca\n 3) Termina "
         "gioco\n");
}

void stampa_stanza(enum Tipo_stanza choice) // definisco il tipo
                                            // della stanza iniziale

{

  switch (choice) {
  case botola:
    printf("la stanza è di tipo botola\n");
    break;
  case quest_semplice:
    printf("la stanza è di tipo quest_semplice\n");
    break;
  case quest_complicata:
    printf("la stanza è di tipo quest_complicata\n");
    break;
  case vuota:
    printf("la stanza è di tipo vuota\n");
    break;
  }
}

void stampa_nome(enum Nome_giocatore nome) {
  switch (nome) {
  case 0:
    printf("il giocatore rosso");
    break;
  case 1:
    printf("il giocatore blu");
    break;
  case 2:
    printf("il giocatore verde");
    break;
  case 3:
    printf("il giocatore giallo");
    break;
  case 4:
    printf("il giocatore bianco");
    break;
  case 5:
    printf("il giocatore nero");
    break;
  case 6:
    printf("il giocatore marrone");
    break;
  case 7:
    printf("il giocatore arancione");
    break;
  case 8:
    printf("il giocatore viola");
    break;
  case 9:
    printf("il giocatore fucsia");
    break;
  }
}
void stampa_stato(enum Stato_giocatore stato) {
  switch (stato) {
  case astronauta:
    printf(" è un astronauta\n");
    break;
  case impostore:
    printf(" è un impostore\n");
    break;
  case assassinato:
    printf(" è stato assassinato\n");
    break;
  case defenestrato:
    printf(" è stato defenestrato\n");
    break;
  }
}

void stampa_giocatori() // stampa le informazioni relative ad ogni
                        // giocatore
{
  for (int i = 0; i < n_giocatori; i++) {
    stampa_nome(giocatori[i].nome);
    stampa_stato(giocatori[i].stato);
  }
}
