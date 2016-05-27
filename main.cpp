#include <std>
#include <cstring>
int intv[15000];
char charv[15000];
short shortv[15000];
string stringv[15000];
float floatv[15000];
double doublev[15000];
bool boolv[15000];
string names[105000];
#define INDINT		0
#define INDSHORT	1
#define INDCHAR		2
#define INDSTRING	3
#define INDFLOAT	4
#define INDDOUBLE	5
#define INDBOOL		6
short indexes[7] = {0,0,0,0,0,0,0};
typedef enum {t_int, t_char, t_short, t_string, t_float, t_double, t_bool} Types;
Types types[105000];
int where[105000];
int indexm = 0;
#define same !strcmp
int find(string name)
{
	for (int i = 0; i < 105000; i++)
	{
		if (name == names[i])
			return i;
	}
	return -1;
}
int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cout << "You must pass name of file to execute!\n";
		return 1;
	}
	FILE * toint = fopen(argv[1], "a+");
	char * instr = (char *) malloc(sizeof(char) * 1000);
	if (!toint)
	{
		cout << "Can't open file named " << argv[1] << '.' << '\n';
		return 2;
	}
	if (!instr)
	{
		cout << "Can't alloc memory for instructions.\nPlease terminate unexpected processes.\n";
		return 3;
	}
	for (;;)
	{
		fscanf(toint, "%s", instr);
		if (same(instr, "exit"))
			break;
		if (same(instr, "int"))
		{
			fscanf(toint, "%s", instr);
			names[indexm] = instr;
			types[indexm] = t_int;
			where[indexm] = indexes[INDINT];
			intv[indexes[INDINT]] = 0;
			indexes[INDINT]++;
			indexm++;
		}
		if (same(instr, "char"))
		{
			fscanf(toint, "%s", instr);
			names[indexm] = instr;
			types[indexm] = t_char;
			where[indexm] = indexes[INDCHAR];
			charv[indexes[INDCHAR]] = 0;
			indexes[INDCHAR]++;
			indexm++;
		}
		if (same(instr, "string"))
		{
			fscanf(toint, "%s", instr);
			names[indexm] = instr;
			types[indexm] = t_string;
			where[indexm] = indexes[INDSTRING];
			stringv[indexes[INDSTRING]] = "";
			indexes[INDSTRING]++;
			indexm++;
		}
		if (same(instr, "reset"))
		{
			rewind(toint);
			for (int i = 0; i < 15000; i++)
			{
				intv[i] = 0;
				charv[i] = 0;
				stringv[i] = "";
			}
			for (int i = 0; i < 7; i++)
				indexes[i] = 0;
			for (int i = 0; i < 105000; i++)
			{
				types[i] = t_int;
				where[i] = 0;
				names[i] = "";
			}
		}
		if (same(instr, "print"))
		{
			do
			{
				fscanf(toint, "%s", instr);
				if (instr[0] != '@' && strcmp(instr, "#ENDPRINT"))
					cout << instr << ' ';
				else if (instr[0] == '@' && strcmp(instr, "#ENDPRINT"))
				{
					instr++;
					int ind = find(instr);
					if (ind == -1)
					{
						cout << instr << ": Variable not found.\n";
						return 4;
					}
					if (types[ind] == t_int)
					{
						cout << intv[where[ind]] << ' ';
					}
					else if (types[ind] == t_char)
					{
						cout << charv[where[ind]] << ' ';
					}
					else if (types[ind] == t_string)
					{
						cout << stringv[where[ind]] << ' ';
					}
				}
			}
			while (strcmp(instr, "#ENDPRINT"));
		}
		if (same(instr, "println"))
			cout << "\n";
		if (same(instr, "system"))
		{
			char * sysstr = (char *) malloc(3000);
			do
			{
				fscanf(toint, "%s", instr);
				if (instr[0] != '@' && strcmp(instr, "#ENDSYSCMD"))
				{
					strcat(sysstr, instr);
					strcat(sysstr, " ");
				}
				else if (instr[0] == '@' && strcmp(instr, "#ENDSYSCMD"))
				{
					instr++;
					int ind = find(instr);
					if (ind == -1)
					{
						cout << instr << ": Variable not found.\n";
						return 4;
					}
					if (types[ind] == t_int)
					{
						char * tocat = (char *) malloc(1000);
						sprintf(tocat, "%d", intv[where[ind]]);
						strcat(sysstr, tocat);
						strcat(sysstr, " ");
						free(tocat);
					}
					else if (types[ind] == t_char)
					{
						char * tocat = (char *) malloc(1000);
						sprintf(tocat, "%c", charv[where[ind]]);
						strcat(sysstr, tocat);
						strcat(sysstr, " ");
						free(tocat);
					}
					else if (types[ind] == t_string)
					{
						char * tocat = (char *) malloc(1000);
						sprintf(tocat, "%s", stringv[where[ind]].c_str());
						strcat(sysstr, tocat);
						strcat(sysstr, " ");
						free(tocat);
					}
				}
			}
			while (strcmp(instr, "#ENDSYSCMD"));
			system(sysstr);
			free(sysstr);
		}
		if (same(instr, "get"))
		{
			fscanf(toint, "%s", instr);
			if (instr[0] != '@')
			{
				cout << "Expected @ before variable name!\n";
				return 5;
			}
			char * tmpzm = (char *) malloc(sizeof(char) * 1000);
			fscanf(toint, "%s", tmpzm);
			if (strcmp(tmpzm, "from"))
			{
				cout << "Expected 'from' after 'get @[variablename]'.\n";
				return 6;
			}
			instr++;
			fscanf(toint, "%s", tmpzm);
			if (same(tmpzm, "keyboard"))
			{
				int ind = find(instr);
				if (ind == -1)
				{
					cout << instr << ": Variable not found.\n";
					return 4;
				}
				if (types[ind] == t_int)
				{
					int tog;
					cin >> tog;
					intv[where[ind]] = tog;
				}
				else if (types[ind] == t_char)
				{
					char tog;
					cin >> tog;
					charv[where[ind]] = tog;
				}
				else if (types[ind] == t_string)
				{
					string tog = "";
					char * tmz213 = (char *) malloc(1000);
					gets(tmz213);
					tog = tmz213;
					stringv[where[ind]] = tog;
				}
			}
		}
	}
}
