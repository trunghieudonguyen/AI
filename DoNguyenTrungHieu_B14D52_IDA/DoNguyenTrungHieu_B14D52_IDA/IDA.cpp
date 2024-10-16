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
        cout << dinh << "\t";
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
        cout << dinh << "\t";
    }
    cout << endl;
}

double heuristic(const DoThi& doThi, int dinhHienTai, int dinhDich)
{
    int soBacDinhHienTai = doThi.danhSachKe[dinhHienTai].size();
    return static_cast<double>(soBacDinhHienTai);
}

bool IDAStar(const DoThi& dothi, int dinhXuatPhat, int dinhDich, double& chiPhi, vector<int>& duongDi, double i)
{
    if (dinhXuatPhat == dinhDich)
    {
        return true;
    }

    vector<int> duongDiHienTai;
    vector<int> nganXep;
    vector<bool> daDuyet(dothi.soDinh, false);

    nganXep.push_back(dinhXuatPhat);

    while (!nganXep.empty())
    {
        int hienTai = nganXep.back();
        nganXep.pop_back();
        duongDiHienTai.push_back(hienTai);

        if (hienTai == dinhDich)
        {
            double chiPhiHienTai = 0.0;
            for (int j = 1; j < duongDiHienTai.size(); j++)
            {
                int tu = duongDiHienTai[j - 1];
                int toi = duongDiHienTai[j];
                for (const pair<int, double>& canh : dothi.danhSachKe[tu])
                {
                    if (canh.first == toi)
                    {
                        chiPhiHienTai += canh.second;
                        break;
                    }
                }
            }

            if (chiPhiHienTai <= i)
            {
                chiPhi = chiPhiHienTai;
                duongDi = duongDiHienTai;
                return true;
            }
            continue;
        }

        daDuyet[hienTai] = true;
        bool coNhungDinhChuaDuyet = false;

        for (const pair<int, double>& canh : dothi.danhSachKe[hienTai])
        {
            int dinhKe = canh.first;
            if (!daDuyet[dinhKe])
            {
                double chiPhiDinhKe = 0.0;
                int soBacDinhKe = dothi.danhSachKe[dinhKe].size();
                for (int j = 1; j < duongDiHienTai.size(); j++)
                {
                    int tu = duongDiHienTai[j - 1];
                    int toi = duongDiHienTai[j];
                    for (const pair<int, double>& e : dothi.danhSachKe[tu])
                    {
                        if (e.first == toi)
                        {
                            chiPhiDinhKe += e.second;
                            break;
                        }
                    }
                }
                double heuristicValue = heuristic(dothi, dinhKe, dinhDich);
                if (chiPhiDinhKe + canh.second + heuristicValue <= i)
                {
                    nganXep.push_back(dinhKe);
                    coNhungDinhChuaDuyet = true;
                }
            }
        }

        if (!coNhungDinhChuaDuyet)
        {
            duongDiHienTai.pop_back();
        }
    }

    return false;
}

int main()
{
    string tenTep = "dataout.txt";
    double i = 0.0;
    double alpha = 2.0;
    vector<int> duongDi;
    double chiPhi = 0.0;

    DoThi dothi = xayDungDoThiTuTep(tenTep);

    int dinhXuatPhat = 0;
    int dinhDich = 7;

    doKiemTraHieuSuat("Best First Search", bestFirstSearch, dothi, dinhXuatPhat, dinhDich);
    doKiemTraHieuSuat("Steepest-Ascent Hill-Climbing", steepestAscentHillClimbing, dothi, dinhXuatPhat, dinhDich);

    while (true)
    {
        if (IDAStar(dothi, dinhXuatPhat, dinhDich, chiPhi, duongDi, i))
        {
            cout << "Duong di IDA*: ";
            for (int i : duongDi)
            {
                cout << i << "\t";
            }
            cout << "\nTong chi phi: " << chiPhi << " don vi\n";
            break;
        }
        i += alpha;  // Tang i khi khong tim thay duong di
    }

    return 0;
}
