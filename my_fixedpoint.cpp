#include "my_fixedpoint.h" 
#include <string>
#include <iostream>



myfixedpoint::myfixedpoint(int res, int precision)
{
	//ustawiamy pola
	res_bits = res;
	cnb = this->res_bits - precision;
	dnb = precision;
	
	int n = 0;		//ilosc segmentow potrzebnych do utworzenia liczby
	
	seg_num = (res_bits)/(sizeof(seg_t)*8);
	
	//gdy jest jakas reszta to dodajemy jeszcze jednen segment
	if((res_bits)%(sizeof(seg_t)*8))
		seg_num++;
		
	//alokujemy miejsce w wektorze
	bits.resize(seg_num);
	
	//zerujemy bity we wszystkich segmentach
	for(int i = 0; i < bits.size(); i++)
		bits[i] = 0;
}
 
void myfixedpoint::shift_left(int rep)
{
	bool carry = false;
	
	//wielokrotne przesuniecie
	for(int i = 0; i < rep; i++)
	{
		carry = 0;
		
		//prechodzimy przez wszystkie segmenty liczby
		for(int j = 0; j < seg_num; j++)
		{
			seg_t seg = bits[j];
			int mask = (1<<(sizeof(seg_t)*8-1));
			
			(bits[j]) <<= 1;			//przesuwamy segment
			(bits[j]) |= (seg_t)carry;	//uzupelniamy z wczesniejszego segmentu
			
			//obliczamy nowe przeniesienie dla kolejnego segmentu
			//na podstawie dawnej wartosci jeszcze nie przesunietego segmentu
			if(seg & mask) 	carry = true;
			else			carry = false;
		}
	}
}

