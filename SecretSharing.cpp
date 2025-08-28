#include <bits/stdc++.h>
#include "json.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>

using json = nlohmann::json;
using boost::multiprecision::cpp_int;
using Rational = boost::rational<cpp_int>;
using namespace std;

int charToDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'z') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'Z') return 10 + (c - 'A');
    return -1;
}

cpp_int parseInBase(const string& s, int base) {
    cpp_int result = 0;
    for (char c : s) {
        if (c == '_' || isspace(static_cast<unsigned char>(c))) continue;
        int digit = charToDigit(c);
        if (digit == -1 || digit >= base) throw invalid_argument("Invalid digit");
        result = result * base + digit;
    }
    return result;
}

cpp_int lagrangeInterpolation(const vector<pair<long long, cpp_int>>& points, int k) {
    Rational result(0);
    for (int i = 0; i < k; i++) {
        long long xi = points[i].first;
        cpp_int yi = points[i].second;
        Rational li(1);
        for (int j = 0; j < k; j++) {
            if (i != j) {
                long long xj = points[j].first;
                li *= Rational(-xj, xi - xj);
            }
        }
        result += li * Rational(yi);
    }
    if (result.denominator() != 1) throw runtime_error("Non-integer secret");
    return result.numerator();
}

int main() {
    ifstream file("input.json");
    if (!file) {
        cerr << "Error: Cannot open input.json\n";
        return 1;
    }
    json data;
    file >> data;
    int n = data["keys"]["n"];
    int k = data["keys"]["k"];
    vector<pair<long long, cpp_int>> points;
    for (auto& item : data.items()) {
        if (item.key() == "keys") continue;
        long long x = stoll(item.key());
        int base = stoi(item.value()["base"].get<string>());
        string value = item.value()["value"];
        cpp_int y = parseInBase(value, base);
        points.emplace_back(x, y);
    }
    sort(points.begin(), points.end());
    if ((int)points.size() != n) {
        cerr << "Warning: JSON points (" << points.size() << ") != n (" << n << ")\n";
    }
    cpp_int secret = lagrangeInterpolation(points, k);
    cout << "Secret (c) = " << secret << endl;
    return 0;
}
