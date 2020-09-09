#include <iostream>
int G[100100][20][20];

int main() {
    int M = 5, N = 7;

    G[1][1][0] = 1;
    G[2][2][0] = 1;
    G[3][3][0] = 1;
    G[4][4][0] = 1;
    G[5][5][0] = 1;

    for(int n = 1; n <= N; ++n) {
        for(int t = 1; t <= M; ++t) {
            for(int k = 1; k <= M; ++k) {
                if(n < k + t || k == t) {
                    G[n][k][t] = 0;
                }
                else if(n == k + t) {
                    G[n][k][t] = 1;
                }
                else {
                    for(int i = 1; i <= M; ++i) {
                        for(int j = 0; j <= M; ++j) {
                            if(i != k && i != t && i !=j && j != t) {
                                G[n][k][t] += G[n - k - t][i][j];
                            }
                        }
                    }
                }
            }
        }
    }
    int res = 0;
    for(int i = 0; i <= M; ++i) {
        for(int j = 0; j <= M; ++j) {
            if(i != j)
            res += G[N][i][j];
        }
    }
    std::cout << res << std::endl;
    return 0;
}