﻿// załączenie bibliotek
#include <iostream>
#include <fstream>
#include <cmath>
#include <conio.h>
// deklaracje zasięgów
using namespace std;
// stałe w programie
#define h 0.001 // krok obliczeń
#define T 10.0 // całkowity czas symulacji – przedział [0 , T]
#define L1 2.5 // liczba okresów sygnału sinus w przedziale T
#define M 8.0 // amplituda sygnału sinus
#define PI 3.14159265 // liczba PI
// pomocniczy typ – kolejne bajty danej ’double’
typedef union { char c[sizeof(double)]; double d; } Box;
// zmienne globalne w programie
double u[(int)(1.0 * T / h) + 1]; // sygnał wejściowy
double u1p[(int)(1.0 * T / h) + 1]; // pierwsza pochodna sygnału wejściowego
double y_theta[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy theta
double y1p_theta[(int)(1.0 * T / h) + 1]; // pierwsza pochodna sygnału wyjściowego theta
double y2p_theta[(int)(1.0 * T / h) + 1]; // druga pochodna sygnału wyjściowego theta
double y_i[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy i
double y1p_i[(int)(1.0 * T / h) + 1]; // pierwsza pochodna sygnału wyjściowego i
double y2p_i[(int)(1.0 * T / h) + 1]; // druga pochodna sygnału wyjściowego i
Box z[(int)(1.0 * T / h) + 1]; // zmienna: pojedyncza wartość sygnału (u lub y)
// program główny
int main()
{
	double R, L, J, b, Kt, Ke;
	//wprowadzenie zmiennych ukladu
	cout << "Podaj zmienne ukladu:\n";
	cout << "R: "; cin >> R;
	cout << "\nL: "; cin >> L;
	cout << "\nJ: "; cin >> J;
	cout << "\nb: "; cin >> b;
	cout << "\nKt: "; cin >> Kt;
	cout << "\nKe: "; cin >> Ke;

	int i, j, total;
	double a1, a0, b0_theta, w, b0_i, b1_i;
	a1 = (b / J + R / L);
	a0 = ((Kt * Ke + R * b) / (L * J));
	b0_theta = (Kt / (L * J));
	b0_i = (b / (L * J));
	b1_i = (1 / L);

	total = sizeof(u) / sizeof(u[0]); // rozmiar wektorów danych
	w = 2.0 * PI * L1 / T; // częstotliwość sinusoidy
	for (i = 0; i < total; i++) // sygnał wejściowy i jego pochodne
	{
		u[i] = M * sin(w * i * h); // sygnał wejściowy: u=M*sin(w*t) , t=i*
		u1p[i] = M * w * cos(w * i * h); // pochodna 1: u’(t)

	}
	//warunki poczatkowe - zerowe
	y_theta[0] = 0; y1p_theta[0] = 0; y2p_theta[0] = 0;

	for (i = 0; i < total - 1; i++) // główna pętla obliczeń dla y_theta
	{
		y2p_theta[i] = -a1 * y1p_theta[i] - a0 * y_theta[i] + b0_theta * u[i];
		y1p_theta[i + 1] = y1p_theta[i] + h * y2p_theta[i];
		y_theta[i + 1] = y_theta[i] + h * y1p_theta[i] + (h * h / 2.0) * y2p_theta[i];
	}
	for (i = 0; i < total - 1; i++) // główna pętla obliczeń dla y_i
	{
		y2p_i[i] = -a1 * y1p_i[i] - a0 * y_i[i] + b1_i * u1p[i] + b0_i * u[i];
		y1p_i[i + 1] = y1p_i[i] + h * y2p_i[i];
		y_i[i + 1] = y_i[i] + h * y1p_i[i] + (h * h / 2.0) * y2p_i[i];
	}
	fstream daneU;
	daneU.open("daneU.txt", ios::out | ios::trunc);
	if (daneU.good() == true)
	{
		for (int i = 0; i < total; i++) {
			daneU << i * h << " " << u[i] << endl; // Zapis pary (czas, wartość_sygnalu) do pliku
		}
		daneU.close();
	}
	fstream daneY_theta;
	daneY_theta.open("daneY_theta.txt", ios::out | ios::trunc);
	if (daneY_theta.good() == true)
	{
		for (int i = 0; i < total; i++) {
			daneY_theta << i * h << " " << y_theta[i] << endl; // Zapis pary (czas, wartość_sygnalu) do pliku
		}
		daneY_theta.close();
	}
	fstream daneY_i;
	daneY_i.open("daneY_i.txt", ios::out | ios::trunc);
	if (daneY_i.good() == true)
	{
		for (int i = 0; i < total; i++) {
			daneY_i << i * h << " " << y_i[i] << endl; // Zapis pary (czas, wartość_sygnalu) do pliku
		}
		daneY_i.close();
	}
	system("gnuplot -p -e \"plot 'daneU.txt' with lines\"");
	system("gnuplot -p -e \"plot 'daneY_i.txt' with lines\"");
	system("gnuplot -p -e \"plot 'daneY_theta.txt' with lines\"");

	return(0);
}