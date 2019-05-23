#include <iostream>
#include <vector>

#define SEGMENT_SIZE unsigned int		//maksymalne 32bit
#define MAX_SEGMENT 0xffffffff		//to wazne 
#define CARRY_SUM_BIT	0x100000000

typedef SEGMENT_SIZE seg_t;			//wielkosc przedstawiajaca segment obliczeniowy

class myfixedpoint
{
	private:
			int res_bits;	//rozdzielczoscc bitowa liczby czyli ilosc wszystkich bitow
			int cnb;			//ilosc bitow czesci calkowitej
			int dnb;			//ilosc bitow czesci po przecinku
			int seg_num;		//ilosc segmentow w liczbie
			
			std::vector<seg_t> bits;		//bity liczby sa tu zakodowane binarnie
	public:
			myfixedpoint(int res, int precision);
			void neg();								//funkcja zmienia znak liczby
			void shift_left(int rep);
			void shift_right(int rep);
			void setValue(std::string dec);
			myfixedpoint operator+(myfixedpoint& mf);	//dodawanie
			myfixedpoint operator-(myfixedpoint& mf);
			myfixedpoint operator*(myfixedpoint& mf);
			myfixedpoint operator/(myfixedpoint& mf);
			bool operator ==(myfixedpoint& mf);
			friend std::istream& operator>>(std::istream& is, myfixedpoint& mf);
			friend std::ostream& operator<<(std::ostream& os, myfixedpoint& mf);
};
