/**********************************************************************
 * Copyright (c) 2020
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "types.h"

#define MAX_NR_TOKENS 32	/* Maximum number of tokens in a command */
#define MAX_TOKEN_LEN 64	/* Maximum length of single token */
#define MAX_COMMAND	256		/* Maximum length of command string */

/***********************************************************************
 * parse_command
 *
 * DESCRIPTION
 *	Parse @command, put each command token into @tokens[], and 
 *	set @nr_tokens with the number of tokens.
 *
 * A command token is defined as a string without any whitespace (i.e., *space*
 * and *tab* in this programming assignment). Suppose @command as follow;
 *
 *   @command = "  Hello world   Ajou   University!!  "
 *
 * Then, @nr_tokens = 4 and @tokens should be
 *
 *   tokens[0] = "Hello"
 *   tokens[1] = "world"
 *   tokens[2] = "Ajou"
 *   tokens[3] = "University!!"
 *
 * Another exmaple is;
 *   command = "ls  -al   /home/operating_system /hw0  "
 *
 * then, nr_tokens = 4, and tokens is
 *   tokens[0] = "ls"
 *   tokens[1] = "-al"
 *   tokens[2] = "/home/operating_system"
 *   tokens[3] = "/hw0"
 *
 *
 * @command can be expressed with double quotation mark(") to quote a string
 * that contains whitespaces. Suppose following @command;
 *
 *   @command = "  We will get over with the "COVID 19" virus   "
 *
 * Then, @nr_tokens = 8, and @tokens are
 *
 *   tokens[0] = "We"
 *   tokens[1] = "will"
 *   tokens[2] = "get"
 *   tokens[3] = "over"
 *   tokens[4] = "with"
 *   tokens[5] = "the"
 *   tokens[6] = "COVID 19"
 *   tokens[7] = "virus"
 *
 * Note that tokens[6] does not contain the quotation marks.
 * Also, one @command can have multiple quoted strings, but they will not be
 * nested
 * (i.e., quote another string in a quote)
 *
 *   This "is a possible" case for a command --> This, is a possible, case, for,
 *	                                             a, command
 *   "This " is "what I told you" --> This, is, what I told you
 *
 * RETURN VALUE
 *	Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static int parse_command(char *command, int *nr_tokens, char *tokens[])
{
	int i = 0;
	int flag = 0;
	char * tokenptr;
	char * command_ptr;
	command_ptr = command;
	while(1){ //한 커맨드 끝날때 까지 돌림
		tokenptr = (char *)malloc(MAX_TOKEN_LEN+1);
		if(*command_ptr=='\0')
			break;
		while(1){ // 토큰 하나 끝날때까지 돌림
			if(flag==1){ //더블 쿼터 발견 상태
				if(*command_ptr=='"'){//더블쿼터 만날때까지 계속 추가 
					flag=0;//더블 쿼터 미발견 상태
					tokenptr[i]='\0';
					tokens[(*nr_tokens)++]=tokenptr;//토큰 개수 추가
					i=0;
					command_ptr++;
					break;
				}
				else{//더블쿼터 발견상태에서 더블쿼터 이외의 문자는 모조리 추가
					tokenptr[i++]=*command_ptr++;
				}
			}
			else if(flag==2){//공백이 앞에서 발견되었을 때
				if(isspace(*command_ptr)){ //또 공백이 나옴
					if(i>0){//일반 문자가 여러개 추가된 다음에 공백이 또 발견
						command_ptr++;
						tokenptr[i]='\0';
						tokens[(*nr_tokens)++]=tokenptr;//토큰 개수 추가
						i=0;
						break;
					}
					else if(i==0){ //중복되는 공백은 버린다.
						*command_ptr++;
					}
				}
				else{ //일반 문자가 나옴
					tokenptr[i++]=*command_ptr++;
				}
			}
			else{ //공백을 발견한 적도 더블쿼터도 발견한 적 없는 상태
				if(isspace(*command_ptr)){//공백나온 상태로 변환
					flag=2;
				}
				else if((*command_ptr)=='"'){//쿼터나온 상태로 변환
					flag=1;
				}
				command_ptr++;
			}
		}
	}
	return 0;
}


/***********************************************************************
 * The main function of this program.
 * SHOULD NOT CHANGE THE CODE BELOW THIS LINE
 */
int main(int argc, char *argv[])
{
	char line[MAX_COMMAND] = { '\0' };
	FILE *input = stdin;

	if (argc == 2) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[1]);
			return -EINVAL;
		}
	}

	while (fgets(line, sizeof(line), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens= 0;

		parse_command(line, &nr_tokens, tokens);

		fprintf(stderr, "nr_tokens = %d\n", nr_tokens);
		for (int i = 0; i < nr_tokens; i++) {
			fprintf(stderr, "tokens[%d] = %s\n", i, tokens[i]);
		}
		printf("\n");
	}

	if (input != stdin) fclose(input);

	return 0;
}
