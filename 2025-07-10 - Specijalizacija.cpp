#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <mutex>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;
const char* PORUKA = "\n----------------------------------------------"
"-------------------------------- - \n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA"
"DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG"
"ZADATKA\n"
"4. ATRIBUTI, NAZIVI METODA (SVE ISTO VAZI I ZA FUNKCIJE), TE BROJ I"
"TIP PARAMETARA MORAJU BITI IDENTICNI "
"ONIMA KOJI SU KORISTENI U TESTNOM CODE - U, OSIM U SLUCAJU DA POSTOJI"
"ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. "
"OSTALE POMOCNE METODE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U METODAMA U KOJIMA JE TO NAZNACENO.\n"
"6. SVE METODE POZVANE U MAIN-U ZADATKA TREBAJU POSTOJATI. UKOLIKO"
"NISTE ZADOVOLJNI IMPLEMENTACIJOM "
"POTREBNO JE DA IMPLEMENTIRATE BAREM TIJELO TIH METODA (METODA MOZE"
"BITI PRAZNA), "
"A AKO METODA TREBA VRATITI NEKI PODATAK ONDA MOZETE VRATITI BILO KOJU"
"TJ.ZELJENU VRIJEDNOST ZAHTIJEVANOG TIPA.!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRAJTE U .DOCX FAJL (IMENOVAN"
"BROJEM INDEKSA npr.IB150051.docx)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA"
"VASIM RJESENJEM!\n"
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11.ZA POTREBE TESTIRANJA, U MAIN-U, BUDITE SLOBODNI DODATI TESTNIH"
"PODATAKA(POZIVA METODA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"12.ZA IZRADU ISPITNOG RJESENJA KORISTITI VISUAL STUDIO 2022 I"
"RJESENJE TESTIRAJTE U OBA MODA(F5 i Ctrl + F5)!\n"
"13.NA KRAJU ISPITA PROVJERITE DA LI STE RJEŠENJE KOPIRALI U ADEKVATAN"
"FOLDER NA FTP SERVERU\n"
"---------------------------------------------------------------------"
"----------\n";
enum Specializacija {
	KARDIOLOGIJA, ORTOPEDIJA, DERMATOLOGIJA,
	PEDIJATRIJA, OPSTA_MEDICINA
};
const char* SpecializacijaNazivi[] = {
 "KARDIOLOGIJA", "ORTOPEDIJA", "DERMATOLOGIJA", "PEDIJATRIJA",
"OPSTA MEDICINA"
};

const char* crt = "\n-------------------------------------------\n";

char* GenerisiSifru(const char* ime, int id)
{
	string sifra;
	time_t trenutnoVrijeme = time(nullptr);
	tm timeinfo{};
	localtime_s(&timeinfo, &trenutnoVrijeme);
	int godina = (timeinfo.tm_year + 1900) % 100;
	sifra += to_string(godina);
	sifra += "-";
	sifra += toupper(ime[0]);
	const char* pozicijaDrugogInicijala = strchr(ime, ' ');
	sifra += toupper(pozicijaDrugogInicijala[1]);
	sifra += "-";
	if (id > 99)
	{
		sifra += to_string(id);
	}
	else if (id > 9)
	{
		sifra += to_string(0);
		sifra += to_string(id);
	}
	else
	{
		sifra += to_string(0);
		sifra += to_string(0);
		sifra += to_string(id);
	}
	char* temp = new char[sifra.size() + 1];
	strcpy_s(temp, sifra.size() + 1, sifra.c_str());
	return temp;
}

bool ValidirajSifru(string sifra)
{
	return regex_match(sifra, regex("\\d{2}(-| )[A-Z]{2}(-| )\\d{3}"));
}


