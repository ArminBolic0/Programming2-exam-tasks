#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <mutex>
#include <fstream>

using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, "
"OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE "
"POMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10.SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAINU, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA FUNKCIJA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum Kriteriji { CISTOCA, USLUGA, LOKACIJA, UDOBNOST };

bool ValidirajBrojPasosa(string pasos)
{
    return regex_match(pasos, regex("[A-Z]{1,2}\\d{3,4}(-| )?\\d{2,4}"));
}

char* GetNizKaraktera(const char* sadrzaj)
{
    if (sadrzaj == nullptr) return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

unique_ptr<char[]> GetUniqueNizKaraktera(const char* sadrzaj)
{
    if (sadrzaj == nullptr) return nullptr;
    int vel = strlen(sadrzaj) + 1;
    unique_ptr<char[]> temp(new char[vel]);
    strcpy_s(temp.get(), vel, sadrzaj);
    return temp;
}

template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    bool _omoguciDupliranje;
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = new int(0);
        _omoguciDupliranje = omoguciDupliranje;
    }

    Kolekcija(const Kolekcija& obj)
    {
        _trenutno = new int(*obj._trenutno);
        _omoguciDupliranje = obj._omoguciDupliranje;
        _elementi1 = new T1[*_trenutno];
        _elementi2 = new T2[*_trenutno];
        for (size_t i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
        }
    }

    Kolekcija& operator=(const Kolekcija& obj)
    {
        if (this != &obj)
        {
            delete[]_elementi1; _elementi1 = nullptr;
            delete[]_elementi2; _elementi2 = nullptr;
            delete _trenutno; _trenutno = nullptr;
            _trenutno = new int(*obj._trenutno);
            _omoguciDupliranje = obj._omoguciDupliranje;
            _elementi1 = new T1[*_trenutno];
            _elementi2 = new T2[*_trenutno];
            for (size_t i = 0; i < *_trenutno; i++)
            {
                _elementi1[i] = obj.getElement1(i);
                _elementi2[i] = obj.getElement2(i);
            }
        }
        return *this;
    }

    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }

    void AddElement(T1 el1, T2 el2)
    {
        if (!_omoguciDupliranje)
        {
            for (size_t i = 0; i < *_trenutno; i++)
            {
                if (el1 == _elementi1[i] && el2 == _elementi2[i]) throw exception("Dupliranje onemoguceno");
            }
        }
        T1* temp1 = new T1[*_trenutno + 1];
        T2* temp2 = new T2[*_trenutno + 1];
        for (size_t i = 0; i < *_trenutno; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        temp1[*_trenutno] = el1;
        temp2[*_trenutno] = el2;
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
        _elementi1 = temp1;
        _elementi2 = temp2;
        (*_trenutno)++;
    }

    Kolekcija<int, int> InsertAt(int lokacija, T1 el1, T2 el2)
    {
        if (!_omoguciDupliranje)
        {
            for (size_t i = 0; i < *_trenutno; i++)
            {
                if (el1 == _elementi1[i] && el2 == _elementi2[i]) throw exception("Dupliranje onemoguceno");
            }
        }

        T1* temp1 = new T1[*_trenutno + 1];
        T2* temp2 = new T2[*_trenutno + 1];
        for (size_t i = 0; i < lokacija; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        temp1[lokacija] = el1;
        temp2[lokacija] = el2;
        for (size_t i = lokacija; i < *_trenutno; i++)
        {
            temp1[i + 1] = _elementi1[i];
            temp2[i + 1] = _elementi2[i];
        }
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
        _elementi1 = temp1;
        _elementi2 = temp2;
        (*_trenutno)++;

        return *this;
    }

    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() { return *_trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};
class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }

    Datum(const Datum& obj)
    {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }

    Datum& operator=(const Datum& obj)
    {
        if (this != &obj)
        {
            *_dan = *obj._dan;
            *_mjesec = *obj._mjesec;
            *_godina = *obj._godina;
        }
        return *this;
    }

    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
};

class Komentar {
    char* _sadrzajKomentara;
    //uz komentar, gosti mogu ocijeniti svaki od kriterija zasebno, a int se odnosi na ocjenu u rasponu od 1 - 10.
    //onemoguciti dupliranje kriterija tj. svaki kriterij se moze ocijeniti samo jednom...u suprotnom baciti objekat tipa exception
    Kolekcija<Kriteriji, int>* _ocjeneKriterija;
public:
    Komentar(const char* sadrzajKomentara = "") {
        //kreirajte funkciju GetNizKaraktera ili na drugi adekvatan nacin izvrsite inicijalizaciju pokazivaca
        _sadrzajKomentara = GetNizKaraktera(sadrzajKomentara);
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>();
    }

