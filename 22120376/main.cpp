#include<iostream>
#include"SinhVien.h"

using namespace std;

int main() {

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	wstring mssvThucHien = L"22120376";
	wstring NguoiThucHien = L"Nguyễn Đức Toàn";

	wcout << L"CHƯƠNG TRÌNH PHÁT SINH TỰ ĐỘNG PROFILE PAGE\n\n";
	wcout << "============================================\n\n";

	vector<SinhVien> a = DocFile(L"data.csv");

	PhatSinhProfile(a, mssvThucHien, NguoiThucHien);


	return 0;
}