//Copyright GRIGORE MIHNEA - ANDREI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IZOLATE 200

// pixel RGB
typedef struct {
	int r, g, b, average;
} pixels;

// aloca matricea de pixeli
void pixels_alloc(int l, int h, pixels ***pixel);
// aloca orice matrice de tip int
void matrix_alloc(int l, int h, int ***temp);
// elibereaza memoria din matricea de pixeli
void pixels_free(int l, pixels **pixel);
// elibereaza memoria din matricea tip int
void matrix_free(int h, int **temp);
// citeste pixelii
int read_pixels(int l, int h, pixels **pixel);
// citeste comenzile si aplica functia corespunzatoare
int read_commands(int l, int h, int max_intensity, pixels **pixel);
// creeaza matricea grid
int **grid(int l, int h, pixels **pixel);
// modifica gridul in functie de valoarea de referinta
void tresholding(int l, int col, int **temp);
// aplica conturul corespunzator
void bits(int l, int col, int **temp, pixels **pixel, int contour[][16]);
// redimensioneaza matricea
void resize(int l, int col, pixels ***pixel);

int main(void)
{
	int read = 1;
	char command[10];
	scanf("%s", command);
	while (strcmp(command, "EXIT") && read) {
		if (!strcmp(command, "READ") && read == 1) {
			char type, color, buffer;
			int l, h, max_intensity;
			pixels **pixel = NULL;
			scanf("%c%c%c", &type, &color, &buffer);
			scanf("%d%d%d", &l, &h, &max_intensity);
			pixels_alloc(l, h, &pixel);
			if ((type == 'P' || color == 'P' || buffer == 'P') &&
				(type == '3' || color == '3' || buffer == '3')) {

				int status = read_pixels(l, h, pixel);
				if (!status) {
					pixels_free(l, pixel);
					return 0;
				}
				printf("Imagine citita cu succes [%d-%d]\n", l, h);
				read = read_commands(l, h, max_intensity, pixel);
			} else {
				printf("Eroare: trebuie sa fie P3\n");
				pixels_free(l, pixel);
				return 0;
			}
		}
	}
	return 0;
}

void pixels_alloc(int l, int h, pixels ***pixel)
{
	*pixel = (pixels **)calloc(l, sizeof(pixels *));
	if (!(*pixel)) {
		printf("Error on memory allocation for rows\n");
		return;
	}

	for (int i = 0; i < l; i++) {
		(*pixel)[i] = (pixels *)calloc(h, sizeof(pixels));
		if (!(*pixel)[i]) {
			printf("Error on memory allocation for columns at row %d\n", i);
			for (int k = 0; k < i; k++)
				free(pixel[k]);
			free(pixel);
			return;
		}
	}
}

void pixels_free(int l, pixels **pixel)
{
	for (int i = 0; i < l; i++)
		free(pixel[i]);
	free(pixel);
}

void matrix_free(int l, int **temp)
{
	for (int i = 0; i < l; i++)
		free(temp[i]);
	free(temp);
}

void matrix_alloc(int l, int h, int ***temp)
{
	*temp = (int **)calloc(l, sizeof(int *));
	if (!(*temp)) {
		printf("Error on memory calloc on rows\n");
		return;
	}
	for (int i = 0; i < l; i++) {
		(*temp)[i] = (int *)calloc(h, sizeof(int));
		if (!(*temp)[i]) {
			printf("Error on memory calloc on columns\n");
			return;
		}
	}
}

int read_pixels(int l, int h, pixels **pixel)
{
	for (int i = 0; i < l; i++)
		for (int j = 0; j < h; j++) {
			if (scanf("%d%d%d", &pixel[i][j].r,
					  &pixel[i][j].g, &pixel[i][j].b) == 3) {
				if (pixel[i][j].r > 255 || pixel[i][j].g > 255 ||
					pixel[i][j].b > 255) {
					printf("Eroare: valoare pixel necorespunzatoare\n");
					return 0;
				} pixel[i][j].average = (pixel[i][j].r
										+ pixel[i][j].g + pixel[i][j].b) / 3;
			} else {
				printf("Eroare: eroare citire pixeli\n");
				return 0;
			}
		}
	return 1;
}

