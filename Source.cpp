#include <iostream>
#include <string>
#include <vector>

using namespace std;

class bigNumber
{
public:
	//convert string to vector in reverse order
	vector<long long> stringToVectorRev(string s)
	{
		vector<long long> v;
		string stringSegment = "";
		int stringLength = s.length();
		if (s.length() < 9)
		{
			v.push_back(stoll(s));
		}
		for (int i = 0; i < (stringLength / 9); i++)
		{
			stringSegment = (s.substr((s.length() - 9), 9));
			v.push_back(stoll(stringSegment));
			if (s.length() == 9)
				break;
			else
				s = s.substr(0, (s.length() - 9));
			if (s.length() < 9)
			{
				v.push_back(stoll(s));
			}
		}
		return v;
	}

	//convert reverse vector to string
	string vectorToStringRev(vector<long long> v)
	{
		string s = "";
		int elementSize = 0;
		for (int i = (v.size() - 1); i >= 0; i--)
		{
			elementSize = getElementSize(v[i]);
			while (elementSize < 9)
			{
				s += "0";
				elementSize++;
			}
			s += to_string(v[i]);
		}
		while (s[0] == '0' && s.length() != 1)
		{
			s.erase(0, 1);
		}
		return s;
	}

	//get size of element in a vector
	int getElementSize(long long e)
	{
		string s = to_string(e);
		return s.size();
	}

	//adding two big numbers via vectors
	vector<long long> add(vector<long long> v1, vector<long long> v2)
	{
		int maxVecLength;
		int v1_size = v1.size();
		int v2_size = v2.size();
		if (v1_size > v2_size)
			maxVecLength = v1_size;
		else
			maxVecLength = v2_size;

		int carry = 0;
		vector<long long> sum;

		if (v1_size < v2_size)
		{
			for (int j = 0; j < (v2_size - v1_size); j++)
				v1.push_back(0);
		}
		else if (v1_size > v2_size)
		{
			for (int j = 0; j < (v1_size - v2_size); j++)
				v2.push_back(0);
		}

		for (int i = 0; i < maxVecLength; i++)
		{
			sum.push_back((v1[i] + v2[i] + carry) % 1000000000);
			if ((v1[i] + v2[i] + carry) >= 1000000000)
				carry = 1;
			else
				carry = 0;
		}

		if (carry == 1)
			sum.push_back(carry);
		return sum;
	}

	//subtracting two big numbers via vectors
	vector<long long> sub(vector<long long> v1, vector<long long> v2)
	{
		vector<long long> sub;
		int maxVecLength;
		int v1_size = v1.size();
		int v2_size = v2.size();
		bool negFlag = false;
		if (v1_size > v2_size)
			maxVecLength = v1_size;
		else
			maxVecLength = v2_size;

		if (v1_size < v2_size)
		{
			for (int j = 0; j < (v2_size - v1_size); j++)
				v1.push_back(0);
		}
		else if (v1_size > v2_size)
		{
			for (int j = 0; j < (v1_size - v2_size); j++)
				v2.push_back(0);
		}

		for (int i = 0; i < maxVecLength; i++)
		{
			//handling negative answers
			if (!isGreaterOrEqual(v1, v2)) //swap
			{
				vector<long long> v1tmp;
				vector<long long> v2tmp;
				v1tmp = v1;
				v2tmp = v2;
				v2 = v1tmp;
				v1 = v2tmp;
				negFlag = true;
			}

			if (v1[i] < v2[i])
			{
				v1[i] += 1000000000;
				v1[i + 1]--;
				sub.push_back(v1[i] - v2[i]);
			}
			else
				sub.push_back(v1[i] - v2[i]);
		}
		if (negFlag)
			sub[sub.size() - 1] *= -1;
		sub = eleminateExtraZeros(sub);
		return sub;
	}

	//multiplying two big number via vectors
	vector<long long> mult(vector<long long> v1, vector<long long> v2)
	{
		vector<long long> mult;
		vector<long long> sum;
		long long res = 0;
		long long carry = 0;
		int countZero = 0;

		for (int j = 0; j < v2.size(); j++)
		{
			countZero = 0;
			for (int i = 0; i < v1.size(); i++)
			{
				//add shift zeros of multiplication
				for (int k = countZero; k < j; k++)
				{
					mult.push_back(0);
					countZero++;
				}

				res = v2[j] * v1[i] + carry;
				mult.push_back(res % 1000000000);

				if (res <= 1000000000)
					carry = 0;
				else
					carry = res / 1000000000;
			}
			//add the last carry of multiplication process, if exists
			if (carry != 0)
				mult.push_back(carry);
			//reset the carry after each multiplication process
			carry = 0;
			//perform addition between multiplication outputs to get the final multiplication result
			sum = add(sum, mult);

			mult.clear();
		}
		return sum;
	}

