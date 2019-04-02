parser <- function(line){
  the_data = numeric(0)
  count = 0
  for (x in line){
    l = strsplit(x, "\t")
    l = unlist(l)
    l = l[6]
    l = as.numeric(l)
    l = log10(l)
    the_data = append(the_data, l)
  }
  the_data <- replace(the_data, the_data<1, 1)
  return(the_data)
}

parser2 <- function(line){
  the_data <- rep(list(numeric()), 286)
  for (i in 1:length(line)){
    l = strsplit(line[i], "\t")
    l = unlist(l)
    l = l[3:length(l)]
    l = as.numeric(l)
    l = replace(l, l<1, 1)
    l = log10(l)
    for (j in 1:length(l)){
      the_data[[j]] = c(the_data[[j]], l[j])
    }
  }
  return(the_data)
}

index_to_mean <- function(my_index, my_mean){
  return(my_mean[my_index])
}

quantile_normalisation <- function(df){
  df_rank <- data.frame(apply(df,2,rank,ties.method="min"))
  df_sorted <- data.frame(apply(df, 2, sort))
  df_mean <- apply(df_sorted, 1, mean)
  
  df_final <- apply(df_rank, 2, index_to_mean, my_mean=df_mean)
  rownames(df_final) <- rownames(df)
  return(data.frame(df_final))
}

t_tester <- function(m1, m2){ #assumes input are matricies
  results <- matrix(ncol = 2)
  for ( i in 1:(length(m1[,1])-1) ){
    # print(i)
    result <- t.test(m1[i,], m2[i,])
    if (result[[3]] < 0.05){
      results <- rbind(results, c(i, result[[3]]))
    }
  }
  return(results)
}

WRS_tester <- function(m1, m2){
  results <- matrix(ncol = 2)
  for ( i in 1:(length(m1[,1])-1) ){
    result <- wilcox.test(m1[i,], m2[i,])
    if (result[[3]] < 0.05){
      results <- rbind(results, c(i, result[[3]]))
    }
  }
  return(results)
}

WRS_bonferroni <- function(m1, m2){
  results <- matrix(ncol = 2)
  significant <- matrix(ncol = 2)
  for ( i in 1:(length(m1[,1])-1) ){
    result <- wilcox.test(m1[i,], m2[i,])
    results <- rbind(results, c(i, result[[3]]))
  }
  results[,2] <- c(p.adjust(results[,2], "bonferroni", length(results[,1])))
  for (i in 1:length(results[,1])){
    if (results[i,2] < 0.05){
      significant <- rbind(significant, c(results[i,1], results[i,2]))
    }
  }
  return(significant)
}