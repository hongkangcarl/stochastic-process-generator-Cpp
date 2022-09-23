# stochastic-process-generator-Cpp
A fast generator of stochastic processes (Brownian motion and Ornstein-Uhlenbeck process) written in C++.

## Overview
This program can generate multiple independent stochastic processes (Brownian motion and Ornstein-Uhlenbeck process) and save the trajectories into a csv file `output.csv`. The program will first ask for user's inputs about the type of trajectories, how many trajectories to generate, how many data points per trajectories and what is the time step.

## Mathematical background

### Brownian motion
A stochastic process is defined as a series of random variables, widely used to model systems with thermal fluctuations or stock prices in financial markets. A basic stochastic process is the Brownian motion $W_t$ (also called Wiener process). Every increment in the Brownian motion is independent and follows a standard normal distribution

$$
dW_t \sim N(0, 1)
$$

In physics, this corresponds to the trajectory of an inertia-free Brownian particle. The motion of this Brownian particle $W_t$ can be described by a Langevin equation

$$
\frac{dW_t}{dt} = \xi_t
$$

or

$$
dW_t = \xi_tdt
$$

where $\xi_t$ is a random velocity (due to a random force) that is uncorrelated in time and $\xi_tdt$ follows a standard normal distribution $N(0, 1)$.

### Ornstein-Uhlenbeck process

The Ornstein-Uhlenbeck process is another basic but interesting stochastic process. Physically motivated, this process is the trajectory of an inertia-free Brownian particle connected by a spring to a fixed point in the space. So we can write the Langevin equation of this particle:

$$
\frac{dX_t}{dt} = \theta (\mu - X_t) + \sigma \xi_t
$$

where $\mu$ is the position of the fixed point the particle is connected to, $\theta$ measures the strength of the spring, $\xi_t$ is the random velocity, and $\sigma$ measures its strength.

As we see from the Brownian motion, $dX_t = \xi_t dt$. Therefore, we can write the stochastic differential equation (SDE) of the Ornstein-Uhlenbeck process as

$$
dX_t = \theta (\mu - X_t)dt + dW_t
$$

where $dW_t$ is the increment of the Brownian motion.

## Numerical solution
To realize these stochastic processes, one has to numerically solve the SDEs. A property of Brownian motion is that, any finite increment $W_{t+\Delta t} - W_t = ∆W_{t}$ follows a normal distribution $N(0, \sqrt{\Delta t}) = \sqrt{\Delta t}N(0, 1)$. Thus Brownian motion can be generated using a random number generator with standard normal distribution. So the full trajectory can be realized by numerically integrating the increments

$$
W_t = \int_0^t dW_t \approx \sum_0^{t-1} \Delta W_t
$$

To realize the Ornstein-Uhlenbeck process, we first descretize the SDE

$$
X_{t+\Delta t} - X_t = \theta \mu \Delta t - \theta X_{t(?)} \Delta t + \sigma \Delta W_t
$$

The reason why I put a (?) there is that term in the RHS can be approximated as a known $X_t$ or an unknown $X_{t+\Delta t}$. If we choose to use $X_t$, then this numerical method is called the explicit method. If we choose $X_{t+\Delta t}$, it is called the implicit method.

### Explicit method realizing the Ornstein-Uhlenbeck process

If we choose $X_t$, the discretized SDE will read

$$
X_{t+\Delta t} = (1 - \theta \Delta t) X_t + \theta \mu \Delta t + \sigma \Delta W_t
$$

And we can iteratively generate $X_t$ for all $t$ by this equaiton. However, explicit method does not guarantee convergence, which means if the time step is chosen too big (compared to the timescale of the spring $1/\theta$), the system will blow up. We can see the blow up if we run this program, choose "OUE" as the model, and set time step equal to 2 ($1/\theta = 1$ in this system).

### Implicit method realizing the Ornstein-Uhlenbeck process

If we choose $X_{t+\Delta t}$, the discretized SDE will read

$$
X_{t+\Delta t} = \frac{1}{1 + \theta \Delta t} (X_t + \theta \mu \Delta t + \sigma \Delta W_t)
$$

Similarly, we can iterate this equation to generate $X_t$ for all $t$. The implicit method guarantees convergence, so that even if we choose a very big time step, the system will not blow up. So the implicit method is a better solution in generating the Ornstein-Uhlenbeck process.

