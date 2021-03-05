void menu(); //Dichiarazione del prototipo di funzione
void imposta_gioco();
void gioca();
void termina_gioco();

enum Stato_giocatore    {astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore     {rosso, blu, verde, giallo, bianco, nero, marrone, arancione, viola, fucsia};
enum Tipo_stanza        {vuota, quest_semplice, quest_complicata, botola};
enum Boolean            {effettuata, non_effettuata};

struct Stanza
{
  struct Stanza* avanti;
  struct Stanza* sinistra;
  struct Stanza* destra;
  struct Stanza* stanza_precedente;
  enum Tipo_stanza descrizione;
  enum Boolean emergenza_chiamata;

};

struct Giocatore
{
  struct Stanza* posizione_stanza;
  enum Stato_giocatore stato;
  enum Nome_giocatore nome;
};
