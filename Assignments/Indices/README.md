To ćwiczenie ma za zadanie zaznajomienie Państwa z działaniem bufora indeksów. Jak Państwo pewnie zauważyli część wierchołków w buforze wierzchołków się powtarza. W przypadku dużych siatek każdy wierzchołek może powtarzać się kilka razy co prowadzi do marnotrawienia zasobów, a co za tym idzie wolniejszego działania aplikacji.  Żeby temu zaradzić można posłużyć się buforem indeksów. Idea polega na załadowaniu do bufora wierzchołków tylko niepowtarzające się wierzchołki. Następnie w celu zdefiniowania trókątów  posługujemy  się _indeksami_ tych wierzchołków które przekazujemu właście w buforze indeksów.


1. Na początek nie będziemy usuwać wierzchołków, tylko dodamy bufor indeksów.  W tym celu należy zadeklarować nowy wektor `vector<std::GLushort>` który będzie zawierał indeksy. Ponieważ na razie nic nie zmieniamy w buforze wierzchołków wypełniamy go po kolei indeksami od `0` do `liczba wierzchołków -1`. 

1. Następnie  poleceniem `glGenBuffers` tworzymy nowy bufor indkesów w taki sam sposób jak tworzyliśmy bufor wierchołków. Następnie bindujemy ten bufor poleceniem `glBindBuffer` podając za pierwszy argument `GL_ELEMENT_ARRAY_BUFFER`. Potem poleceniem `glBufferData` kopiujemy do niego dane z wektora indeksów i odbindowujemy bufor. 

1. Teraz   jeszcze raz bindujemy ten bufor, ale robimy to pomiędzy wywołaniami `glBindVertexArray(vao_)` i `glBindVertexArray(0)`. W ten sposób  zostanie to zapomiętane w Vertex Array Object (VAO) `vao_`. 

1. I na koniec w funkcji `frame` podmieniamy  funkcję rysującą na `glDrawElements` oczywiście zmieniając odpowiednio argumenty. 