#include "or.h"

int main(int argc, char *argv[])
{
  /* START VARIABLES */
  unsigned char onion_plain[268]; /* test onion */
  /* onion_passx = ciphertext after x passes */
  unsigned char onion_pass1[268];
  unsigned char onion_pass2[268];
  unsigned char onion_pass3[268];
  unsigned char onion_pass4[268];
  unsigned char onion_pass5[268];
  unsigned char onion_pass6[268];
  
  /* RSA keys for the six layers */
  crypto_pk_env_t *key1 = crypto_new_pk_env(CRYPTO_PK_RSA);
  crypto_pk_env_t *key2 = crypto_new_pk_env(CRYPTO_PK_RSA);
  crypto_pk_env_t *key3 = crypto_new_pk_env(CRYPTO_PK_RSA);
  crypto_pk_env_t *key4 = crypto_new_pk_env(CRYPTO_PK_RSA);
  crypto_pk_env_t *key5 = crypto_new_pk_env(CRYPTO_PK_RSA);
  crypto_pk_env_t *key6 = crypto_new_pk_env(CRYPTO_PK_RSA);

  
  /* END VARIABLES */

  crypto_global_init();
  printf("onion.c test suite ...\n");
  
  printf("\nGenerating 6 RSA keys ...\n");
  crypto_pk_generate_key(key1);
  crypto_pk_generate_key(key2);
  crypto_pk_generate_key(key3);
  crypto_pk_generate_key(key4);
  crypto_pk_generate_key(key5);
  crypto_pk_generate_key(key6);
  
  printf("\ndone.\n");
  
  printf("\nGenerating onion ...\n");
  memset((void *)onion_plain, 1, 28);
  memset((void *)(onion_plain+28), 2, 28);
  memset((void *)(onion_plain+56), 3, 28);
  memset((void *)(onion_plain+84), 4, 28);
  memset((void *)(onion_plain+112), 5, 28);
  memset((void *)(onion_plain+140), 6, 28);
  memset((void *)(onion_plain+168), 9, 100);
  
  printf("\ndone.\n");
  
  printf("\nEncrypting first layer ...\n");
  memcpy((void *)onion_pass1, (void *)onion_plain, 268);
  if (!encrypt_onion((onion_layer_t *)(onion_pass1+140), 128, key1)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  
  printf("\nEncrypting second layer ...\n");
  memcpy((void *)onion_pass2, (void *)onion_pass1, 268);
  if (!encrypt_onion((onion_layer_t *)(onion_pass2+112), 156, key2)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  
  printf("\nEncrypting third layer ...\n");
  memcpy((void *)onion_pass3, (void *)onion_pass2, 268);
  if (!encrypt_onion((onion_layer_t *)(onion_pass3+84), 184, key3)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");

  printf("\nEncrypting fourth layer ...\n");
  memcpy((void *)onion_pass4, (void *)onion_pass3, 268);
  if (!encrypt_onion((onion_layer_t *)(onion_pass4+56), 212, key4)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  
  printf("\nEncrypting fifth layer ...\n");
  memcpy((void *)onion_pass5, (void *)onion_pass4, 268);
  if (!encrypt_onion((onion_layer_t *)(onion_pass5+28), 240, key5)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  
  printf("\nEncrypting sixth layer ...\n");
  memcpy((void *)onion_pass6, (void *)onion_pass5, 268);
  if (!encrypt_onion((onion_layer_t *)onion_pass6, 268, key6)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  
    printf("\nDecrypting sixth layer ...\n");
    if (!decrypt_onion((onion_layer_t *)onion_pass6, 268, key6)) {
          printf("\nFailed!\n\n");
          exit(1);
    }
    printf("\ndone.\n");
    printf("\nChecking validity ...\n");
    if (memcmp((void *)onion_pass6, (void *)onion_pass5, 268)) {
          printf("\nTEST FAILED!\n\n");
          exit(1);
    }
    printf("\nTEST PASSED.\n");
  
    printf("\nDecrypting fifth layer ...\n");
    if (!decrypt_onion((onion_layer_t *)(onion_pass5+28), 240, key5)) {
          printf("\nFailed!\n\n");
          exit(1);
    }
    printf("\ndone.\n");
    printf("\nChecking validity ...\n");
    if (memcmp((void *)onion_pass5, (void *)onion_pass4, 268)) {
          printf("\nTEST FAILED!\n\n");
          exit(1);
    }
    printf("\nTEST PASSED.\n");
  
  printf("\nDecrypting fourth layer ...\n");
  if (!decrypt_onion((onion_layer_t *)(onion_pass4+56), 212, key4)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  printf("\nChecking validity ...\n");
  if (memcmp((void *)onion_pass4, (void *)onion_pass3, 268)) {
    printf("\nTEST FAILED!\n\n");
    exit(1);
  }
  printf("\nTEST PASSED.\n");
  
  printf("\nDecrypting third layer ...\n");
  if (!decrypt_onion((onion_layer_t *)(onion_pass3+84), 184, key3)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  printf("\nChecking validity ...\n");
  if (memcmp((void *)(onion_pass3), (void *)(onion_pass2), 268)) {
    printf("\nTEST FAILED!\n\n");
    exit(1);
  }
  printf("\nTEST PASSED.\n");

  printf("\nDecrypting second layer ...\n");
  if (!decrypt_onion((onion_layer_t *)(onion_pass2+112), 156, key2)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  printf("\nChecking validity ...\n");
  if (memcmp((void *)(onion_pass1), (void *)(onion_pass2), 268)) {
    printf("\nTEST FAILED!\n\n");
    exit(1);
  }
  printf("\nTEST PASSED.\n");

  printf("\nDecrypting first layer ...\n");
  if (!decrypt_onion((onion_layer_t *)(onion_pass1+140), 128, key1)) {
    printf("\nFailed!\n\n");
    exit(1);
  }
  printf("\ndone.\n");
  printf("\nChecking validity ...\n");
  if (memcmp((void *)(onion_plain), (void *)(onion_pass1), 268)) {
    printf("\nTEST FAILED!\n\n");
    exit(1);
  }
  printf("\nTEST PASSED.\n");
  crypto_global_cleanup();
  return 0;
}