    Komentar(const Komentar& obj)
    {
        _sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
        _ocjeneKriterija = new Kolekcija<Kriteriji, int>(*obj._ocjeneKriterija);
    }

    Komentar& operator=(const Komentar& obj)
    {
        if (this != &obj)
        {
            delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
            delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
            _sadrzajKomentara = GetNizKaraktera(obj._sadrzajKomentara);
            _ocjeneKriterija = new Kolekcija<Kriteriji, int>(*obj._ocjeneKriterija);
        }
        return *this;
    }

    ~Komentar() {
        delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
        delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
    }

    int brojOcjenaManjihOdPet() const
    {
        int brojac = 0;
        for (size_t i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            if (_ocjeneKriterija->getElement2(i) < 5) ++brojac;
        }
        return brojac;
    }

    float prosjecnaOcjena() const
    {
        float suma = 0;
        for (size_t i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            suma += _ocjeneKriterija->getElement2(i);
        }
        return suma / _ocjeneKriterija->getTrenutno();
    }

    void AddOcjenuKriterija(Kriteriji kriteriji, int ocjena)
    {
        for (size_t i = 0; i < _ocjeneKriterija->getTrenutno(); i++)
        {
            if (_ocjeneKriterija->getElement1(i) == kriteriji) throw exception("Kriteriji vec ocjenjen");
        }
        _ocjeneKriterija->AddElement(kriteriji, ocjena);
    }
    char* GetSadrzajKomentara() { return _sadrzajKomentara; }
    Kolekcija<Kriteriji, int>* GetOcjeneKriterija() { return _ocjeneKriterija; }
    friend ostream& operator<<(ostream& cout, const Komentar& obj)
    {
        cout << obj._sadrzajKomentara << endl;
        for (size_t i = 0; i < obj._ocjeneKriterija->getTrenutno(); i++)
        {
            cout << obj._ocjeneKriterija->getElement1(i) << " " << obj._ocjeneKriterija->getElement2(i) << endl;
        }
        return cout;
    }
};



class Gost {
    unique_ptr<char[]> _imePrezime;
    string _emailAdresa;
    string _brojPasosa;
public:
    Gost(const char* imePrezime, string emailAdresa, string brojPasosa) {
        _imePrezime = GetUniqueNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _brojPasosa = ValidirajBrojPasosa(brojPasosa) ? brojPasosa : "NOT VALID";
    }

    Gost(const Gost& obj)
    {
        _imePrezime = GetUniqueNizKaraktera(obj._imePrezime.get());
        _emailAdresa = obj._emailAdresa;
        _brojPasosa = obj._brojPasosa;
    }

    Gost& operator=(const Gost& obj)
    {
        if (this != &obj)
        {
            _imePrezime = GetUniqueNizKaraktera(obj._imePrezime.get());
            _emailAdresa = obj._emailAdresa;
            _brojPasosa = obj._brojPasosa;
        }
        return *this;
    }

    string GetEmail() const { return _emailAdresa; }
    string GetBrojPasosa() const { return _brojPasosa; }
    char* GetImePrezime() { return _imePrezime.get(); }

    friend ostream& operator<< (ostream& COUT, const Gost& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojPasosa << endl;
        return COUT;
    }
};

mutex mx;

class Rezervacija {
    Datum _OD;
    Datum _DO;
    //onemoguciti dodavanje gostiju sa istom email adresom ili brojem pasosa u okviru jedne rezervacije
    vector<Gost> _gosti;
    //za svaku rezervaciju se moze ostaviti samo jedan komentar, bez obzira na broj gostiju
    Komentar _komentar;
    // //ukoliko se rezervaciji doda komentar koji sadrzi najmanje dvije ocjene kriterija sa vrijednoscu manjom od 5, svim gostima, 
// //koji su dodati u rezervaciju, u zasebnom thread-u, se salje email sa sljedecim sadrzajem:
///*
//    To: denis@fit.ba;jasmin@fit.ba;
//    Subject: Informacija

//    Postovani,

//    Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je X.X.
//    Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.

//    Ugodan boravak Vam zelimo
//    Puno pozdrava
//*/
    void SaljiEmail(const Komentar& obj)
    {
        mx.lock();
        cout << "To: ";
        for (size_t i = 0; i < _gosti.size(); i++)
        {
            cout << _gosti[i].GetEmail() << ";";
        }
        cout << "\nSubject: Informacija" << endl;
        cout << "\n\nPostovani,\nZaprimili smo vase ocjene, a njihova prosjecna vrijenost je " << obj.prosjecnaOcjena() << "." << endl;
        cout << "Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa sluzba za odnose sa gostima.\nUgodan boravak vam zelimo\nPuno pozdrava" << endl;
        mx.unlock();
    }
public:
    Rezervacija(Datum& OD, Datum& DO, Gost& gost) :_OD(OD), _DO(DO) {
        _gosti.push_back(gost);
    }

