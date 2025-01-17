#include "gamelib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {

  int scelta = 0;

  do {

    menu(); // richiama il menù principale definito in gamelib.c
    printf("Scegliere tra le opzioni elencate: ");

    scanf("%d", &scelta);
    while (getchar() != '\n')
      ;

    if (scelta != 1 && scelta != 2 && scelta != 3) {
      printf("la sua scelta non è tra le opzioni elencate, riprovare\n");
    }
    switch (scelta) {
    case 1:
      imposta_gioco();
      break;
    case 2:
      gioca();
      break;
    case 3:
      termina_gioco();
      break;
    }
  } while (scelta != 3);

  return 0;
}
