#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char Look = 0;

void GetChar()
{
	Look = getc(stdin);
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
	if (Look == c) GetChar();
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

bool IsAddop(char c)
{
	return (c == '+' || c == '-');
}

char GetName()
{
	if(!IsAlpha(Look)) Abort("Name Expected");
	char buff = Look;
	GetChar();
	return buff;
}

char GetNum()
{
	if(!IsDigit(Look)) Abort("Integer Expected");
	char buff = Look;
	GetChar();
	return buff;
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
	char Name = GetName();
	if (Look == '(')
	{
		Match('(');
      		Match(')');
		char string[] = "call X";
		string[5] = Name;
      		EmitLn(string);
	} else
	{
		char string[] = "mov rax,[X]";
		string[9] = Name;
		EmitLn(string);
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
		char string[] = "mov rax,N";
		string[8] = GetNum();
		EmitLn(string);
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
	char Name = GetName();
	Match('=');
	Expression();
	char string[] = "mov [X],rax";
	string[5] = Name;
	EmitLn(string);

}

int main(int argc, char *argv[])
{
	init();
	Assigment();
	if(Look == ' ') Abort("newLine Expected");
	return 0;
}
