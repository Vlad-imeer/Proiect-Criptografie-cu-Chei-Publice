﻿#include "EntitateRSA.h"

const string EntitateRSA::NEDEFINIT = "NEDEFINIT";

const size_t EntitateRSA::LIMITA_INFERIOARA_NUMAR_ALEATOR = 10;
const size_t EntitateRSA::LIMITA_SUPERIOARA_NUMAR_ALEATOR = 20;

// ”
// This compliant solution uses std::random_device to generate a random value for seeding the Mersenne Twister engine object.
// The values generated by std::random_device are nondeterministic random numbers when possible, relying on random number generation devices, such as /dev/random.
// When such a device is not available, std::random_device may employ a random number engine; however, the initial value generated should have sufficient randomness to serve as a seed value.
// ”
// Sursă: https://wiki.sei.cmu.edu/confluence/display/cplusplus/MSC51-CPP.+Ensure+your+random+number+generator+is+properly+seeded
random_device EntitateRSA::_generator_seed = {};
mt19937 EntitateRSA::_generator_numere_aleatorii(EntitateRSA::_generator_seed());

size_t EntitateRSA::GetCelMaiMareDivizorComun(const size_t& p_numar_1, const size_t& p_numar_2)
{ 
    return !p_numar_2 ? p_numar_1 : GetCelMaiMareDivizorComun(p_numar_2, p_numar_1 % p_numar_2);
}

size_t EntitateRSA::GenerareNumarRandom()
{
    auto numar_aleator =
        LIMITA_INFERIOARA_NUMAR_ALEATOR +
        EntitateRSA::_generator_numere_aleatorii() %
        LIMITA_SUPERIOARA_NUMAR_ALEATOR;
    return numar_aleator;
}

bool EntitateRSA::EstePrim(const size_t& p_numar)
{
    for (size_t div = 2; div * div < p_numar; div++)
    {
        if (p_numar % div == 0)
        {
            return false;
        }
    }
    return true;
}

size_t EntitateRSA::GenerareNumarPrimRandom()
{
    auto numar_random = GenerareNumarRandom();
    while (!EntitateRSA::EstePrim(numar_random))
    {
        numar_random++;
    }
    return numar_random;
}

// ReSharper disable once CppMemberFunctionMayBeStatic <=== Pentru consistență
size_t EntitateRSA::GenerareCheiePrivata(const unsigned long long& p_fi)
{
    size_t cheie_privata = EntitateRSA::GenerareNumarPrimRandom() % p_fi;
    while (cheie_privata < 2 || EntitateRSA::GetCelMaiMareDivizorComun(cheie_privata, p_fi) != 1)
    {
        cheie_privata = EntitateRSA::GenerareNumarPrimRandom() % p_fi;
    }
    return cheie_privata;
}

size_t EntitateRSA::GenerareCheiePublica(const unsigned long long& p_fi) const
{
    size_t cheie_publica = p_fi / this->_cheie_privata;
    while (cheie_publica * this->_cheie_privata % p_fi != 1)
    {
        cheie_publica++;
    }
    return cheie_publica;
}

size_t EntitateRSA::GetCheieFolosita(const TipCheie& p_tip_cheie) const
{
    switch (p_tip_cheie)
    {
    case EntitateRSA::TipCheie::Privata:
        return this->_cheie_privata;
    case EntitateRSA::TipCheie::Publica:
        return this->_cheie_publica;
    }
    throw logic_error("Tipul de cheie " + to_string(p_tip_cheie) + " neimplementat");
}

unsigned long long EntitateRSA::EncriptareDecriptare(const unsigned long long& p_mesaj, const TipCheie& p_tip_cheie) const
{
    size_t cheie_folosita = this->GetCheieFolosita(p_tip_cheie);
    unsigned long long mesaj_procesat = 1;
    unsigned long long putere = p_mesaj;

    while (cheie_folosita)
    {
        if (cheie_folosita & 1)
        {
            mesaj_procesat = mesaj_procesat * putere % this->_produs_numere_prime;
        }
        cheie_folosita >>= 1;
        putere = putere * putere % this->_produs_numere_prime;
    }

    return mesaj_procesat;
}

EntitateRSA::EntitateRSA() : _cheie_privata(0), _cheie_publica(0), _produs_numere_prime(0),
                             _nume(EntitateRSA::NEDEFINIT)
{
}

EntitateRSA::EntitateRSA(const string& p_nume) : EntitateRSA()
{
    this->_nume = p_nume; // NOLINT(cppcoreguidelines-prefer-member-initializer)
}

EntitateRSA::~EntitateRSA() = default;

void EntitateRSA::GenerareChei()
{
    auto numar_prim_1 = EntitateRSA::GenerareNumarPrimRandom();
    auto numar_prim_2 = EntitateRSA::GenerareNumarPrimRandom();
    while (numar_prim_1 == numar_prim_2)
    {
        numar_prim_2 = EntitateRSA::GenerareNumarPrimRandom();
    }

    this->_produs_numere_prime = numar_prim_1 * numar_prim_2;

    unsigned long long fi = (numar_prim_1 - 1) * (numar_prim_2 - 1);
    this->_cheie_privata = this->GenerareCheiePrivata(fi);
    this->_cheie_publica = this->GenerareCheiePublica(fi);

    cout << "\nChei generate pentru " << this->_nume << "!";
}

size_t EntitateRSA::EncriptareDecriptarePrivata(const size_t& p_mesaj) const
{
    return this->EncriptareDecriptare(p_mesaj, EntitateRSA::TipCheie::Privata);
}

size_t EntitateRSA::EncriptareDecriptarePublica(const size_t& p_mesaj) const
{
    return this->EncriptareDecriptare(p_mesaj, EntitateRSA::TipCheie::Publica);
}

ostream& operator<<(ostream& p_stream, const EntitateRSA& p_entitate)
{
    p_stream << p_entitate._nume;
    return p_stream;
}
