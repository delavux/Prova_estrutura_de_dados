#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SENSORES 10
#define TAM_NOME 20
#define TAM_STR 17

time_t cria_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm tm_temp = {0};
    tm_temp.tm_year = ano - 1900;
    tm_temp.tm_mon = mes - 1;
    tm_temp.tm_mday = dia;
    tm_temp.tm_hour = hora;
    tm_temp.tm_min = min;
    tm_temp.tm_sec = seg;
    tm_temp.tm_isdst = -1;
    return mktime(&tm_temp);
}

time_t timestamp_aleatorio(time_t inicio, time_t fim) {
    return inicio + rand() % (fim - inicio + 1);
}

void gera_string(char *str, int tamanho) {
    int comprimento = 1 + rand() % (tamanho - 1);
    for (int i = 0; i < comprimento; i++) {
        str[i] = 'A' + rand() % 26;
    }
    str[comprimento] = '\0';
}

int main() {
    srand(time(NULL));
    
    int dia_inicio, mes_inicio, ano_inicio, hora_inicio, min_inicio, seg_inicio;
    int dia_fim, mes_fim, ano_fim, hora_fim, min_fim, seg_fim;
    int num_sensores;

    printf("Data/hora inicial (dd mm aaaa HH MM SS): ");
    scanf("%d %d %d %d %d %d", &dia_inicio, &mes_inicio, &ano_inicio, &hora_inicio, &min_inicio, &seg_inicio);
    
    printf("Data/hora final (dd mm aaaa HH MM SS): ");
    scanf("%d %d %d %d %d %d", &dia_fim, &mes_fim, &ano_fim, &hora_fim, &min_fim, &seg_fim);

    time_t inicio = cria_timestamp(dia_inicio, mes_inicio, ano_inicio, hora_inicio, min_inicio, seg_inicio);
    time_t fim = cria_timestamp(dia_fim, mes_fim, ano_fim, hora_fim, min_fim, seg_fim);
    
    if (fim <= inicio) {
        printf("Erro: Data final deve ser depois da inicial\n");
        return 1;
    }

    printf("Quantidade de sensores (max %d): ", MAX_SENSORES);
    scanf("%d", &num_sensores);
    
    if (num_sensores < 1 || num_sensores > MAX_SENSORES) {
        printf("Erro: Numero de sensores invalido\n");
        return 1;
    }

    char nomes_sensores[MAX_SENSORES][TAM_NOME];
    char tipos_sensores[MAX_SENSORES];

    for (int i = 0; i < num_sensores; i++) {
        printf("Nome do sensor %d: ", i+1);
        scanf("%s", nomes_sensores[i]);
        
        printf("Tipo do sensor %d (i=inteiro, b=booleano, f=float, s=string): ", i+1);
        scanf(" %c", &tipos_sensores[i]);
        
        if (tipos_sensores[i] != 'i' && tipos_sensores[i] != 'b' && tipos_sensores[i] != 'f' && tipos_sensores[i] != 's') {
            printf("Erro: Tipo de sensor invalido\n");
            return 1;
        }
    }

    FILE *arquivo = fopen("dados_sensores.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar arquivo\n");
        return 1;
    }

    for (int i = 0; i < num_sensores; i++) {
        for (int j = 0; j < 2000; j++) {
            time_t ts = timestamp_aleatorio(inicio, fim);

            switch(tipos_sensores[i]) {
                case 'i': {
                    int valor = rand() % 1000;
                    fprintf(arquivo, "%ld %s %d\n", ts, nomes_sensores[i], valor);
                    break;
                }
                case 'b': {
                    int valor = rand() % 2;
                    fprintf(arquivo, "%ld %s %s\n", ts, nomes_sensores[i], valor ? "true" : "false");
                    break;
                }
                case 'f': {
                    float valor = ((float)rand() / RAND_MAX) * 100.0f;
                    fprintf(arquivo, "%ld %s %.2f\n", ts, nomes_sensores[i], valor);
                    break;
                }
                case 's': {
                    char valor[TAM_STR];
                    gera_string(valor, TAM_STR);
                    fprintf(arquivo, "%ld %s %s\n", ts, nomes_sensores[i], valor);
                    break;
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo gerado: dados_sensores.txt\n");

    return 0;
}