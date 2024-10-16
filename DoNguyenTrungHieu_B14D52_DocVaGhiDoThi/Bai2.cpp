#include <bits/stdc++.h>
using namespace std;

vector<int> a[5000];
pair<int, int> ver[100000];
int deg_in[5000], deg_out[5000];
bool avail[5000];

void BFS(int s)
{
    queue<int> q;
    q.push(s);
    avail[s] = true;
    while (!q.empty())
    {
        int u = q.front();
        cout << u << ' ' << deg_in[u] + deg_out[u] << "\n";
        q.pop();
        for (int v : a[u])
            if (!avail[v])
            {
                avail[v] = true;
                q.push(v);
            }
    }
}

int main()
{
    ifstream inputFile("fcb.txt");
    if (!inputFile)
    {
        cerr << "Khong the mo tep fcb.txt" << endl;
        return 1;
    }

    ofstream outputFile("dataout.txt");
    if (!outputFile)
    {
        cerr << "Khong the mo tep dataout.txt" << endl;
        return 1;
    }

    int u, v;
    fill_n(deg_in, 5000, 0);
    fill_n(deg_out, 5000, 0);
    fill_n(avail, 5000, false);
    int id = 1;

    unordered_map<int, unordered_map<int, double> > edgeWeights;

    while (inputFile >> u >> v)
    {
        ver[id].first = u, ver[id].second = v;
        id++;
        a[u].push_back(v);
        deg_out[u]++;
        deg_in[v]++;
    }

    // In tap dinh va bac cua tung dinh
    cout << "Tap dinh va bac cua tung dinh:" << endl;
    for (int i = 1; i < id; i++)
    {
        cout << "Dinh " << ver[i].first << " co bac vao (in-degree) " << deg_in[ver[i].first]
             << ", bac ra (out-degree) " << deg_out[ver[i].first] << "\n";
    }

    BFS(0);
    cout << endl;

    // Sinh so va luu trong so ngau nhien cua cac canh vao file dataout.txt
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<double> distribution(0, 1);

    for (int i = 1; i < id; i++)
    {
        double w = distribution(generator);
        outputFile << ver[i].first << ' ' << ver[i].second << ' ' << w << "\n";
    }

    return 0;
}
