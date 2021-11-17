1) sizeof(GLfloat) wynosi 4 bajty zatem jeden wierzchołek, razem z kolorem, to 6 * 4B = 24B
2) Bez indeksowania mamy aż 9 wierzchołków, czyli 216B danych
3) Po usunięciu dwóch wierzchołków, przy zachowaniu kolorów per vertex, mamy 168B danych
4) sizeof(GLushort) wynosi 2 bajty dla pojedynczego indeksu, zatem wszystkie indeksy (9) zajmują 18B pamięci

ODP: Oszczędzamy 30 bajtów (2 * 24B - 18B), przy zachowaniu kolorów per vertex.
     Ignorując kolorowanie per vertex możnaby oszczędzić jeszcze więcej pamięci (2 powtarzające się wierzchołki i indeksy).
