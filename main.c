
#include <stdio.h>
#include <string.h>
#include "MainConsuleMode.h"
#include "MainGuiMode.h"


int main(int argc, char** argv) {
	if (argc>2)
		return 0;
	if (argc ==1)
		ConsuleMode();
	if ( strcmp(argv[1],"-g") == 0)
		GuiMode();
	else
		ConsuleMode();


	return 0;
}

