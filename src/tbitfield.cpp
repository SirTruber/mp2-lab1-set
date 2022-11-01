// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include "tbitfield.h"

TBitField::TBitField(int len) : BitLen(len), MemLen(1 + len / (sizeof(TELEM) * 8))
{
  if (len <= 0)throw std::invalid_argument{ "TBitField(int len)" };
  pMem = new TELEM[MemLen]();
  }

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  this->operator=(bf);
}

TBitField::~TBitField()
{
  delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  return n / (sizeof(TELEM) * 8);
}
TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  return(1 << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw std::invalid_argument{"void TBitField::SetBit(const int n)"};
    if (n >= BitLen) throw std::out_of_range{ "void TBitField::SetBit(const int n)"};
  *(pMem + GetMemIndex(n)) |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw std::invalid_argument{ "void TBitField::ClrBit(const int n)" };
    if (n >= BitLen) throw std::out_of_range{ "void TBitField::ClrBit(const int n)" };
    *(pMem + GetMemIndex(n))&=~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw std::invalid_argument{ "void TBitField::GetBit(const int n)" };
    if (n >= BitLen) throw std::out_of_range{ "void TBitField::GetBit(const int n) out_of_range" };
    TELEM data = *(pMem + GetMemIndex(n));
    if(data&=this->GetMemMask(n))
      return 1;
    else
      return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this != &bf){
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[bf.MemLen];
    std::copy(bf.pMem,bf.pMem + bf.MemLen,pMem);
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (BitLen == bf.GetLength()){
    for (size_t i = MemLen-1; i > 0;i--){
      if (pMem[i]!=(bf.pMem[i]))
        return 0;
    }
    for (size_t i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++) {
        if (GetBit(i) != bf.GetBit(i))
            return 0;
    }
    return 1;
  }
  return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  if (*this == bf){
    return 0;
  }
  else {
    return 1;
  }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int len = BitLen;
  if (BitLen < bf.GetLength())
     len = bf.GetLength();
  TBitField tmp(len);
    std::copy(bf.pMem, bf.pMem + bf.MemLen, tmp.pMem);
    for (size_t i = 0; i < bf.MemLen; i++) {
        tmp.pMem[i] |= pMem[i];
    }
    return (tmp);
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
  int len = BitLen;
  if (BitLen < bf.GetLength()) {
    len = bf.GetLength();
  }
  TBitField tmp(len);
  for (size_t i = 0; i < bf.MemLen && i < MemLen; i++) {
    tmp.pMem[i] = pMem[i] & bf.pMem[i];
  }
return (tmp);
}


TBitField TBitField::operator~(void) // отрицание
{
  for(size_t i = 0; i<BitLen;i++){
    if(GetBit(i))
      ClrBit(i);
    else
      SetBit(i);
  }
  return(*this);
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  char *ch = new char[bf.GetLength()];
  for(int i = 0; i < bf.GetLength();i++){
    istr >> ch[i];
    if (ch[i] == '0')
      bf.ClrBit(i);
     else if (ch[i] == '1')
       bf.SetBit(i);
     else{
       throw std::length_error("istream & operator>>(istream & istr, TBitField & bf)");
     }
   }
  return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (size_t  i = 1 ; i < bf.GetLength() ;i++) {
        ostr << bf.GetBit(i - 1);
        if (i % 16== 0) ostr << '\n';
    }
    return ostr;
}
