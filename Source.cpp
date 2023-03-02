#include <iostream>
#include <string>
#include <vector>
#include <stack>
using std::vector;
using std::string;
using std::stack;
using std::cin;
using std::cout;
using std::endl;
using std::exception;
using std::cerr;
using std::invalid_argument;

bool check_symbols(string s) {
	int pos = 0;
	while (pos >= 0 && pos < s.size()) {
		pos = s.find("pow");
		if (pos < 0 || pos >= s.size()) break;
		s.replace(pos, 3, "");
	}

	string alph = "1234567890-*/+().,";
	for (int i = 0; i < s.size(); i++) {
		int pos = 0;
		pos = alph.find(s[i]);
		if (pos >= 0 || pos < s.size()) {
			continue;
		}
		else return false;
	}

	return true;
}

bool check_dots(string s) {
	int pos = 0;
	int count = 0;
	while (pos >= 0 && pos < s.size()) {
		pos = s.find('.');
		if (pos < 0 || pos >= s.size()) break;
		s.replace(pos, 1, "");
		count++;
	}

	if (count > 1) return false;
	return true;
}

int get_symbol_priority(const char& symbol) {
	switch (symbol) {
	case '(': return 1;
	case ')': return 1;
	case '+': return 2;
	case '-': return 2;
	case '*': return 3;
	case '/': return 3;
	case '^': return 4;
	default: return -1;
	}
}

void erase_spaces(string& s) {
	int pos = 0;
	while (pos >= 0 && pos < s.size()) {
		pos = s.find(' ');
		if (pos < 0 || pos >= s.size()) break;
		s.replace(pos, 1, "");
	}
}

void breaks_multiplication(string& s) {
	int pos = 0;
	string sub1 = "", sub2 = "";
	while (pos < s.size()) {
		char let = s[pos];
		switch (let)
		{
		case '(':
			if ((pos > 0 && s[pos - 1] == ')') || (pos > 0 && isdigit(s[pos - 1])) ) {
				sub1 += s[0];
				for (int it = 1; it < pos; it++) {
					sub1 += s[it];
				}
				sub1 += '*';
				for (int it = pos; it < s.size(); it++) {
					sub1 += s[it];
				}
				s = sub1;
				sub1 = "";
			}
			break;
		case ')':
			if ( (pos <s.size() && s[pos + 1] == '(') || (isdigit(s[pos + 1]) && pos < s.size()) ) {
				sub2 += s[0];
				for (int it = 1; it < pos+1; it++) {
					sub2 += s[it];
				}
				sub2 += '*';
				for (int it = pos+1; it < s.size(); it++) {
					sub2 += s[it];
				}
				s = sub2;
				sub2 = "";
			}
			break;
		}
		pos++;
	}
	//cout << s <<endl;
}

bool validate_expression(string& s) {
	int left = 0;
	int right = 0;

	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '(') left++;
		if (s[i] == ')') right++;
	}

	if (left == right) return true;

	return false;
}

void erase_double_unary_minus(string& s) {
	int pos = 0;
	while (pos >= 0 && pos < s.size()) {
		pos = s.find("--");
		if (pos < 0 || pos >= s.size()) break;
		if (pos != 0) {
			if (isdigit(s[pos - 1])) {
				throw invalid_argument("invalid input");
			}
		}
		s.replace(pos, 2, "");
	}
}
//add brackets (use brackets count)
void add_zero_for_unary_minus(string& s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '-' && i == 0) {
			s.insert(0, "0");
			i++;
		}
		else if (s[i] == '-' && s[i - 1] == '(') {
			s.insert(i, "0");
			i++;
		}
	}
}

bool check_double_commas_in_new_expression(string& s) {
	string tmp = s;
	int pos = 0;
	int count_pow = 0;
	int count_commas = 0;
	while (pos >= 0 && pos < tmp.size()) {
		pos = tmp.find("pow");
		if (pos < 0 || pos >= s.size()) break;

		count_pow++;
		tmp.replace(pos, 3, "");
	}

	pos = 0;
	while (pos >= 0 && pos < tmp.size()) {
		pos = tmp.find(",");
		if (pos < 0 || pos >= tmp.size()) break;

		count_commas++;
		tmp.replace(pos, 1, "");
	}

	if (count_commas != count_pow) return false;

	return true;
}

void parse_new_expression(string& s, string& exp) {
	int pos = 0;
	while (pos >= 0 && pos < s.size()) {
		int pos = s.find(exp);
		if (pos < 0 || pos >= s.size()) break;

		string string_first_argument;
		string string_second_argument;

		int tmp = pos + 4;
		int left = 1;
		int right = 0;
		while (s[tmp] != ',') {
			string_first_argument.push_back(s[tmp]);
			tmp++;
			if (tmp == s.size()) throw invalid_argument("invalid input");
		}
		tmp++;
		while (left != right) {
			if (tmp == s.size()) throw invalid_argument("invalid input");
			if (s[tmp] == '(') left++;
			if (s[tmp] == ')') right++;
			if (left != right) {
				string_second_argument.push_back(s[tmp]);
				tmp++;
			}
		}

		//now s[tmp] == ')'
		string replace;
		replace.append("((");
		replace.append(string_first_argument);
		replace.push_back(')');
		replace.push_back('^');
		replace.push_back('(');
		replace.append(string_second_argument);
		replace.append("))");

		if (string_first_argument == "" || string_second_argument == "") {
			throw invalid_argument("incorrect input");
		}

		s.replace(pos, tmp - pos + 1, replace);
	}
}

