#include "BB.h"
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

// zmienne globalne
int liczba_miast;

// Stores the final minimum weight of shortest tour.
int najlepsza_dlugosc = INT_MAX;

// vector najlepsza_sciezka przechowuje najlepsze rozwiązanie
// w danym momencie.
vector<int> najlepsza_sciezka;

// odwiedzone przechowuje odwiedzone misata w danym momencie przechodzenia drogi
vector<bool> odwiedzone;


// wczytuje liczbe miast z maina
void wczytaj_liczbe_miast_BB(int wczytana) {
    liczba_miast = wczytana;
}

// zeruje zmienne globalne
void wyzeruj_zmienne() {
    najlepsza_sciezka.clear();
    odwiedzone.clear();
    najlepsza_dlugosc = INT_MAX;

}

int licznik = 0;




// funkcja znajdująca pierwsze (najmniejsze) minimum dla danego miasta i
int min_pierwsze(vector<vector<int>> &macierz, int i)
{
    int min = INT_MAX;
    for (int k=0; k<liczba_miast; k++)
        if (macierz[i][k]<min && i != k)
            min = macierz[i][k];
    return min;
}


int min_drugie(vector<vector<int>> &macierz, int i){  // funkcja znajdująca drugie (drugie najmniejsze) minimum dla danego miasta i

    int first = INT_MAX;
    int drugie = INT_MAX;
    for (int j=0; j<liczba_miast; j++){
        if (i == j){
            continue;
        }

        if (macierz[i][j] <= first){
            drugie = first;
            first = macierz[i][j];
        }

        else if (macierz[i][j] <= drugie and macierz[i][j] != first){
            drugie = macierz[i][j];
        }

    }
    return drugie;
}


void Szukaj(vector<vector<int>> &macierz, int ograniczenie, int dlugosc_sciezek, int warstwa_drzewa, vector<int> obecna_sciezka){


    if (warstwa_drzewa == liczba_miast){ // jeżeli dotarłem do końcca o obliczam końcową scieżkę i sprawdzam czy lepsza od dotychczasowego najlepszego rozwiązania

        int obecna_dlugosc = dlugosc_sciezek + macierz[obecna_sciezka[warstwa_drzewa - 1]][obecna_sciezka[0]]; // obliczam całkowitą długość ścieżki

        if (obecna_dlugosc < najlepsza_dlugosc){ // aktualizuje najlepszą ścieżkę jeśli długość nowej jest mniejsza

            for (int i=0; i<obecna_sciezka.size()-1; i++){
                najlepsza_sciezka[i] = obecna_sciezka[i];
            }
            najlepsza_sciezka[liczba_miast] = obecna_sciezka[0];
            najlepsza_dlugosc = obecna_dlugosc;

            /*
            // wypisanie nowego najlepszego rozwiązania
            licznik++;
            cout<<"nowa naj: "<<counter<<endl;
            for (int i : najlepsza_sciezka)
                cout<<i<<" ";
            cout<<endl;
            cout<<najlepsza_dlugosc<<endl;
            */
        }
        return;
    }

    // na kolejnych poziomach drzewa przeszukuje dalej po kolei rekurencyjnie
    for (int i=0; i<liczba_miast; i++){
        // przeszukuje kolejne wierzchołki (tylko te co nie są odwiedzone)
        if (macierz[obecna_sciezka[warstwa_drzewa - 1]][i] != 0 && !odwiedzone[i]){
            int tymczasowe_ograniczenie = ograniczenie;
            dlugosc_sciezek += macierz[obecna_sciezka[warstwa_drzewa - 1]][i];

            // jeżeli poziom 1 drzewa to obliczam ograniczenie z dwóch pierwszych minimów miasta startowego i następnego miasta
            if (warstwa_drzewa == 1){
                ograniczenie -= ((min_pierwsze(macierz, obecna_sciezka[warstwa_drzewa - 1]) + min_pierwsze(macierz, i)) / 2);
            }
            else{  // jeżeli poziom 2 drzewa to obliczam ograniczenie z pierwszego minimum miasta następnego i drugie minimum miasta obecnego
                ograniczenie -= ((min_drugie(macierz, obecna_sciezka[warstwa_drzewa - 1]) + min_pierwsze(macierz, i)) / 2);
            }

            int dolne_ograniczenie = ograniczenie + dlugosc_sciezek;  // suma obecnej długość ścieżki (niepełnej) oraz ograniczenie to jest faktyczne dolne ograniczenie dla danego miasta w drzewie

            if (dolne_ograniczenie < najlepsza_dlugosc){   // sprawdzam czy dolne ograniczenie nie przekracza dotychczasowego najlepszego rozwiązania

                obecna_sciezka[warstwa_drzewa] = i;
                odwiedzone[i] = true;

                // rekurencyjnie przeszukuje kolejny poziom drzewa permutacji miast
                Szukaj(macierz, ograniczenie, dlugosc_sciezek, warstwa_drzewa + 1, obecna_sciezka);
            }

            dlugosc_sciezek -= macierz[obecna_sciezka[warstwa_drzewa - 1]][i]; // jeżeli przekracza to cofam się w drzewie aby odciąć gałąź drzewa bo się nie opłaci jej badać
            ograniczenie = tymczasowe_ograniczenie;


            for (int l = 0; l < odwiedzone.size(); l++) {  // resetuje wektor przechwujący odwiedzone miasta
                odwiedzone[l] = false;
            }


            for (int j=0; j <= warstwa_drzewa - 1; j++){  // ustawiam odwiedzone miasta ponownie po resecie
                odwiedzone[obecna_sciezka[j]] = true;
            }

        }
    }
}

void Branch_and_Bound_start(vector<vector<int>> &macierz){
    int ograniczenie = 0;

    vector<int> obecna_sciezka(liczba_miast + 1, -1); // przygotowuje wektor na sciezke (-1 znaczy brak)


    for (int l = 0; l < obecna_sciezka.size() - 1; l++) {  //wstawiam false do sciezki bo nic nie jest jescze odwiedzone
        odwiedzone.push_back(false);
    }


    najlepsza_sciezka.resize(liczba_miast + 1); // dostosowuje rozmiar wektora do liczby miast (+1 bo jeszcze sciezka powrotna do startu)

    // obliczam startowe (na mieście startowym 0 ) ograniczenie ( (pierwsze minimum + drugie minmum) dzielone przez 2) z każdego miasta
    for (int i=0; i<liczba_miast; i++)
        ograniczenie += (min_pierwsze(macierz, i) + min_drugie(macierz, i));


    if (ograniczenie % 2 == 1) {  // poprawiam ograniczenie by dzielło się przez 2
        ograniczenie = (ograniczenie / 2) + 1;
    } else {
        ograniczenie = ograniczenie / 2;
    }

    odwiedzone[0] = true;   // miasto startowe jest odwiedzone na samym początku więc ustawiam na true
    obecna_sciezka[0] = 0;

    // szukam ścieżek
    Szukaj(macierz, ograniczenie, 0, 1, obecna_sciezka);
}


vector<int> pobierz_sciezke(){
    return najlepsza_sciezka;
}

int pobierz_dlugosc_sciezki(){
    return najlepsza_dlugosc;
}

// todo jako pierwsze rozwiązanie dodać 0123456...