int read_commands(int l, int h, int max_intensity, pixels **pixel)
{
	char command[10];
	int contour[16][16] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 0, 180, 255, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 0, 255, 180},
		{0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 180, 180, 180, 180},
		{0, 0, 255, 180, 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 255, 180, 0, 0, 0, 255, 255, 0, 0, 0, 180, 255, 0, 0},
		{0, 0, 255, 180, 0, 0, 255, 180, 0, 0,
		255, 180, 0, 0, 255, 180},
		{0, 255, 180, 180, 255, 180, 180, 180, 180,
		180, 180, 180, 180, 180, 180, 180},
		{180, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{180, 255, 0, 0, 180, 255, 0, 0, 180, 255, 0, 0, 180, 255, 0, 0},
		{180, 255, 0, 0, 255, 0, 0, 0, 0, 0, 0,
		255, 0, 0, 255, 180},
		{180, 180, 255, 0, 180, 180, 180, 255, 180,
		180, 180, 180, 180, 180, 180, 180},
		{180, 180, 180, 180, 255, 255, 255, 255,
		0, 0, 0, 0, 0, 0, 0, 0},
		{180, 180, 180, 180, 180, 180, 180, 180, 180,
		180, 180, 255, 180, 180, 255, 0},
		{180, 180, 180, 180, 180, 180, 180, 180, 255,
		180, 180, 180, 0, 255, 180, 180},
		{255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255}
	};
	scanf("%s", command);
	while (strcmp(command, "EXIT") && strcmp(command, "READ")) {
		if (!strcmp(command, "GRID")) {
			printf("Grid calculat cu succes [%d-%d]\n", l / 4, h / 4);
			int **grid_matrix = grid(l, h, pixel);
			tresholding(l / 4 + 1, h / 4 + 1, grid_matrix);
			for (int i = 0; i < l / 4; i++) {
				for (int j = 0; j < h / 4; j++)
					printf("%d ", grid_matrix[i][j]);
				printf("\n");
			}
			matrix_free(l / 4 + 1, grid_matrix);
		}
		if (!strcmp(command, "RESIZE")) {
			resize(l, h, &pixel);
			l *= 4;
			h *= 4;
			printf("Imagine redimensionata cu succes [%d-%d]\n", l, h);
		}
		if (!strcmp(command, "INIT_CONTUR"))
			for (int i = 0; i < 16; i++) {
				printf("P3\n4 4\n255\n");
				for (int j = 0; j < 16; j++)
					printf("%d %d %d\n", contour[i][j], contour[i][j],
						   contour[i][j]);
				}
		if (!strcmp(command, "MARCH")) {
			int **grid_march = grid(l, h, pixel);
			tresholding(l / 4 + 1, h / 4 + 1, grid_march);
			bits(l / 4, h / 4, grid_march, pixel, contour);
			printf("Marching Squares aplicat cu succes [%d-%d]\n", l, h);
			matrix_free(l / 4 + 1, grid_march);
		}
		if (!strcmp(command, "WRITE")) {
			printf("P3\n%d %d\n%d\n", l, h, max_intensity);
			for (int i = 0; i < l; i++) {
				for (int j = 0; j < h; j++)
					printf("%d %d %d\n", pixel[i][j].r, pixel[i][j].g,
						   pixel[i][j].b);
			}
		}
		scanf("%s", command);
	}
	pixels_free(l, pixel);
	if (!strcmp(command, "EXIT")) {
		printf("Gigel a terminat\n");
		return 0;
	}
	if (!strcmp(command, "READ"))
		return 1;
	return -1;
}

