rm ./output/images/*
date
./CA_Control
date
R < ./viz/fitness.R --vanilla
eog ./output/fitness.png
