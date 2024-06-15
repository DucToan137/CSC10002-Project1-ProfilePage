#pragma once

#include <iostream>
#include <fstream>
#include<sstream>
#include<vector>
#include<string>
#include<io.h>
#include<fcntl.h>
#include<locale>
#include<filesystem>

using namespace std;
namespace fs = std::filesystem;



struct SinhVien {
	wstring mssv;
	wstring hoten;
	wstring khoa;
	int khoa_tuyen;
	wstring ngsinh;
	wstring email;
	wstring anh;
	wstring mota;
	vector<wstring> sothich;
};


vector<wstring> TachChuoi(const wstring& input);
wstring ChuanHoaChuoi(const wstring& str);
wstring TaoTenFile(SinhVien sv);
wstring InHoa(wstring input);
wstring TaoThuMuc(const wstring& folderPath);
void CopyFile(const fs::path& source, const fs::path& dest);
vector<SinhVien> DocFile(wstring filename);
void XuatSinhVien(SinhVien sv);
vector<int> CauHinh();
vector<SinhVien> ChonPhatSinh(vector<SinhVien>a);
void PhatSinhProfile(vector<SinhVien> a, wstring mssv, wstring hoten);