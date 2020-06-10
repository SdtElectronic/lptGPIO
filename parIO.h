#include <cstddef>
#include <stdexcept>
#include <sys/io.h>
#include <map>
#pragma once

class parIO{	
	public:
		using addr2data_t = std::map<unsigned short, char>;
		static addr2data_t dataReg, ctrlReg;
		const unsigned short addr;
		const unsigned short ctrlAddr;

		parIO(const unsigned short gAddr);

		static int dataWrite(char val, unsigned short addr);
		static int ctrlWrite(char val, unsigned short addr);
		static char statRead(unsigned short addr);
		static char ctrlRead(unsigned short addr);

		static int getPerm(bool perm, unsigned short addr);
};

class parO: private parIO{	
	public:
		enum dataPin: unsigned char{	
			D0 = 1, D1 = 2, D2 = 4, D3 = 8,
			D4 = 16, D5 = 32, D6 = 64, D7 = 128
		};
		enum ctrlPin: unsigned char{	
			STROBE = 1, AUTO_FEED = 2, INIT = 4, SELECT_IN = 8
		};
		
		parO(dataPin dPin, unsigned short addr = 0x378);
		parO(ctrlPin cPin, unsigned short addr = 0x378);
		int operator = (bool val);
		int write(bool val);
	private:
		using mfPtr = int (parO::*)(bool);
		mfPtr wr;
		
		union pinU{	
			dataPin dPin;
			ctrlPin cPin;
			pinU(dataPin pin): dPin(pin){}
			pinU(ctrlPin pin): cPin(pin){}
		}pin;

		int wrData(bool val);
		int wrCtrl(bool val);
};

class parI: private parIO{	
	public:
		enum statPin: unsigned char{	
			ERROR = 1, SELECT = 2, PE = 4, ACK = 8,	BUSY = 16
		};
		enum ctrlPin: unsigned char{	
			STROBE = 1, AUTO_FEED = 2, INIT = 4, SELECT_IN = 8
		};

		parI(statPin sPin, unsigned short addr = 0x378);
		parI(ctrlPin cPin, unsigned short addr = 0x378);
		int read();
		operator int();
	private:
		using mfPtr = bool (parI::*)();
		mfPtr re;

		union pinU{	
			statPin sPin;
			ctrlPin cPin;
			pinU(statPin pin): sPin(pin){}
			pinU(ctrlPin pin): cPin(pin){}
		}pin;

		bool reStat();
		bool reCtrl();
};

inline int parO::write(bool val){	
	return (this->*wr)(val);
}

inline int parO::operator = (bool val){	
	return parO::write(val);
}

inline int parI::read(){	
	return static_cast<int>((this->*re)());
}

inline parI::operator int (){	
	return read();
}

