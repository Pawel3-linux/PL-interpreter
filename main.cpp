#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
using namespace std;
typedef enum {t_int, t_short, t_char, t_string, t_float, t_double, t_bool} vartype;
string tymcz;
struct
{
	int * intvars;
	short * shortvars;
	char * charvars;
	string * stringvars;
	float * floatvars;
	double * doublevars;
	bool * boolvars;
	int intsize;
	int shortsize;
	int charsize;
	int stringssize;
	int floatsize;
	int doublesize;
	int boolsize;
	string * names;
	int * onval;
	int index;
	vartype * types;
} resources;
void init_resources(void)
{
	resources.intvars = NULL;
	resources.shortvars = NULL;
	resources.charvars = NULL;
	resources.stringvars = NULL;
	resources.floatvars = NULL;
	resources.doublevars = NULL;
	resources.boolvars = NULL;
	resources.intsize = 0;
	resources.shortsize = 0;
	resources.charsize = 0;
	resources.stringssize = 0;
	resources.floatsize = 0;
	resources.doublesize = 0;
	resources.boolsize = 0;
	resources.names = NULL;
	resources.index = 0;
	resources.types = NULL;
	resources.onval = NULL;
}
void cpprealloc(string * ptr, int size, int oldsize)
{
	string * newptr = new string[size];
	for (int i = 0; i < size; i++)
	{
		if (i < oldsize)
			newptr[i] = ptr[oldsize];
		else
			newptr[i] = "";
	}
	delete ptr;
}
int find(string varname)
{
	for (int i = 0; i < resources.index; i++)
	{
		if (varname == resources.names[i])
			return i;
	}
	return -1; // Variable not found
}
int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cout << "Please give file to execute!\n";
		exit(1);
	}
	FILE * toint = fopen(argv[1], "a+");
	if (!toint)
	{
		cout << "Error in opening file " << argv[1] << '.';
		exit(1);
	}
	init_resources();
	char * instr = (char *) malloc(1000);
	for (int counter = 0; 1 ; counter++)
	{
		fscanf(toint, "%s", instr);
		if (!strcmp(instr, "exit"))
			exit(0);
		else if (!strcmp(instr, "get"))
		{
			fscanf(toint, "%s", instr);
			if (instr[0] != '@')
			{
				cout << "Syntax error in get: Expected @ before variable name.\n";
				break;
			}
			instr++;
			int ind = find(instr);
			char * varname2 = (char *) malloc(1000);
			if (ind == -1)
			{
				cout << "ExecuteError: Variable named '" << instr << "' not found.\n";
				break;
			}
			fscanf(toint, "%s", varname2);
			if (strcmp(varname2, "from"))
			{
				cout << "Syntax error: Expected from after 'get @" << instr << "'.\n";
				break;
			}
			fscanf(toint, "%s", instr);
			if (!strcmp(instr, "keyboard"))
			{
				if (resources.types[ind] == t_int)
				{
					int worth;
					cin >> worth;
					resources.intvars[resources.onval[ind]] = worth;
				}
			}
		}
		else if (!strcmp(instr, "print"))
		{
			do
			{
				fscanf(toint, "%s", instr);
				if (instr[0] != '@' && strcmp(instr, "#ENDPRINT"))
					cout << instr << ' ';
				else if (instr[0] == '@')
				{
					int ind = find(&(instr[1]));
					if (resources.types[ind] == t_int)
						cout << resources.intvars[resources.onval[ind]] << ' ';
				}
			}
			while (strcmp(instr, "#ENDPRINT"));
		}
		else if (!strcmp(instr, "println"))
			cout << '\n';
		else if (!strcmp(instr, "printendprint"))
			cout << "#ENDPRINT";
		else if (!strcmp(instr, "printspace"))
			cout << ' ';
		else if (!strcmp(instr, "int"))
		{
			fscanf(toint, "%s", instr);
			if (resources.intsize == 0)
			{
				resources.intvars = (int *) malloc(sizeof(int));
				resources.intsize++;
			}
			else
			{
				realloc(resources.intvars, (sizeof(int) * (resources.intsize + 1)));
				resources.intsize++;
			}
			if (resources.index == 0)
			{
				resources.onval = (int *) malloc(sizeof(int));
				resources.types = (vartype *) malloc(sizeof(vartype));
				resources.names = new string[1];
				resources.index++;
			}
			else
			{
				realloc(resources.onval, (sizeof(int) * (resources.index + 1)));
				realloc(resources.types, (sizeof(vartype) * (resources.index + 1)));
				cpprealloc(resources.names, (sizeof(string) * (resources.index + 1)), (sizeof(string) * resources.index));
				resources.index++;
			}
			tymcz = instr;
			resources.names[(resources.index - 1)] = tymcz;
			resources.types[(resources.index - 1)] = t_int;
			resources.onval[(resources.index - 1)] = resources.intsize;
			resources.intvars[resources.intsize] = 0;
		}
		else
			cout << instr << ": Unknown command.\n";
	}
}
