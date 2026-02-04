# Algorytmy optymalizacji inspirowane naturą
## Christmas Tree Packing Challenge

Projekt implementuje kilka metaheurystyk dla problemu upakowania choinek w jak najmniejszym kwadratowym pudełku (Kaggle: *Santa 2025 – Christmas Tree Packing Challenge*). Przestrzeń rozwiązań jest ciągła – położenia choinek opisują współrzędne \((x,y)\) oraz kąty obrotu w stopniach.

Zaimplementowane metody:
- algorytm zachłanny (Greedy),
- symulowane wyżarzanie (SA),
- ewolucja różnicowa (DE),
- przeszukiwanie tabu (TS),
- wstępna implementacja PSO. 

---

## Struktura projektu (najważniejsze pliki)

- `main.cpp` – punkt wejścia; w zależności od zakomentowanych bloków uruchamia DE, SA lub TS.
- `GreedyPacker.h / .cpp` – algorytm zachłanny, generuje rozwiązania startowe i przepakowuje choinki.
- `DE.h / .cpp` – implementacja ewolucji różnicowej.
- `simulated_annealing.h / .cpp` – implementacja SA.
- `Tabu.h / .cpp` – implementacja przeszukiwania tabu.
- `Pso.h / .cpp` – implementacja PSO (eksperymentalna).
- `ChristmasTree.h / .cpp` – reprezentacja choinki (pozycja, kąt, kształt).
- `evaluation.h / .cpp` – liczenie funkcji celu i wczytywanie plików submission.
- `submission_file.h / .cpp`, `CSVWriter.h` – zapis wyników i statystyk do plików CSV.
- katalog`data/*`, – wyjściowe dane.

---

## Wymagania

- kompilator C++ (zalecane C++17),
- biblioteka standardowa + GEOS

---

## Jak wybrać metody w `main.cpp`

W pliku `main.cpp` znajdują się duże bloki kodu odpowiadające różnym metaheurystykom (np. DE, SA, TS), każdy z własną funkcją `int main()`.

Aby uruchomić konkretną metodę:

1. Otwórz `main.cpp`.
2. **Zakomentuj** cały blok `main()` dla wszystkich metod, których nie chcesz aktualnie uruchamiać.
3. **Odkomentuj** blok `main()` dla metaheurystyki, którą chcesz uruchomić, tak aby w projekcie istniała dokładnie jedna funkcja `int main()`.
4. Zapisz plik i przebuduj projekt.

---

## Uwaga dotycząca innych plików

Oprócz zmiany aktywnego `main()` w `main.cpp` **w niektórych pozostałych plikach źródłowych również trzeba zakomentować / odkomentować odpowiednie fragmenty kodu**, tak aby kompilowana była tylko logika dla aktualnie używanej metaheurystyki.




## Budowanie i uruchamianie (Windows, CMake + Visual Studio)

W katalogu głównym projektu w PowerShellu:

```powershell
cd ścieżka\do\katalogu\projektu

# budowanie
Remove-Item -Recurse -Force build
mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

# powrót do katalogu projektu - tam są wymagane pliki wejściowe
cd ..

.\build\Release\christmas_greedy_opt.exe
```