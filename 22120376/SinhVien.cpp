#include"SinhVien.h"


// Hàm tách chuỗi trong file dữ liệu sinh viên
// Đầu vào: chuỗi kiểu wstring
// Đầu ra: vector chứa các chuỗi con theo yêu cầu
vector<wstring> TachChuoi(const wstring& input)
{
	vector<wstring> tokens;
	wstring token;
	bool flag = false;
	for (wchar_t ch : input) {
		if (ch == L'"') {
			flag = !flag;
		}
		else if (ch == L',' && !flag) {
			tokens.push_back(token);
			token.clear();
		}
		else {
			if (flag || ch != L'"')
				token.push_back(ch);
		}
	}
	tokens.push_back(token);

	for (wstring& f : tokens) {
		size_t start = f.find_first_not_of(L" \t");
		size_t end = f.find_last_not_of(L" \t");
		if (start != wstring::npos && end != wstring::npos)
			f = f.substr(start, end - start + 1);
		else
			f.clear();
	}

	return tokens;
}

// Hàm chuẩn hóa chuỗi theo định dạng thích hợp
// Đầu vào: chuỗi kiểu wstring
// ĐẦu ra: chuỗi đã được chuẩn hóa
wstring ChuanHoaChuoi(const wstring& str)
{
	wstring result;
	for (wchar_t ch : str) {
		if (iswalpha(ch) || iswdigit(ch)) {
			result += ch;
		}
	}
	return result;
}

// Hàm tạo tên file HTML theo định dạng mssv.html
// Đầu vào: biến cáu trúc kiểu SinhVien
// Đầu ra: chuỗi đầu ra là tên file theo định dạng
wstring TaoTenFile(SinhVien sv)
{
	wstring result;
	wstringstream ss;

	ss << sv.mssv << L".html";

	result = ss.str();
	return result;
}

// Hàm in hoa toàn bộ chuỗi
// Đầu vào: chuỗi kiểu wstring
// Đầu ra: chuỗi được in hoa toàn bộ
wstring InHoa(wstring str)
{
	wstring result;
	locale loc("");

	for (wchar_t ch : str) {
		result += toupper(ch, loc);
	}

	return result;
}

// Hàm copy nội dung trong thư mục nguồn đến thư mục đích
// Đầu vào: đường dần thư mục nguồn và thưc mục đích
// Đầu ra: không có
void CopyFile(const fs::path& source, const fs::path& dest)
{
	if (!fs::exists(dest))
		fs::create_directory(dest);

	for (const auto& file : fs::directory_iterator(source))
	{
		const auto& path = file.path();
		const auto& filename = path.filename();

		if (fs::is_directory(path)) {
			CopyFile(path, dest / filename);
		}
		else if (fs::is_regular_file(path)) {
			fs::copy(path, dest / filename, fs::copy_options::overwrite_existing);
		}
	}
}

