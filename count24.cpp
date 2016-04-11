#include <iostream>
#include <stack>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <float.h>

using namespace std;

char ops[] = { '+', '-', '*', '/'};
//------
class Exp
{
	Exp* subExpl;
	Exp* subExpr;
	char op;
	float value;
public:
	float getvalue()
	{
		return value;
	}
	Exp(){};
	Exp(float val)
	{
		value = val;
		subExpl = subExpr = NULL;
		op = 0;
	}
	Exp(Exp* expL, Exp* expR, char op_in)
	{
		subExpl = expL;
		subExpr = expR;
		op = op_in;
	
		switch (op){
		case '+':
			value = expL->value + expR->value;
			break;
		case '-':
			value = expL->value - expR->value;
			break;
		case '*':
			value = expL->value * expR->value;
			break;
		case '/':
            if (expR->value == 0)
                value == FLT_MAX;
            else
                value = expL->value / expR->value;
			break;
		default:
			assert(false);
		}
	}

	string toString()
	{
		if (subExpl && subExpr)
		{
			return string("(") + subExpl->toString() + op + subExpr->toString() + string(")");
		}
		else
		{
			stringstream ss;
			ss << value;
			return ss.str();
		}
	}
};
//return false if dived by 0
bool mergeexps(Exp* const exps, int size, int lindex, int rindex, char op, Exp*  outexps)
{
    if (exps[rindex].getvalue()==0 && op=='/') return false;

	Exp* first = new((void*)&(outexps[0]))Exp(&(exps[lindex]), &(exps[rindex]), op);
	for (int i = 0; i < size; i++)
	{
		if (i == lindex || i == rindex)
			continue;
		*(++outexps) = exps[i];
	}
    return true;
}

void calcnew(Exp* exps, int expsnum)
{
	if (expsnum == 1){
		if (fabs(exps->getvalue() - 24) < 0.0001)
		{//get the expected result, output the expression
			string a;
			cout << exps->toString();
			cout << endl;
		}
		return;
	}

	//caculate 2 exps to 1 exp
	Exp* merged_exps = new Exp[expsnum - 1];
	for (int i = 0; i < expsnum; i++)
	{
		for (int j = i + 1; j < expsnum; j++)
		{
			for (int k = 0; k < sizeof(ops) / sizeof(char); k++)
			{
				//skip the condition div by 0 

				if (!mergeexps(exps, expsnum, i, j, ops[k], merged_exps)) 
                    continue;
				calcnew(merged_exps, expsnum - 1);

				if (ops[k] == '-' || ops[k] == '/')
				{
					//swap the left and right
                    if (!mergeexps(exps, expsnum, j, i, ops[k], merged_exps)) 
                        continue;
					calcnew(merged_exps, expsnum - 1);
				}
			}

		}
	}
	delete merged_exps;
}
void test_cacunew()
{
	while (1){
		cout << "please input numbers to be caculated, seprated by space, the dest value is 24." << endl;
		string str;
		getline(cin, str);

		stringstream ss(str);
		vector<Exp> vec;

		int Integer;
		while (true)
		{
			ss >> Integer;
			if (!ss)
				break;

			Exp exp(Integer);
			vec.push_back(exp);
		}
		calcnew(vec.data(), vec.size());
	}
}
//----------
int main()
{
   test_cacunew();
}
