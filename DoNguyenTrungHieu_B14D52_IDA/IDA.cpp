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

double TinhTrongSoDinh(const DoThi& dothi, int dinh)
{
    double trongSo = 0.0;
    for (const pair<int, double>& ke : dothi.danhSachKe[dinh])
    {
        trongSo += 1.0; // Cong them bac ra
    }
    for (int i = 0; i < dothi.soDinh; ++i)
    {
        for (const pair<int, double>& ke : dothi.danhSachKe[i])
        {
            if (ke.first == dinh)
            {
                trongSo += 1.0; // Cong them bac vao
            }
        }
    }
    return trongSo;
}

double heuristic(const DoThi& dothi, int hienTai, int dich)
{
    double heuristic(const DoThi& doThi, int dinhHienTai, int dinhDich) {
    // Thuc hien tinh toan heuristic o day, vi du, dua vao so bac cua dinh ke.
    // Trong vi du nay, su dung so bac lam heuristic.
    int soBacDinhHienTai = doThi.danhSachKe[dinhHienTai].size();
    return static_cast<double>(soBacDinhHienTai);
}
}

vector<pair<int, int>> IDAStar(const DoThi& dothi, int batDau, int dich, double alpha)
{
    vector<int> mo;
    vector<pair<int, int>> ketQua;
    vector<bool> daTham(dothi.soDinh, false);

    double gioiHan = heuristic(dothi, batDau, dich);
    unordered_map<int, double> giaTriG;

    while (true)
    {
        mo.push_back(batDau);
        daTham[batDau] = true;
        while (!mo.empty())
        {
            int n = mo.back();
            mo.pop_back();

            if (n == dich)
            {
                return ketQua;
            }

            vector<pair<int, double>> danhSachKeSapXep = dothi.danhSachKe[n];
            sort(danhSachKeSapXep.begin(), danhSachKeSapXep.end(),
                 std::function<bool(const pair<int, double>&, const pair<int, double>&)>(
                     [&](const pair<int, double>& a, const pair<int, double>& b)
                        {
                             double f_a = giaTriG[n] + a.second + heuristic(dothi, a.first, dich);
                             double f_b = giaTriG[n] + b.second + heuristic(dothi, b.first, dich);
                             return f_a < f_b;
                        }));

            for (const pair<int, double>& ke : danhSachKeSapXep)
            {
                int m = ke.first;
                if (!daTham[m])
                {
                    double giaTriG_m = giaTriG[n] + ke.second;
                    double giaTriF_m = giaTriG_m + heuristic(dothi, m, dich);

                    if (giaTriF_m <= alpha)
                    {
                        mo.push_back(m);
                        ketQua.push_back(make_pair(m, n));
                        daTham[m] = true;
                        giaTriG[m] = giaTriG_m;
                    }
                }
            }
        }
        if (gioiHan == numeric_limits<double>::max())
        {
            return vector<pair<int, int>>();
        }
        alpha += alpha;
        for (int i = 0; i < dothi.soDinh; ++i)
        {
            daTham[i] = false;
        }
        mo.clear();
        ketQua.clear();
        giaTriG.clear();
    }
}


int main()
{
    string tenTep = "dataout.txt";
    double i = 0.0;
    double alpha = 0.2;
    //vector<int> duongDi;
    double chiPhi = 0.0;

    DoThi dothi = xayDungDoThiTuTep(tenTep);

    int dinhXuatPhat = 0;
    int dinhDich = 7;
    //int dinhDich = rand() % dothi.soDinh;

    doKiemTraHieuSuat("Best First Search", bestFirstSearch, dothi, dinhXuatPhat, dinhDich);
    doKiemTraHieuSuat("Steepest-Ascent Hill-Climbing", steepestAscentHillClimbing, dothi, dinhXuatPhat, dinhDich);

    vector<int> ketQuaCuoi;
    // Su dung IDA* de tim duong di
    vector<pair<int, int>> duongDi = IDAStar(dothi, dinhXuatPhat, dinhDich, alpha);
    if (duongDi.empty())
    {
        cout << "Khong tim thay tu " << dinhXuatPhat << " den " << dinhDich << endl;
    }
    else
    {
        cout << "Duong di IDA*: ";
        int hienTai = dinhDich;
        for (int i = duongDi.size() - 1; i >= 0; i--)
        {
            if (duongDi[i].first == hienTai)
            {
                ketQuaCuoi.push_back(duongDi[i].first);
                hienTai = duongDi[i].second;
            }
            if (duongDi[i].second == 0)
            {
                ketQuaCuoi.push_back(0);
                break;
            }
        }
        reverse(ketQuaCuoi.begin(), ketQuaCuoi.end());
        for (int i : ketQuaCuoi)
        {
            cout << i << "\t";
        }
        cout << endl;
        double tongChiPhi = 0.0;
        for (int i = 0; i < ketQuaCuoi.size() - 1; ++i)
        {
            for (const pair<int, double>& ke : dothi.danhSachKe[ketQuaCuoi[i]])
            {
                if (ke.first == ketQuaCuoi[i + 1])
                {
                    tongChiPhi += ke.second;
                    break;
                }
            }
        }
        cout << "Tong chi phi: " << tongChiPhi << endl;
    }

    return 0;
}