// Hàm tạo thư mục Website theo đường dẫn ..../Website hoặc ...\Website
// và copy file "Personal.css" và thư mục "Images" vào thư mục Website
// Đầu vào: chuỗi wstring là đường dẫn đến thư mục Website
// Đầu ra: đường dẫn đến thư mục Website
wstring TaoThuMuc(const wstring& folderPath)
{
	wstring cssFile = L"Personal.css";
	wstring imagesFolder = L"Images";

	wstringstream ss;
	ss << folderPath;
	wstring path = ss.str();

	if (!fs::exists(path))
	{
		fs::create_directories(path);
	}
	else
	{
		if (fs::exists(cssFile))
		{
			fs::copy(cssFile, path + L"/" + cssFile, fs::copy_options::overwrite_existing);
		}
		else
		{
			wcout << L"Sao chép thất bại file " << cssFile << "\n";
		}

		if (fs::exists(imagesFolder))
		{
			fs::copy(imagesFolder, path + L"/" + imagesFolder, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
		}
		else
		{
			wcout << L"Sao chép thất bại thư mục " << imagesFolder << "\n";
		}

		return path;
	}

	if (fs::exists(cssFile))
	{
		fs::copy(cssFile, path + L"/" + cssFile, fs::copy_options::overwrite_existing);
	}
	else
	{
		wcout << L"Sao chép thất bại file " << cssFile << "\n";
	}

	if (fs::exists(imagesFolder))
	{
		fs::copy(imagesFolder, path + L"/" + imagesFolder, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
	}
	else
	{
		wcout << L"Sao chép thất bại thư mục " << imagesFolder << "\n";
	}

	return path;
}



// Hàm đọc file chứa dữ liệu sinh viên
// Đầu vào: chuỗi chứa tên file cần đọc
// Đầu ra: vector kiểu SinhVien
vector<SinhVien> DocFile(wstring filename)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	locale::global(std::locale("en_US.UTF-8"));
	wifstream input(filename);

	vector<SinhVien>a;


	if (!input.is_open())
	{
		wcout << L"Không thể mở file " << filename << "\n";
		return a;
	}
	else
	{
		wcout << L"Mở file " << filename << L" thành công\n";
	}

	wstring line;

	while (getline(input, line)) {
		SinhVien sv;
		vector<wstring> data = TachChuoi(line);

		sv.mssv = ChuanHoaChuoi(data[0]);
		sv.hoten = data[1];
		sv.khoa = data[2];
		sv.khoa_tuyen = stoi(data[3]);
		sv.ngsinh = data[4];
		sv.email = data[5];
		sv.anh = data[6];
		sv.mota = data[7];

		for (int i = 8; i < data.size(); i++)
		{
			sv.sothich.push_back(data[i]);
		}
		a.push_back(sv);
	}
	input.close();
	return a;
}

// Hàm xuất thông tin của một sinh viên ra màn hình
// Đầu vào: biến cấu trúc kiểu SinhVien
// Đầu ra: không có
void XuatSinhVien(SinhVien sv)
{
	wcout << L"MSSV: " << sv.mssv << "\n"
		<< L"Họ tên: " << sv.hoten << "\n"
		<< L"Khoa: " << sv.khoa << "\n"
		<< L"Khóa tuyển: " << sv.khoa_tuyen << "\n"
		<< L"Ngày sinh: " << sv.ngsinh << "\n"
		<< L"Email: " << sv.email << "\n"
		<< L"Ảnh: " << sv.anh << "\n"
		<< L"Mô tả: " << sv.mota << "\n";
	if (sv.sothich.size() > 0)
	{
		for (int j = 0; j < sv.sothich.size(); j++)
		{
			wcout << L"Sở thích " << j + 1 << ": ";
			wcout << sv.sothich[j] << "\n";
		}
	}
	else
	{
		wcout << L"Sở thích: Không có\n";
	}
	wcout << "\n";
}

// Hàm tùy chọn cấu hình thông tin của sinh viên
// Đầu vào: không có
// Đầu ra: vector lưu trữ trang thái cấu hình thông tin (1:có, 0:không)
vector<int> CauHinh()
{
	vector<int> res;

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	locale::global(std::locale("en_US.UTF-8"));


	int flagKhoa = -1;
	int flagKhoaTuyen = -1;
	int flagEmail = -1;
	int flagNgaySinh = -1;
	int flagHinh = -1;
	int flagMoTa = -1;
	int flagSoThich = -1;

	wcout << "\n========================================\n\n";
	wcout << L"\nTùy chọn cấu hình phát sinh cho profile page\n";
	wcout << L"Mã số sinh viên và họ tên được mặc định phát sinh trong profile page\n";
	wcout << L"Nhập 1 để có phát sinh thông tin trong profile page\n";
	wcout << L"Nhập 0 để không phát sinh thông tin trong profile page\n";


	do {
		wcout << L"Phát sinh thông tin khoa: ";
		wcin >> flagKhoa;
	} while (flagKhoa != 0 && flagKhoa != 1);

	do {
		wcout << L"Phát sinh thông tin khóa tuyển: ";
		wcin >> flagKhoaTuyen;
	} while (flagKhoaTuyen != 0 && flagKhoaTuyen != 1);

	do {
		wcout << L"Phát sinh thông tin ngày sinh: ";
		wcin >> flagNgaySinh;
	} while (flagNgaySinh != 0 && flagNgaySinh != 1);

	do {
		wcout << "Phát sinh thông tin email: ";
		wcin >> flagEmail;
	} while (flagEmail != 0 && flagEmail != 1);

	do {
		wcout << L"Phát sinh thông tin hình ảnh: ";
		wcin >> flagHinh;
	} while (flagHinh != 0 && flagHinh != 1);

	do {
		wcout << L"Phát sinh thông tin mô tả: ";
		wcin >> flagMoTa;
	} while (flagMoTa != 0 && flagMoTa != 1);

	do {
		wcout << L"Phát sinh thông tin sở thích: ";
		wcin >> flagSoThich;
	} while (flagSoThich != 0 && flagSoThich != 1);

	wcout << "\n\n";

	res.push_back(flagHinh);
	res.push_back(flagKhoaTuyen);
	res.push_back(flagNgaySinh);
	res.push_back(flagEmail);
	res.push_back(flagHinh);
	res.push_back(flagMoTa);
	res.push_back(flagSoThich);

	return res;
}

// Hàm lưu trữ sinh viên có phát sinh profile page
// Đầu vào: vector kiểu SinhVien
// Đầu ra: vector kiểu SinhVien lưu trữ các sinh viên có phát sinh profile page
vector<SinhVien> ChonPhatSinh(vector<SinhVien>a)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	locale::global(std::locale("en_US.UTF-8"));

	vector<SinhVien>res;

	int flag = -1;

	wcout << L"Người dùng có muốn phát sinh toàn bộ profile page không ? ";
	wcout << L"Nếu có nhập 1, ngược lại nhập 0\n";

	do {
		wcout << L"Phát sinh toàn bộ profile page: ";
		wcin >> flag;
	} while (flag != 0 && flag != 1);

	if (flag == 1)
	{
		return a;
	}
	else
	{
		flag = -1;

		for (int i = 0; i < a.size(); i++)
		{
			wcout << "\n========================================\n\n";
			wcout << L"Thông tin của sinh viên " << i + 1 << "\n";
			XuatSinhVien(a[i]);
			wcout << L"Người dùng có phát sinh profile page cho sinh viên này không\n";
			wcout << L"Nếu có phát sinh nhập 1\n";
			wcout << L"Nếu không phát sinh nhập 0\n";
			do {
				wcout << L"Phát sinh profile page: ";
				wcin >> flag;
			} while (flag != 0 && flag != 1);
			//wcout << "\n";

			if (flag == 1)
			{
				res.push_back(a[i]);
			}
		}


		return res;
	}

	return res;
}

