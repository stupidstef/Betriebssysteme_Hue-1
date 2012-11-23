typedef struct liste {
  void *kopf;
  struct liste *rest;
} *Liste;
  
EXTERNCPP Liste listeLeer(void);
EXTERNCPP Liste listeNeu(void *element);
EXTERNCPP Liste listeAnfuegen(Liste l, void* elem);
EXTERNCPP void  listeLoeschen(Liste l);
EXTERNCPP void* listeKopf(Liste l);
EXTERNCPP Liste listeRest(Liste l);
EXTERNCPP int   listeIstleer(Liste l);
EXTERNCPP int   listeLaenge(Liste l);
