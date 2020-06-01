// Copyright 2020, https://github.com/PatWie/CppNumericalSolvers

#include <iostream>

#include "include/cppoptlib/function.h"
#include "include/cppoptlib/solver/gradient_descent.h"
#include "include/cppoptlib/solver/newton_descent.h"

constexpr int Order = 2;
constexpr int Dim = 2;

template <typename T>
using SecondOrderProblem = cppoptlib::function::Function<T, Order, Dim>;

template <typename T>
class Simple : public SecondOrderProblem<T> {
 public:
  using vector_t = typename SecondOrderProblem<T>::vector_t;
  using scalar_t = typename SecondOrderProblem<T>::scalar_t;

  scalar_t operator()(const vector_t &x) const override {
    return 5 * x[0] * x[0] + 100 * x[1] * x[1] + 5;
  }

  void Gradient(const vector_t &x, vector_t *grad) const override {
    (*grad)[0] = 2 * 5 * x[0];
    (*grad)[1] = 2 * 100 * x[1];
  }
};

int main(int argc, char const *argv[]) {
  using Function = Simple<double>;
  using Solver = cppoptlib::solver::NewtonDescent<Function>;

  Function f;
  Function::vector_t x(2);
  x << -1, 2;

  auto state = f.Eval(x);

  std::cout << f(x) << std::endl;
  std::cout << state.gradient << std::endl;
  std::cout << state.hessian << std::endl;

  std::cout << cppoptlib::utils::IsGradientCorrect(f, x) << std::endl;
  std::cout << cppoptlib::utils::IsHessianCorrect(f, x) << std::endl;

  Solver solver;
  Solver::function_state_t solution;
  Solver::solver_state_t solver_state;

  std::tie(solution, solver_state) = solver.minimize(f, x);
  std::cout << "argmin " << solution.x.transpose() << std::endl;
  std::cout << "f in argmin " << solution.value << std::endl;
  std::cout << "solver state " << solver_state.num_iterations << std::endl;

  return 0;
}
