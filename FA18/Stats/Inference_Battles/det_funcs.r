#Determinisitc Function Definitions

det_linear <- function(x, a, b){
  return(b* x + a)
}

det_exp_decay <- function(x, a, b){
  return(a * exp(-1 * b * x))
}

det_logistic <- function(x, a, b){
  tmp <- exp(a + b*x)
  return(tmp / (1 + tmp))
}

det_power_law <- function(x, a, b){
  return(a * x^b)
}