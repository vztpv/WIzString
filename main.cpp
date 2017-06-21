
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

#include "WizString.h"


int main(void)
{
	wiz::WizString test;
	FILE* file = fopen("test.txt", "rt");

	for (int i = 0; i < 10; ++i) {
		if (!feof(file)) {
			std::cout << i << std::endl;
			wiz::WizString::GetMultiLine(file, &test, 3);
			std::cout << "//" << test.CStr() << "//" << std::endl;
		}
		else {
			break;
		}
		
	}

	fclose(file);
	return 0;
}
