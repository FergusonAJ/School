#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class Trail{
    public:
        Trail():
            totalFood(0),
            width(0),
            height(0){
        }
        Trail(const Trail& original){
            width = original.GetWidth();
            height = original.GetHeight();
            totalFood = original.GetTotalFood();
            field = original.GetFieldCopy(); 
        }

        void LoadFromFile(std::string filename, size_t width_, size_t height_){
            width = width_;
            height = height_;
            totalFood = 0;
            std::ifstream stream;
            stream.open(filename, std::ios::in);
            std::string line;

            //Correctly size the vector
            field.resize(width * height, 0);

            int rowNum = 0;
            int colNum = 0;
            while(std::getline(stream, line)){
                colNum = 0;
                for(int i = 0; i < line.length(); i++){
                    if(line[i] == '#' || line[i] == '1'){
                        field[colNum + rowNum * width] = 1;
                        colNum++;
                        totalFood++;
                    }
                    else if(line[i] == '.' || line[i] == '0'){
                        field[colNum + rowNum * width] = 0;
                        colNum++;
                    }
                    if(colNum > width){
                        std::cerr << "Error! Incorrectly sized trial!\nExpected width of " << width;
                        std::cerr << " was exceeded!" << std::endl;
                    }
                }
                rowNum++;
            }
            stream.close();
        }
        
        size_t GetWidth() const{ return width;}
        size_t GetHeight() const { return height;}
        size_t GetTotalFood() const{ return totalFood;}
        unsigned char GetAt(size_t x, size_t y){
            x %= width;
            y %= height;
            return field[x + y * width];
        }
        void SetAt(size_t x, size_t y, unsigned char val=0){
            x %= width;
            y %= height;
            field[x + y * width] = val;
        }
        std::vector<unsigned char> GetFieldCopy() const{
            return std::vector<unsigned char>(field);
        }
    private:
        std::vector<unsigned char> field;
        size_t totalFood;
        size_t width;
        size_t height;
};
