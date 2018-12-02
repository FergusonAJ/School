#Possible distributions

#Normal - Deterministic function for mean
d_dist_normal_mean <- function(test_val, predictor_p, extra){
  return(dnorm(test_val, mean=predictor_p, sd = extra, log=T) )
}
r_dist_normal_mean <- function(n, predictor_p, extra){
  return(rnorm(n, mean=predictor_p, sd = extra) )
}
dist_normal_mean = setNames(c(d_dist_normal_mean, r_dist_normal_mean), c('d_dist', 'r_dist'))


#Normal - Deterministic function for standard deviation
d_dist_normal_sd <- function(test_val, predictor_p, extra){
  return(dnorm(test_val, mean=extra, sd=predictor_p, log=T))
}
r_dist_normal_sd <- function(n, predictor_p, extra){
  return(rnorm(n, mean=extra, sd = predictor_p) )
}
dist_normal_sd = setNames(c(d_dist_normal_sd, r_dist_normal_sd), c('d_dist', 'r_dist'))

#Exponential
d_dist_exp <- function(test_val, predictor_p, extra){
  return(dexp(test_val, rate = predictor_p, log=T))
}
r_dist_exp <- function(n, predictor_p, extra){
  return(rexp(n, rate=predictor_p) )
}
dist_exp = setNames(c(d_dist_exp, r_dist_exp), c('d_dist', 'r_dist'))

#Binomial
d_dist_binom <- function(test_val, predictor_p, extra){
  return(dbinom(test_val, p = predictor_p, size=500, log=T))
}
r_dist_binom <- function(n, predictor_p, extra){
  return(rbinom(n, p=predictor_p, size=500))
}
dist_binom = setNames(c(d_dist_binom, r_dist_binom), c('d_dist', 'r_dist'))

#Poisson
d_dist_poisson <- function(test_val, predictor_p, extra){
  return(dpois(test_val, lambda = predictor_p, log=T))
}
r_dist_poisson <- function(n, predictor_p, extra){
  return(rpois(n, lambda=predictor_p) )
}
dist_poisson = setNames(c(d_dist_poisson, r_dist_poisson), c('d_dist', 'r_dist'))


