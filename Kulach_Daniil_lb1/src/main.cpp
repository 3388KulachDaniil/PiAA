#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

bool can_place(const vector<vector<int>>& grid, int x, int y, int size, int N) {
    if (x + size > N || y + size > N) return false;
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            if (grid[i][j] != 0) return false;
    return true;
}

void place_square(vector<vector<int>>& grid, int x, int y, int size, int label) {
    for (int i = x; i < x + size; i++)
        for (int j = y; j < y + size; j++)
            grid[i][j] = label;
}

pair<int,int> find_empty(const vector<vector<int>>& grid, int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (grid[i][j] == 0) return {i, j};
    return {-1, -1};
}

void backtrack(vector<vector<int>>& grid, vector<tuple<int,int,int>>& squares, int N,
               vector<tuple<int,int,int>>& best_solution, int remaining_area) {

    if (!best_solution.empty() && squares.size() >= best_solution.size())
        return;

    auto [x, y] = find_empty(grid, N);
    if (x == -1) {
        if (best_solution.empty() || squares.size() < best_solution.size())
            best_solution = squares;
        return;
    }

    int max_size = min(N - x, N - y);
    int limit = N - (N + 1) / 2;
    if (max_size > limit)
        max_size = limit;

    for (int size = max_size; size >= 1; size--) {
        int area = size * size;
        if (area <= remaining_area && can_place(grid, x, y, size, N)) {
            place_square(grid, x, y, size, squares.size() + 1);
            squares.emplace_back(x, y, size);
            backtrack(grid, squares, N, best_solution, remaining_area - area);
            squares.pop_back();
            place_square(grid, x, y, size, 0);
        }
    }
}

vector<tuple<int,int,int>> squaring_the_square(int N) {
    vector<vector<int>> grid(N, vector<int>(N, 0));
    vector<tuple<int,int,int>> best_solution;

    if (N % 2 == 0) {
        best_solution = {{0, 0, N/2}, {N/2, 0, N/2}, {0, N/2, N/2}, {N/2, N/2, N/2}};
        return best_solution;
    }
    if (N % 3 == 0) {
        int t2 = N*2/3, t1 = N/3;
        best_solution = {
            {0, 0, t2}, {0, t2, t1}, {t1, t2, t1},
            {t2, 0, t1}, {t2, t1, t1}, {t2, t2, t1}
        };
        return best_solution;
    }
    if (N % 5 == 0) {
        int t3 = N*3/5, t2 = N*2/5, t1 = N/5;
        best_solution = {
            {0, 0, t3}, {t3, 0, t2}, {t3, t2, t2}, {0, t3, t2},
            {t2, t3, t1}, {t2, 4*t1, t1}, {t3, 4*t1, t1}, {4*t1, 4*t1, t1}
        };
        return best_solution;
    }

    int maxW = (N + 1) / 2;
    int bigW = N - maxW;
    vector<tuple<int,int,int>> squares = {
        {0, 0, maxW},
        {0, maxW, bigW},
        {maxW, 0, bigW}
    };
    place_square(grid, 0, 0, maxW, 1);
    place_square(grid, 0, maxW, bigW, 2);
    place_square(grid, maxW, 0, bigW, 3);

    backtrack(grid, squares, N, best_solution, N*N);

    return best_solution;
}

int main() {
    int N; cin >> N;
    auto solution = squaring_the_square(N);

    cout << solution.size() << "\n";
    for (auto& [x, y, size] : solution)
        cout << x + 1 << " " << y + 1 << " " << size << "\n";

    return 0;
}
