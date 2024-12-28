#include <stdio.h>
#include <math.h>

#include "prng/boxmuller.h"

struct option_metrics {
    double price;
    double delta;
    double gamma;
    double theta;
};

double simulate_single_path(const double S0, const double r, const double sigma, const double T, const int num_steps) {
    // Simulate the path of underlying asset (Stocks)
    const double dt = T / num_steps;
    double current_price = S0;

    for(int step = 1; step <= num_steps; step++) {
        const double Z = genrand_normal_distributed_double();
        // Geometric Brownian Motion to calculate Price
        current_price = current_price * exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Z);
    }

    return current_price;
}


double monte_carlo_price(double S0, double K, double r, double sigma, double T, int num_steps, int num_paths,
                         _Bool is_call) {
    double sum_payoffs = 0.0;

    for(int path = 0; path < num_paths; path++) {
        const double ST = simulate_single_path(S0, r, sigma, T, num_steps);

        // Only valid for european options (Only exercisable at T)
        if (is_call) {
            sum_payoffs += fmax(ST - K, 0.0);
        } else {
            sum_payoffs += fmax(K - ST, 0.0);
        }
    }

    // Bring future values back to today (risk-free rate etc.) + average payoffs
    return exp(-r * T) * (sum_payoffs / num_paths);
}

struct option_metrics calculate_all_greeks(double S0, double K, double r, double sigma, double T, int num_steps,
                                           int num_paths, _Bool is_call) {
    struct option_metrics  metrics;

    // Calculate fair price of option
    metrics.price = monte_carlo_price(S0, K, r, sigma, T, num_steps, num_paths, is_call);

    // Change the price of underlying, to see how much the option price would change
    const double dS = 0.01 * S0;
    const double dT = 1.0/365.0;
    const double price_up = monte_carlo_price(S0 + dS, K, r, sigma, T, num_steps, num_paths, is_call);
    const double price_down = monte_carlo_price(S0 - dS, K, r, sigma, T, num_steps, num_paths, is_call);
    const double price_next = monte_carlo_price(S0, K, r, sigma, T - dT, num_steps, num_paths, is_call);

    // Delta: Change the Value of underlying and see how much option price changes
    metrics.delta = (price_up - price_down) / (2.0 * dS);

    // Gamma: Taylor
    metrics.gamma = (price_up - 2.0 * metrics.price + price_down) / (dS * dS);

    // Theta: Taylor
    metrics.theta = -(price_next - metrics.price) / dT;

    return metrics;
}

int main() {
    /* Price of European options (only exercisable at the end of the term)
     * Valued using Monte Carlo simulation */

    const double S0 = 100.0;   // Initial stock price
    const double K = 100.0;    // Strike price
    const double r = 0.05;     // Risk-free rate
    const double sigma = 0.2;  // Volatility
    const double T = 1.0;      // 1 year
    const int num_steps = 252; // Daily steps
    const int num_paths = 10000; // Number of simulated paths
    const _Bool is_call = 0;

    const double call_price = monte_carlo_price(S0, K, r, sigma, T,
                                              num_steps, num_paths, is_call);

    printf("Call Option Price (using %d paths): %.4f\n", num_paths, call_price);

    return 0;
}