char* AlocirajNiz(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

bool UcitajUcesnike(const char* filename, vector<Tim>& timovi) {
	ifstream file(filename);
	if (!file.is_open())
		return false;

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		stringstream ss(line);
		string imePrezime, nazivTima;

		if (!getline(ss, imePrezime, '|')) return false;
		if (!getline(ss, nazivTima)) return false;

		imePrezime.erase(0, imePrezime.find_first_not_of(" \t\n\r"));
		imePrezime.erase(imePrezime.find_last_not_of(" \t\n\r") + 1);
		nazivTima.erase(0, nazivTima.find_first_not_of(" \t\n\r"));
		nazivTima.erase(nazivTima.find_last_not_of(" \t\n\r") + 1);

		auto it = find_if(timovi.begin(), timovi.end(), [&](const Tim& t) {
			return strcmp(t.getNaziv(), nazivTima.c_str()) == 0;
			});

		if (it == timovi.end()) {
			timovi.emplace_back(nazivTima.c_str());
			it = timovi.end() - 1;
		}

		bool exists = false;
		for (auto& u : it->getUcesnici()) {
			if (strcmp(u.getImePrezime(), imePrezime.c_str()) == 0) {
				exists = true;
				break;
			}
		}

		if (!exists) {
			Ucesnik novi(imePrezime.c_str());
			it->DodajUcesnika(novi);
		}
	}

	file.close();
	return true;
}


template<class T1, class T2, int max>
class KolekcijaG1 {
	T1* _elementi1[max] = {nullptr};
	T2* _elementi2[max] = {nullptr};
	int _trenutno;
public:

	KolekcijaG1() {
		_trenutno = 0;
	}

	KolekcijaG1(const KolekcijaG1& obj)
	{
		_trenutno = obj.getTrenutno();
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}

	KolekcijaG1& operator=(const KolekcijaG1& obj)
	{
		if (this != &obj)
		{
			for (size_t i = 0; i < _trenutno; i++)
			{
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}

			_trenutno = obj.getTrenutno();
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}
		return *this;
	}

	~KolekcijaG1()
	{
		for (size_t i = 0; i < _trenutno; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	void AddElement(T1 el1, T2 el2)
	{
		if (_trenutno >= max) throw exception("Maksimalni broj elemenata dostignut. ");
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno] = new T2(el2);
		_trenutno++;
	}

	KolekcijaG1<T1, T2, max> InsertAt(int lokacija, T1 el1, T2 el2)
	{
		for (size_t i = _trenutno; i > lokacija; i--)
		{
			_elementi1[i] = new T1(*_elementi1[i - 1]);
			_elementi2[i] = new T2(*_elementi2[i - 1]);
		}

		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);

		_trenutno++;
		return *this;
	}

	KolekcijaG1<T1, T2, max>* RemoveRange(int start, int end)
	{
		if (start < 0 || end < start || end >= _trenutno)
			throw exception("Nepostojeci elementi.");

		int brojMjesta = end - start + 1;
		KolekcijaG1<T1, T2, max>* nova = new KolekcijaG1<T1, T2, max>();

		for (int i = start; i <= end; i++)
			nova->AddElement(*_elementi1[i], *_elementi2[i]);

		for (int i = start; i <= end; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		for (int i = end + 1; i < _trenutno; i++) {
			_elementi1[i - brojMjesta] = _elementi1[i];
			_elementi2[i - brojMjesta] = _elementi2[i];
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}

		_trenutno -= brojMjesta;

		return nova;
	}



	T1& getElement1(int lokacija) const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija) const { return *_elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }
	friend ostream& operator<< (ostream& COUT, KolekcijaG1& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<
			endl;
		return COUT;
	}
};
class Termin {
	int* _sati; int* _minute; int* _sekunde;
public:
	Termin(int s = 0, int m = 0, int k = 0) {
		_sati = new int(s);
		_minute = new int(m);
		_sekunde = new int(k);
	}

	Termin(const Termin& obj)
	{
		_sati = new int(obj.getSati());
		_minute = new int(obj.getMinute());
		_sekunde = new int(obj.getSekunde());
	}

