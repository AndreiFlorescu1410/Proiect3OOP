#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <vector>

using namespace std;

#define capacitate_sala_scena 100
#define antet {system("CLS");cout<<"       Proiect 3 OOP -- Gestiunea unui Hotel -- Florescu Andrei Gr. 212         "<<endl<<endl;}

class camera
{
	int capacitate;
	vector<char*> nume_ocupant[365];
	int data_rezervarii[365] = { 0 };
public:
	bool verifica_disponibilitatea(int, int);
	void ocupa(int, int);
	void elibereaza(int, int);
	void SetNumeOcupanti(char*,int);
	void AfiseazaOcupanti(int);

	void SetCapacitate(int n) { capacitate = n; }
	void SetCapacitateSali(int n) 
	{
		capacitate = n;
		for (int i = 0; i < 365; i++)
			data_rezervarii[i] = n;
	}
	int GetLocuriDisponibile(int i) {
		return data_rezervarii[i]; }
	void SetLocuriDisponibile(int i, int nr) { data_rezervarii[i] = nr; }

	camera() : capacitate(2) {  };
	camera(int n) : capacitate(n) { };
	camera(int n, bool restaurant) :capacitate(n) { for (int i = 0; i < 365; i++) data_rezervarii[i] = n;}
	~camera()
	{
	}
};

class apartament : public camera
{
public:
	apartament() : camera(4) {};

};

class restaurant :  public camera
{
	int locuri_rezervate;
public:
	bool verifica_disponibilitatea(int, int, int);
	void ocupa(int, int, int);
	void elibereaza(int, int, int);
	restaurant() : locuri_rezervate(0)
	{
	}
	restaurant(int n) : camera(n,1), locuri_rezervate(0) 
	{
	};
};

class sala_de_conferinta : public restaurant
{
	bool proiector;
	int mese_individuale;
public:
	void SetProiector(int n) { proiector = n; }

	sala_de_conferinta() : proiector(0), mese_individuale(0), restaurant(0) {};

	sala_de_conferinta(bool p, int nr_mese) : proiector(p), mese_individuale(nr_mese), restaurant((nr_mese * 8) == 0 ? capacitate_sala_scena : nr_mese * 8) {};
};


class Cerere_Cazare
{
	int zile, camere, apartamente, sali, nr_mic_dejun, data_start;
	vector<bool> mic_dejun;
	vector<int> camere_rezervate, apartamente_rezervate, sali_rezervate;
	vector<char*> persoane;
public:
	int GetZile() { return zile; }
	void SetZile(int n) { zile = n; }
	int GetCamere() { return camere; }
	void SetCamere(int n) { camere = n; }
	int GetApartamente() { return apartamente; }
	void SetApartamente(int n) { apartamente = n; }
	int GetSali() { return sali; }
	void SetSali(int n) { sali = n; }
	int GetMicDejun() { return nr_mic_dejun; }
	void SetData(int n) { data_start = n; }
	int GetData() { return data_start; }
	char* GetPersoana()
	{
		char* persoana = persoane.back();
		persoane.pop_back();
		return persoana;
	}


	void AdaugaCamera(int n) { camere_rezervate.push_back(n); }
	int GetCamera(int n) { return camere_rezervate[n]; }
	void AdaugaApartament(int n) { apartamente_rezervate.push_back(n); }
	int GetApartament(int n) { return apartamente_rezervate[n]; }
	void AdaugaSala(int n) { sali_rezervate.push_back(n); }
	int GetSala(int n) { return sali_rezervate[n]; }

	void read();
	void clear() { camere_rezervate.clear(); apartamente_rezervate.clear(); sali_rezervate.clear(); }
	Cerere_Cazare() : zile(0), camere(0), apartamente(0), nr_mic_dejun(0), sali(0), data_start(-1)
	{
		persoane.reserve(0);
	};
	Cerere_Cazare(int Zile, int Camere, int Apartamente) : zile(Zile), camere(Camere), apartamente(Apartamente), data_start(0), nr_mic_dejun(0), sali(0)
	{
	};
	~Cerere_Cazare()
	{
	}
};

class Hotel
{
	int nr_camere, nr_apartamente, nr_sali;
	camera* Camera;
	apartament* Apartament;
	sala_de_conferinta* Sala_de_conferinta;
	restaurant* Restaurant;
public:
	void Read();

	void AfiseazaOcupantiCamera(int i,int zi) { Camera[i].AfiseazaOcupanti(zi); }
	void AfiseazaOcupantiApartament(int i, int zi) { Apartament[i].AfiseazaOcupanti(zi); }

	void ocupaCamera(int i, int start, int zile) { Camera[i].ocupa(start, zile); }
	void ocupaApartament(int i, int start, int zile) { 
		Apartament[i].ocupa(start, zile); };
	void ocupaSala(int i, int start, int zile, int persoane) { 
		Sala_de_conferinta[i].ocupa(start, zile, persoane); }
	void ocupaRestaurant(int i, int start, int zile, int persoane) { Restaurant->ocupa(start, zile, persoane); }

	void elibereazaCamera(int i, int start, int zile) { Camera[i].elibereaza(start, zile); }
	void elibereazaApartament(int i, int start, int zile) { 
		Apartament[i].elibereaza(start, zile); };
	void elibereazaSala(int i, int start, int zile, int persoane) { Sala_de_conferinta[i].elibereaza(start, zile, persoane); }
	void elibereazaRestaurant(int i, int start, int zile, int persoane) { Restaurant->elibereaza(start, zile, persoane); }



	int Cazare(Cerere_Cazare&);
	void AnulareCerere(Cerere_Cazare&);
	Hotel() : nr_camere(0), nr_apartamente(0), nr_sali(0) 
	{
		Camera = new camera;
		Apartament = new apartament;
		Sala_de_conferinta = new sala_de_conferinta;
		Restaurant = new restaurant;
	};
	Hotel(int nr_c, int nr_a, int nr_s, int nr_r) : nr_camere(nr_c), nr_apartamente(nr_a), nr_sali(nr_s)
	{
		Camera = new camera[nr_c];
		Apartament = new apartament[nr_a];
		Sala_de_conferinta = new sala_de_conferinta[nr_s];
		Restaurant = new restaurant(nr_r);
	}
	~Hotel()
	{
		delete[] Camera;
		delete[] Apartament;
		delete[] Sala_de_conferinta;
		delete Restaurant;
	}
};