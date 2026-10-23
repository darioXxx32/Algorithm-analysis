# Adam: Theoretical and Experimental Analysis

This project was developed for **ECMC1018 — Analysis of Algorithms, Presentation I**. It studies Adam (Adaptive Moment Estimation) through a C++17 implementation and compares its behavior with SGD using Momentum and AMSGrad.

The project focuses on deterministic optimization, convergence behavior, learning-rate sensitivity, and computational complexity. It uses analytical benchmark functions rather than neural networks or real-world datasets. PyTorch is not used.

The study is configuration-specific: its results describe the tested objectives, starting points, hyperparameters, and stopping criteria. They do not establish universal superiority or convergence guarantees for any optimizer.

## Research Question

How does Adam behave when minimizing representative benchmark functions, and how do its convergence behavior and computational costs compare with Momentum and AMSGrad under different objectives and learning rates?

The investigation combines theoretical analysis with experimental evidence. It examines iteration counts, objective values, gradient norms, distances to known solutions, parameter trajectories, and the limitations of single-run execution-time measurements.

## Mathematical Formulation

Let $f:\mathbb{R}^{d}\rightarrow\mathbb{R}$ be a differentiable objective function, and let $\theta_t$ denote the parameter vector at iteration $t$. The deterministic experiments use the full analytical gradient:

$$
g_t=\nabla f(\theta_t)
$$

Adam maintains exponential moving averages of the gradient and its element-wise squared values:

$$
m_t=\beta_1m\_{t-1}+(1-\beta_1)g_t
$$

$$
v_t=\beta_2v\_{t-1}+(1-\beta_2)g_t^2
$$

Both moment vectors are initialized to zero. Bias correction is applied using the iteration counter $t$:

$$
\hat m_t=\frac{m_t}{1-\beta_1^t},
\qquad
\hat v_t=\frac{v_t}{1-\beta_2^t}
$$

The parameter update is:

$$
\theta\_{t+1}=\theta_t-
\alpha\frac{\hat m_t}{\sqrt{\hat v_t}+\epsilon}
$$

The square root, division, and addition of $\epsilon$ are applied coordinate-wise. The implementation increments the iteration counter before bias correction and adds epsilon outside the square root.

### Default Hyperparameters

| Parameter                                |     Value |
| ---------------------------------------- | --------: |
| Learning rate, $\alpha$                  |      0.01 |
| First-moment coefficient, $\beta_1$      |       0.9 |
| Second-moment coefficient, $\beta_2$     |     0.999 |
| Numerical stability constant, $\epsilon$ | $10^{-8}$ |

Individual experiments may override these values, as specified below.

### Comparison Algorithms

**SGD with Momentum**

Momentum accumulates a velocity vector:

$$
u_t=\beta u\_{t-1}+g_t
$$

$$
\theta\_{t+1}=\theta_t-\alpha u_t
$$

The implementation stores the scaled velocity term $\alpha u_t$. The momentum coefficient used in the main comparisons is 0.9.

**AMSGrad**

The implementation follows the AMSGrad variant described by Reddi, Kale, and Kumar. It maintains the first and second exponential moving averages and a coordinate-wise running maximum of the second moment:

$$
v_t^{\max}=\max(v\_{t-1}^{\max},v_t)
$$

The maximum is taken coordinate-wise. The update uses the uncorrected first moment and the accumulated maximum in the denominator:

$$
\theta\_{t+1}=\theta_t-
\alpha\frac{m_t}{\sqrt{v_t^{\max}}+\epsilon}
$$

This formulation uses the specified epsilon placement and does not apply bias correction to the moments in the AMSGrad update. It is distinct from bias-corrected Adam and should not be confused with AdamW or other Adam-family implementations.

## Project Structure

```text
Adam-Analysis/
├── src/                       # Optimizers, objective functions, and runner
├── experiments/               # Experiment implementations
├── tests/
│   └── optimizer_tests.cpp    # Optimizer and validation tests
├── results/
│   ├── csv/                   # Experimental traces and summaries
│   └── plots/                 # Figures generated from CSV files
├── docs/                      # Proposal, diagnostics, report, slides,
│                              # presentation script, and checklist
├── plot_results.py            # Plot generation script
└── README.md
```