	Termin& operator=(const Termin& obj)
	{
		if (this != &obj)
		{
			delete _sati; delete _minute; delete _sekunde;
			_sati = new int(obj.getSati());
			_minute = new int(obj.getMinute());
			_sekunde = new int(obj.getSekunde());
		}
		return *this;
	}

	~Termin() {
		delete _sati; delete _minute; delete _sekunde;
	}

	bool operator==(const Termin& obj) const
	{
		return getSati() == obj.getSati() && getMinute() == obj.getMinute() && getSekunde() == obj.getSekunde();
	}

	int getSati() const { return *_sati; }
	int getMinute() const { return *_minute; }
	int getSekunde() const { return *_sekunde; }
	friend ostream& operator<<(ostream& COUT, const Termin& t) {
		COUT << *t._sati << ":" << *t._minute << ":" << *t._sekunde;
		return COUT;
	}
};
class Dogadjaj {
protected:
	Termin _termin;
public:
	Dogadjaj(Termin termin) : _termin(termin) {}

	Dogadjaj(const Dogadjaj& obj)
	{
		_termin = obj.getTermin();
	}

	Dogadjaj& operator=(const Dogadjaj& obj)
	{
		if (this != &obj)
		{
			_termin = obj.getTermin();
		}
		return *this;
	}

	virtual Dogadjaj* GetParentObject() const = 0;

	virtual ~Dogadjaj() {}
	virtual string Info() const = 0;
	const Termin& getTermin() const { return _termin; }
};

class Predavanje : public Dogadjaj{
	char* _tema;
	Specializacija _specijalizacija;
public:

	Predavanje(Termin termin, const char* tema, Specializacija specijalizacija): Dogadjaj(termin)
	{
		_tema = AlocirajNiz(tema);
		_specijalizacija = specijalizacija;
	}

	Predavanje(const Predavanje& obj) : Dogadjaj(obj)
	{
		_tema = AlocirajNiz(obj.GetTema());
		_specijalizacija = obj.GetSpecijalizacija();
	}

	Predavanje& operator=(const Predavanje& obj)
	{
		if (this != &obj)
		{
			delete[] _tema;
			Dogadjaj::operator=(obj);
			_tema = AlocirajNiz(obj.GetTema());
			_specijalizacija = obj.GetSpecijalizacija();
		}
		return *this;
	}
	//info metoda vraca sve detalje o dogadjaju u string formatu npr.: 19:02 : 30 Oboljenja srca KARDIOLOGIJA
	string Info() const 
	{
		return to_string(_termin.getSati()) + ":" + to_string(_termin.getMinute()) + ":" + to_string(_termin.getSekunde()) + " - " + _tema + " " + SpecializacijaNazivi[GetSpecijalizacija()];
	}

	~Predavanje() {
		delete[] _tema;
	}

	Dogadjaj* GetParentObject() const
	{
		return new Predavanje(*this);
	}

	const char* GetTema() const { return _tema; }
	Specializacija GetSpecijalizacija() const {
		return
			_specijalizacija;
	}
};
class Ucesnik {
	static int _id; //iskoristiti za praćenje rednog broja ucesnika i generisanje jedinstvene sifre
	char* _sifra; //sifra u formatu GG-IN-BBB, pojasnjena u main funkciji, generisati prilikom kreiranja objekta
	char* _imePrezime;
	vector<Dogadjaj*> _prijavljeni;
public:
	Ucesnik(const char* imePrezime) {
		_imePrezime = AlocirajNiz(imePrezime);
		_sifra = GenerisiSifru(_imePrezime, _id++);
	}

	Ucesnik(const Ucesnik& obj)
	{
		_sifra = AlocirajNiz(obj.getSifra());
		_imePrezime = AlocirajNiz(obj.getImePrezime());
		for (size_t i = 0; i < obj._prijavljeni.size(); i++)
		{
			_prijavljeni.push_back(obj._prijavljeni[i]->GetParentObject());
		}
	}