	//long division, dividend must be greater than the divisor, else return 0
	vector<long long> longDiv(vector<long long> v1, vector<long long> v2)
	{
		string s1 = vectorToStringRev(v1);
		string s2 = vectorToStringRev(v2);
		int cond = s1.length() - s2.length();
		string s1tmp = s1.substr(0, s2.length());
		if (cond > 0)
			s1 = s1.substr(s2.length(), s1.length() - s2.length());
		short count = 0;
		vector<long long> ans;
		vector<long long> zero;
		zero.push_back(0);
		string s3 = "";
		if (cond < 0)
			return zero;
		while (cond >= 0)
		{
			v1 = stringToVectorRev(s1tmp);
			v2 = stringToVectorRev(s2);
			while (isGreaterOrEqual(v1, v2))
			{
				v1 = sub(v1, v2);
				count++;
			}
			s3 += to_string(count);
			count = 0;
			s1tmp = vectorToStringRev(v1);
			s1tmp += s1[0];
			s1.erase(0, 1);
			cond--;
		}
		return ans = stringToVectorRev(s3);
	}

	//mod
	vector<long long> mod(vector<long long> v1, vector<long long> v2)
	{
		string s1 = vectorToStringRev(v1);
		string s2 = vectorToStringRev(v2);
		int cond = s1.length() - s2.length();
		string s1tmp = s1.substr(0, s2.length());
		if (cond > 0)
			s1 = s1.substr(s2.length(), s1.length() - s2.length());

		while (cond >= 0)
		{
			v1 = stringToVectorRev(s1tmp);
			while (isGreaterOrEqual(v1, v2))
				v1 = sub(v1, v2);
			s1tmp = vectorToStringRev(v1);
			s1tmp += s1[0];
			s1.erase(0, 1);
			cond--;
		}
		return v1;
	}

	//compare numbers
	bool isGreaterOrEqual(vector<long long> v1, vector<long long> v2)
	{
		if (v1.size() == v2.size() && v1[v1.size() - 1] > v2[v2.size() - 1])
			return true;
		else if (v1.size() == v2.size() && v1[v1.size() - 1] < v2[v2.size() - 1])
			return false;
		else if (v1.size() == v2.size() && v1[v1.size() - 1] == v2[v2.size() - 1])
		{
			//check the rest of elements in vector
			for (int i = v2.size() - 1; i >= 0; i--)
			{
				if (v1[i] > v2[i])
					return true;
				else if (v1[i] < v2[i])
					return false;
				else
				{
				}
			}
			return true;
		}
		else if (v1.size() > v2.size())
			return true;
		else
			return false;
	}

	//modular exponentiation	v1^v2 mod v3
	vector<long long> modPower(vector<long long> v1, vector<long long> v2, vector<long long> v3)
	{
		vector<long long> res;
		res.push_back(1);
		v1 = mod(v1, v3);

		while ((v2[0] > 0 && v2.size() >= 1) || (v2[0] >= 0 && v2.size() != 1))
		{
			if (!isEven(v2))
				res = mod(mult(res, v1), v3);
			v2 = dividBy2(v2);
			v1 = mod(mult(v1, v1), v3);
		}
		return res;
	}

	//by 2 divider
	vector<long long> dividBy2(vector<long long> v)
	{
		long long etmp = 0;
		for (int i = v.size() - 1; i >= 0; i--)
		{
			etmp = v[i];
			v[i] /= 2;
			if (!isElementEven(etmp))
			{
				if (i == 0)
					break;
				else
					v[i - 1] += 1000000000;
			}
		}
		v = eleminateExtraZeros(v);
		return v;
	}

	//check if power is divisible by 2
	bool isEven(vector<long long> v)
	{
		if (v[0] % 2 == 0)
			return true;
		else
			return false;
	}

	//check if element in vector is even
	bool isElementEven(long long e)
	{
		e %= 2;
		if (e == 0)
			return true;
		else
			return false;
	}

	//using EUCLID Algorithm to get the inverse of a number	(b mod n)
	vector<long long> getInverse(vector<long long> v1, vector<long long> v2)
	{
		vector<long long> A2, A3, B2, B3, Q, A2tmp, A3tmp, B2tmp, B3tmp;
		A2.push_back(0);
		A3 = v2;
		B2.push_back(1);
		B3 = v1;
		while (true)
		{
			if (B3[0] == 0 && B3.size() == 1)
			{
				cout << "WARNING!\nThe entered number of public key E has no inverse value.\nType -Quit- and run the program again." << endl;
				return v1;
			}
			if (B3[0] == 1 && B3.size() == 1)
			{
				return B2;
			}

			Q = longDiv(A3, B3);
			A2tmp = A2; A3tmp = A3;
			A2 = B2; A3 = B3;
			B2tmp = mult(B2, Q);
			B3tmp = mult(B3, Q);

			while (!isGreaterOrEqual(A2tmp, B2tmp))
				A2tmp = add(A2tmp, v2);
			B2 = sub(A2tmp, B2tmp);

			while (!isGreaterOrEqual(A3tmp, B3tmp))
				A3tmp = add(A3tmp, v2);
			B3 = sub(A3tmp, B3tmp);
		}
	}

