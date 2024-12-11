#include <iostream>
#include <cmath>

double normalCDF(double x) {
    return 0.5 * erfc(-x * std::sqrt(0.5));
}

double blackScholesPrice(double S, double K, double r, double T, double sigma, bool isCall) {
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    if (isCall) {
        return S * normalCDF(d1) - K * std::exp(-r * T) * normalCDF(d2);
    }
    else {
        return K * std::exp(-r * T) * normalCDF(-d2) - S * normalCDF(-d1);
    }
}

double impliedVolatility(double marketPrice, double S, double K, double r, double T, bool isCall) {
    double sigma = 0.2; // Initial guess
    double tol = 1e-6;
    double maxIter = 100;
    for (int i = 0; i < maxIter; i++) {
        double price = blackScholesPrice(S, K, r, T, sigma, isCall);
        double vega = S * std::sqrt(T) * exp(-0.5 * std::pow((std::log(S / K) + (r + sigma * sigma / 2) * T) / (sigma * std::sqrt(T)), 2)) / std::sqrt(2 * M_PI);

        double diff = marketPrice - price;
        if (std::abs(diff) < tol) return sigma;

        sigma += diff / vega;
    }
    return sigma;
}

int main() {
    double marketPrice = 10.5;
    double S = 100, K = 100, r = 0.05, T = 1.0;
    bool isCall = true;

    double iv = impliedVolatility(marketPrice, S, K, r, T, isCall);
    std::cout << "Implied Volatility: " << iv << std::endl;

    return 0;
}