void push_element_to_stack(stack<char>& st, const char& symbol, string& polish) {
	if (st.empty()) {
		st.push(symbol);
		return;
	}

	if (symbol == ')') {
		while (st.top() != '(') {
			polish.push_back(st.top());
			polish.push_back(' ');
			st.pop();
		}

		st.pop();
		return;
	}

	if (symbol == '(') {
		st.push(symbol);
		return;
	}

	while (!st.empty()) {
		if (get_symbol_priority(st.top()) >= get_symbol_priority(symbol)) {
			polish.push_back(st.top());
			polish.push_back(' ');
			st.pop();
		}
		else break;
	}

	st.push(symbol);
};

string make_polish(string s) {
	stack<char> st;
	string polish;

	for (int i = 0; i < s.size(); i++) {
		if (isdigit(s[i]) || s[i] == '.') {
			polish.push_back(s[i]);
			continue;
		}

		if (s[i] == '-' && i == 0) {
			polish.push_back(s[i]);
			continue;
		}

		if (s[i] == '-' && s[i - 1] == '(') {
			polish.push_back(s[i]);
			continue;

		}

		if (get_symbol_priority(s[i]) != -1) {
			if (polish.size() != 0) {
				if (polish[polish.size() - 1] != ' ') {
					polish.push_back(' ');
				}
			}

			push_element_to_stack(st, s[i], polish);
		}
	}

	while (!st.empty()) {
		polish.push_back(' ');
		polish.push_back(st.top());
		st.pop();
	}

	return polish;
}

vector<string> make_vector_from_string(string s) {
	if (s.size() == 0) {
		throw invalid_argument("incorrect input\n");
	}

	if (s.size() == 1) {
		vector<string> vec;
		vec.push_back(s);
		return vec;
	}

	while (s[s.size() - 1] == ' ') {
		s.erase(s.end() - 1);
	}

	vector<string> vec;
	while (s.find("  ") && s.size() != 1) {
		int pos = s.find("  ");
		if (pos < 0 || pos >= s.size()) break;

		s.erase(pos, 1);
	}

	while (s.find(' ') && s.size() != 1) {
		int pos = s.find(' ');
		if (pos < 0 || pos >= s.size()) break;

		vec.push_back(s.substr(0, pos));
		s = s.substr(pos + 1);
	}

	vec.push_back(s);
	return vec;
}

double calculate_polish(vector<string>& tokens) {
	stack<double> st;
	for (auto& i : tokens) {
		if (i == "+" || i == "-" || i == "*" || i == "/" || i == "^") {
			if (st.empty())  throw invalid_argument("incorrect input");
			double e1 = st.top();
			st.pop();

			if (st.empty())  throw invalid_argument("incorrect input");
			double e2 = st.top();
			st.pop();

			if (i == "+")  e1 = e2 + e1;
			if (i == "-")  e1 = e2 - e1;
			if (i == "*")  e1 = e2 * e1;
			if (i == "/") {
				if (e1 == 0)  throw invalid_argument("zero division");
				e1 = e2 / e1;
			}
			if (i == "^") {
				if (e2 == 0 && e1 == -1) throw invalid_argument("zero division");
				e1 = pow(e2, e1);
			}

			st.push(e1);
		}
		else {
			bool check = check_dots(i);
			if (!check) throw invalid_argument("too many dots");
			st.push(stod(i));
		}
	}

	return st.top();
}

bool check_brackets(string& s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '(') {
			if (i != 0) {
				if (isdigit(s[i - 1])) {
					return false;
				}
			}
			continue;
		}
		if (s[i] == ')') {
			if (i != s.size()) {
				if (isdigit(s[i + 1])) {
					return false;
				}
			}
			continue;
		}
	}

	return true;
}

int main() {
	string exp3;
	string ans;
	double top;
	string pow_expression = "pow";
	getline(cin, exp3);

	erase_spaces(exp3);
	breaks_multiplication(exp3);
	bool check_symb = check_symbols(exp3);
	if (!check_symb) {
		cerr << "invalid input";
		return 0;
	}

	cout << exp3 << endl;

	bool check_br;
	check_br = check_brackets(exp3);
	if (!check_br) {
		cerr << "invalid input";
		return 0;
	}


	bool check_pow = check_double_commas_in_new_expression(exp3);
	if (!check_pow) {
		cerr << "invalid commas count";
		return 0;
	}

	bool check = validate_expression(exp3);
	if (!check) {
		cerr << "invalid brackets count";
		return 0;
	}

	try {
		parse_new_expression(exp3, pow_expression);
	}
	catch (const exception& ex) {
		std::cerr << ex.what() << endl << "invalid input";
		return 0;
	}

	breaks_multiplication(exp3);

	try {
		erase_double_unary_minus(exp3);
	}
	catch (const exception& ex) {
		std::cerr << ex.what();
		return 0;
	}

	try {
		add_zero_for_unary_minus(exp3);
	}
	catch (const exception& ex) {
		std::cerr << ex.what() << endl << "error with unary minus";
		return 0;
	}
	cout << exp3 << endl;

	try {
		ans.append(make_polish(exp3));
	}
	catch (const exception& ex) {
		cerr << ex.what() << endl << "error while parsing input string, incorrect input";
		return 0;
	}

	cout << ans << endl;

	vector<string> v;
	try {
		v = make_vector_from_string(ans);
	}
	catch (const exception& ex) {
		cerr << ex.what() << "incorrect input";
		return 0;
	}

	try {
		top = calculate_polish(v);
	}
	catch (const exception& ex) {
		cerr << ex.what();
		return 0;
	}

	cout << top << endl;
	return 0;
}