	Ucesnik& operator=(const Ucesnik& obj)
	{
		if (this != &obj)
		{
			delete[] _imePrezime;
			delete[] _sifra;
			for (auto prijava : _prijavljeni)
				delete prijava;
			_prijavljeni.clear();
			_sifra = AlocirajNiz(obj.getSifra());
			_imePrezime = AlocirajNiz(obj.getImePrezime());
			for (size_t i = 0; i < obj._prijavljeni.size(); i++)
			{
				_prijavljeni.push_back(obj._prijavljeni[i]->GetParentObject());
			}
		}
		return *this;
	}

	~Ucesnik() {
		delete[] _imePrezime;
		delete[] _sifra;
		for (auto prijava : _prijavljeni)
			delete prijava;
		_prijavljeni.clear();
	}

	bool brojPrijavljenihPredavanja(int brojPredavanja)
	{
		return _prijavljeni.size() >= brojPredavanja;
	}

	bool operator==(const Ucesnik& obj) const
	{
		return strcmp(_sifra, obj.getSifra()) == 0;
	}

	bool PrijaviSe(const Predavanje& obj)
	{
		for (size_t i = 0; i < _prijavljeni.size(); i++)
		{
			if (obj.getTermin() == _prijavljeni[i]->getTermin())
			{
				return false;
			}
		}

		_prijavljeni.push_back(obj.GetParentObject());
		return true;
	}

	const char* getSifra() const { return _sifra; }
	const char* getImePrezime() const { return _imePrezime; }
	vector<Dogadjaj*>& getDogadjaji() { return _prijavljeni; }
};

int Ucesnik::_id = 1;

class Tim {
	char* _naziv;
	vector<Ucesnik> _clanovi;
public:
	Tim(const char* naziv) {
		_naziv = AlocirajNiz(naziv);
	}

	Tim(const Tim& obj)
	{
		_naziv = AlocirajNiz(obj._naziv);
		_clanovi = obj._clanovi;
	}

	~Tim() {
		delete[] _naziv;
	}
	
	void DodajUcesnika(const Ucesnik& obj)
	{
		if (_clanovi.size() == 0)
		{
			_clanovi.push_back(obj);
			return;
		}
		for (size_t i = 0; i < _clanovi.size(); i++)
		{
			if (obj == _clanovi[i]) throw exception("Clan je vec dio tima.");
		}
		_clanovi.push_back(obj);
	}

	const char* getNaziv() const { return _naziv; };

	vector<Ucesnik>& getUcesnici()  { return _clanovi; };

	friend ostream& operator<<(ostream& cout, const Tim& obj)
	{
		cout << obj._naziv << endl;
		for (size_t i = 0; i < obj._clanovi.size(); i++)
		{
			cout << obj._clanovi[i].getImePrezime() << endl;
		}

		return cout;
	}
};
/*
Klasa Konferencija omogucava organizaciju i pracenje koji timovi i
ucesnici prisustvuju kojim predavanjima,
ali sama ne sadrzi direktno predavanja, vec ih povezuje preko ucesnika
i timova.
*/
class Konferencija {
	char* _naziv;
	KolekcijaG1<Tim*, Tim*, 20> _timovi;
public:
	Konferencija(const char* naziv)
	{
		_naziv = AlocirajNiz(naziv);
	}

	Konferencija(const Konferencija& obj)
	{
		_naziv = AlocirajNiz(obj.getNaziv());
		_timovi = obj._timovi;
	}

