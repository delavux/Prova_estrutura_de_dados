#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

time_t gerar_timestamp_aleatorio(int dia, int mes, int ano) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_isdst = -1;
    
    time_t inicio = mktime(&t);
    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;
    time_t fim = mktime(&t);
    
    return inicio + rand() % (fim - inicio + 1);
}

int main() {
    srand(time(NULL));
    int dia, mes, ano;
    
    printf("Digite a data (dd mm aaaa): ");
    scanf("%d %d %d", &dia, &mes, &ano);
    
    const char *sensores[] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"};
    FILE *saida = fopen("dados_teste.txt", "w");
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1000; j++) {
            time_t ts = gerar_timestamp_aleatorio(dia, mes, ano);
            float valor = (float)rand() / RAND_MAX * 100.0f;
            fprintf(saida, "%ld %s %.2f\n", ts, sensores[i], valor);
        }
    }
    
    fclose(saida);
    printf("Arquivo de teste feito com sucesso\n");
    return 0;
}