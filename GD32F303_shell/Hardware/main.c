#include "gd32f30x.h"
#include "led.h"
#include "systick.h"
#include "timer.h"
#include <stdio.h>
#include "key.h"
#include "gpio.h"
#include "usart_shell.h"
#include "shell_port.h"

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
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func, func, test2);
//导出到命令列表里
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test, test, test);

/* USER CODE END 0 */

int main(void)
{

    delay_init();
	usart0_shell();
	userShellInit();
    while(1){

    }
}
