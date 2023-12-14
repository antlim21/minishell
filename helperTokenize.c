
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define CAP_STEP 256

static int space_token;
static int cap_tok; // initial capacity (should be enough for most applications)
static char **tokens = NULL;

static int read_string(const char *input, char *buf);
static void init_tokens();
static void add_token(const char *token);

extern char *strdup(const char *);

char **token_retrieve(const char *input) {
  char buf[256];
  unsigned int i = 0;
  unsigned int b = 0;

  init_tokens();

  while (input[i]) {
    if (isspace(input[i])) {
      if (b > 0) {
        buf[b] = '\0';
        add_token(buf);
        b = 0;
      }
    } else if (strchr("()&><|;", input[i])) {
      if (b > 0) {
        buf[b] = '\0';
        add_token(buf);
        b = 0;
      }
      buf[0] = input[i];
      buf[1] = '\0';
      add_token(buf);
    } else if (input[i] == '"') {
      i++; // skip the quote
      b += read_string(&input[i], &buf[b]);
      i += b;
    } else {
      buf[b++] = input[i];
    }
    i++;
  }

  if (b > 0) {
    buf[b] = '\0';
    add_token(buf);
  }

  return tokens;
}

void token_release(char **tokens) {
  char **iter = tokens;

  while (*iter != NULL) {
    free(*iter);
    ++iter;
  }

  free(tokens);
}

int read_string(const char *input, char *buf) {
  unsigned int space = 0;
  while (*input && *input != '"') {
    *buf = *input;
    ++buf;
    ++input;
    ++space;
  }
  *buf = '\0';

  return space;
}

void init_tokens() {
  space_token = 0;
  cap_tok = CAP_STEP;
  tokens = malloc(sizeof(char *) * cap_tok);
  assert(tokens != NULL);
  tokens[0] = NULL;
}

void add_token(const char* token) {

  int at_cap = (space_token + 1 == cap_tok);
  if (at_cap) {
    
    cap_tok = cap_tok + CAP_STEP;
    tokens = realloc(tokens, sizeof(char *) * cap_tok);
    assert(tokens != NULL);
  }
  char *new = strdup(token);
  tokens[space_token] = new;
  ++space_token;
  tokens[space_token] = NULL;
}
