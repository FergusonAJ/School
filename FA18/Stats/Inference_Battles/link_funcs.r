#Link functions

link_inv_logit <- function(x){
  return(1/(1+exp(-x)))
}

link_inv_log <- function(x){
  return(exp(x))
}

link_identity <- function(x){
  return(x)
}
