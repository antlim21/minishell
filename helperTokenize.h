#ifndef _HELPERTOKENIZE_H
#define _HELPERTOKENIZE_H

//gets the input in string format from command line and puts them as tokens
char **token_retrieve(const char *input);

//frees the tokens
void token_release(char **tokens);

#endif 
