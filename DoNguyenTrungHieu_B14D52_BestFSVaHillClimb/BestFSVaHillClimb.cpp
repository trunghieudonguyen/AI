#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

// Dinh nghia cau truc do thi dang danh sach ke
struct DoThi
{
    int soDinh;
    vector<vector<pair<int, double>> > danhSachKe;
};

// Doc du lieu tu tep va xay dung do thi
DoThi xayDungDoThiTuTep(const string& tenTep)
{
    DoThi dothi;
    ifstream tepNhap(tenTep);

    int dinhGoc, dinhDich;
    double trongSo;
    int dinhLonNhat = -1;

    while (tepNhap >> dinhGoc >> dinhDich >> trongSo)
    {
        dinhLonNhat = max(dinhLonNhat, max(dinhGoc, dinhDich));
    }

    tepNhap.close();

    dothi.soDinh = dinhLonNhat + 1;
    dothi.danhSachKe.resize(dothi.soDinh);

    tepNhap.open(tenTep);

    while (tepNhap >> dinhGoc >> dinhDich >> trongSo)
    {
        dothi.danhSachKe[dinhGoc].push_back({dinhDich, trongSo});
    }

    tepNhap.close();

    return dothi;
}

// Ham de do thoi gian va bo nho tieu ton
void doKiemTraHieuSuat(const string& tenThuatToan, void (*hamThuatToan)(const DoThi&, int, int),
                       const DoThi& dothi, int dinhXuatPhat, int dinhDich)
{
    LARGE_INTEGER tanSo, thoiGianBatDau, thoiGianKetThuc;
    MEMORYSTATUSEX thongTinBoNho;

    QueryPerformanceFrequency(&tanSo);
    QueryPerformanceCounter(&thoiGianBatDau);

    hamThuatToan(dothi, dinhXuatPhat, dinhDich);

    QueryPerformanceCounter(&thoiGianKetThuc);

    double thoiGianThucThi = static_cast<double>(thoiGianKetThuc.QuadPart - thoiGianBatDau.QuadPart) / tanSo.QuadPart;

    cout << tenThuatToan << " thoi gian thuc thi: " << thoiGianThucThi << " giay" << endl;

    thongTinBoNho.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&thongTinBoNho);

    cout << tenThuatToan << " su dung bo nho: " << thongTinBoNho.ullTotalPageFile - thongTinBoNho.ullAvailPageFile << " byte" << endl;
}

// Ham thuc hien Best First Search (BFS)
void bestFirstSearch(const DoThi& dothi, int dinhXuatPhat, int dinhDich)
{
    vector<bool> daDuyet(dothi.soDinh, false);
    priority_queue<pair<double, int>> hangDoiUuTien;

    vector<int> duongDiBFS;
    hangDoiUuTien.push({0.0, dinhXuatPhat});
    daDuyet[dinhXuatPhat] = true;

    while (!hangDoiUuTien.empty())
    {
        int dinhHienTai = hangDoiUuTien.top().second;
        hangDoiUuTien.pop();
        duongDiBFS.push_back(dinhHienTai);

        if (dinhHienTai == dinhDich)
        {
            break;
        }

        for (const pair<int, double>& keXom : dothi.danhSachKe[dinhHienTai])
        {
            int dinhKeTiep = keXom.first;
            if (!daDuyet[dinhKeTiep])
            {
                hangDoiUuTien.push({keXom.second, dinhKeTiep});
                daDuyet[dinhKeTiep] = true;
            }
        }
    }
    cout << "Duong di Best First Search: ";
    for (int dinh : duongDiBFS)
    {
        cout << dinh << " ";
    }
    cout << endl;
}

// Ham thuc hien Steepest-Ascent Hill-Climbing
void steepestAscentHillClimbing(const DoThi& dothi, int dinhXuatPhat, int dinhDich)
{
    vector<bool> daDuyet(dothi.soDinh, false);

    vector<int> duongDiSAHC;
    int dinhHienTai = dinhXuatPhat;
    duongDiSAHC.push_back(dinhHienTai);

    while (dinhHienTai != dinhDich)
    {
        double giaTriHueristicTotNhat = DBL_MAX;
        int dinhTiepTheo = -1;

        for (const pair<int, double>& keXom : dothi.danhSachKe[dinhHienTai])
        {
            int dinhKeXom = keXom.first;
            if (!daDuyet[dinhKeXom])
            {
                double heuristic = keXom.second;
                if (heuristic < giaTriHueristicTotNhat)
                {
                    giaTriHueristicTotNhat = heuristic;
                    dinhTiepTheo = dinhKeXom;
                }
            }
        }

        if (dinhTiepTheo == -1)
        {
            break;
        }

        dinhHienTai = dinhTiepTheo;
        duongDiSAHC.push_back(dinhHienTai);
        daDuyet[dinhHienTai] = true;
    }
    cout << "Duong di Steepest-Ascent Hill-Climbing: ";
    for (int dinh : duongDiSAHC)
    {
        cout << dinh << " ";
    }
    cout << endl;
}

int main()
{
    string tenTep = "dataout.txt";
    DoThi dothi = xayDungDoThiTuTep(tenTep);

    int dinhXuatPhat = 0;
    int dinhDich = rand() % dothi.soDinh;

    doKiemTraHieuSuat("Best First Search", bestFirstSearch, dothi, dinhXuatPhat, dinhDich);
    doKiemTraHieuSuat("Steepest-Ascent Hill-Climbing", steepestAscentHillClimbing, dothi, dinhXuatPhat, dinhDich);

    return 0;
}
