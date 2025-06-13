#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SENSORES 30      
#define MAX_LEITURAS_POR_SENSOR 20000  
#define TAMANHO_MAX_NOME 25 


// pesquisei no site https://www.w3schools.com/c/c_enums.php, acho que usar ENUMs foi mais facil.
//não sabia direito como seria a melhor forma de iniciar a aplicação, achei que ficou mais facil de organizar o codigo e eu poder entender melhor a propria aplicação. 
typedef enum {
    DADO_INTEIRO,
    DADO_DECIMAL,
    DADO_BOOLEANO,
    DADO_TEXTO
} TipoDoDado;

// usar union deu mais flexibilidade e creio que economizou linhas de codigo. 
//Pesquisei sobre nesse site: https://www.w3schools.com/c/c_unions.php. eu vi essa video aula: https://www.youtube.com/watch?v=67WyWjAL7Hg para ajudar tambem.
typedef union {
    int valor_inteiro;
    float valor_decimal;
    bool valor_booleano;
    char valor_texto[17];  
} ValorDoSensor;

typedef struct {
    long momento;
    ValorDoSensor valor;
} LeituraDoSensor;

typedef struct {
    char identificador[TAMANHO_MAX_NOME];
    TipoDoDado tipo_de_dado;
    LeituraDoSensor historico[MAX_LEITURAS_POR_SENSOR];
    int total_de_leituras;
} Sensor;

TipoDoDado descobrir_tipo(const char *valor) {
    // true/false 
    if (strcmp(valor, "true") == 0 || strcmp(valor, "false") == 0)
        return DADO_BOOLEANO;

    // se tiver ponto vai float
    if (strchr(valor, '.') != NULL) {
        char *rest;
        strtof(valor, &rest);
        if (*rest == '\0')
            return DADO_DECIMAL;
    }

    // tenta inteiro
    char *rest;
    strtol(valor, &rest, 10);
    if (*rest == '\0')
        return DADO_INTEIRO;

    // se não deu, é texto mesmo
    return DADO_TEXTO;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso certinho: %s arquivo.txt\n", argv[0]);
        return 1;
    }

    Sensor lista_de_sensores[MAX_SENSORES];
    int sensores_cadastrados = 0;

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        printf("Não abriu '%s' aqui\n", argv[1]);
        return 2;
    }

    long timestamp;
    char nome_do_sensor[TAMANHO_MAX_NOME];
    char valor_lido[50];

    printf("Abrindo %s...\n", argv[1]);


    while (fscanf(arquivo, "%ld %24s %49s", &timestamp, nome_do_sensor, valor_lido) == 3) {
        int idx = -1;
        for (int i = 0; i < sensores_cadastrados; i++) {
            if (strcmp(lista_de_sensores[i].identificador, nome_do_sensor) == 0) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            if (sensores_cadastrados >= MAX_SENSORES) {
                printf("Chegou no limite de sensores, pulando %s\n", nome_do_sensor);
                continue;
            }

            idx = sensores_cadastrados++;
            strncpy(lista_de_sensores[idx].identificador, nome_do_sensor, TAMANHO_MAX_NOME-1);
            lista_de_sensores[idx].identificador[TAMANHO_MAX_NOME-1] = '\0';
            lista_de_sensores[idx].total_de_leituras = 0;
            lista_de_sensores[idx].tipo_de_dado = descobrir_tipo(valor_lido);
        }

        if (lista_de_sensores[idx].total_de_leituras < MAX_LEITURAS_POR_SENSOR) {
            LeituraDoSensor *nova = &lista_de_sensores[idx].historico[lista_de_sensores[idx].total_de_leituras];
            nova->momento = timestamp;

            switch (lista_de_sensores[idx].tipo_de_dado) {
                case DADO_BOOLEANO:
                    nova->valor.valor_booleano = (strcmp(valor_lido, "true") == 0);
                    break;
                case DADO_INTEIRO:
                    nova->valor.valor_inteiro = atoi(valor_lido);
                    break;
                case DADO_DECIMAL:
                    nova->valor.valor_decimal = atof(valor_lido);
                    break;
                case DADO_TEXTO:
                    strncpy(nova->valor.valor_texto, valor_lido, 16);
                    nova->valor.valor_texto[16] = '\0';
                    break;
            }
            lista_de_sensores[idx].total_de_leituras++;
        } else {
            printf("Sensor %s cheio, ignore essa leitura\n", nome_do_sensor);
        }
    }
    fclose(arquivo);

    printf("Hora de arrumar a bagunça...\n");

    for (int i = 0; i < sensores_cadastrados; i++) {
        for (int j = 0; j < lista_de_sensores[i].total_de_leituras - 1; j++) {
            for (int k = 0; k < lista_de_sensores[i].total_de_leituras - j - 1; k++) {
                if (lista_de_sensores[i].historico[k].momento > lista_de_sensores[i].historico[k + 1].momento) {
                    LeituraDoSensor temp = lista_de_sensores[i].historico[k];
                    lista_de_sensores[i].historico[k] = lista_de_sensores[i].historico[k + 1];
                    lista_de_sensores[i].historico[k + 1] = temp;
                }
            }
        }

        char nome_arquivo[50];
        snprintf(nome_arquivo, 50, "sensor_%s_ordenado.txt", lista_de_sensores[i].identificador);

        FILE *saida = fopen(nome_arquivo, "w");
        if (!saida) {
            printf("Falhou criar %s\n", nome_arquivo);
            continue;
        }

        printf("Salvando %s...\n", lista_de_sensores[i].identificador);

        for (int j = 0; j < lista_de_sensores[i].total_de_leituras; j++) {
            LeituraDoSensor *leitura = &lista_de_sensores[i].historico[j];
            switch (lista_de_sensores[i].tipo_de_dado) {
                case DADO_BOOLEANO:
                    fprintf(saida, "%ld %s\n", leitura->momento, leitura->valor.valor_booleano ? "true" : "false");
                    break;
                case DADO_INTEIRO:
                    fprintf(saida, "%ld %d\n", leitura->momento, leitura->valor.valor_inteiro);
                    break;
                case DADO_DECIMAL:
                    fprintf(saida, "%ld %.4f\n", leitura->momento, leitura->valor.valor_decimal);
                    break;
                case DADO_TEXTO:
                    fprintf(saida, "%ld %s\n", leitura->momento, leitura->valor.valor_texto);
                    break;
            }
        }
        fclose(saida);
    }

    printf("Terminou, %d sensores processados.\n", sensores_cadastrados);
    return 0;
}
