#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "token.h"

#define LINE_BUF_SIZE (1024)

static Token st_look_ahead_token;
static bool st_look_ahead_token_exists;

static void
get_cache_token(Token *token)
{
  if (st_look_ahead_token_exists)
  {
    *token = st_look_ahead_token;
    st_look_ahead_token_exists = false;
  }
  else
  {
    get_token(token);
  }
}

static void
free_cache_token(Token *token)
{
  st_look_ahead_token = *token;
  st_look_ahead_token_exists = true;
}

static double
parse_primary_expression()
{
  Token token;
  get_cache_token(&token);

  if (token.kind == NUMBER_TOKEN)
  {
    return token.value;
  }
  fprintf(stderr, "syntax error \n");
  exit(1);
  return 0;
}

static double
parse_term()
{
  double v1;
  double v2;
  Token token;
  v1 = parse_primary_expression();
  while (true)
  {
    get_cache_token(&token);

    if (token.kind != MUL_OPERATOR_TOKEN && token.kind != DIV_OPERATOR_TOKEN)
    {
      free_cache_token(&token);
      break;
    }

    v2 = parse_primary_expression();

    if (token.kind == MUL_OPERATOR_TOKEN)
    {
      v1 *= v2;
    }
    else if (token.kind == DIV_OPERATOR_TOKEN)
    {
      v1 /= v2;
    }
  }
  return v1;
}

static double
parse_expression()
{
  double v1;
  double v2;
  Token token;

  v1 = parse_term();

  while (true)
  {
    get_cache_token(&token);

    if (token.kind != ADD_OPERATOR_TOKEN && token.kind != SUB_OPERATOR_TOKEN)
    {
      free_cache_token(&token);
      break;
    }

    v2 = parse_term();

    if (token.kind == ADD_OPERATOR_TOKEN)
    {
      v1 += v2;
    }
    else if (token.kind == SUB_OPERATOR_TOKEN)
    {
      v1 -= v2;
    }
    else
    {
      free_cache_token(&token);
    }
  }
  return v1;
}

static double
parse_line()
{
  double value;
  st_look_ahead_token_exists = false;
  value = parse_expression();
  return value;
}

int main(int argc, char const *argv[])
{
  char line[LINE_BUF_SIZE];
  double value;

  while (fgets(line, LINE_BUF_SIZE, stdin) != NULL)
  {
    set_line(line);
    value = parse_line();
    printf(">> %f\n", value);
  }
  return 0;
}
