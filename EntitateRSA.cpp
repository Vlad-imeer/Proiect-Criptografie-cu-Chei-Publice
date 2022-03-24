﻿#include "EntitateRSA.h"

const string EntitateRSA::NEDEFINIT = "NEDEFINIT";
const size_t EntitateRSA::LIMITA_NUMAR_ALEATOR = 100;

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
    auto numar_aleator = EntitateRSA::_generator_numere_aleatorii() % LIMITA_NUMAR_ALEATOR;
    while (numar_aleator <= 1)
    {
        numar_aleator = EntitateRSA::_generator_numere_aleatorii() % LIMITA_NUMAR_ALEATOR;
    }
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

size_t EntitateRSA::GenerareCheiePrivata(const size_t& p_fi)
{
    while (true)
    {
        auto cheie_privata = EntitateRSA::GenerareNumarPrimRandom() % p_fi;
        if (cheie_privata >= 2 && EntitateRSA::GetCelMaiMareDivizorComun(cheie_privata, p_fi) == 1)
        {
            return cheie_privata;
        }
    }
}

size_t EntitateRSA::GenerareCheiePublica(const size_t& p_fi) const
{
    size_t cheie_publica = p_fi / this->_cheie_privata;
    while (true)
    {
        if (cheie_publica * this->_cheie_privata % p_fi == 1)
        {
            return cheie_publica;
        }
        cheie_publica++;
    }
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

size_t EntitateRSA::Encriptare(const size_t& p_mesaj) const
{
    long msg_des = 1;
    long root = p_mesaj;
    long index = this->_cheie_privata;
    while (index)
    {
        if (index & 1)
            msg_des = (msg_des * root) % this->_produs_numere_prime;
        index >>= 1;
        root = (root * root) % this->_produs_numere_prime;
    }
    return msg_des;
}

size_t EntitateRSA::Decriptare(const size_t& p_mesaj) const
{
    long msg_des = 1;
    long root = p_mesaj;
    long index = this->_cheie_publica;
    while (index)
    {
        if (index & 1)
            msg_des = (msg_des * root) % this->_produs_numere_prime;
        index >>= 1;
        root = (root * root) % this->_produs_numere_prime;
    }
    return msg_des;
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

    auto fi = (numar_prim_1 - 1) * (numar_prim_2 - 1);
    this->_cheie_privata = EntitateRSA::GenerareCheiePrivata(fi);
    this->_cheie_publica = this->GenerareCheiePublica(fi);

    cout << "\nChei generate pentru " << this->_nume << "!";
}

size_t EntitateRSA::EncriptareCuCheiePrivata(const size_t& p_mesaj) const
{
    return 0;
}

size_t EntitateRSA::EncriptareCuCheiePublica(const size_t& p_mesaj) const
{
    return 0;
}

size_t EntitateRSA::DecriptareCuCheiePrivata(const size_t& p_mesaj) const
{
    return 0;
}

size_t EntitateRSA::DecriptareCuCheiePublica(const size_t& p_mesaj) const
{
    return 0;
}

ostream& operator<<(ostream& p_stream, const EntitateRSA& p_entitate)
{
    p_stream << p_entitate._nume;
    return p_stream;
}
