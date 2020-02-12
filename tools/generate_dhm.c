#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mbedtls/bignum.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#pragma comment(lib, "mbedTLS.lib")

// cl generate_dhm.c -I ..\mbedtls-2.4.0\include /MDd /link ..\vc\Debug\mbedTLS.lib Advapi32.lib

static void print_mpi(mbedtls_mpi *number) {
    uint8_t buffer[64];
    mbedtls_mpi_write_binary(number, buffer, 64);

    for (int i = 0; i < 64; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
}

static void fwrite_mpi(FILE *file, mbedtls_mpi *number, int nbytes)
{
    char *buffer = malloc(nbytes);
    mbedtls_mpi_write_binary(number, buffer, nbytes);
    if (fwrite(buffer, nbytes, 1, file) != 1) {
        printf("failed in 'fwrite_mpi'\n");
        exit(1);
    }
}

char *strcat2(const char *s1, const char *s2)
{
    size_t s1_len = strlen(s1);
    size_t s2_len = strlen(s2);

    char *res = malloc(s1_len + s2_len +1);
    strcpy(res, s1);
    strcpy(res + s1_len, s2);

    return res;
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("Set the file output path in parameters\n");
        return 1;
    }

    const char secret[] = "Wyatt";

    mbedtls_entropy_context  entropy;
    mbedtls_ctr_drbg_context ctr_drbg;

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
        (uint8_t*)secret, sizeof(secret) - 1);

    const char *path = argv[1];

    {
        FILE *file = fopen(path, "wb");

        mbedtls_mpi G = {0};
        mbedtls_mpi_read_binary(&G, "\x00\x00\x00\x04", 4);
        fwrite_mpi(file, &G, 4);

        mbedtls_mpi P = {0};
        mbedtls_mpi_gen_prime(&P, 512, 0, mbedtls_ctr_drbg_random, &ctr_drbg);
        fwrite_mpi(file, &P, 64);

        mbedtls_mpi S = {0};
        mbedtls_mpi_fill_random(&S, 64, mbedtls_ctr_drbg_random, &ctr_drbg);
        fwrite_mpi(file, &S, 64);

        fclose(file);
        file = fopen(strcat2(path, ".pub"), "wb");

        mbedtls_mpi pub = {0};
        mbedtls_mpi_exp_mod(&pub, &G, &S, &P, NULL);
        fwrite_mpi(file, &G, 4);
        fwrite_mpi(file, &P, 64);
        fwrite_mpi(file, &pub, 64);

        fclose(file);
    }

    return 0;
}
