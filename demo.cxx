#include "parIO.h"
#include <stdio.h>

#include <sys/sysinfo.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string>
#include <iomanip>
#include <array>
#include <thread>
#include <sstream>

parI iAck(parI::ACK);
parO oD3(parO::D3);

int main(void){	
	int rt = iAck;
	oD3 = 1;
	return 0;
}
