#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// Convert a character to integer value
int charToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return 0;
}

// Convert an integer to character
char valueToChar(int val) {
    if (val >= 0 && val <= 9) return '0' + val;
    if (val >= 10 && val <= 35) return 'A' + val - 10;
    return '0';
}

// Integer Addition using School Method
string add(string num1, string num2, int base) {
    int carry = 0;
    string result = "";

    while (!num1.empty() || !num2.empty() || carry) {
        int x = !num1.empty() ? charToValue(num1.back()) : 0;
        int y = !num2.empty() ? charToValue(num2.back()) : 0;

        int sum = x + y + carry;
        carry = sum / base;
        result.push_back(valueToChar(sum % base));

        if (!num1.empty()) num1.pop_back();
        if (!num2.empty()) num2.pop_back();
    }

    reverse(result.begin(), result.end());
    return result;
}

// Integer Multiplication using simple digit-by-digit method
string multiply(string num1, string num2, int base) {
    int n = num1.size();
    int m = num2.size();

    if (n == 0 || m == 0) return "0";

    // Convert characters to integer values
    for (int i = 0; i < n; i++) num1[i] = charToValue(num1[i]);
    for (int i = 0; i < m; i++) num2[i] = charToValue(num2[i]);

    vector<int> result(n + m, 0);

    for (int i = n - 1; i >= 0; i--) {
        for (int j = m - 1; j >= 0; j--) {
            int mul = num1[i] * num2[j];
            int pos1 = i + j, pos2 = i + j + 1;
            int sum = mul + result[pos2];

            result[pos1] += sum / base;
            result[pos2] = sum % base;
        }
    }

    string res = "";
    for (int i = 0; i < n + m; i++) {
        if (!(res.empty() && result[i] == 0)) {
            res += valueToChar(result[i]);
        }
    }

    return res.empty() ? "0" : res;
}

int main() {
    string num1, num2;
    int base;

    cin >> num1 >> num2 >> base;

    string sum_result = add(num1, num2, base);
    string mul_result = multiply(num1, num2, base);

    cout << sum_result << " " << mul_result << endl;

    return 0;
}