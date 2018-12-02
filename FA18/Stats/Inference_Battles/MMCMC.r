#Warning! This code is super gross!
#May work, may not?


# Modified Monte Carlo Markov Chain
# Austin Ferguson
# 11/27/2018

rm(list=ls())
assign("last.warning", NULL, envir = baseenv())
#options(warn=2)
setwd("~/Documents/School/FA18/Stats/Inference_Battles")
source('det_funcs.r')
source('link_funcs.r')
source('distributions.r')

id <- round(runif(1, min=0, max = 1e6))
args <- commandArgs(trailingOnly=T)
if(length(args) > 0){
  id = args[1]
  for(i in 2:length(args)){
    id <- paste(sep='_', id, args[i])
  }
}

print(paste('Run ID:', id))



# MCMC parameters
num_iters <- 1e3
sample_freq <- round(num_iters / 1000)
cur_dist <- dist_binom
cur_det <- det_linear
cur_link <- link_inv_logit



cur_dist_r = cur_dist$r_dist
cur_dist_d = cur_dist$d_dist


#Get the data
data <- read.csv('Polynom_nom_nomials_dataset.csv')


####     NOTE:    ####
#Much of this is Gideon Bradburn's work that I stole from class


#Distribution (Log) Likelihoodsd
log_likelihood <- function(dist, func, link, data, a, b, c){
  total <- 0
  for(i in 1:nrow(data)){
    predictor_p <- link(func(data[i,1], a, b))
    total <- total + dist(data[i,2], predictor_p, c)# dnorm(data[i,2], mean=mean, sd = c, log=T)   
  }
  return(total)
}

#(Log) Prior
log_prior <- function(x){
  return(dnorm(x, mean=0, sd = 1e6, log=T))
}

#(Log) Posterior Probability
#Note, we don't have any preference on the prior, so we are ignoring them. 
log_posterior_prob <- function(dist, func, link, data, a, b, c){
  val <- log_likelihood(dist, func, link, data, a, b, c)
  #val <- val + log_prior(a)
  #val <- val + log_prior(b)
  #val <- val + log_prior(c)
  return(val)
}

#	-sampled parameter estimates
samples_a <- rep(NA,1 + num_iters/sample_freq)
samples_b <- rep(NA,1 + num_iters/sample_freq)
samples_c <- rep(NA,1 + num_iters/sample_freq)
samples_post_prob <- rep(NA,1 + num_iters/sample_freq)
#	-randomly generate parameter values (within prior bounds)
a <- rnorm(1,mean=0,sd=1)
b <- rnorm(1,mean=0,sd=1)
c <- rexp(1,rate=1)
#	-calculate posterior probability
post_prob <- log_posterior_prob(cur_dist_d, cur_det, cur_link, data, a, b, c)
#Log initial values
samples_a[1] <- a
samples_b[1] <- b
samples_c[1] <- c
samples_post_prob[1] <- post_prob

# run the MCMC for the specified number of iterations
for(i in 2:num_iters){
  #		-propose an update a parameter value
  a_prime <- a + rnorm(1,0,0.1)
  b_prime <- b + rnorm(1,0,0.1)
  c_prime <- c + rnorm(1,0,0.1)
  
  #		-calculate the posterior probability of the data given the proposed value
  post_prob_prime <- log_posterior_prob(cur_dist_d, cur_det, cur_link, data, a_prime, b_prime, c_prime)
  #		-calculate R, the ratio of the proposed and current posterior probabilities
  R <- exp(post_prob_prime - post_prob)
  
  #		-generate a random, uniformly distributed number between 0 and 1
  U <- runif(1,min=0,max=1) 
  #		-if R is greater than the random draw from U(0,1), accept the move
  if(!is.na(R) && R > U){
    #			-new param value is proposed param value
    a <- a_prime
    b <- b_prime
    c <- c_prime
    #			-new posterior probability is the posterior probability of the proposed param value
    post_prob <- post_prob_prime
  }
  
  #	   # do book-keeping (log parameter values and posterior probability values at pre-specified sampling frequencies
  if(i %% sample_freq == 0){
    samples_a[1+i/sample_freq] <- a
    samples_b[1+i/sample_freq] <- b
    samples_c[1+i/sample_freq] <- c
    samples_post_prob[1+i/sample_freq] <- post_prob
  }
  #Print out progress every now and again.
  if(i %% 100 == 0){
    print(i)
  }
}

#Generate some beautiful plots!
png(filename = paste(sep='', id, '_posterior_prob_log.png'), width = 800, height = 800, units = 'px')
plot(samples_post_prob[round(length(samples_post_prob)/10):length(samples_post_prob)], xlab="Sample", ylab="(Log) Posterior Probability")
dev.off()

png(filename = paste(sep='', id, '_param_a_log.png'), width = 800, height = 800, units = 'px')
plot(samples_a[round(length(samples_a) / 10):length(samples_a)], xlab="Sample", ylab="Parameter a")
dev.off()

png(filename = paste(sep='', id, '_param_b_log.png'), width = 800, height = 800, units = 'px')
plot(samples_b[round(length(samples_b) / 10):length(samples_b)], xlab="Sample", ylab="Parameter b")
dev.off()

png(filename = paste(sep='', id, '_param_c_log.png'), width = 800, height = 800, units = 'px')
plot(samples_c[round(length(samples_c) / 10):length(samples_c)], xlab="Sample", ylab="Parameter c")
dev.off()

#Output the estimated parameter values
print(paste("a=", a))
print(paste("b=", b))
print(paste("c=", c))


#Generate some data using our estimated parameters to see if it is even close.
data2 <- data.frame(matrix(nrow=1000, ncol=2))
for(i in 1:1000){
  rand <- rnorm(1, 6.3, 1) #This is my guess as to what their values were for generating predictor values
  data2[i,1] = rand #Store as predictor
  data2[i,2] = cur_dist_r(1, cur_link(cur_det(rand,a,b)), c) #Generate response for that predictor
}

png(filename = paste(sep='', id, '_estimation.png'), width = 800, height = 800, units = 'px')
#Plot the original data
plot(x=data[,1], y=data[,2], col="blue", xlab="Predictor", ylab="Response")
#Add our new data
points(data2, col="red")
#Also add some linear regression lines
linreg1 = lm(data[,2] ~ data[,1])
linreg2 = lm(data2[,2] ~ data2[,1])
abline(a=linreg1$coefficients[1], b=linreg1$coefficients[2], col="blue")
abline(a=linreg2$coefficients[1], b=linreg2$coefficients[2], col="red")
dev.off()
print('Done!')  
