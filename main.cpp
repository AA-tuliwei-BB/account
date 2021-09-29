#include <bits/stdc++.h>

using namespace std;

// {{{ Date
struct Date {
	int year, month, day;

	bool IsLeapYear() {
		if (year % 4 == 0 and year % 100) return true;
		if (year % 400 == 0) return true;
		return false;
	}

	int Days() {
		int m = month;
		if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8
				|| m == 10 || m == 12) return 31;
		else if (m == 2) return IsLeapYear() ? 29 : 28;
		else return 30;
	}

	bool operator == (Date b) {
		return b.year == year and b.month == month and b.day == day;
	}

	bool operator != (Date b) {
		return !(*this == b);
	}

	bool operator < (Date b) {
		if (year == b.year) 
			if (month == b.month)
				if (day == b.day) return false;
				else return day < b.day;
			else return month < b.month;
		else return year < b.year;
	}

	bool operator > (Date b) {
		if (*this == b) return false;
		else return b < *this;
	}

	void operator ++ () {
		if (day == Days()) {
			day = 1;
			if (month == 12)
				month = 1, ++year;
			else ++month;
		} else ++day;
	}

	void operator -- () {
		if (day == 1) {
			if (month == 1)
				--year, month = 12;
			else --month;
			day = Days();
		} else --day;
	}

	int operator - (Date b) { // result won't be too big
		int res = 0;
		for (; b != *this; ++b) ++res;
		return res;
	}
};
// }}}

// {{{ record
struct record {
	float amount;
	string tag; //
	string des;
	Date date;
	inline bool operator < (record b) {
		return date < b.date;
	}
};
// }}}

vector<record> r;
Date today, free_st;
float Free;

void GetToday() {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	today.year = 1900 + ltm->tm_year;
	today.month = 1 + ltm->tm_mon;
	today.day = ltm->tm_mday;
}

// {{{ get and check
void get_f() {
	ifstream fin;
	fin.open("data/free.dat");
	fin >> free_st.year >> free_st.month >> free_st.day;
	fin >> Free;
	Free += (today - free_st) * 50;
	return void();
}

void get_r() {
	ifstream fin;
	fin.open("data/record.dat");
	float a;
	int y, m, d;
	string t, des;
	while (fin >> y >> m >> d >> a >> t >> des) {
		r.push_back((record) { a, t, des, (Date) { y, m, d }});
		if ((Date) { y, m, d } > free_st) Free -= a;
	}
	return void();
}

void CheckConvention() {
	ifstream fin;
	fin.open("data/convention.dat");
	Date st; int a;
	fin >> st.year >> st.month >> st.day >> a;

	int d = today - st;
	if (1 < a / 50 - d && a / 50 - d <= 3)
		printf("Convention: %d days left!\n", a / 50 - d);
	if (a / 50 <= 1)
		puts("Convention: used up!");
}
// }}}

// {{{ set
void SetConvention(Date d, int a) {
	ofstream fout;
	fout.open("data/convention.dat");
	fout << d.year << ' ' << d.month << ' ' << d.day << '\n';
	fout << a << '\n';
	return void();
}

void SetFree(Date d, float f) {
	ofstream fout;
	fout.open("data/free.dat");
	fout << d.year << ' ' << d.month << ' ' << d.day << '\n';
	fout << f << '\n';
	return void();
}
//}}}

// {{{ input and output and print a record
record input_a_record(bool is_today = false) {
	Date d;
	float amount;
	string tag, des;
	if (!is_today) {
		printf("Date: ");
		cin >>  d.year >> d.month >> d.day;
	} else d = today;

	printf("amount: ");
	cin >> amount;
	printf("tag: ");
	cin >> tag;
	printf("des: ");
	cin >> des;
	return (record) { amount, tag, des, d };
}

void output_a_record(record a, string pos = "data/record.dat") {
	ofstream fout;
	fout.open(pos, ios::app);
	fout << a.date.year << ' ' << a.date.month << ' ' <<
		a.date.day << ' ' << a.amount <<
		" " + a.tag + " " + a.des + '\n';
	fout.close();
}

void print_a_record(record a) {
	cout << a.date.year << ' ' << a.date.month << ' ' <<
		a.date.day << ' ' << a.amount <<
		" " + a.tag + " " + a.des + '\n';
}
// }}}

void MakeABackup() {
	system("cp data/record.dat data/.record.dat.back");
	system("cp data/convention.dat data/.convention.dat.back");
	system("cp data/free.dat data/.free.dat.back");
}

void init() {
	GetToday();
	get_f();
	get_r();
	CheckConvention();
}

int main() {
	init();

	while (true) {
		printf(">>");
		string order;
		cin >> order;

		if (order == "quit" or order == "q") {
			MakeABackup();
			return 0;
		}

		if (order == "add") {
			record tmp = input_a_record(true);
			output_a_record(tmp);
			r.push_back(tmp);
		}

		if (order == "add_") {
			record tmp = input_a_record();
			output_a_record(tmp);
			r.push_back(tmp);
		}

		// {{{ order "query"
		if (order == "query") {
			string type;
			cin >> type;

			// {{{ time
			if (type == "time") {
				int limit;
				cin >> limit;

				if (limit > 100) {
					printf("Too big !!\n");
					continue;
				}
				Date st = today, now;
				for (int i = 1; i < limit; ++i) --st;
				now = st;

				float sum = 0;
				sort(r.begin(), r.end());
				for (auto re : r) {
					if (re.date < st) continue;
					if (re.date > now) {
						printf("%d.%d.%d : cost %.2f¥ totally\n",
								now.year, now.month, now.day, sum);
						sum = 0, now = re.date;
					}
					sum += re.amount;
				}
				if (now == today)
					printf("today : cost %.2f¥ totally\n", sum);
				else printf("%d.%d.%d : cost %.2f¥ totally\n",
						now.year, now.month, now.day, sum);
			}
			// }}}

			else if (type == "free")
				printf("%.2f\n",  Free);
			else if (type == "convention") {
				ifstream fin;
				fin.open("data/convention.dat");
				Date st; int a;
				fin >> st.year >> st.month >> st.day >> a;
				int d = today - st;
				printf("%d days left\n", a / 50 - d);
			}

			else if (type == "list") {
				for (auto re : r) {
					if (re.date != today) continue;
					else print_a_record(re);
				}
			}

		}
		// }}}

		if (order == "free")
			printf("%.2f\n",  Free);

		if (order == "add_free") {
			printf("amount: ");
			float a; cin >> a;
			Free += a;
			Date yesterday = today; --yesterday;
			SetFree(yesterday, Free + 50);
		}

		if (order == "set_convention") {
			int a; cin >> a;
			SetConvention(today, a);
		}

		if (order == "sort") {
			sort(r.begin(), r.end());
			system("touch data/record.tmp");
			for (auto re : r)
				output_a_record(re, "data/record.tmp");
			system("mv data/record.tmp data/record.dat");
		}
	}

	return 0;
}