	Konferencija& operator=(const Konferencija& obj)
	{
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;
			_naziv = AlocirajNiz(obj.getNaziv());
			_timovi = obj._timovi;
		}
		return *this;
	}

	void DodajTimove(const Tim& tim1, const Tim& tim2)
	{
		_timovi.AddElement(new Tim(tim1), new Tim(tim2));
	}

	bool PrijaviDogadjaj(const char* tim, const char* ucesnik, const Predavanje& predavanje)
	{
		for (size_t i = 0; i < _timovi.getTrenutno(); i++)
		{
			if (strcmp(tim, _timovi.getElement1(i)->getNaziv()) == 0)
			{
				for (size_t j = 0; j < _timovi.getElement1(i)->getUcesnici().size(); j++)
				{
					if (strcmp(ucesnik, _timovi.getElement1(i)->getUcesnici()[j].getSifra()) == 0)
					{
						if (_timovi.getElement1(i)->getUcesnici()[j].PrijaviSe(predavanje)) return true;
						else return false;
					};
				}
			}
			else if(strcmp(tim, _timovi.getElement2(i)->getNaziv()) == 0)
			{
				for (size_t j = 0; j < _timovi.getElement2(i)->getUcesnici().size(); j++)
				{
					if (strcmp(ucesnik, _timovi.getElement2(i)->getUcesnici()[j].getSifra()) == 0)
					{
						if (_timovi.getElement2(i)->getUcesnici()[j].PrijaviSe(predavanje)) return true;
						else return false;
					};
				}
			}
		}
		return false;
	}

	~Konferencija() {
		delete[] _naziv; _naziv = nullptr;
	}

	vector<Ucesnik*> operator()(int brojPredavanja)
	{
		//metoda vraca sve ucesnike koji su se na odredjenoj konferenciji prijavili na minimalno proslijedjeni broj predavanja
		vector<Ucesnik*> ucesnici;
		for (size_t i = 0; i < _timovi.getTrenutno(); i++)
		{
			for (size_t j = 0; j < _timovi.getElement1(i)->getUcesnici().size(); j++)
			{
				if (_timovi.getElement1(i)->getUcesnici()[j].brojPrijavljenihPredavanja(brojPredavanja)) ucesnici.push_back(&(_timovi.getElement1(i)->getUcesnici()[j]));
			}
			for (size_t j = 0; j < _timovi.getElement2(i)->getUcesnici().size(); j++)
			{
				if (_timovi.getElement2(i)->getUcesnici()[j].brojPrijavljenihPredavanja(brojPredavanja)) ucesnici.push_back(&(_timovi.getElement1(i)->getUcesnici()[j]));
			}
		}

		return ucesnici;
	}

	//ispisuje naziv konferencije, nazive timova i podatke o svim clanovima tima

	friend ostream& operator<<(ostream& cout, const Konferencija& obj)
	{
		cout << "Naziv konferencija: " << obj.getNaziv() << endl;
		cout << "Timovi: \n";
		for (size_t i = 0; i < obj._timovi.getTrenutno(); i++)
		{
			cout << *obj._timovi.getElement1(i) << crt << *obj._timovi.getElement2(i) << endl;
		}

		return cout;
	}
	char* getNaziv() const { return _naziv; }
	KolekcijaG1<Tim*, Tim*, 20>& getTimovi() { return _timovi; }
};