    Rezervacija(const Rezervacija& obj)
    {
        _OD = obj._OD;
        _DO = obj._DO;
        _gosti = obj._gosti;
        _komentar = obj._komentar;
    }

    Rezervacija& operator=(const Rezervacija& obj)
    {
        if (this != &obj)
        {
            _OD = obj._OD;
            _DO = obj._DO;
            _gosti = obj._gosti;
            _komentar = obj._komentar;
        }
        return *this;
    }

    bool AddGost(const Gost& obj)
    {
        for (size_t i = 0; i < _gosti.size(); i++)
        {
            if (obj.GetEmail() == _gosti[i].GetEmail() || obj.GetBrojPasosa() == _gosti[i].GetBrojPasosa()) return false;
        }
        _gosti.push_back(obj);
        return true;
    }

    void SetKomentar(const Komentar& obj)
    {
        _komentar = obj;
        if (_komentar.brojOcjenaManjihOdPet() >= 2)
        {
            thread t1(&Rezervacija::SaljiEmail, this, obj);
            t1.join();
        }
    }

    pair <int, int> GetBrojZnakova(const char* putanja, const char* karakteri)
    {
        int brojacUkupno = 0;
        int brojacIstih = 0;
        ifstream fajl;
        fajl.open(putanja);

        char znak;

        while (fajl.get(znak))
        {
            for (int i = 0; i < strlen(karakteri); i++)
            {
                if (znak == karakteri[i])
                    brojacIstih++;
            }
            brojacUkupno++;
        }

        pair<int, int>par;
        par.first = brojacIstih;
        par.second = brojacUkupno;
        fajl.close();
        return par;
    }

    vector<Gost>& GetGosti() { return _gosti; }
    Komentar GetKomentar() { return _komentar; }
    friend ostream& operator<< (ostream& COUT, Rezervacija& obj) {
        COUT << crt << "Rezervacija " << obj._OD << " - " << obj._DO << " za goste: " << endl;
        for (size_t i = 0; i < obj._gosti.size(); i++)
            COUT << "\t" << i + 1 << "." << obj._gosti[i];
        cout << crt << "Komentar rezervacije: " << endl << obj._komentar << crt;
        cout << "Prosjecna ocjena: " << obj.GetKomentar().prosjecnaOcjena() << endl;
        return COUT;
    }
};

