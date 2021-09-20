#include <bits/stdc++.h>

using namespace std;

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

struct record {
	float amount;
	string tag; //
	string des;
	Date date;
};

vector<record> r;

void get_r() {
	ifstream fin;
	fin.open("data/record.dat");
	float a;
	int y, m, d;
	string t, des;
	while (fin >> y >> m >> d >> a >> t >> des)
		r.push_back((record) { a, t, des, (Date) { y, m, d }});
	return void();
}

Date today;

int main() {
	get_r();
	while (true) {
		printf(">>");
		string order;
		cin >> order;

		if (order == "quit" or order == "q")
			return 0;

		if (order == "add") {
			if (!today.year) {
				printf("What's the date today?\n>>: ");
				cin >> today.year >> today.month >> today.day;
			}

			ofstream fout;
			fout.open("data/record.dat", ios::app);

			float amount;
			string tag, des;
			printf("The amout: ");
			cin >> amount;
			printf("The tag: ");
			cin >> tag;
			printf("The description: ");
			cin >> des;

			r.push_back((record) { amount, tag, des, today });
			fout << today.year << ' ' << today.month << ' ' <<
				today.day << ' ' << amount <<
				" " + tag + " " + des + '\n';
			fout.close();
		}

		if (order == "query") {
			string type;
			cin >> type;

			if (type == "time") {
				int limit;
				cin >> limit;
				if (!today.year) {
					printf("What's the date today?\n>>: ");
					cin >> today.year >> today.month >> today.day;
				}

				if (limit > 100) printf("Too big !!\n");
				Date st = today, now;
				for (int i = 1; i < limit; ++i) --st;
				now = st;

				float sum = 0;
				for (auto re : r) {
					if (re.date < st) continue;
					if (re.date > now) {
						printf("%d.%d.%d : cost %.2f¥ totally\n",
								now.year, now.month, now.day, sum);
						sum = 0, now = re.date;
					}
					sum += re.amount;
				}
				printf("today : cost %.2f¥ totally\n", sum);
			}
		}
	}

	return 0;
}
