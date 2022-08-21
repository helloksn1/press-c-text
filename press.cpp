#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char txt[2000000];
unsigned char binaries[10000000];

// ѹ��
// ��ȡ�ļ�·��in_path
// ����ļ� out_path
void press(char in_path[], char out_path[]) {
	fstream inf(in_path, ios::in);
	int i, j;
	int idx = 0;
	// ��ȡ�ļ�����
	while (!inf.eof()) {
		inf.get(txt[idx]);
		idx++;
	}
	inf.close();
	idx--;
	for (i = 0; i < idx; i++) {
		// ÿ����ĸ���ɶ�Ӧ�Ķ�����
		int alphaIdx = txt[i] - 'a';
		for (j = 4; j >= 0; j--) {
			binaries[i * 5 + j] = alphaIdx % 2; // binaries: ����������
			alphaIdx /= 2;
		}
	}
	
	ofstream outf(out_path, ios::out|ios::binary);
	outf.write((char*)&idx, sizeof(idx)); // ��д����ĸ��������
	
	// ��binaries �˸�������Ϊһ��д��8bit��1byte 1bit����ֱ��д�� һ��������д1byte
	int pressLen = idx * 5 % 8 == 0 ? idx * 5 / 8 : idx * 5 / 8 + 1; // ѹ����ĳ���
	for (i = 0; i < pressLen; i++) {
		unsigned char t = 0;
		for (j = 0; j < 8; j++) t = t * 2 + binaries[i * 8 + j]; // ÿ8bit����1byte��д��
		outf.write((char*)&t, 1);
	}
	outf.close();

	cout << "Original size: " << idx << endl;
	cout << "Pressed size: " << pressLen + 4 << endl;
}

// ��ѹ����
void unpress(char in_path[], char out_path[]) {
	ifstream inf(in_path, ios::in|ios::binary);
	int len, i, j, idx = 0;
	inf.read((char*)&len, sizeof(len)); // �ȶ�ȡ����
	// ��ȡ���ļ�����bit����
	while (!inf.eof()) {
		unsigned char t;
		inf.read((char*)&t, 1);
		for (i = 7; i >= 0; i--) { // ��ȡ1byte(8bit)��ֳ�8��bit
			binaries[idx * 8 + i] = t % 2;
			t /= 2;
		}
		idx++;
	}
	inf.close();

	fstream outf(out_path, ios::out);
	for (i = 0; i < len; i++) {
		int v = 0;
		for (j = 0; j < 5; j++) v = v * 2 + binaries[i * 5 + j]; // ÿ5��bitΪһ�� ��ȡ��Ӧ��Ӣ����ĸ
			outf.put(char('a' + v));
	}
	outf.close();
}

int main() {
	char in_path[100];
	char out_path[100];
	int mode;

	cout <<"ѹ����0�����ǽ�ѹ��1����";
	cin >> mode;

	if (mode == 0) {
		cout << "�����ı��ļ�·����";
		cin >> in_path;
		strcpy(out_path, in_path);
		char ext[10] = "pre"; // ���ǽ���ļ��ĺ�׺ txt�������
		strcpy(out_path + strlen(out_path) - 3, ext);
		press(in_path, out_path);
	}
	else {
		cout << "�����ı��ļ�·����";
		cin >> in_path;
		strcpy(out_path, in_path);
		char ext[10] = "txt";
		strcpy(out_path + strlen(out_path) - 3, ext);
		unpress(in_path, out_path);
	}

	return 0;
}