void resize(int l, int col, pixels ***pixel)
{
	pixels **new_pixel = malloc(l * 4 * sizeof(pixels *));
	if (!new_pixel) {
		printf("Error at malloc for new rows!\n");
		return;
	}

	for (int i = 0; i < l * 4; i++) {
		new_pixel[i] = malloc(col * 4 * sizeof(pixels));
		if (!new_pixel[i]) {
			printf("Error at malloc for new row %d!\n", i);
			for (int k = 0; k < i; k++) {
				free(new_pixel[k]);
			}
			free(new_pixel);
			return;
		}
	}

	for (int i = 0; i < l * 4; i++) {
		for (int j = 0; j < col * 4; j++) {
			new_pixel[i][j].r = (*pixel)[i / 4][j / 4].r;
			new_pixel[i][j].g = (*pixel)[i / 4][j / 4].g;
			new_pixel[i][j].b = (*pixel)[i / 4][j / 4].b;
			new_pixel[i][j].average = (*pixel)[i / 4][j / 4].average;
		}
	}

	for (int i = 0; i < l; i++) {
		free((*pixel)[i]);
	}
	free(*pixel);

	*pixel = new_pixel;
}

int **grid(int l, int h, pixels **pixel)
{
	int **temp = NULL, line = l / 4, col = h / 4;

	matrix_alloc(line + 1, col + 1, &temp);

	for (int i = 0; i < l; i += 4) {
		for (int j = 0; j < h; j += 4) {
			if (i == 0 && j == 0) { // colt stanga sus
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i + 1][j + 1].average) / 2;
			} else if (i == l - 1 && j == h - 1) { // colt dreapta jos
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i - 1][j - 1].average) / 2;
			} else if (i == 0 && j == col - 1) { // colt stanga jos
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i - 1][j + 1].average) / 2;
			} else if (i == l - 1 && j == 0) { // colt dreapta sus
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i + 1][j - 1].average) / 2;
			} else if (i == 0) { //prima linie
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i + 1][j + 1].average +
									  pixel[i + 1][j - 1].average) / 3;
			} else if (i == l - 1) { // ultima linie
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i - 1][j + 1].average +
									  pixel[i - 1][j - 1].average) / 3;
			} else if (j == 0) { // prima coloana
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i - 1][j + 1].average +
									  pixel[i + 1][j + 1].average) / 3;
			} else if (j == h - 1) { // ultima coloana
				temp[i / 4][j / 4] = (pixel[i][j].average +
									  pixel[i - 1][j].average +
									  pixel[i + 1][j - 1].average) / 3;
			} else { // interior
				temp[i / 4][j / 4] = (pixel[i][j].average
								+ pixel[i - 1][j - 1].average
								+ pixel[i - 1][j + 1].average
								+ pixel[i + 1][j - 1].average
								+ pixel[i + 1][j + 1].average) / 5;
			}
		}
	}

	temp[0][col] = pixel[0][h - 1].average;
	temp[line][0] = pixel[l - 1][0].average;
	for (int i = 4; i < l; i += 4) // coloana suplimentara
		temp[i / 4][col] = (pixel[i - 1][h - 1].average +
							pixel[i + 1][h - 1].average) / 2;
	for (int i = 4; i < h; i += 4) // linie suplimentara
		temp[line][i / 4] = (pixel[l - 1][i + 1].average +
							 pixel[l - 1][i - 1].average) / 2;
	temp[line][col] = pixel[l - 1][h - 1].average; //colt

	return temp;
}

void bits(int l, int col, int **temp, pixels **pixel, int contour[][16])
{
	for (int i = 0; i < l; i++)
		for (int j = 0; j < col; j++) {
			int binary = 0;
			if (temp[i][j] == 1)
				binary += 8;
			if (temp[i][j + 1] == 1)
				binary += 4;
			if (temp[i + 1][j + 1] == 1)
				binary += 2;
			if (temp[i + 1][j] == 1)
				binary += 1;
			for (int k = 0; k < 4; k++) //copiaza conturul corespunzator
				for (int t = 0; t < 4; t++) {
					pixel[i * 4 + k][j * 4 + t].r = contour[binary][k * 4 + t];
					pixel[i * 4 + k][j * 4 + t].g = contour[binary][k * 4 + t];
					pixel[i * 4 + k][j * 4 + t].b = contour[binary][k * 4 + t];
				}
		}
}

void tresholding(int l, int col, int **temp)
{
	for (int i = 0; i < l; i++)
		for (int j = 0; j < col; j++)
			if (temp[i][j] <= IZOLATE)
				temp[i][j] = 1;
			else
				temp[i][j] = 0;
}
