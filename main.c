#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    double speed;
    double fractions[15];
    int fractions_count;
} CalculatorAntenna;

void getLength(double rawValue, char *result) {
    double resultValue = rawValue;
    char resultUnit[4];
    
    if (resultValue > 1000) {
        resultValue /= 1000;
        strcpy(resultUnit, "km");
    } else if (resultValue < 0.01) {
        resultValue *= 1000;
        strcpy(resultUnit, "mm");
    } else if (resultValue < 1) {
        resultValue *= 100;
        strcpy(resultUnit, "cm");
    } else
        strcpy(resultUnit, "m");

    sprintf(result, "%.2f %s", resultValue, resultUnit);

    char *dot_pos = strchr(result, '.');
    if (dot_pos != NULL)
        *dot_pos = ',';
}

int main (int argc, char *argv[]) {
    char output[4096];

	system("Color 0A");

    printf("Antenna length calculator by 13MAD86\n\n");

    if (argc < 3) {
        printf("- Usage: AntDXCalc [frequency] [factor]\n");
        return 0;
    }

    CalculatorAntenna* calc = (CalculatorAntenna*)malloc(sizeof(CalculatorAntenna));
    if (calc == NULL) {
        printf("Failed to create calculator\n");
        return 0;
    }
    
    calc->speed = 299792458;

    double temp_fractions[] = {128, 64, 32, 16, 8, 4, 2, 1, 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125};
    calc->fractions_count = 15;
    
    for (int i = 0; i < calc->fractions_count; i++)
        calc->fractions[i] = temp_fractions[i];

    printf("To determine the optimal length of a required antenna, first calculate the wavelength (L):\n\n");
    printf("(C) = The speed of light (299792458 m/s)\n");
    printf("(F) = The frequency of the radio module used (for example 433 MHz = 433000000 Hz)\n");
    printf("(L) = (C) / (F)\n\n");

    double freq = atof(argv[1]) * atof(argv[2]);
    if (isnan(freq) || freq <= 0)
        strcpy(output, "-----");
    else {
        double lambda = calc->speed / freq;
        char result[4096] = "\n- Antenna lengths:\n\nFractions of the wavelength can then be used as a measure for an antenna:\n\n";
        bool insane = true;

        for (int i = 0; i < calc->fractions_count; i++) {
            double fr = calc->fractions[i];
            double len = lambda * fr;
            
            if (len < 10.0 && len > 0.01) {
                insane = false;
                char lengthStr[64];
                getLength(len, lengthStr);
                
                char row[256];
                sprintf(row, "(L) x %.8g = %s\n", fr, lengthStr);
                strcat(result, row);
            }
        }

        if (insane)
            strcpy(output, "(Antenna too big/small)");
        else
            strcpy(output, result);
    }

    printf("Frequency: %s / Factor: %s\n%s\n\n", argv[1], argv[2], output);
	printf("Lambda: (L) = %.2lf\n\n", (calc->speed / 1000000) / atof(argv[1]));

    if (calc != NULL)
        free(calc);

	return 0;
}