int main() {
	/* sifra korisnika treba biti u formatu GG-IN-BBB pri cemu su:

	GG - posljednje dvije cifre trenutne godine (npr.za 2025 ->
   25), preuzeti vrijednost iz sistema
	IN - inicijali ucesnika, velika slova(prvo slovo imena i
   prezimena)
	BBB → trocifreni redni broj ucesnika kreiran na osnovu _id-a
   (npr. 001, 023, 105)

	validnom sifrom treba smatrati i onu koja umjesto znaka crtica
   '-' ima znak razmak npr: 25 DM 003 ili 25 DM-003
	*/
	cout << GenerisiSifru("Denis Music", 3) << endl;//treba vratiti 25 - DM - 003
	cout << GenerisiSifru("Jasmin Azemovic", 14) << endl;//treba vratiti 25 - JA - 014
	cout << GenerisiSifru("Goran skondric", 156) << endl;//treba vratiti 25 - GS - 156
	cout << GenerisiSifru("emina junuz", 798) << endl;//treba vratiti 25 - EJ - 798
	//Za validaciju sifre koristiti funkciju ValidirajSifru kojatreba, koristeci regex, osigurati postivanje osnovnih pravila
	//vezanih za format koja su definisana u prethodnom dijelu zadatka.Pored navedenih,
	if (ValidirajSifru("25-DM-003"))
		cout << "SIFRA VALIDNA" << endl;
	if (ValidirajSifru("25-JA-014") && ValidirajSifru("25-JA 014"))
		cout << "SIFRA VALIDNA" << endl;
	if (!ValidirajSifru("25-GS-15") || !ValidirajSifru("25-Gs-135") ||
		!ValidirajSifru("25-GS-153G"))
		cout << "SIFRA NIJE VALIDNA" << endl;


	int kolekcijaTestSize = 9;
	KolekcijaG1<int, string, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, "Vrijednost -> " + to_string(i));
	cout << kolekcija1 << crt;
	/* metoda InsertAt treba da doda vrijednosti drugog i treceg
   parametra na lokaciju koja je definisana prvim parametrom. Povratna
   vrijednost metode
	je objekat (pozivaoc metode, u konkretnom slucaju objekat
   kolekcija1) u okviru koga su, na definisanu lokaciju, dodati
   proslijedjeni parametri.
	Nakon izvrsenja metode InsertAt, oba objekta, parovi1 i parovi2,
   bi trebali posjedovati sljedeci sadrzaj:
	0 Vrijednost -> 0
	10 Vrijednost -> 10
	1 Vrijednost -> 1
	2 Vrijednost -> 2
	* ....
	*/
	KolekcijaG1<int, string, 10> kolekcija2 = kolekcija1.InsertAt(1,
		10, "Vrijednost -> 10");
	cout << kolekcija2 << crt;
	/*Metoda RemoveRange kao prvi parametar prihvata pocetnu lokaciju
   a kao drugi parametar broj elemenata koje, od pocetne lokacije uklanja
   iz kolekcije koja je pozvala
	tu metodu. U slucaju da zahtijevani broj elemenata ne postoji u
   kolekciji metoda treba baciti izuzetak.
	Na kraju, metoda treba da vrati pokazivac na novi objekat tipa
   Kolekcija koji sadrzi samo uklonjene elemente*/
	KolekcijaG1<int, string, 10>* kolekcija3 = kolekcija1.RemoveRange(4, 7);//uklanja 3 elementa pocevsi od lokacije 1
	cout << "Uklonjeni:\n" << *kolekcija3;
	cout << "Preostali:\n" << kolekcija1;
	try {
		kolekcija3->RemoveRange(2, 5); //pokusavaju se ukloniti nepostojeći elementi
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}
	delete kolekcija3;
	kolekcija1 = kolekcija2;
	cout << kolekcija1 << crt;

	Termin termin1(19, 02, 30), termin2(10, 30, 40), termin3(14, 15, 20), termin4(16, 45, 20);
	 
	Predavanje oboljenja_srca(termin1, "Oboljenja srca", KARDIOLOGIJA);
	Predavanje uv_zracenja(termin2, "Uloga UV zracenja u koznim oboljenjima", DERMATOLOGIJA);
	Predavanje anemije(termin3, "Anemije u svakodnevnoj praksi", OPSTA_MEDICINA);
	 
	Ucesnik emina("Emina Junuz"), goran("Goran Skondric"), azra("Azra Maric"), tajib("Tajib Hero");
	//metoda PrijaviSe dodaje prijavu na predavanje ucesniku, ukoliko je prijava uspjesna, vraca true, a u suprotnom false.
	//onemoguciti dupliranje prijava na isto predavanje
	 
	emina.PrijaviSe(oboljenja_srca);
	emina.PrijaviSe(anemije);
	goran.PrijaviSe(oboljenja_srca);
	goran.PrijaviSe(uv_zracenja);
	tajib.PrijaviSe(uv_zracenja);
	//info metoda vraca sve detalje o dogadjaju u string formatu npr.: 19:02 : 30 Oboljenja srca KARDIOLOGIJA
	if (!emina.PrijaviSe(oboljenja_srca)) {
		cout << "Ucesnik " << emina.getImePrezime() << " vec prijavljen na predavanje " << oboljenja_srca.Info() << crt;
	}
	Tim timAlpha("Tim Alpha"), timBeta("Tim Beta");
	//metoda DodajUcesnika treba da doda ucesnika u tim, ukoliko ucesnik vec nije clan tima, u suprotnom treba baciti izuzetak.
	timAlpha.DodajUcesnika(emina); timAlpha.DodajUcesnika(goran);
	timBeta.DodajUcesnika(azra); timBeta.DodajUcesnika(tajib);
	try {
		timAlpha.DodajUcesnika(emina);//emina je vec clanica tima Alpha
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << "\n";
	}
	Konferencija savremena_medicina("Umjetna inteligencija u dijagnostici i lijecenju – novo lice medicine");
	savremena_medicina.DodajTimove(timAlpha, timBeta);
	//ispisuje naziv konferencije, nazive timova i podatke o svim clanovima tima
	cout << savremena_medicina;
	//metoda PrijaviDogadjaj treba omoguciti prijavu dogadjaja / predavanja ucesniku / clanu proslijedjenog tima.na osnovu poruka
	//koje se ispisuju u nastavku, implementirati metodu PrijaviDogadjaj tako da se prijave vrse samo na osnovu ispravnih podataka.
	if (savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), emina.getSifra(), uv_zracenja))
		cout << "Prijava uspjesna" << crt;
	if (!savremena_medicina.PrijaviDogadjaj("Tim Gamma", emina.getSifra(), anemije))
		cout << "Pokusaj prijave dogadjaja za nepostojeci tim" << crt;
	if (!savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), emina.getSifra(), uv_zracenja))
		cout << "Pokusaj dupliranja prijave predavanja" << crt;
	if (!savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), azra.getSifra(), uv_zracenja))
		cout << "Ucesnik nije clan proslijedjenog tima" << crt;
	if (!savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), "24-GX - 002", anemije))
		cout << "Prijava sa nepostojecom sifrom nije uspjela." << crt;
	//metoda vraca sve ucesnike koji su se na odredjenoj konferenciji prijavili na minimalno proslijedjeni broj predavanja
	vector<Ucesnik*> vrijedniUcesnici = savremena_medicina(2);
	for (auto ucesnik : vrijedniUcesnici)
		cout << ucesnik->getImePrezime() << "\n";
	/*
	//Funkcija UcitajUcesnike ima zadatak ucitati podatke o ucesnicima i
 //  njihovim timovima iz fajla cije ime se proslijedjuje kao parametar
	//(fajl mozete pronaci zajedno sa ispitnim zadatkom). Svaka linija u
 //  fajlu treba biti u formatu "ime i prezime|naziv tima". Funkcija za
	//svaki red u fajlu:
	//- unutar vector-a, po nazivu, pronadje ranije dodati ili
 //  kreira novi tim,
	//- kreira novog ucesnika ukoliko vec nije dio tog tima,
	//- dodaje ucesnika u odgovarajuci tim (onemoguciti dupliciranje
 //  korisnika u istom timu).
	//Na kraju, svi timovi sa svojim clanovima se nalaze u
 //  proslijedjenom vektoru timovi.
	//Funkcija vraca true ako je ucitavanje podataka bilo ouspjesno, a
 //  false ako se desilo nesto neocekivano.
	//Primjer sadrzaja fajla:
	//Goran Skondric|Tim Alpha
	//Emina Junuz|Tim Alpha
	//Azra Maric|Tim Beta
	//Tajib Hero|Tim Beta
	//*/
	//vector<Tim> timoviIzFajla;
	//if (UcitajUcesnike("ucesnici.txt", timoviIzFajla))
	//	cout << "Ucitavanje podataka USPJESNO.\n";
	//else
	//	cout << "Ucitavanje podataka NEUSPJESNO.\n";
	//for (auto& tim : timoviIzFajla)
	//	cout << tim << crt;
	//cin.get();
	//return 0;
}