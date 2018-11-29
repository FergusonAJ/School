rm ./output/images/*
date
./CA_Classic
date
cd ./viz/
python3 classScrape.py 20
R < ./viz/classic.R --vanilla
cd ..
eog ./output/classic.png
