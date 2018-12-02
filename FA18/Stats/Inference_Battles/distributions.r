#Possible distributions

#Normal - Deterministic function for mean
dist_normal_mean <- function(test_val, predictor_p, extra){
  return(dnorm(test_val, mean=predictor_p, sd = extra, log=T) )
}
#Normal - Deterministic function for standard deviation
dist_normal_sd <- function(test_val, predictor_p, extra){
  return(dnorm(test_val, mean=extra, sd=predictor_p, log=T))
}
#Exponential
dist_exp <- function(test_val, predictor_p, extra){
  return(dexp(test_val, rate = predictor_p, log=T))
}
#Binomial
dist_binom <- function(test_val, predictor_p, extra){
  return(dbinom(test_val, p = predictor_p, size=500, log=T))
}
#Poisson
dist_poisson <- function(test_val, predictor_p, extra){
  return(dpois(test_val, lambda = predictor_p, log=T))
}