The experiment executables include:

- `AdamAnalysis.exe`
- `exp_rosenbrock.exe`
- `exp_rastrigin.exe`
- `exp_reddi.exe`

The validated build configuration reportedly produces five executables. Consult the CMake configuration for the complete list of targets.

## Requirements

The project requires:

- CMake 3.10 or later.
- A C++17-compatible compiler.
- Ninja.
- Python 3.
- Python packages `pandas` and `matplotlib`.

The environment used for the reported experiments included CMake 3.29, MinGW g++ 13.2, and Python 3.13.

## Build and Run on Windows

Run the following commands in PowerShell from the project root:

```powershell
cd C:\Users\User\Desktop\8vo\Algorithms\proyect\Adam-Analysis

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

cmake --build build --clean-first

ctest --test-dir build --output-on-failure

New-Item -ItemType Directory -Force results\csv, results\plots | Out-Null

.\build\AdamAnalysis.exe

python plot_results.py
```

The commands must be executed from the `Adam-Analysis` directory because the output paths are relative to the project root.

Individual experiment executables can also be run after building:

```powershell
.\build\exp_rosenbrock.exe
.\build\exp_rastrigin.exe
.\build\exp_reddi.exe
```

## Experimental Design

The study evaluates five principal benchmark scenarios:

1. A two-dimensional separable quadratic function.
2. A ten-dimensional separable quadratic function.
3. The Rosenbrock function.
4. The Rastrigin function.
5. Reddi's periodic-gradient example.

A separate learning-rate sweep evaluates Adam on Rosenbrock using seven learning rates.

The main experiments use analytical full gradients, with no stochastic data sampling. Each optimizer comparison uses the same starting point, objective implementation, maximum iteration budget, and stopping tolerance. Learning rates are method-specific and were not selected through an equal-budget hyperparameter search.

### Starting Points and Hyperparameters

| Experiment    | Starting point                     | Adam | Momentum | AMSGrad |
| ------------- | ---------------------------------- | ---: | -------: | ------: |
| Quadratic 2D  | $(5,-3)$                           | 0.01 |     0.01 |    0.01 |
| Quadratic 10D | Fixed vector defined in the runner | 0.01 |     0.01 |    0.01 |
| Rosenbrock    | $(-1.5,2)$                         | 0.01 |    0.001 |    0.01 |
| Rastrigin     | $(2.5,2.5)$                        | 0.05 |    0.005 |    0.05 |

Momentum uses a coefficient of 0.9 in the main comparisons. Adam and AMSGrad use $\beta_1=0.9$, $\beta_2=0.999$, and $\epsilon=10^{-8}$, except in the Reddi example, where $\beta_2=0.99$.

The stopping criteria are based on the gradient norm:

- Quadratic functions: $10^{-8}$.
- Rosenbrock and Rastrigin: $10^{-6}$.
- Reddi example: a fixed budget of 300,000 updates.

The iteration counts therefore compare the declared configurations, not an intrinsic ranking of the algorithms.

## Results

The following table reports the observed iterations required to satisfy the configured gradient-norm tolerance.

| Objective     | Tolerance |  Adam | Momentum | AMSGrad |
| ------------- | --------: | ----: | -------: | ------: |
| Quadratic 2D  | $10^{-8}$ | 2,619 |      339 |   1,639 |
| Quadratic 10D | $10^{-8}$ | 2,639 |      362 |   1,666 |
| Rosenbrock    | $10^{-6}$ | 4,443 |    3,118 |  16,057 |
| Rastrigin     | $10^{-6}$ |   318 |      304 |     291 |

### Quadratic Functions

All methods reached the configured gradient-norm threshold in both quadratic experiments. Momentum required fewer iterations than Adam and AMSGrad under the tested settings.

The similar iteration counts between the two dimensions are consistent with the behavior observed on these separable quadratic objectives. However, comparing only dimensions two and ten is insufficient to establish an empirical scaling law.

### Rosenbrock Function

All three methods reached the specified gradient-norm threshold. Momentum required 3,118 iterations, Adam required 4,443, and AMSGrad required 16,057.

Adam and Momentum reached objective values approximately on the order of $10^{-12}$, with final parameter vectors close to the known minimum $(1,1)$.

