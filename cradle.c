#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFF_SIZE 64

char Look = 0;

void GetChar()
{
	Look = getc(stdin);
}

void SkipWhite()
{
	while(IsWhite(Look)) GetChar();
}
void Error(char *string)
{
	printf("\n\aERROR: %s.\n", string);
}

void Abort(char *string)
{
	Error(string);
	exit(1);
}

void Match(char c)
{
	if (Look == c) {
		GetChar();
		SkipWhite();
	}
	else {
		char string[] = "\'X\' Expected";
		string[1] = c;
		Abort(string);
	}
}

bool IsAlpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool IsDigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool IsAlNum(char c)
{
	return (IsAlpha(c) || IsDigit(c));
}

bool IsWhite(char c)
{
	return ((c == ' ') || (c == '\t'));
}

bool IsAddop(char c)
{
	return (c == '+' || c == '-');
}

char *GetName()
{
	char *Token =  malloc(sizeof(char) * BUFF_SIZE);
	char buff = Look;
	int position = 0;
	int buffsize = BUFF_SIZE;
	if(!IsAlpha(Look)) Abort("Name Expected");
	while(IsAlNum(Look)){
		Token[position] = Look;
		position++;
		GetChar();
		if (position >= buffsize) {
      			buffsize += BUFF_SIZE;
      			Token = (char*)realloc(Token, buffsize);
      			if (!Token) {
        			Abort("Couldn't reallocate buffer");
      			}
    		}
     	}
	/*char *Token = malloc(sizeof(char) * BUFF_SIZE);
	if(!IsAlpha(Look)) Abort("Name Expected");
	for(int i = 0; (i < BUFF_SIZE || IsAlNum(Look)); i++){
		Token[i] = Look;
		GetChar();
	}*/
   	return Token;
}

char *GetNum()
{
	char *Token =  malloc(sizeof(char) * BUFF_SIZE);
	char buff = Look;
	int position = 0;
	int buffsize = BUFF_SIZE;
	if(!IsDigit(Look)) Abort("Integer Expected");
	while(IsDigit(Look)){
		Token[position] = Look;
		position++;
		GetChar();
		if (position >= buffsize) {
      			buffsize += BUFF_SIZE;
      			Token = (char*)realloc(Token, buffsize);
      			if (!Token) {
        			Abort("Couldn't reallocate buffer");
      			}
    		}
     	}
   	return Token;

}

void Emit(char *string)
{
	printf("\t%s", string);
}

void EmitLn(char *string)
{
	Emit(string);
	printf("\n");
}

void init()
{
	GetChar();
}



void Expression();

void Ident()
{
	char *Name = GetName();
	if (Look == '(')
	{
		Match('(');
      		Match(')');
      		printf("\tcall %s\n", Name);
	} else
	{
		printf("\tmov rax, [%s]\n", Name);
	}
}

void Factor()
{
	if(Look == '(')
	{
		Match('(');
		Expression();
		Match(')');
	} else if(IsAlpha(Look)) 
	{
		Ident();
	}else {
		char *num = GetNum();
		printf("\tmov rax,%s\n", num);
	}
}

void Multiply()
{
	Match('*');
	Factor();
	EmitLn("pop rbx");
	EmitLn("imul rax,rbx");
}

void Divide()
{
	Match('/');
	Factor();
	EmitLn("pop rbx");
	EmitLn("idiv rbx,rax");
	EmitLn("mov rax,rbx");
}

void Term()
{
	Factor();
	while(Look == '*' || Look == '/')
	{
		EmitLn("push rax");
		switch(Look) {
			case '*':
				Multiply();
				break;
			case '/':
				Divide();
				break;
			default:
				Abort("Mulop Expected");
				break;
		}

	}
}

void Add()
{
	Match('+');
	Term();
	EmitLn("pop rbx");
	EmitLn("add rax,rbx");
}

void Subtract()
{
	Match('-');
	Term();
	EmitLn("pop rbx");
	EmitLn("sub rax,rbx");
	EmitLn("neg rax");
}

void Expression()
{
	if(IsAddop(Look)) EmitLn("xor rax,rax");
	else Term();
	while (IsAddop(Look)){
		EmitLn("push rax");
		switch(Look) {
			case '+':
				Add();
				break;
			case '-':
				Subtract();
				break;
			default:
				Abort("Addop Expected");
				break;
		}
	}
}

void Assigment()
{
	char *Name = GetName();
	Match('=');
	Expression();
	printf("\tmov [%s],rax\n", Name);

}

int main(int argc, char *argv[])
{
	init();
	Assigment();
	if(Look == ' ') Abort("newLine Expected");
	return 0;
}