// Hàm phát sinh profile page
// Đầu vào: vector kiểu SinhVien, chuỗi wstring lưu mã số sinh viên người thực hiện
// chuỗi wstring lưu họ tên người thực hiện
// Đầu ra: không có
void PhatSinhProfile(vector<SinhVien> a, wstring mssv, wstring hoten)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	locale::global(std::locale("en_US.UTF-8"));

	wstring path;

	wcout << L"\nNhập đường dẫn folder Website\n";
	wcout << L"Đường dẫn hợp lệ phải có dạng\n"
		<< L"Tên ổ đĩa:/Tên thư mục/.../Website\n"
		<< L"Tên ổ đĩa://Tên thư mục//...//Website\n"
		<< L"Tên ổ đĩa:\\\\Tên thư mục\\\\...\\\\Website\n"
		<< L"Tên ổ đĩa:\\Tên thư mục\\...\\Website\n";
	wcout << L"Nhập đường dẫn: ";
	getline(wcin, path);

	wcout << "\n";

	wstring folderName = TaoThuMuc(path);

	vector<SinhVien> SinhVienPhatSinhProfile = ChonPhatSinh(a);
	vector<int>cauhinh = CauHinh();

	for (SinhVien sv : SinhVienPhatSinhProfile) {
		wstring fileName = folderName + L"/" + TaoTenFile(sv);

		wofstream outFile(fileName);

		if (!outFile.is_open()) {
			wcout << L"Không thể tạo file " << fileName << "\n";
			//return;
		}
		else
		{
			outFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
			outFile << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
			outFile << "<head>\n";
			outFile << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
			outFile << "<link rel=\"stylesheet\" type=\"text/css\" href=\"personal.css\" />\n";
			outFile << L"<title>HCMUS - " << sv.hoten << "</title>\n";
			outFile << "</head>\n";
			outFile << "<body>\n";
			outFile << "<div class=\"Layout_container\">\n";
			outFile << "<!-- Begin Layout MainContents Region -->\n";
			outFile << "<div class=\"Layout_MainContents\">\n";
			outFile << "<!-- Begin Below Banner Region -->\n";
			outFile << "<div class=\"Personal_Main_Information\">\n";
			outFile << "<div class=\"Personal_Location\">\n";
			outFile << L"<div class=\"Personal_FullName\">" << InHoa(sv.hoten) << " - " << sv.mssv << "</div>\n";
			if (cauhinh[0] == 1)
			{
				outFile << L"<div class=\"Personal_Department\">" << L"KHOA " << InHoa(sv.khoa) << "</div>\n";

			}
			outFile << "<br />\n";
			outFile << "<div class=\"Personal_Phone\">\n";
			if (cauhinh[3] == 1)
			{
				outFile << "Email: " << sv.email << "\n";
			}
			outFile << "</div>\n";
			outFile << "<br />\n";
			outFile << "<br />\n";
			outFile << "</div>\n";
			outFile << "<div class=\"Personal_HinhcanhanKhung\">\n";
			if (cauhinh[4] == 1)
			{
				outFile << "<img src=\"Images/" << sv.anh << "\" class=\"Personal_Hinhcanhan\" />\n";
			}
			outFile << "</div>\n";
			outFile << "</div>\n";
			outFile << "<!-- End Below Banner Region -->\n";
			outFile << "<!-- Begin MainContents Region -->\n";
			outFile << "<div class=\"MainContain\">\n";
			outFile << "<!-- Begin Top Region -->\n";
			outFile << "<div class=\"MainContain_Top\">\n";
			outFile << L"<div class=\"InfoGroup\">Thông tin cá nhân</div>\n";
			outFile << "<div>\n";
			outFile << "<ul class=\"TextInList\">\n";
			outFile << L"<li>Họ và tên: " << sv.hoten << "</li>\n";
			outFile << "<li>MSSV: " << sv.mssv << "</li>\n";
			if (cauhinh[0] == 1)
			{
				outFile << L"<li>Sinh viên khoa " << sv.khoa << "</li>\n";
			}
			if (cauhinh[2] == 1)
			{
				outFile << L"<li>Ngày sinh: " << sv.ngsinh << "</li>\n";
			}
			if (cauhinh[3] == 1)
			{
				outFile << "<li>Email: " << sv.email << "</li>\n";
			}
			outFile << "</ul>\n";
			outFile << "</div>\n";
			if (cauhinh[6] == 1)
			{
				outFile << L"<div class=\"InfoGroup\">Sở thích</div>\n";
				outFile << "<div>\n";
				outFile << "<ul class=\"TextInList\">\n";
				if (sv.sothich.size() == 0)
				{
					outFile << L"<li>Không có</li>\n";
				}
				else
				{
					for (int i = 0; i < sv.sothich.size(); i++)
					{
						outFile << L"<li>" << sv.sothich[i] << "</li>\n";
					}
				}
			}
			outFile << "</ul>\n";
			outFile << "</div>\n";
			if (cauhinh[5] == 1)
			{
				outFile << L"<div class=\"InfoGroup\">Mô tả</div>\n";
				outFile << "<div class=\"Description\">\n";
				outFile << sv.mota << endl;
			}
			outFile << "</div>\n";
			outFile << "</div>\n";
			outFile << "</div>\n";
			outFile << "</div>\n";
			outFile << "<!-- Begin Layout Footer -->\n";
			outFile << "<div class=\"Layout_Footer\">\n";
			outFile << "<div class=\"divCopyright\">\n";
			outFile << "<br />\n";
			outFile << "<br />\n";
			outFile << "@2024</br>\n";
			outFile << L"Đồ án giữa kỳ</br>\n";
			outFile << L"Kỹ thuật lập trình</br>\n";
			outFile << "TH2024</br>\n";
			outFile << mssv << " - " << hoten << " </br>\n";
			outFile << "</div>\n";
			outFile << "</div>\n";
			outFile << "<!-- End Layout Footer -->\n";
			outFile << "</body>\n";
			outFile << "</html>\n";

			outFile.close();

			wcout << L"File HTML đã được tạo: " << TaoTenFile(sv) << "\n";
		}
	}
}