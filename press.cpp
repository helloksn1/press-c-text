#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char txt[2000000];
unsigned char binaries[10000000];

// 压缩
// 读取文件路径in_path
// 结果文件 out_path
void press(char in_path[], char out_path[]) {
	fstream inf(in_path, ios::in);
	int i, j;
	int idx = 0;
	// 读取文件内容
	while (!inf.eof()) {
		inf.get(txt[idx]);
		idx++;
	}
	inf.close();
	idx--;
	for (i = 0; i < idx; i++) {
		// 每个字母换成对应的二进制
		int alphaIdx = txt[i] - 'a';
		for (j = 4; j >= 0; j--) {
			binaries[i * 5 + j] = alphaIdx % 2; // binaries: 二进制数组
			alphaIdx /= 2;
		}
	}
	
	ofstream outf(out_path, ios::out|ios::binary);
	outf.write((char*)&idx, sizeof(idx)); // 先写入字母总体数量
	
	// 把binaries 八个二进制为一组写入8bit是1byte 1bit不能直接写入 一次最少能写1byte
	int pressLen = idx * 5 % 8 == 0 ? idx * 5 / 8 : idx * 5 / 8 + 1; // 压缩后的长度
	for (i = 0; i < pressLen; i++) {
		unsigned char t = 0;
		for (j = 0; j < 8; j++) t = t * 2 + binaries[i * 8 + j]; // 每8bit换成1byte后写入
		outf.write((char*)&t, 1);
	}
	outf.close();

	cout << "Original size: " << idx << endl;
	cout << "Pressed size: " << pressLen + 4 << endl;
}

// 解压函数
void unpress(char in_path[], char out_path[]) {
	ifstream inf(in_path, ios::in|ios::binary);
	int len, i, j, idx = 0;
	inf.read((char*)&len, sizeof(len)); // 先读取长度
	// 读取的文件换成bit数组
	while (!inf.eof()) {
		unsigned char t;
		inf.read((char*)&t, 1);
		for (i = 7; i >= 0; i--) { // 读取1byte(8bit)后分成8个bit
			binaries[idx * 8 + i] = t % 2;
			t /= 2;
		}
		idx++;
	}
	inf.close();

	fstream outf(out_path, ios::out);
	for (i = 0; i < len; i++) {
		int v = 0;
		for (j = 0; j < 5; j++) v = v * 2 + binaries[i * 5 + j]; // 每5个bit为一组 获取对应的英文字母
			outf.put(char('a' + v));
	}
	outf.close();
}

int main() {
	char in_path[100];
	char out_path[100];
	int mode;

	cout <<"压缩（0）还是解压（1）？";
	cin >> mode;

	if (mode == 0) {
		cout << "输入文本文件路径：";
		cin >> in_path;
		strcpy(out_path, in_path);
		char ext[10] = "pre"; // 这是结果文件的后缀 txt换成这个
		strcpy(out_path + strlen(out_path) - 3, ext);
		press(in_path, out_path);
	}
	else {
		cout << "输入文本文件路径：";
		cin >> in_path;
		strcpy(out_path, in_path);
		char ext[10] = "txt";
		strcpy(out_path + strlen(out_path) - 3, ext);
		unpress(in_path, out_path);
	}

	return 0;
}