#include "gd32f30x.h"
#include "led.h"
#include "systick.h"
#include "timer.h"
#include "key.h"
#include "gpio.h"
#include "usart_shell.h"
#include "shell_port.h"

#include "jsmn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
    "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
    "\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}
/* USER CODE BEGIN 0 */
int test(int i, char ch, char *str)
{
    printf("input int: %d, char: %c, string: %s\r\n", i, ch, str);
	
	return 0;
}
int func(int argc, char *agrv[])
{
    printf("%dparameter(s)\r\n", argc);
    for (char i = 1; i < argc; i++)
    {
        printf("%s\r\n", agrv[i]);
    }
	return 0;
}
int jsmn_test(void)
{
  int i;
  int r;
  jsmn_parser p;
  jsmntok_t t[128]; /* We expect no more than 128 tokens */

  jsmn_init(&p);
  r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
                 sizeof(t) / sizeof(t[0]));
  if (r < 0) {
    printf("Failed to parse JSON: %d\n", r);
    return 1;
  }

  /* Assume the top-level element is an object */
  if (r < 1 || t[0].type != JSMN_OBJECT) {
    printf("Object expected\n");
    return 1;
  }

  /* Loop over all keys of the root object */
  for (i = 1; i < r; i++) {
    if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
      /* We may use strndup() to fetch string value */
      printf("- User: %.*s\n", t[i + 1].end - t[i + 1].start,
             JSON_STRING + t[i + 1].start);
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
      /* We may additionally check if the value is either "true" or "false" */
      printf("- Admin: %.*s\n", t[i + 1].end - t[i + 1].start,
             JSON_STRING + t[i + 1].start);
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
      /* We may want to do strtol() here to get numeric value */
      printf("- UID: %.*s\n", t[i + 1].end - t[i + 1].start,
             JSON_STRING + t[i + 1].start);
      i++;
    } else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
      int j;
      printf("- Groups:\n");
      if (t[i + 1].type != JSMN_ARRAY) {
        continue; /* We expect groups to be an array of strings */
      }
      for (j = 0; j < t[i + 1].size; j++) {
        jsmntok_t *g = &t[i + j + 2];
        printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
      }
      i += t[i + 1].size + 1;
    } else {
      printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
             JSON_STRING + t[i].start);
    }
  }
  return EXIT_SUCCESS;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func, func, test2);
//导出到命令列表里
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test, test, test);
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), jsmn_test, jsmn_test, jsmn_test);
/* USER CODE END 0 */

int main(void)
{
    delay_init();
	usart0_shell();
	userShellInit();

    while(1){

    }
}
