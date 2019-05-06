#include <fstream>
#include <conio.h>
#include "camere.h"


bool camera::verifica_disponibilitatea(int start, int nr_zile)
{
	for (int i = start; i < start + nr_zile; i++)
		if (data_rezervarii[i])
			return false;
	return true;
}
void camera::AfiseazaOcupanti(int zi)
{
	for (int i = 0; i < nume_ocupant[zi].size(); i++)
		cout << nume_ocupant[zi][i] << endl;
}
bool restaurant::verifica_disponibilitatea(int start, int nr_zile, int nr_persoane)
{
	for (int i = start; i < start + nr_zile; i++)
		if (GetLocuriDisponibile(i) - nr_persoane < 0)
			return false;
	return true;
}

void camera::ocupa(int start, int nr_zile)
{
	for (int i = start; i < start + nr_zile; i++)
		data_rezervarii[i] = 1;
}
void camera::elibereaza(int start, int nr_zile)
{
	for (int i = start; i < start + nr_zile; i++)
	{
		data_rezervarii[i] = 0;
		nume_ocupant[i].clear();
	}
}
void camera::SetNumeOcupanti(char nume[20],	int zi)
{
	nume_ocupant[zi].push_back(nume);
}

void restaurant::ocupa(int start, int nr_zile, int nr_persoane)
{
	for (int i = start; i < start + nr_zile; i++)
		SetLocuriDisponibile(i, GetLocuriDisponibile(i) - nr_persoane);
}

void restaurant::elibereaza(int start, int nr_zile, int nr_persoane)
{
	for (int i = start; i < start + nr_zile; i++)
		SetLocuriDisponibile(i, GetLocuriDisponibile(i) + nr_persoane);
}

int Hotel::Cazare(Cerere_Cazare &booking)
{
	int i,j,k,l, camere_rezervate,apartamente_rezervate, sali_rezervate;
	for (i = 0; i < 365; i++)
	{
		if (Restaurant->GetLocuriDisponibile(i) < booking.GetMicDejun())
			continue;
		camere_rezervate = 0;
		booking.clear();
		for (j = 0; j < nr_camere; j++)
		{
			if (camere_rezervate == booking.GetCamere())
			{
				break;
			}
			if (Camera[j].verifica_disponibilitatea(i, booking.GetZile()))
			{
				booking.AdaugaCamera(j);
				camere_rezervate++;
			}
		}
		if (camere_rezervate != booking.GetCamere())
			continue;
		apartamente_rezervate = 0;
		for (j = 0; j < nr_apartamente; j++)
		{
			if (apartamente_rezervate == booking.GetApartamente())
			{
				break;
			}
			if (Apartament[j].verifica_disponibilitatea(i, booking.GetZile()))
			{
				booking.AdaugaApartament(j);
				apartamente_rezervate++;
			}
		}
		if (apartamente_rezervate != booking.GetApartamente())
			continue;
		sali_rezervate = 0;
		for(j=0;j<nr_sali;j++)
		{
			if (sali_rezervate == booking.GetSali())
			{
				break;
			}
			if (Sala_de_conferinta[j].verifica_disponibilitatea(i, booking.GetZile(), booking.GetCamere()*2+booking.GetApartamente()))
			{
				booking.AdaugaSala(j);
				sali_rezervate++;
			}
		}

		if (sali_rezervate == booking.GetSali())
		{
			int k1;
			char* persoana=new char[20];
			for (j = 0; j < booking.GetCamere(); j++)
			{
				ocupaCamera(booking.GetCamera(j), i, booking.GetZile());
				for (k = 0; k < 2; k++)
				{
					persoana =  booking.GetPersoana();
					for (k1 = i; k1 < booking.GetZile() + i; k1++)
						Camera[booking.GetCamera(j)].SetNumeOcupanti(persoana, k1);
				}
			}
			for (j = 0; j < booking.GetApartamente(); j++)
			{
				ocupaApartament(booking.GetApartament(j), i, booking.GetZile());
				for (k = 0; k < 4; k++)
				{
					persoana = booking.GetPersoana();
					for (k1 = i; k1 < booking.GetZile() + i; k1++)
						Apartament[booking.GetApartament(j)].SetNumeOcupanti(persoana,k1);
				}
			}
			for (j = 0; j < booking.GetSali(); j++)
				ocupaSala(j, i, booking.GetZile(), booking.GetCamere() * 2 + booking.GetApartamente()*4);
			ocupaRestaurant(0, i, booking.GetZile(), booking.GetCamere() * 2 + booking.GetApartamente()*4);
			return i;
		}
				
	}
	return -1;
}

void Hotel::AnulareCerere(Cerere_Cazare& booking)
{
	int j;
	for (j = 0; j < booking.GetCamere(); j++)
	{
		elibereazaCamera(booking.GetCamera(j), booking.GetData(), booking.GetZile());
	}
	for (j = 0; j < booking.GetApartamente(); j++)
	{
		elibereazaApartament(booking.GetApartament(j), booking.GetData(), booking.GetZile());
	}
	for (j = 0; j < booking.GetSali(); j++)
		elibereazaSala(booking.GetApartament(j), booking.GetData(), booking.GetZile(), booking.GetCamere() * 2 + booking.GetApartamente());
	elibereazaRestaurant(0, booking.GetData(), booking.GetZile(), booking.GetCamere() * 2 + booking.GetApartamente());
}