void main() {

    Datum
        datum19062022(19, 6, 2022),
        datum20062022(20, 6, 2022),
        datum30062022(30, 6, 2022),
        datum05072022(5, 7, 2022);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement treba baciti objekat tipa exception
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    /* metoda InsertAt treba da doda vrijednosti drugog i treceg parametra na lokaciju koja je definisana prvim parametrom. Povratna vrijednost metode
    * je objekat (pozivaoc metode, u konkretnom slucaju objekat kolekcija1) u okviru koga su, na definisanu lokaciju, dodati zahtijevani parametri.
    * Nakon izvrsenja metode InsertAt, oba objekta, kolekcija1 i kolekcija2, bi trebali posjedovati sljedeci sadrzaj:
    * 10 10
    * 0  0
    * 1  1
    * 2  2
    * ....
    */
    Kolekcija<int, int> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
    cout << kolekcija2 << crt;


    /*
    * za validaciju broja pasosa koristiti funkciju ValidirajBrojPasosa koja treba, koristeci regex, osigurati postivanje
    sljedeceg formata:
    - pocetak sadrzi jedan ili dva velika slova
    - nakon toga slijede tri ili cetiri broja
    - nakon toga moze, a ne mora, doci znak crtica (-) ili razmak ( )
    - nakon toga slijede dva do cetiri broja

    pomenutu funkciju iskoristiti u konstruktoru klase Gost, te ukoliko broj pasosa nije u validnom formatu onda njegova vrijednost treba biti postavljena na NOT VALID
    */

    if (ValidirajBrojPasosa("BH235-532"))
        cout << "Broj pasosa validan" << endl;
    if (ValidirajBrojPasosa("B123321"))
        cout << "Broj pasosa validan" << endl;
    if (ValidirajBrojPasosa("B1252 521"))
        cout << "Broj pasosa validan" << endl;
    if (!ValidirajBrojPasosa("H4521"))
        cout << "Broj pasosa NIJE validan" << endl;
    if (!ValidirajBrojPasosa("b1252 521"))
        cout << "Broj pasosa NIJE validan" << endl;


    Gost denis("Denis Music", "denis@fit.ba", "BH235-532");
    Gost jasmin("Jasmin Azemovic", "jasmin@fit.ba", "B123321");
    Gost adel("Adel Handzic", "adel@edu.fit.ba", "B1252 521");
    Gost gostPasosNotValid("Ime Prezime", "korisnik@klix.ba", "H4521");//_brojPasosa = NOT VALID

    //rezervaciji se prilikom kreiranja mora dodijeliti gost koji je nosilac rezervacije
    Rezervacija rezervacija(datum19062022, datum20062022, denis);
    if (rezervacija.AddGost(jasmin))
        cout << "Gost uspjesno dodan!";

    Komentar komentarRezervacija("Nismo pretjerano zadovoljni uslugom, a ni lokacijom.");
    komentarRezervacija.AddOcjenuKriterija(CISTOCA, 7);
    komentarRezervacija.AddOcjenuKriterija(USLUGA, 4);
    komentarRezervacija.AddOcjenuKriterija(LOKACIJA, 3);
    komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);

    try {
        komentarRezervacija.AddOcjenuKriterija(UDOBNOST, 6);//kriterij udobnosti je vec ocijenjen!
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }

    komentarRezervacija.brojOcjenaManjihOdPet();
    //u okviru rezervacije postavlja vrijednost atributa _komentar 
    rezervacija.SetKomentar(komentarRezervacija);
   // //ukoliko se rezervaciji doda komentar koji sadrzi najmanje dvije ocjene kriterija sa vrijednoscu manjom od 5, svim gostima, 
   // //koji su dodati u rezervaciju, u zasebnom thread-u, se salje email sa sljedecim sadrzajem:
   ///*
   //    To: denis@fit.ba;jasmin@fit.ba;
   //    Subject: Informacija

   //    Postovani,

   //    Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je X.X.
   //    Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.

   //    Ugodan boravak Vam zelimo
   //    Puno pozdrava
   //*/

   ////ispisuje sve podatke o rezervaciji, gostima, komentar, ocjene pojedinih kriterija i prosjecnu ocjenu.
    cout << rezervacija << endl;
   // /* Primjer ispisa:
   //    -------------------------------------------
   //    Rezervacija 19.6.2022 - 20.6.2022 za goste:
   //            1.Denis Music denis@fit.ba BH235-532
   //            2.Jasmin Azemovic jasmin@fit.ba B123321
   //    -------------------------------------------
   //    Komentar rezervacije:
   //    Nismo pretjerano zadovoljni uslugom, a ni lokacijom.
   //            CISTOCA(7)
   //            USLUGA(4)
   //            LOKACIJA(3)
   //            UDOBNOST(6)
   //    -------------------------------------------
   //    Prosjecna ocjena -> 5
   //*/

    Rezervacija rezervacijaSaAdelom = rezervacija;
    if (rezervacijaSaAdelom.AddGost(adel))
        cout << "Gost uspjesno dodan!";
    if (!rezervacijaSaAdelom.AddGost(denis))
        cout << "Gost je vec dodan na rezervaciju!";

    cout << rezervacijaSaAdelom << endl;

    /*metoda GetBrojZnakova treba prebrojati koliko se unutar fajla, cije ime je proslijedjeno kao prvi parametar (relativna putanja), nalazi ukupno znakova, te koliko
    se unutar pomenutog fajla nalazi trazenih znakova koji su proslijedjeni kao drugi parametar. Vrijednosti brojaca se vracaju kao par.*/
    pair<int, int> brojac = rezervacijaSaAdelom.GetBrojZnakova("rezervacije.txt", "*_+");
    cout << "Ukupno znakova u fajlu: " << brojac.first << endl;
    cout << "Ukupno trazenih znakova: " << brojac.second << endl;
    /*
    ako se u fajlu rezervacije.txt nalazi sadrzaj "denis+music*_" onda povratna vrijednost metode GetBrojZnakova treba biti par 13 i 3. za potrebe testiranja kreirajte
    fajl sa zeljenim sadrzajem na istoj lokaciji kao i vas .exe fajl. vodite racuna o tome da se zeljeni naziv fajla definise vrijednoscu parametra.
    */

   // cin.get();
   // system("pause>0");
}