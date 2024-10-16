
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

vector<int> a[5000];
pair<int, int> ver[100000];
int deg_in[5000], deg_out[5000];
bool avail[5000];

// Bo sung ham DFS
vector<int> DFS(int start_node, int end_node)
{
    stack<int> stk;
    vector<int> path;
    vector<bool> visited(5000, false);

    stk.push(start_node);
    visited[start_node] = true;

    while (!stk.empty())
    {
        int u = stk.top();
        stk.pop();

        path.push_back(u);

        if (u == end_node)
        {
            break;  // Khi tim thay dich, dung tim kiem.
        }

        for (int v : a[u])
        {
            if (!visited[v])
            {
                stk.push(v);
                visited[v] = true;
            }
        }
    }

    return path;
}

// Bo sung ham BFS
vector<int> BFS(int start_node, int end_node)
{
    queue<int> q;
    vector<int> path;
    vector<bool> visited(5000, false);

    q.push(start_node);
    visited[start_node] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        path.push_back(u);

        if (u == end_node)
        {
            break;  // Khi tim thay dich, dung tim kiem.
        }

        for (int v : a[u])
        {
            if (!visited[v])
            {
                q.push(v);
                visited[v] = true;
            }
        }
    }

    return path;
}

// Do thoi gian
double measure_time(function<vector<int>(int, int)> search_func, int start_node, int end_node)
{
    auto start_time = high_resolution_clock::now();
    search_func(start_node, end_node);
    auto end_time = high_resolution_clock::now();
    microseconds elapsed_time = duration_cast<microseconds>(end_time - start_time);
    return elapsed_time.count() / 1.0e6;  // Chia cho 10^6 de doi thanh giay
}

int main()
{
    ifstream inputFile("fcb.txt");
    if (!inputFile)
    {
        cerr << "Khong the mo tep fcb.txt" << endl;
        return 1;
    }

    int u, v;
    fill_n(deg_in, 5000, 0);
    fill_n(deg_out, 5000, 0);
    fill_n(avail, 5000, false);
    int id = 1;

    unordered_map<int, unordered_map<int, double>> edgeWeights;

    while (inputFile >> u >> v)
    {
        ver[id].first = u, ver[id].second = v;
        id++;
        a[u].push_back(v);
        deg_out[u]++;
        deg_in[v]++;
    }

    int start_node = 0;
    int end_node;
    // Sinh dinh cuoi ngau nhien
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<double> distribution(0, 4038);
    end_node = distribution(generator);
    //int end_node = 4002;

    // In ket qua DFS va do thoi gian
    cout << "Duyet DFS tu " << start_node << " den " << end_node << ": \n";
    auto dfs_path = DFS(start_node, end_node);
    for (int node : dfs_path)
    {
        cout << node << "\t";
    }
    cout << endl;
    double dfs_time = measure_time(DFS, start_node, end_node);
    cout << "Thoi gian chay DFS: " << dfs_time << " giay" << endl;

    // In ket qua BFS va do thoi gian
    cout << "Duyet BFS tu " << start_node << " den " << end_node << ": \n";
    auto bfs_path = BFS(start_node, end_node);
    for (int node : bfs_path)
    {
        cout << node << "\t";
    }
    cout << endl;
    double bfs_time = measure_time(BFS, start_node, end_node);
    cout << "Thoi gian chay BFS: " << bfs_time << " giay" << endl;

    return 0;
}
