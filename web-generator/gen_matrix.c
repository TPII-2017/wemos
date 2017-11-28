#include <stdio.h>
#include <stdlib.h>

/* 1er param = filas, 2do param = columnas */
int main(int argc, const char* argv[])
{
	if (argc != 3) {
		printf("Solo 2 argumentos.\n");
		printf("- 1er arg fila, 2do arg columna.\n");
		return -1;
	}
	int c;
	int r;
	const char *item = "<input type='checkbox' value=1 name='%d-%d'>";
	const char *newline = "<br>\n";
	int row = atoi(argv[1]);
	int colum = atoi(argv[2]);

	printf("fila = %d\n", row);
	printf("colum = %d\n", colum);

	FILE *f = fopen("matrix.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for(r=0; r<row; r++)
	{
		for(c=0; c<colum; c++)
			fprintf(f, item, r, c);
		fprintf(f, "%s", newline);
	}
	fclose(f);

	return 0;
}