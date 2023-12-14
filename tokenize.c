#include <stdio.h>
#include "tokens.h"

#include <assert.h>

#define ARG_CAP 255
int main(int argc, char **argv) {
  char from_cli[ARG_CAP];

  fgets(from_cli, ARG_CAP, stdin);


  char **retrieved_token = token_retrieve(from_cli);

  assert(retrieved_token != NULL);

  char **token_current = retrieved_token;
  while (*token_current != NULL) {
    printf("%s\n", *token_current);
    ++token_current;
  }

  token_release(retrieved_token);
  return 0;
}