	//eleminate extra zeros in vector
	vector<long long> eleminateExtraZeros(vector<long long> v)
	{
		while (v[v.size() - 1] == 0 && v[0] != 0)
			v.pop_back();
		return v;
	}

	//check the primability of any number
	void isPrime(vector<long long> v)
	{
		vector<long long> n = v;
		vector<long long> a; a.push_back(3);	//a choosen 3
		vector<long long> x2; x2.push_back(2);
		vector<long long> tmp, tmpn;
		int k = 0;
		v = subBy1(v);
		tmpn = v;	//contains the value n-1

		if (n[0] == 2 && n.size() == 1)
		{
			cout << "Yes" << endl;
			return;
		}

		if ((n[0] == 0 || n[0] == 1) && n.size() == 1)
		{
			cout << "No" << endl;
			return;
		}

		while (true)
		{
			v = dividBy2(v);	//where q = v
			k++;
			if (!isEven(v))
				break;
		}

		tmp = modPower(a, v, n);

		if (tmp == tmpn || (tmp[0] == 1 && tmp.size() == 1))
		{
			cout << "Yes" << endl;
			return;
		}
		else
		{
			for (int j = 1; j < k; j++)
			{
				tmp = x2;
				for (int i = 1; i < j; i++)
					tmp = mult(tmp, x2);
				v = mult(v, tmp);
				if (modPower(a, v, n) == tmpn)
				{
					cout << "Yes" << endl;
					return;
				}
			}
			cout << "No" << endl;
			return;
		}
	}

	//subtract big number by 1
	vector<long long> subBy1(vector<long long> v)
	{
		v[0] -= 1;
		return v;
	}

	//encrypt message	M^e mod n	&	decrypt message C^d mod n
	vector<long long> encrypt(vector<long long> v1, vector<long long> v2, vector<long long> v3)
	{
		return modPower(v1, v2, v3);
	}

	//print strings to console
	void print(vector<long long> v)
	{
		string s = vectorToStringRev(v);
		cout << s << endl;
	}
};

string formatInputString(string s)
{
	int pos = s.find('=');
	s = s.substr(pos + 1, s.length() - pos - 1);
	return s;
}

int main()
{
	bigNumber obj;
	string P, Q, E, M, cmd;
	vector<long long> p, q, n, phi, e, d, m, c, one; one.push_back(1);
	int pos;

	//taking inputs from user
	cin >> P;
	P = formatInputString(P);
	p = obj.stringToVectorRev(P);

	cin >> Q;
	Q = formatInputString(Q);
	q = obj.stringToVectorRev(Q);

	cin >> E;
	E = formatInputString(E);
	e = obj.stringToVectorRev(E);

	n = obj.mult(p, q);	//calculate n = p*q
	phi = obj.mult(obj.sub(p, one), obj.sub(q, one));	//calculate phi = (p-1)*(q-1)
	d = obj.getInverse(e, phi);	//calculate d = e^-1 mod phi
	while (true)
	{
		cin >> cmd;
		if (cmd == "IsPPrime")
			obj.isPrime(p);
		else if (cmd == "IsQPrime")
			obj.isPrime(q);
		else if (cmd == "PrintN")
			obj.print(n);
		else if (cmd == "PrintPhi")
			obj.print(phi);
		else if (cmd == "PrintD")
			obj.print(d);
		else if (cmd.substr(0, 13) == "EncryptPublic")
		{
			pos = cmd.find('<');
			M = cmd.substr(pos + 1, cmd.length() - pos - 2);
			m = obj.stringToVectorRev(M);
			c = obj.encrypt(m, e, n);
			obj.print(c);
		}
		else if (cmd.substr(0, 14) == "EncryptPrivate")
		{
			pos = cmd.find('<');
			M = cmd.substr(pos + 1, cmd.length() - pos - 2);
			m = obj.stringToVectorRev(M);
			c = obj.encrypt(m, d, n);
			obj.print(c);
		}
		else if (cmd == "Quit")
			exit(0);
		else
			cout << "WRONG ENTRY! PLEASE RE-ENTER YOUR REQUEST." << endl;
	}
	return 0;
}