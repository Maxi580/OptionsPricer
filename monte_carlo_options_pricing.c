#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "prng/boxmuller.h"


double simulate_single_path(const double S0, const double r, const double sigma, const double T, const int num_steps) {
    const double dt = T / num_steps;
    double current_price = S0;

    for(int step = 1; step <= num_steps; step++) {
        const double Z = genrand_normal_distributed_double();
        // Brownian Motion to calculate Price
        current_price = current_price * exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Z);
    }

    return current_price;
}

double monte_carlo_price_option(const double S0, const double K, const double r, const double sigma,
                               const double T, const int num_steps, const int num_paths, bool is_call) {
    double sum_payoffs = 0.0;

    for(int path = 0; path < num_paths; path++) {
        const double ST = simulate_single_path(S0, r, sigma, T, num_steps);

        if (is_call) {
            sum_payoffs += fmax(ST - K, 0.0);
        } else {
            sum_payoffs += fmax(K - ST, 0.0);
        }
    }

    // Bring future values back to today (risk-free rate etc.) + average payoffs
    return exp(-r * T) * (sum_payoffs / num_paths);
}

int main() {
    const double S0 = 100.0;   // Initial stock price
    const double K = 100.0;    // Strike price
    const double r = 0.05;     // Risk-free rate
    const double sigma = 0.2;  // Volatility
    const double T = 1.0;      // 1 year
    const int num_steps = 252; // Daily steps
    const int num_paths = 10000; // Number of simulated paths
    const bool is_call = false;

    const double call_price = monte_carlo_price_option(S0, K, r, sigma, T,
                                              num_steps, num_paths, is_call);

    printf("Call Option Price (using %d paths): %.4f\n", num_paths, call_price);

    return 0;
}