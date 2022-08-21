#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

char in_path[100] = "in.txt";
char out_path[100] = "out.txt";
char txt[2000000];
int count[26], total;

int main() {
	fstream inf, outf;
	int i;

	cout << "输入导入的文件路径：";
	cin >> in_path;
	cout << "输入导出的文件路径：";
	cin >> out_path;

	// 读取文件
	inf.open(in_path, ios::in);
	int idx = 0;
	while (!inf.eof()) {
		inf.get(txt[idx]);
		idx++;
	}
	inf.close();

	outf.open(out_path, ios::out);
	for (i = 0; i < idx; i++) {
		// 只写入英文字母
		if (!((txt[i] >= 'A' && txt[i] <= 'Z') || (txt[i] >= 'a' && txt[i] <= 'z'))) continue;
		char c;
		// 大字母换成小字母
		if (txt[i] >= 'A' && txt[i] <= 'Z') {
			c = char(txt[i] - 'A' + 'a');
		} else {
			c = txt[i];
		}
		// 数量++
		outf.put(c);
		count[c - 'a']++;
		total++;
	}

	// 输出
	for (i = 0; i < 26; i++) {
		cout << char('a' + i) << ' ' << fixed << setprecision(2) << count[i] * 100.0 / total << '%' << endl;
	}
	return 0;
}