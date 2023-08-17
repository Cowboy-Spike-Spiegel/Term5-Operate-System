#include<iostream>
#include<vector>
#include<set>

using namespace std;

int generateAddress(int m);
void Optimal(int cmds[], int pageSize);
void LRU(int cmds[], int pageSize);
void FIFO(int cmds[], int pageSize);

int main()
{
	// define pageSize and cmd lsit
	int pageSize;
	int cmds[100];

	while(1) {
		cout << "\nPlease input the page size.\n";
		cin >> pageSize;
		
		int m = 0;
		for (int i = 0; i < 100; i++)
			cmds[i] = m = generateAddress(m);
		cout << "This is cmds:\n";
		for (int i = 0;i < 100;i++)
			cout << " " << cmds[i];
		cout << "\n\n";

		cout << "This is Optimal:\n";
		Optimal(cmds, pageSize);
		cout << "This is Least Recently Used:\n";
		LRU(cmds, pageSize);
		cout << "This is Fisrt In First Out:\n";
		FIFO(cmds, pageSize);
	}
}

// generate address depend on m
int generateAddress(int m) {
	int tmp = rand() % 10;
	// +1
	if (tmp < 7) {
		if (m == 99)
			return 99;
		return m + 1;
	}
	// 后地址
	else if (tmp == 9) {
		if (m == 99)
			return 99;
		return (rand() % (99 - m)) + m + 1;
	}
	// 前地址
	else {
		if (m == 0)
			return 0;
		return rand() % m;
	}
}

void Optimal(int cmds[], int pageSize) {
	set<int> page;
	int calls = 0;
	bool full = false;
	for (int i = 0; i < 100;i++) {
		int val = cmds[i];
		// not full
		if (!full) {
			if (page.find(val) == page.end()) {
				page.insert(val);
				calls++;
			}
			if (page.size() == pageSize)
				full = true;
		}
		// full
		else {
			if (page.find(val) == page.end()) {
				set<int> dismatch = page;
				for (int j = i+1; j < 100; j++) {
					auto it = dismatch.find(cmds[j]);
					if (it != dismatch.end())
						dismatch.erase(cmds[j]);
					if (dismatch.size() == 1)
						break;
				}
				page.erase(*dismatch.begin());
				page.insert(val);
				calls++;
			}
		}
	}
	cout <<"Missing page rate: "<<calls<<"%\n";
}

void LRU(int cmds[], int pageSize) {
	set<int> page;
	int calls = 0;
	bool full = false;
	for (int i=0; i < 100; i++) {
		int val = cmds[i];
		// not full
		if (!full) {
			if (page.find(val) == page.end()) {
				page.insert(val);
				calls++;
			}
			if (page.size() == pageSize)
				full = true;
		}
		else {
			if (page.find(val) == page.end()) {
				set<int> dismatch = page;
				for (int j=i-1; j >=0; j--) {
					auto it = dismatch.find(cmds[j]);
					if (it != dismatch.end())
						dismatch.erase(cmds[j]);
					if (dismatch.size() == 1)
						break;
				}
				page.erase(*dismatch.begin());
				page.insert(val);
				calls++;
			}
		}
	}
	cout << "Missing page rate: " << calls << "%\n";
}

void FIFO(int cmds[], int pageSize) {
	vector<int> page;
	int calls = 0;
	bool full = false;
	for (int i = 0;i < 100;i++) {
		int val = cmds[i];
		// not full
		if (!full) {
			if (find(page.begin(), page.end(),val) == page.end()) {
				page.push_back(val);
				calls++;
			}
			if (page.size() == pageSize)
				full = true;
		}
		else {
			if (find(page.begin(), page.end(), val) == page.end()) {
				page.erase(page.begin());
				page.push_back(val);
				calls++;
			}
		}
	}
	cout << "Missing page rate: " << calls << "%\n";
}