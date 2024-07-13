#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int minEditCost(const string str1, const string str2, int ic, int dc, int rc){
    int len1 = str1.size();
    int len2 = str2.size();
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));

    for (int i = 0; i <= len1; i++){
        dp[i][0] = i * dc;
    }
    for (int j = 0; j <= len2; j++){
        dp[0][j] = j * ic;
    }

    for (int i = 1; i <= len1; i++){
        for(int j = 1; j <= len2; j++){
            if (str1[i - 1] == str2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1];
            }else{
                dp[i][j] = min({dp[i - 1][j - 1] + rc, dp[i - 1][j] + dc, dp[i][j - 1] + ic});
            }
        }
    }

    return dp[len1][len2];
}

int main() {
     std::string str1 = "abc";
     std::string str2 = "adc";
     int ic = 5, dc = 3, rc = 2;
     std::cout << minEditCost(str1, str2, ic, dc, rc) << std::endl;
     return 0;
}