void Cerere_Cazare::read()
{
	antet
	cout << "Alegeti metoda de citire cererii:\n";
	cout << "1. Fisier" << endl;
	cout << "2. Tastatura" << endl;
	int caz, i;
	cin >> caz;
	switch (caz)
	{
	case 1:  //fisier
	{
		antet
			cout << "Dati numele fisierului: ";
		char txt_name[100];
		cin >> txt_name;          //citesc numele fisierului
		ifstream f(txt_name);
		if (f.is_open() == 0)    //testez daca exista fisierul citit
		{
			antet
				cout << "Nu am putut deschide fisierul." << endl;
			cout << "Apasa orice tasta pentru a te intoarce..." << endl;
			_getch();
		}
		else                    //am putut sa deschid fisierul
		{
			int  nr, capacitate_hotel;
			bool micdejun;
			char* persoana=new  char[20];
			f >> zile >> camere >> apartamente;
			int marime = camere * 2 + apartamente * 4 + 1;
			for (i = 0; i < camere * 2 + apartamente * 4; i++)
			{
				f.get();
				f.getline(persoana, 20);
				persoane.push_back(persoana);
				f >> micdejun;
				mic_dejun.push_back(micdejun);
				if (mic_dejun[i])
					nr_mic_dejun++;
				persoana = new char[20];
			}
			f >> sali;
			delete[] persoana;
		}
		break;
	}
	case 2: //tastatura
	{
		int  nr, capacitate_hotel;
		bool micdejun;
		char* persoana = new  char[20];
		antet
		cout << "Cate zile doriti sa rezervati: "; cin >> zile;
		cout << "Cate camere doriti sa rezervati: "; cin >> camere;
		cout << "Cate apartamente doriti sa rezervati: "; cin >> apartamente;
		int marime = camere * 2 + apartamente * 4 + 1;
		for (i = 0; i < marime; i++)
		{
			cout << "Dati numele persoanei " << i+1 << "/" << marime;
			cin.get();
			cin.get(persoana,20);
			persoane.push_back(persoana);
			cout << "Doreste micul de jun in camera(0) sau in restaurant(1): "; cin >> micdejun;
			mic_dejun.push_back(micdejun);
			if (mic_dejun[i])
				nr_mic_dejun++;
			persoana = new char[20];
		}
		cout << "Cate sali doriti sa rezervati: "; cin >> sali;
		delete[] persoana;


		break;
	}
	}
}

void Hotel::Read()
{

	antet
		cout << "Alegeti metoda de citire configuratiei Hotelului:\n";
	cout << "1. Fisier" << endl;
	cout << "2. Tastatura" << endl;
	int caz, i;
	cin >> caz;
	switch (caz)
	{
	case 1:  //fisier
	{
		antet
			cout << "Dati numele fisierului: ";
		char txt_name[100];
		cin >> txt_name;          //citesc numele fisierului
		ifstream f(txt_name);
		if (f.is_open() == 0)    //testez daca exista fisierul citit
		{
			antet
				cout << "Nu am putut deschide fisierul." << endl;
			cout << "Apasa orice tasta pentru a te intoarce..." << endl;
			_getch();
		}
		else                    //am putut sa deschid fisierul
		{
			int  nr, capacitate_hotel;

			f >> nr_camere;			//numarul de camere
			capacitate_hotel = nr_camere * 2;
			Camera = new camera[nr_camere];

			f >> nr_apartamente;			//numarul de apartamente
			capacitate_hotel += nr_apartamente * 4;
			Apartament = new apartament[nr_apartamente];

			f >> nr;			//hotelul are restaurant? 0-NU, capacitate>0-DA
			if (nr < capacitate_hotel)
				Restaurant = new restaurant(nr);
			else
			{
				//TODO: Handle the case when input is wrong.
			}


			f >> nr_sali;			//numarul de sali de conferinte
			if (nr_sali)
			{
				Sala_de_conferinta = new sala_de_conferinta[nr_sali];
				bool proiector;
				int capacitate_sala;
				for (i = 0; i < nr_sali; i++)
				{
					f >> proiector;			//0-Nu are proiector, 1-Are proiector
					if (proiector)
						Sala_de_conferinta[i].SetProiector(proiector);
					f >> capacitate_sala;	//0-Tip scena, nr>0 capacitate
					Sala_de_conferinta[i].SetCapacitateSali((capacitate_sala == 0) ? capacitate_sala_scena : capacitate_sala);
				}
			}
		}
		break;
	}
	case 2: //tastatura
	{
		int  nr, capacitate_hotel;

		cout << "Cate camere are hotelul:"; cin >> nr_camere;			//numarul de camere
		capacitate_hotel = nr_camere * 2;
		Camera = new camera[nr_camere];

		cout << "Cate apartamente are hotelul:"; cin >> nr_apartamente;			//numarul de apartamente
		capacitate_hotel += nr_apartamente * 4;
		Apartament = new apartament[nr_apartamente];

		cout << "Hotelul are restaurant?(0-Nu, capacitate>0-DA):"; cin >> nr;			//hotelul are restaurant? 0-NU, capacitate>0-DA
		if (nr < capacitate_hotel)
			Restaurant = new restaurant(nr);
		else
		{
			//TODO: Handle the case when input is wrong.
		}


		cout << "Cate sali de conferinta are hotelul"; cin >> nr_sali;			//numarul de sali de conferinte
		if (nr_sali)
		{
			Sala_de_conferinta = new sala_de_conferinta[nr_sali];
			bool proiector;
			int capacitate_sala;
			for (i = 0; i < nr_sali; i++)
			{
				cout << "Are sala " << i << " proiector?(0-NU,1-DA):";
				cin >> proiector;			//0-Nu are proiector, 1-Are proiector
				if (proiector)
					Sala_de_conferinta[i].SetProiector(proiector);
				cout << "Ce capacitate are sala " << i <<" ";
				cin >> capacitate_sala;	//0-Tip scena, nr>0 capacitate
				Sala_de_conferinta[i].SetCapacitateSali((capacitate_sala == 0) ? capacitate_sala_scena : capacitate_sala);
			}
		}
		break;
	}
	}
}
