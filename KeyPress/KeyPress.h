#ifndef __KEYPRESS__
#define	__KEYPRESS__

#include <sys/ioctl.h>

void SetupConsole();
void RestoreConsole();
int GetKey();
int getTerminalHeight();
int getTerminalWidth();

#endif	/* __KEYPRESS__ */