The results illustrate that optimizer behavior depends on the objective geometry, learning rate, and stopping criterion.

### Rastrigin Function

All methods reached the configured gradient-norm threshold, but none found the known global minimum.

The final objective values were approximately:

| Algorithm | Final objective value |
| --------- | --------------------: |
| Adam      |                  7.96 |
| Momentum  |                  1.99 |
| AMSGrad   |                  7.96 |

The global minimum of the Rastrigin function is zero. Its multimodal landscape contains multiple local minima and stationary points, so a small gradient norm does not necessarily indicate global optimality.

Momentum obtained the lowest final objective value for this starting point and configuration. This single run does not demonstrate that Momentum is generally more effective at finding global minima.

### Reddi's Periodic-Gradient Example

After 300,000 updates, the recorded parameter values were approximately:

| Algorithm |   Final parameter |
| --------- | ----------------: |
| Adam      |  $x\approx0.9454$ |
| AMSGrad   | $x\approx-0.9982$ |

The reference target is $x=-1$, with an objective value of $-1$.

This experiment illustrates the difference between Adam and the tested AMSGrad formulation on a constructed gradient sequence. It is consistent with the motivation for AMSGrad but does not prove universal convergence of AMSGrad or universal failure of Adam.

## Learning-Rate Sensitivity

The Adam learning-rate sweep on Rosenbrock used the same starting point and a maximum budget of 20,000 iterations.

| Learning rate |   Iterations to tolerance |
| ------------: | ------------------------: |
|        0.0001 | Not reached within 20,000 |
|        0.0005 |                    15,919 |
|         0.001 |                    11,647 |
|         0.005 |                     6,059 |
|          0.01 |                     4,443 |
|          0.05 |                     2,022 |
|           0.1 |                     1,463 |

For $\alpha=10^{-4}$, the gradient-norm tolerance was not reached within the iteration budget; the final gradient norm was approximately 2.41.

For the remaining tested learning rates, the iteration count decreased as the learning rate increased. This pattern applies only to the finite grid and starting point used in the experiment. It is not a general convergence rule and does not establish that larger learning rates are always more stable or effective.

## Output Files and Reproducibility

The C++ runner generates experimental traces and summary files in `results/csv/`. These files record the information needed to analyze optimizer trajectories and compare final outcomes.

The recorded fields include:

- Iteration number.
- Objective value.
- Gradient norm.
- Distance to a known optimum.
- Parameter coordinates.
- Hyperparameters and stopping configuration.
- Iteration count and convergence status.
- Stopping reason.
- Elapsed time in milliseconds for a single run.

Trajectory CSV files record the state before the update associated with the next parameter vector: the runner evaluates the objective and gradient, logs the current point, checks the stopping condition, and then applies the update. Consequently, row $t$ represents the pre-update state used for that iteration.

The plotting script regenerates the figures from the canonical CSV files. The latest report records that **eight figures were recreated** from these outputs.

### Timing Limitations

Execution times were measured once per run. They may be affected by process scheduling, system load, compilation settings, logging, and timer overhead.

These values are descriptive rather than statistically robust. The report does not use them to claim general runtime superiority. A more reliable runtime study would require repeated measurements, controlled conditions, and appropriate summaries of variability.

## Theoretical Complexity

Let $d$ denote the number of parameters. Assuming that the gradient is already available, all three optimizers perform a constant amount of work per coordinate during an update.

| Algorithm         | Auxiliary optimizer memory | Update time |
| ----------------- | -------------------------: | ----------: |
| SGD with Momentum |                     $O(d)$ |      $O(d)$ |
| Adam              |                     $O(d)$ |      $O(d)$ |
| AMSGrad           |                     $O(d)$ |      $O(d)$ |

Momentum stores one vector-sized velocity. Adam stores first- and second-moment vectors. AMSGrad stores those two vectors and a coordinate-wise running maximum.

The optimizer update complexity excludes the cost of evaluating the objective gradient:

- Separable quadratic functions: $O(d)$ gradient evaluation.
- Rastrigin: $O(d)$ gradient evaluation.
- The fixed two-dimensional Rosenbrock objective: $O(1)$ gradient evaluation with respect to its fixed dimension.

