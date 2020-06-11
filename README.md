# Mbed-GPIO-Style Parallel Port Driver
Control parallel port signals separately with mbed-like API.

```
	#include "parIO.h"
	parO d0(parO::D0);
	parI pe(parI::PE);
	d0 = 0;
	int peVal = pe;
```

Parallel port signals name:
![lpt signals](https://hw-server.com/files/lpt/images/dsub25_signal3.gif)

Please also refer to signal enumerates in the header.
