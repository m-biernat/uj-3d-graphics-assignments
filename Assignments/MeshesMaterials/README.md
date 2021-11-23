# Meshes and Materials

W tym ćwiczeniu "uporządkujemy" trochę kod, wprowadzając klasę `Mesh` i prosty system materiałów. Cwiczenie wymaga ściągnięcia najnowszej wersji kodu z mojego repozytorium. Może się przy tym okazać, że Państwa kod przestanie działać :( Proszę o zgłaszanie mi wszelkich problemów. 



## Meshes 

W katalogu `src/Engine` powinny się pojawić pliki w których zdefiniowana jest klasa `Mesh`. Jeśli wszystko poszło dobrze to pliki w tej kartotece powinny zostać skompilowane do biblioteki `engine`. Bibliotekę można podłączyć w  pliku `CMakeLists.txt` poleceniem `target_link_libraries`. Jeśli stworzyli Państwo nowy katalog `MeshesMaterials` i przekopiowali Państwo do niego zadanie `Pyramid` to w pliku `CMakeLists.txt` należy dodać po poleceniu `add_executable` polecenie 
```cmake
target_link_libraries(${PROJECT_NAME} PUBLIC engine)
```

Klasa `Mesh` odpowiada jednemu `Vertex Array Object` (VAO)  zawierającemu jeden bufor wierzchołów  i jeden bufor indeksów. Posiada jeden lub więcej obiektów klasy `SubMesh`. Klasa `SubMesh` odpowiada jednemu poleceniu `gLDrawElements` i zawiera początek i koniec zakresu indeksów przekazywanych do  tego polecenia. 

Proszę zapoznać się z tą klasą, a  następnie  wykorzystać ją do wyświetlenia piramidki. 

