#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long timestamp;
    float valor;
} Leitura;

// Busca binária para achar leitura com timestamp mais próximo
//aqui não mudou muita coisa, muito parecido com a primeira prova
int buscar_mais_proximo(Leitura *leituras, int count, long alvo) {
    int esquerda = 0, direita = count - 1;
    int mais_proximo = 0;
    long menor_diff = labs(leituras[0].timestamp - alvo);

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        long diff = leituras[meio].timestamp - alvo;

        if (labs(diff) < menor_diff) {
            menor_diff = labs(diff);
            mais_proximo = meio;
        }

        if (diff < 0) {
            esquerda = meio + 1;
        } else if (diff > 0) {
            direita = meio - 1;
        } else {
            return meio;
        }
    }
    return mais_proximo;
}

int main() {
    char sensor[20];
    long timestamp;

    printf("Digite o nome do sensor: ");
    scanf("%19s", sensor);

    printf("Digite o timestamp desejado: ");
    scanf("%ld", &timestamp);

    
    char nome_arquivo[50];
    sprintf(nome_arquivo, "sensor_%s_ordenado.txt", sensor);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Sensor não encontrado! Arquivo '%s' não existe.\n", nome_arquivo);
        return 1;
    }

    Leitura leituras[10000];
    int count = 0;

    while (fscanf(arquivo, "%ld %f", &leituras[count].timestamp, &leituras[count].valor) == 2) {
        count++;
        if (count >= 10000) break; 
    }
    fclose(arquivo);

    int pos = buscar_mais_proximo(leituras, count, timestamp);

    printf("\nLeitura mais próxima:\n");
    printf("Timestamp: %ld\n", leituras[pos].timestamp);
    printf("Valor: %.2f\n", leituras[pos].valor);

    return 0;
}
