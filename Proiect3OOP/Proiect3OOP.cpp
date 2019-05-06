#include <iostream>
#include <fstream>
#include <conio.h>
#include "camere.h"
using namespace std;

void read() //meniul de citire. Foloesesc reading_list sa stiu daca citesc lista sau elementul 'k'
{
}




void menu(Hotel &hotel, vector<Cerere_Cazare> bookings)//meniul principal al programului
{
	antet
	cout << "Meniu: " << endl;
	cout << "1. Adauga o noua cerere" << endl;
	cout << "2. Afiseaza toate cererile" << endl;
	cout << "3. Anuleaza o cerere" << endl;
	cout << "9. Iesi din program" << endl;

	int caz;
	cin >> caz;

	switch (caz)
	{
	case 1:
	{
		Cerere_Cazare new_booking;
		new_booking.read();
		int i;
		new_booking.SetData(hotel.Cazare(new_booking));
		if (new_booking.GetData() >= 0)
		{
			cout << "Cerere inregistrata cu succes!\n";
			cout << "Detalii despre cerere:\n\n";
			cout << "Data inceperii rezervarii: " << new_booking.GetData()+1 << endl;
			for (i = 0; i < new_booking.GetCamere(); i++)
			{
				cout << "\nCamera #" << new_booking.GetCamera(i)+1 << endl;
				hotel.AfiseazaOcupantiCamera(new_booking.GetCamera(i), new_booking.GetData());
			}
			for (i = 0; i < new_booking.GetCamere(); i++)
			{
				cout << "\nApartamentul #" << new_booking.GetApartament(i)+1 << endl;
				hotel.AfiseazaOcupantiApartament(new_booking.GetApartament(i), new_booking.GetData());
			}
			for (i = 0; i < new_booking.GetSali(); i++)
			{
				cout << "\nSala #" << new_booking.GetSala(i) + 1 << endl;
			}
			bookings.push_back(new_booking);
		}
		_getch();

		break;
	}
	case 2:
	{
		int i,j;
		for (j = 0; j < bookings.size(); j++)
		{
			antet
			cout << "\nCererea #" << j + 1 <<"/"<<bookings.size();

			cout << "\nData inceperii rezervarii: " << bookings[j].GetData() + 1 << endl;
			for (i = 0; i < bookings[j].GetCamere(); i++)
			{
				cout << "\nCamera #" << bookings[j].GetCamera(i) + 1 << endl;
				hotel.AfiseazaOcupantiCamera(bookings[j].GetCamera(i), bookings[j].GetData());
			}
			for (i = 0; i < bookings[j].GetCamere(); i++)
			{
				cout << "\nApartamentul #" << bookings[j].GetApartament(i) + 1 << endl;
				hotel.AfiseazaOcupantiApartament(bookings[j].GetApartament(i), bookings[j].GetData());
			}
			for (i = 0; i < bookings[j].GetSali(); i++)
			{
				cout << "\nSala #" << bookings[j].GetSala(i) + 1 << endl;
			}
			cout << "\nApasati orice tasta pentru a afisa urmatoarea rezervare.";
			_getch();
		}
		break;
	}
	case 3:
	{
		cout << "Ce cerere doriti sa anulati?\n";
		int cerere;
		cin >> cerere;
		cerere--;
		vector<Cerere_Cazare> temp;
		hotel.AnulareCerere(bookings[cerere]);
		cout << "DA";
		for (int k = 0; k < bookings.size(); k++)
			if (k != cerere)
				temp.push_back(bookings[k]);
		bookings.swap(temp);
		break;
	}
	case 9:
		exit(0);
	}
	menu(hotel,bookings); //revin la meniul principal
}


int main()
{
	Hotel hotel;
	hotel.Read();
	vector<Cerere_Cazare> booking;
	menu(hotel, booking);

	return 0;
}