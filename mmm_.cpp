
#include <iostream>
#include <fstream>
#include <cmath>
#include <conio.h>
// deklaracje zasięgów
using namespace std;
// stałe w programie
#define h 0.001 // krok obliczeń
#define PI 3.14159265 // liczba PI
#define T 10.0		// czas trwania sygnałów

// zmienne globalne w programie
double M, L1; // amplituda sygnału, czas trwania i liczba okresów
double u[(int)(1.0 * T / h) + 1]; // sygnał wejściowy
double u1p[(int)(1.0 * T / h) + 1]; // pierwsza pochodna sygnału wejściowego
double y_theta[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy theta
double y1p_theta[(int)(1.0 * T / h) + 1]; // pierwsza pochodna sygnału wyjściowego theta
double y2p_theta[(int)(1.0 * T / h) + 1]; // druga pochodna sygnału wyjściowego theta
double y_i[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy i
double y1p_i[(int)(1.0 * T / h) + 1]; // pierwsza pochodna sygnału wyjściowego i
double y2p_i[(int)(1.0 * T / h) + 1]; // druga pochodna sygnału wyjściowego i

// program główny
int main()
{
	double R, L, J, b, Kt, Ke;
	int pobudzenie;
	//wprowadzenie zmiennych ukladu

	cout << "Podaj zmienne ukladu:\n";
	cout << "R: "; cin >> R;//10
	cout << "\nL: "; cin >> L;//5
	cout << "\nJ: "; cin >> J;//2
	cout << "\nb: "; cin >> b;//3
	cout << "\nKt: "; cin >> Kt;//2
	cout << "\nKe: "; cin >> Ke;//2

		do
	{
		cout << "\nPobudzenie: \n1) Prostokatny okresowy\n2) Sinusoida \n3) Prostokatny skonczony \n4) Trojatny skonczony \n5) Trojkatny okresowy\n"; cin >> pobudzenie;
	} while (pobudzenie < 0 || pobudzenie > 5);

	int i, j, total;
	double a1, a0, b0_theta, w, b0_i, b1_i, t1, t2;
	a1 = (b / J + R / L);//1,5+2=3,5
	a0 = ((Kt * Ke + R * b) / (L * J));//3,4
	b0_theta = (Kt / (L * J));
	b0_i = (b / (L * J));//0.3
	b1_i = (1 / L);//0.2
	total = sizeof(u) / sizeof(u[0]); // rozmiar wektorów danych
	w = 2.0 * PI * L1 / T; // częstotliwość sinusoidy
	cout <<"\nPodaj amplitude sygnalu: "; cin >> M;
	switch (pobudzenie) {// sygnały wejściowe
	case 1://Prostokątny okresowy
		cout << "\nPodaj liczbę okresów: "; cin >> L1;
		for (i = 0; i < total; i++) 
		{
			if ((i * h) - floor(i * h / (T / L1)) * T / L1 < (T / L1) / 2) // sprawdza który to okres zaokrąglając w dół mnoży razy okres i sprawdza czy po odjęciu tego od obecnego czasu czy mieści się w obszarze wartości pozytywnych 
			{
				u[i] = M;
			}
			else
			{
				u[i] = -M;
			}
			if (i == 0) {
				u1p[i] = 0;
			}
			else {
				u1p[i] = (u[i] - u[i - 1]) / h; // pochodna impulsu prostokątnego - 2 delty diraca na plus i minus (dla jednego impulsu)
			}

		}
		break;
	case 2://Sinusoida
		cout << "\nPodaj liczbę okresów: "; cin >> L1;
		for (i = 0; i < total; i++) // sygnał wejściowy i jego pochodne
		{
			u[i] = M * sin(w * i * h); // sygnał wejściowy: u=M*sin(w*t) , t=i*
			u1p[i] = M * w * cos(w * i * h); // pochodna 1: u’(t)

		}
		break;
	case 3://1 Impuls prostokątny
	
		t1 = 2;
		t2 = 1;
		while (t1 > t2)
		{
			cout << "Podaj zakresy impulsu od mniejszego do wiekszego: \n";
			cin >> t1;
			cout << "\n";
			cin >> t2;
			if (t1 > t2)
				cout << "Nieprawidlowe dane podaj jeszcze raz \n";
		}
		for (i = 0; i < total; i++)
		{
			if ((i * h) > t1 && (i*h) < t2)  
			{
				u[i] = M;
			}
			else
			{
				u[i] = 0;
			}
			if (i == 0) {
				u1p[i] = 0;
			}
			else {
				u1p[i] = (u[i] - u[i - 1]); 
			}

		}
		break;
	case 4://1 Impuls trójkątny
	
		t1 = 2;
		t2 = 1;
		while (t1 > t2)
		{
			cout << "Podaj zakresy impulsu od mniejszego do wiekszego: \n";
			cin >> t1;
			cout << "\n";
			cin >> t2;
			if (t1 > t2)
				cout << "Nieprawidlowe dane.\n";
		}
		for (i = 0; i < total; i++) 
		{
			if ((i * h) <= (t1+t2)/2 && (i * h) >= t1) 
			{
				u[i] = (((i) * h)-t1)*M*2/(-t1+t2);
			}
			else if ((i * h) > (t1 + t2) / 2 && (i * h) <= t2)
			{
				u[i] = (M - (i * h - (t1 + t2) / 2)*M * 2 / (-t1 + t2));
			}
			else
			{
				u[i] = 0;
			}
			if (i == 0) {
				u1p[i] = 0;
			}
			else {
				if ((i * h) > t1 && (i * h) < t2)
				{
					u1p[i] = M;
				}
				else
				{
					u1p[i] = 0;
				}
			}

		}
		break;
	case 5://Trójkątny okresowy
		cout << "\nPodaj liczbę okresów: "; cin >> L1;
		double period = T / L1; // period of the triangular wave
		double half_period = period / 2; // half period of the triangular wave
		double slope = 2 * M / half_period; // slope of the triangular wave
		for (i = 0; i < total; i++)
		{
			double t = i * h;
			double time_in_period = t - floor(t / period) * period;
			if (time_in_period < half_period) {
				u[i] = slope * time_in_period - M; // increasing part
			}
			else {
				u[i] = -slope * (time_in_period - half_period) + M; // decreasing part
			}
			if (i == 0) {
				u1p[i] = 0;
			}
			else {
				if ((i * h) - floor(i * h / (T / L1)) * T / L1 < (T / L1) / 2) // sprawdza który to okres zaokrąglając w dół mnoży razy okres i sprawdza czy po odjęciu tego od obecnego czasu czy mieści się w obszarze wartości pozytywnych 
				{
					u1p[i] = M;
				}
				else
				{
					u1p[i] = -M;
				}
			}

		}
		break;

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
	//wykreslenie danych zapisanych do .txt
	system("gnuplot -p -e \"set title 'Wykres sygnalu wejsciowego'; set xlabel 'czas [t]'; set ylabel 'U(t)';plot 'daneU.txt' with lines\"");
	system("gnuplot -p -e \"set title 'Wykres pradu plynacego w obwodzie'; set xlabel 'czas [t]'; set ylabel 'prad plynacy w obwodzie i(t)';plot 'daneY_i.txt' with lines\"");
	system("gnuplot -p -e \"set title 'Wykres predkosci katowej walu silnika'; set xlabel 'czas [t]'; set ylabel 'predkosc katowa walu silnika w(t)';plot 'daneY_theta.txt' with lines\"");

	return(0);
}
