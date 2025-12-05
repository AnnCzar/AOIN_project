# Christmas Tree Packing - Projekt C++


---

## Wymagania

### Oprogramowanie
- **Visual Studio Code** (lub inny edytor)
- **C++ Compiler** (g++ 11.x lub nowszy)
- **CMake** (wersja 3.15+)


---




## Struktura Projektu

```
AOIN_CHOINKI/                      # Główny katalog projektu
│
├── CMakeLists.txt                 # Konfiguracja CMake (budownaie projektu)
├── README.md                      
│
├── src/                           # Kod źródłowy
│   ├── ChristmasTree.h           # Nagłówek klasy choinki
│   ├── ChristmasTree.cpp         # Implementacja klasy choinki
│   ├── Geometry.h                # Nagłówek funkcji geometrycznych
│   ├── Geometry.cpp              # Implementacja geometrii
│   ├── GreedyPacker.h            # Nagłówek algorytmu greedy
│   ├── GreedyPacker.cpp          # Implementacja algorytmu greedy
│   └── main.cpp                  # Punkt wejścia programu
│
├── build/                         # Katalog kompilacji (utworzy CMake)
│   └── christmas_greedy          # Skompilowany program (wykonywalny)
│
└── data/                          # Dane wejściowe/wyjściowe
    └── output/
        └── submission.csv        # Wynikowy plik CSV
```

---


## Po co jest CMakeLists.txt?

Tutaj wrzucanie wszytsko potrzebne do zbudwania projektu, typu:
- nazwa projektu, standard jezyka -- już to jest
- jakie pliki źródłowe mają być kompilowane -- wszytskie pliki .cpp z src
- nazwa programu wynikowego
- jakie biblioteki beda uzywane -- jak cos dodajecie to wpisujcie tutaj jak zainastalwoac 


## GEOS BIBLIOTEKA
1. https://libgeos.org/usage/download/ -> stąd pobrałem 3.14.1
2. w AOIN_project/ zrobiłem mkdir libs
3. w AOIN_project/libs wypakowałem bibliotekę
4. w libs/geos-3.14.1 jest plik INSTALL.md i tam jest opis, ja robiłem dla linuxa i potem sudo make install
5. i się wszystko zainstalowało ogólnie więc chyba ok? sorki ja pierwszy raz używam cmake i cpp też trochę dziki jest

## Kompilacja i Uruchomienie
Strona do instalowania cmake cmake.org/download/
Zainstalujcie sobie wtyczke w VScode CMake Tools
### Metoda 2: Terminal (Zalecana)

```bash

cd build
 
# Konfiguruj projekt CMake
cmake ..

# Kompiluj
make

#  Uruchom program
./christmas_greedy
```

### Metoda 3: Visual Studio Code

1. Otwórz projekt w VS Code
2. Naciśnij **Ctrl+Shift+P**
3. Wpisz: `CMake: Configure`
4. Naciśnij **F7** (Build)
5. Otwórz terminal (Ctrl+`) i uruchom:
   ```bash
   cd build
   ./christmas_greedy
   ```