Trajectory logging and CSV output are experiment infrastructure, not persistent optimizer state. The amount of logged data can grow with the number of recorded iterations and parameter dimensions, but this does not change the auxiliary-memory bound of the optimizer itself.

Linear time per update does not imply fewer iterations, lower total runtime, or universal convergence.

## Implementation and Numerical Validation

The implementation includes input validation and explicit optimizer state management.

The tests cover:

- Objective functions, gradients, and distances at analytical points.
- First updates for Adam, Momentum, and AMSGrad.
- Bias correction and the distinct AMSGrad denominator.
- State reset between independent runs.
- Invalid learning rates and optimizer parameters.
- Non-finite gradients and invalid numerical inputs.
- Parameter and gradient dimension mismatches.
- Rejection of dimension changes without resetting optimizer state.

The runner also checks parameter finiteness after updates. Numerical failures should not be treated as successful convergence.

### Independent Consistency Checks

The latest report documents the following checks:

| Check                                                    | Reported outcome                                       |
| -------------------------------------------------------- | ------------------------------------------------------ |
| Objective, gradient, and distance from final coordinates | Match stored CSV values within output precision        |
| Summary versus final trajectory row                      | Iteration and metrics agree                            |
| Reddi recurrence replay                                  | Final Adam and AMSGrad states match within $10^{-12}$  |
| Learning-rate sweep                                      | Seven rates, iteration cap, and tolerance fields agree |
| Clean build and CTest                                    | Five executables built; 1/1 test passed                |
| Plot regeneration                                        | Eight figures recreated from canonical CSV files       |

These checks provide evidence that the tested implementation and generated outputs are internally consistent. They do not prove correctness for every possible input or establish convergence for arbitrary objective functions.

## Limitations

The conclusions are restricted to the scope of the study:

- The experiments use small deterministic benchmark functions.
- The main comparisons use specific initial points and method-dependent learning rates.
- Hyperparameters were not tuned with a common, equal-budget search.
- Execution times come from individual runs and are not sufficient for robust runtime comparisons.
- The Rastrigin experiment reaches low gradient norms without finding the global minimum.
- The Reddi experiment is a finite reproduction of a constructed gradient sequence, not a general convergence proof.
- The two tested quadratic dimensions do not establish a general empirical scaling law.
- The project does not evaluate neural network training, stochastic minibatch optimization, or real-world datasets.

The theoretical complexity analysis and the empirical iteration counts answer different questions. The former describes asymptotic work per update; the latter records observed behavior under a particular experimental configuration.

## References

1. D. P. Kingma and J. Ba, “Adam: A Method for Stochastic Optimization,” *International Conference on Learning Representations (ICLR)*, 2015. [arXiv:1412.6980](https://arxiv.org/abs/1412.6980).

2. S. J. Reddi, S. Kale, and S. Kumar, “On the Convergence of Adam and Beyond,” *International Conference on Learning Representations (ICLR)*, 2018. [OpenReview](https://openreview.net/forum?id=ryQu7f-RZ).

3. B. T. Polyak, “Some methods of speeding up the convergence of iteration methods,” *USSR Computational Mathematics and Mathematical Physics*, vol. 4, no. 5, pp. 1–17, 1964. [DOI: 10.1016/0041-5553(64)90137-5](https://doi.org/10.1016/0041-5553\(64\)90137-5).

## Report and Submission Status

The project includes an IEEE Conference manuscript, implementation repository, experimental CSV files, generated plots, and presentation materials.

The report's appendix identifies the shared Overleaf project, the Adam-Analysis GitHub repository, and the recorded oral presentation folder on Google Drive.

Before submitting the project, verify the following:

- [x] The final IEEE report satisfies the official 12–15-page requirement.
- [x] The report PDF and presentation slides are included.
- [x] The Overleaf project link is accessible.
- [x] The GitHub repository contains the latest implementation, tests, CSV files, and figures.
- [x] The recorded presentation link works and points to the correct video.
- [x] The appendix contains the actual required URLs rather than placeholder text.
- [x] The final build, tests, and plotting script run successfully from the project root.

For additional project diagnostics and submission requirements, consult `docs/CHECKLIST.md` and `docs/INITIAL_DIAGNOSTIC.md`.
