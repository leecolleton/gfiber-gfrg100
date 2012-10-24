/* libmain - flex run-time support library "main" function */

extern int yylex(void);

int main(int argc, char *argv[])
{
	while ( yylex() != 0 )
		;

	return 0;
}
