#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

vector<double> white_noise(int N) {

    default_random_engine generator;
    normal_distribution<double> distribution(0.0, 1.0);
    random_device rd;
    
    vector<double> result(N);

    generator.seed( rd() );
    for (int i = 0; i < N; i++) {
        double number = distribution(generator);
        result.at(i) = number;
    }

    return result;

}

vector<double> brownian_motion(int N, double dt) {
    vector<double> W(N);
    vector<double> dW = white_noise(N);

    for (int i = 0; i < N; i++) {
        if (i == 0) {
            W.at(i) = 0;
        }
        else {
            W.at(i) = W.at(i - 1) + sqrt(dt) * dW.at(i);
        }
    }

    return W;

}

vector<double> ornstein_uhlenbeck_explicit(int N, double dt, double mu, double theta, double sigma, double U0) {
    vector<double> U(N);
    vector<double> dW = white_noise(N);
    U.at(0) = U0;
    for (int i = 1; i < N; i++) {
        U.at(i) = (1 - theta * dt) * U.at(i - 1) + theta * mu * dt + sigma * sqrt(dt) * dW.at(i);
    }

    return U;

}

vector<double> ornstein_uhlenbeck(int N, double dt, double mu, double theta, double sigma, double U0) {
    vector<double> U(N);
    vector<double> dW = white_noise(N);
    U.at(0) = U0;
    for (int i = 1; i < N; i++) {
        U.at(i) = (U.at(i - 1) + theta * mu * dt + sigma * sqrt(dt) * dW.at(i)) / (1 + theta * dt);
    }

    return U;

}

int main() {

    string model;
    int M;      // number of trajectories to generate
    int N;      // number of data points per trajectory
    double dt;  // time step
    double mu = 0;
    double theta = 1;
    double sigma = 1;
    double U0 = 0;

    cout << "B: brownian\nOU: ornstein-uhlenbeck\nOUE: ornstein_uhlenbeck (explicit)\nWhat trajectory? ";
    cin >> model;
    cout << "How many trajectories to generate: ";
    cin >> M;
    cout << "How many data points: ";
    cin >> N;
    cout << "What is the time step: ";
    cin >> dt;

    // discretized time values
    vector<double> t(N);
    for (int i = 0; i < N; i++) {
        t.at(i) = dt * i;
    }

    // save the discrete time values
    ofstream output_file("./output.csv");
    ostream_iterator<double> output_iterator_t(output_file, ",");
    copy(t.begin(), t.end(), output_iterator_t);
    output_file << "\n";

    // generate and save the stochastic trajcetories
    for (int j = 0; j < M; j++) {
        vector<double> X(N);
        if (model == "B") {
            X = brownian_motion(N, dt);
        }
        else if (model == "OU") {
            X = ornstein_uhlenbeck(N, dt, mu, theta, sigma, U0);
        }
        else if (model == "OUE") {
            X = ornstein_uhlenbeck_explicit(N, dt, mu, theta, sigma, U0);
        }
        ostream_iterator<double> output_iterator_X(output_file, ",");
        copy(X.begin(), X.end(), output_iterator_X);
        output_file << "\n";

    }

    return 0;

}