void myfixedpoint::shift_right(int rep)
{
	int msb = 0;
	bool carry = false;
	
	//zapamietujemy najstarszy bit
	if(bits[bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
	{
		msb = 1;
	}
	
	//wielokrotne przesuniecie
	for(int i = 0; i < rep; i++)
	{
		carry = msb;
		
		//prechodzimy przez wszystkie segmenty liczby
		for(int j = seg_num-1; j >= 0; j--)
		{
			seg_t seg = bits[j];
			int mask = 1;
			
			(bits[j]) >>= 1;									//przesuwamy segment
			(bits[j]) |= ((seg_t)carry)<<(sizeof(seg_t)*8-1);	//uzupelniamy z wczesniejszego segmentu
			
			//obliczamy nowe przeniesienie dla kolejnego segmentu
			//na podstawie dawnej wartosci jeszcze nie przesunietego segmentu
			if(seg & mask) 	carry = true;
			else			carry = false;
		}
	}
}

myfixedpoint myfixedpoint::operator+(myfixedpoint& mf)
{
	myfixedpoint sum(res_bits, dnb);	//taka sama liczba wielkosciowo jak argumenty
	long long unsigned int sum_seg;
	long long unsigned int a,b,c;
	seg_t carry = 0;
	
	//sumujemy wszystkie segmenty obydwu liczb, biorac pod uwage przeniesienia
	for(int i = 0; i < bits.size(); i++)
	{
		a = bits[i] & MAX_SEGMENT;
		b = mf.bits[i] & MAX_SEGMENT;
		c = carry & MAX_SEGMENT;
		
		sum_seg = (a + b + c);	
		
		//sprawdzamy czy bylo przenieisenie
		if(sum_seg & CARRY_SUM_BIT)	carry = 1;
		else									carry = 0;
		
		//wynik 		
		sum.bits[i] = (seg_t)sum_seg;
	}	
	return sum;
}

myfixedpoint myfixedpoint::operator-(myfixedpoint& mf)
{
	myfixedpoint sub(res_bits, dnb);	//taka sama liczba wielkosciowo jak argumenty
	long long int sub_seg;
	long long int a,b,c;
	int carry = 0;
	
	//odejmujemy wszystkie segmenty obydwu liczb, biorac pod uwage przeniesienia
	for(int i = 0; i < bits.size(); i++)
	{
		a = bits[i] & MAX_SEGMENT;
		b = mf.bits[i] & MAX_SEGMENT;
		c = carry & MAX_SEGMENT;
		
		sub_seg = a - b - c;	
		
		//sprawdzamy czy bylo zaporzyczenie
		if(sub_seg < 0)	carry = 1;
		else			carry = 0;
		
		//wynik 		
		sub.bits[i] = (seg_t)sub_seg;
	}	
	return sub;
}


myfixedpoint myfixedpoint::operator*(myfixedpoint& mf)
{
	myfixedpoint multiplicand(res_bits, dnb);		//mnozna
	myfixedpoint multiplier(res_bits, dnb);			//mnoznik
	myfixedpoint product(res_bits, dnb);			//iloczyn
	
	int bit;
	int minus = 0;
	
	multiplicand = *this;
	multiplier = mf;
	
	//kopiujemy jako dodatnie
	if(multiplicand.bits[multiplicand.bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
	{
		multiplicand.neg();
		minus ^= 1;				//zmineiamy znak 
	}

	if(multiplier.bits[multiplier.bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
	{
		multiplier.neg();
		minus ^= 1;				//zmineiamy znak 
	}
	
	//mnozenie polega na poszukiwaniu bit闚 o wartosci 1 w liczbie mnoznika. kazda
	//jedynka to jakas potega 2 w szeregu hornera. znajdujac 1 przesuwamy w lewo mnozna
	//o pozycje znalezionego bitu z wartoscia 1 poczym wynik dodajemy do wyniku ogolnego
	
	for(int b = 0; b < multiplier.res_bits; b++)
	{
		myfixedpoint sum = multiplicand;			//sumator pomocniczy

		//wiedzac ze b jest aktualnie uzupelnianym bitem w liczbie to
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//pobieramy wartoscbitu
		if(multiplier.bits[seg] & (1<<pos))		bit = 1;
		else									bit = 0;
		
		//gdy rowny 1 to obliczamy
		if(bit)
		{
			sum.shift_left(b);
			product = product + sum;		//do glownego wyniku
		}
	}
	//na koniec dodatkowo wyrownujemy wynik 
	//przesuwamy w prawo bitowo o ilosc bitow przecinkowych
	//bo mnozenie przesuwa przecinek podwojnie
	product.shift_right(dnb);
	
	//gdy wynik ma byc ujemny
	if(minus)
	{
		product.neg();
	}
	
	return product;
}




//OPERATOR DZIELENIA
myfixedpoint myfixedpoint::operator/(myfixedpoint& mf)
{
	myfixedpoint dividend(res_bits, dnb);			//dzielna
	myfixedpoint divider(res_bits, dnb);			//dzielnik
	myfixedpoint quotient(res_bits, dnb);			//iloraz
	myfixedpoint sub_temp(res_bits, dnb);			//zmienna pomocniza
	
	int bit;
	int k = 0;
	int minus = 0;
	int end = 0;
	
	dividend = *this;
	divider = mf;
	
	//kopiujemy jako dodatnie
	if(dividend.bits[dividend.bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
	{
		dividend.neg();
		minus ^= 1;				//zmineiamy znak 
	}

	if(divider.bits[divider.bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
	{
		divider.neg();
		minus ^= 1;				//zmineiamy znak 
	}
	
	//dzielenie wykonujemy jako dzielenie przez odejmowanie
	
	//przesuwamy bity w lewo zeby poszerzyc bity przecinkowe 2 razy poniewaz
	//dzielenie spowoduje ze ich ilosc zmaleje podwojnie
	dividend.shift_left(dnb);

	//przesuwamy dzielnik w lewo az pojawi sie 1 bitowa na jego prawie najstarszym bicie
	//co oznacza這 by ze dzielnik stanie sie liczba najwieksza dodatnia jaka moze powstac przez
	//przemnozenie mnoznika przez 2^i
	while(end == 0)
	{
		//sprawdzamy bit na przedostatnij pozycji (dlatego jest -2, bo -1 oznaczalo by ze na ostatniej)
		if(divider.bits[divider.bits.size()-2] & (1<<(sizeof(seg_t)*8-2)))
		{
			//gdy pojawil sie bit o wartosci 1 na przedostatniej pozycji znaczacej
			end = 1;
		}
		else
		{
			//przesuwamy w lewo o 1 bit czyli nozymy dzielnik przez 2
			divider.shift_left(1);
			k++;		//zwiekszamy licznik przesuniec
		}
	}
	
	//gdy uzyskalismy liczbe najwieksza dodatnia zapisana na dzielniku to teraz przystepujemy do
	//czesci obliczajacej bity wyniku
	//przechodzimy przez petla tyle razy ile bylo przesuniec dzielnika w lewo
	for(; k >= 0; k--)
	{
		//obliczamy roznice
		sub_temp = dividend - divider;
		
		//sprawdzamy czy wynik odejmowania jest dodatni jak tak to by oznacza這
		//ze dividend >= divider
		//zeby sprawdzic czy liczba jest ujemna sprawdzamy jego ostatni bit (najstarszy)
		if(sub_temp.bits[sub_temp.bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
		{
			//gdy dividend < divider to wartosc kolejnego bitu to 0
			bit = 0;
		}
		else
		{
			//gdy dividend >= divider to wynik 1
			bit = 1;
			//obliczamy nowa dzielna
			dividend = dividend - divider;
		}
		
		//kodujemy wynik
		quotient.shift_left(1);		//przesuwamy w lewo o 1
		quotient.bits[0] |= bit;	//na najmlodsza pozycje ustawiamy nowy zakodowany bit
		
		//przesuwamy dzielnik o 1 bit w prawo
		divider.shift_right(1);
	}
	
	//jezeli ma byc wynik ujemny
	if(minus)
	{
		quotient.neg();
	}
	return quotient;
}


void myfixedpoint::neg()
{
	myfixedpoint one(res_bits, dnb);	//taka sama liczba wielkosciowo jak argumenty, z ustawiony bitem na najmlodszej pozycji
	one.bits[0] |= 1;
	
	//zanegowujemy wszystkie bity
	for(int i = 0; i < bits.size(); i++)
	{
		bits[i] = ~bits[i];
	}
	
		
	
	//dodjamy liczbe 1 do odwroconej
	(*this) = (*this) + one;
	
	
}

bool myfixedpoint::operator==(myfixedpoint& mf)
{

int length1 = this->bits.size();
int length2 = mf.bits.size();

bool isEqual=false;

if(length1 == length2){

for(int i=0; i<length1; i++)
{
	if(this->bits[i] == mf.bits[i]){
	isEqual=true;
	continue;
	}
	else{
	isEqual = false;
	break;
	}

}
return isEqual;
}
return isEqual;



}	
void myfixedpoint::setValue(std::string dec)
{
	int additive = 0;
	int rest = 0;
	int div_res = 0;
	int digit;
	int new_digit;
	int div_total_info = 1;
	int minus = 0;
	int dot = 0;
	int carry = 0;
	int bit = 0;
	
	
	
	//rozdzielamy na dwa stringi
	std::string c_part;		//czesc calkowita
	std::string f_part;		//czesc ulamkowa
	
	for(int i = 0; i < dec.size(); i++)
	{
		if(dec[i] == '-')	minus = 1;
		if(dec[i] == '.')	dot = 1;
	
		
		if(dec[i] >= '0' && dec[i] <= '9')
		{
			if(dot == 0)	c_part += dec[i];
			else			f_part += dec[i];	//czesc ulamkowa
		}
	}
	
	//CZESC CALKOWITA
	//dopoki jest reszta i wynik dzielenia to liczymy
	//bo to znaczy ze sa jeszcze bity do zakodowania
	for(int b = this->dnb; div_total_info != 0 || rest != 0; b++)
	{
		div_total_info = 0;	//zerujemy po wejsciu
		additive = 0;
		
		for(int i = 0; i < c_part.size(); i++)
		{
			digit = (int)c_part[i] - 0x30;	//konwersja cyfry ascii do cyfry w postaci numerycznej
			rest = digit & 1;		//reszta z dzielenia przez 2
			div_res = digit>>1;	//dzielenie przez 2;
			
			new_digit = div_res + additive;
			c_part[i] = new_digit + 0x30;			//zapisujamy jako nowa w to samo miejsce cyfre ascii
			
			//gdy jest jakas cyfra co jest inna niz zero
			if(new_digit != 0)	div_total_info = 1;
			
			//obliczamy nowy dodatek
			if(rest)	additive = 5; 
			else		additive = 0;
		} 
		
		//wiedzac ze b jest aktualnie uzupelnianym bitem w liczbie to
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//kodujemy bit w liczbie
		this->bits[seg] |= ( ((seg_t)rest)<<pos );
	}
	
	//CZESC ULAMKOWA	
	for(int b = this->dnb-1; b >= 0; b--)
	{
		carry = 0;
		
		for(int i = f_part.size()-1; i >= 0; i--)
		{
			digit = f_part[i] - 0x30;			//cyfra z aktualnego zapisu
			digit <<= 1;						//razy 2
			f_part[i] = digit%10 + carry + 0x30;	//kodujamy nowa cyfre
			carry = digit/10; 					//zapamietujemy przeniesienie
		}
		
		//wiedzac ze b jest aktualnie uzupelnianym bitem w liczbie to
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//jezeli przechodzac przez wszystkie cyfry pozosta這 przeniesienie 
		//to znaczy ze nalezy zakodowac wartosc bit = 1
		if(carry)	bit = 1;
		else		bit = 0;
	
		//kodujemy bit w liczbie
		this->bits[seg] |= ( ((seg_t)bit)<<pos );
	}
	
	//jezeli liczb byla ujemna
	if(minus)
	{
		//negujemy liczbe
		this->neg();
	}
	
}
std::istream& operator>>(std::istream& is, myfixedpoint& mf)
{
	std::string dec;
	int additive = 0;
	int rest = 0;
	int div_res = 0;
	int digit;
	int new_digit;
	int div_total_info = 1;
	int minus = 0;
	int dot = 0;
	int carry = 0;
	int bit = 0;
	
	is >> dec;	//pobieramy napis
	
	//rozdzielamy na dwa stringi
	std::string c_part;		//czesc calkowita
	std::string f_part;		//czesc ulamkowa
	
	for(int i = 0; i < dec.size(); i++)
	{
		if(dec[i] == '-')	minus = 1;
		if(dec[i] == '.')	dot = 1;
	
		
		if(dec[i] >= '0' && dec[i] <= '9')
		{
			if(dot == 0)	c_part += dec[i];
			else			f_part += dec[i];	//czesc ulamkowa
		}
	}
	
	//CZESC CALKOWITA
	//dopoki jest reszta i wynik dzielenia to liczymy
	//bo to znaczy ze sa jeszcze bity do zakodowania
	for(int b = mf.dnb; div_total_info != 0 || rest != 0; b++)
	{
		div_total_info = 0;	//zerujemy po wejsciu
		additive = 0;
		
		for(int i = 0; i < c_part.size(); i++)
		{
			digit = (int)c_part[i] - 0x30;	//konwersja cyfry ascii do cyfry w postaci numerycznej
			rest = digit & 1;		//reszta z dzielenia przez 2
			div_res = digit>>1;	//dzielenie przez 2;
			
			new_digit = div_res + additive;
			c_part[i] = new_digit + 0x30;			//zapisujamy jako nowa w to samo miejsce cyfre ascii
			
			//gdy jest jakas cyfra co jest inna niz zero
			if(new_digit != 0)	div_total_info = 1;
			
			//obliczamy nowy dodatek
			if(rest)	additive = 5; 
			else		additive = 0;
		} 
		
		//wiedzac ze b jest aktualnie uzupelnianym bitem w liczbie to
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//kodujemy bit w liczbie
		mf.bits[seg] |= ( ((seg_t)rest)<<pos );
	}
	
	//CZESC ULAMKOWA	
	for(int b = mf.dnb-1; b >= 0; b--)
	{
		carry = 0;
		
		for(int i = f_part.size()-1; i >= 0; i--)
		{
			digit = f_part[i] - 0x30;			//cyfra z aktualnego zapisu
			digit <<= 1;						//razy 2
			f_part[i] = digit%10 + carry + 0x30;	//kodujamy nowa cyfre
			carry = digit/10; 					//zapamietujemy przeniesienie
		}
		
		//wiedzac ze b jest aktualnie uzupelnianym bitem w liczbie to
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//jezeli przechodzac przez wszystkie cyfry pozosta這 przeniesienie 
		//to znaczy ze nalezy zakodowac wartosc bit = 1
		if(carry)	bit = 1;
		else		bit = 0;
	
		//kodujemy bit w liczbie
		mf.bits[seg] |= ( ((seg_t)bit)<<pos );
	}
	
	//jezeli liczb byla ujemna
	if(minus)
	{
		//negujemy liczbe
		mf.neg();
	}
	return is;
}

std::ostream& operator<<(std::ostream& os, myfixedpoint& mf)
{
	myfixedpoint mf_copy = mf;
	std::string c_part = "0";
	std::string f_part = "0";
	
	//do debugowania
	//for(int i = 0; i < mf_copy.bits.size(); i++)
	//	os << "0x" << std::hex << (unsigned int)mf_copy.bits[i] << "\n";
		
	int seg;
	int pos;	
	int digit;
	int bit;
	int rest;
	int carry;
	char minus = 0;
	
	//na poczatku sprawdzamy czy liczba jest ujemna (najstarszy bit ustawiony)
	if(mf_copy.bits[mf_copy.bits.size()-1] & (1<<(sizeof(seg_t)*8-1)))
	{
		//oznacza to ze liczba jest ujemna
		//wiez zmieniamy ja na dodatnia
		mf_copy.neg();
		minus = '-';
	}	
		
	//przechodzimy przez kolejne bity liczby zaczynajac od najstarszego
	//przechodzimy przez czesc calkowita
	for(int b = mf_copy.res_bits - 1; b >= mf_copy.dnb; b--)	
	{
		carry = 0;
		
		//na poczatku przemnazamy wszystkie cyfry przez dwa aktualizujac wynik 
		for(int i = c_part.size()-1; i >= 0; i--)
		{
			digit = c_part[i] - 0x30;				//cyfra z aktualnego zapisu
			digit <<= 1;						//razy 2
			c_part[i] = digit%10 + carry + 0x30;	//kodujamy nowa cyfre
			carry = digit/10; 					//zapamietujemy przeniesienie
		}
		
		//jezeli przechodzac przez wszystkie cyfry pozosta這 przeniesienie 
		//to rozszezamy stringa
		if(carry)
		{
			c_part = "1" + c_part;
		}
		
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//pobieramy wartosc bitu
		if((mf_copy.bits[seg]) & (1<<pos))	bit = 1;
		else								bit = 0;
		
		digit = c_part[c_part.size()-1] - 0x30;		//cyfra z aktualnego zapisu
		digit += bit;	 						//dodajamy wartosc bitu
		c_part[c_part.size()-1] = digit%10 + 0x30;				//kodujamy nowa cyfre
	}	
	
	//przechodzimy przez kolejne bity liczby zaczynajac od najmlodszego
	//przechodzimy przez czesc ulamkowa
	for(int b = 0; b < mf_copy.dnb; b++)	
	{
		//musimy obliczyc numer segmentu dla tego bitu
		int seg = b/(sizeof(seg_t)*8);
		int pos = b%(sizeof(seg_t)*8);
		
		//pobieramy wartosc bitu
		//w czesci calkowitej wstawiamy 1 gdy bit byl 1
		if((mf_copy.bits[seg]) & (1<<pos))	f_part[0] = '1';		
		else								f_part[0] = '0';	
	
		//na poczatku przemnazamy wszystkie cyfry przez dwa aktualizujac wynik 
		for(int i = f_part.size()-1; i >= 0; i--)
		{
			digit = f_part[i] - 0x30;				//cyfra z aktualnego zapisu
			digit *= 10;							//razy 10
			digit >>= 1;							//dzielimy przez 2
			rest = digit%10;						//reszta z dzielenia
			f_part[i] = digit/10 + 0x30;			//kodujamy nowa cyfre
			
			//jezeli przechodzac przez wszystkie cyfry pozosta豉 reszta
			//to rozszezamy stringa
			if(rest)
			{
				if(i == f_part.size()-1)
				{
					//gdy nie ma tam znaku to rozszez stringa
					f_part += (char)(rest + 0x30);
				}
				else
				{
					//gdy jest to sumuj cyfry
					digit = f_part[i+1] - 0x30;				//cyfra z aktualnego zapisu
					digit += rest;
					f_part[i+1] = digit + 0x30;
				}	
			}
		}	
	}	
	
	//na koniec usuwamy pierwszy znak 0 z f_part dla poprawnosci wyswietlania
	if(f_part.size() > 1)
		f_part.erase(0,1);
	
	os << "Decimal: " << minus << c_part << '.' << f_part <<  std::endl;
	return os;
}
