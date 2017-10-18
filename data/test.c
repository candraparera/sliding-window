#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char* msg = "jauhararifin";

int main() {
	srand(time(0));
	int l = (int) strlen(msg);
	int n; scanf("%d", &n);
	int i = 0;
	for (i = 0; i < n; i++)
		printf("%c", msg[i%l]);
	return 0;
}
