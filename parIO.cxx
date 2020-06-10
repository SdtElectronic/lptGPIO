#include <sys/io.h>
#include <unistd.h>
#include "parIO.h"

parIO::addr2data_t parIO::ctrlReg = parIO::addr2data_t();
parIO::addr2data_t parIO::dataReg = parIO::addr2data_t();

parIO::parIO(const unsigned short gAddr): addr(gAddr),
										  ctrlAddr(gAddr + 2){}

int parIO::dataWrite(char val, unsigned short addr = 0x378){	
	outb(val, addr);
	dataReg[addr] = val;
	return 0;
}

int parIO::ctrlWrite(char val, unsigned short addr = 0x378){	
	outb(val, addr + 2);
	ctrlReg[addr] = val;
	return 0;
}

char parIO::statRead(unsigned short addr = 0x378){	
	return inb(addr + 2);
}

int parIO::getPerm(bool perm = 1, unsigned short addr = 0x378){	
	if(ioperm(addr, 1, 1)){
		throw std::runtime_error("Permission denied");
		return 0;
	}
	return 0;
}

parO::parO(dataPin dPin, unsigned short dAddr): parIO(dAddr),
												wr(&parO::wrData),
												pin(dPin){	
	parIO::getPerm(1, addr);
}

parO::parO(ctrlPin cPin, unsigned short cAddr): parIO(cAddr),
												wr(&parO::wrCtrl),
												pin(cPin){	
	parIO::getPerm(1, ctrlAddr);
}

int parO::wrData(bool val){	
	const char out = val?(this->dataReg[this->addr] |= 
							(static_cast<int>((this->pin).dPin)))
						:(this->dataReg[this->addr] &=
						   ~(static_cast<int>((this->pin).dPin)));
	outb(out, addr);
	return 0;
}

int parO::wrCtrl(bool val){	
	const char out = val?(this->ctrlReg[this->addr] &= 
						   ~(static_cast<int>((this->pin).cPin)))
						:(this->ctrlReg[this->addr] |=
							(static_cast<int>((this->pin).cPin)));
	outb(out^0x04, ctrlAddr);
	return 0;
}

parI::parI(statPin sPin, unsigned short sAddr): parIO(sAddr),
												re(&parI::reStat),
												pin(sPin){	
	parIO::getPerm(1, addr + 1);
}

parI::parI(ctrlPin cPin, unsigned short cAddr): parIO(cAddr),
												re(&parI::reCtrl),
												pin(cPin){	
	parIO::getPerm(1, ctrlAddr);
}

bool parI::reStat(){	
	return ((inb(addr + 1)^0x80) >> 3) & static_cast<int>(pin.sPin);
}

bool parI::reCtrl(){	
	outb(0x0f^0x0b, ctrlAddr);
	return ((inb(ctrlAddr) & 0x0f) ^ 0x0b) & static_cast<int>(pin.cPin);
}
