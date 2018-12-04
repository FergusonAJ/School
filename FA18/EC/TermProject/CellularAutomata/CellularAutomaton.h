#pragma once

#include <iostream>
#include <sstream>
#include <functional>
#include <set>
#include <map>

#include "../source/base/vector.h"
#include "../source/base/Ptr.h"
#include "../source/tools/Random.h"
#include "../source/base/assert.h"
#include "../source/base/macros.h"

#include "./Definitions.h"

namespace empCA{

    struct Rect{
        size_t x, y, w, h;
        Rect(size_t x_, size_t y_, size_t w_, size_t h_){
            x = x_; y = y_; w = w_; h = h_;
        }
        std::string GetString(){
            std::ostringstream oss;
            oss << "(" << x << "," << y << "," << w << "," << h << ")";
            return oss.str();
        }
    };
    
    template <class CELL>
    class CellularAutomaton{
    public:
        using cell_t = emp::Ptr<CELL>;
        using func_update_t = std::function<CELL(CELL&, const emp::vector<cell_t> &)>;
        using func_spawn_t = std::function<CELL(emp::Random &)>;
        using func_print_t = std::function<std::string(CELL&)>;
        using func_cmp_t = std::function<bool(CELL&, CELL&)>;
        using func_alive_t = std::function<bool(CELL&)>;
        using func_from_char_t = std::function<CELL(char)>;
        using func_get_base_t = std::function<CELL()>;

    private:
        emp::vector<cell_t> field;
        emp::vector<cell_t> field_temp;
        size_t width, height;
        emp::Ptr<emp::Random> random_ptr;
        emp::vector<std::set<size_t>> groupData;
        emp::vector<std::set<size_t>> groupData_temp;
        size_t maxGroup;
        bool doGrouping=false;
        //Functions
        func_update_t func_update;
        func_spawn_t func_spawn;
        func_print_t func_print;
        func_cmp_t func_cmp;
        func_alive_t func_alive;
        func_from_char_t func_from_char;
        func_get_base_t func_get_base;

    public:
        CellularAutomaton<CELL>(emp::Random& random, size_t _width, size_t _height): 
            width(_width), height(_height){
            random_ptr = &random;
            field.resize(width * height);
            field_temp.resize(width * height);
            
        }
        
        CellularAutomaton<CELL>(size_t _width, size_t _height): 
            width(_width), height(_height){
            field.resize(width * height);
            field_temp.resize(width * height);
        }
        
        CellularAutomaton<CELL>(): 
            width(0), height(0){
        } 
        
        CellularAutomaton<CELL>(CellularAutomaton<CELL>& orig){
            width = orig.width;
            height = orig.height;
            doGrouping = orig.doGrouping;
            func_update    = orig.func_update;
            func_spawn     = orig.func_spawn;
            func_print     = orig.func_print;
            func_cmp       = orig.func_cmp;
            func_alive     = orig.func_alive;
            func_from_char = orig.func_from_char;
            func_get_base  = orig.func_get_base;
            field.resize(width * height);
            field_temp.resize(width * height);
            Populate(func_get_base());
        }

        ~CellularAutomaton(){ 
            Clear();
            field.resize(0);
            field_temp.resize(0);
           
        }

        void Resize(size_t _width, size_t _height){
            width = _width;
            height = _height;
            Clear();
            field.resize(width * height);
            field_temp.resize(width * height);
        }         
     
        void Update(){
            emp::vector<emp::Ptr<CELL>> neighbors;
            neighbors.resize(8);
            int left, right, up, down;
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    left = (x + width - 1) % width; 
                    right =  (x + width + 1)  % width;
                    up =  (y + height - 1) % height;
                    down = (y + height + 1) % height;
                    //left = (x > 0) ? (x - 1) : width - 1; 
                    //right = (x < width - 1) ? (x + 1) : 0;
                    //up =  (y > 0) ? (y - 1) : height - 1;
                    //down = (y < height - 1) ? (y + 1)  : 0;
                    neighbors[0] = field[x     + (up * width)];   //Top
                    neighbors[1] = field[right + (up * width)];   //Top Right
                    neighbors[2] = field[right + (y * width)];    //Right
                    neighbors[3] = field[right + (down * width)]; //Bottom Right
                    neighbors[4] = field[x     + (down * width)]; //Bottom
                    neighbors[5] = field[left  + (down * width)]; //Bottom Left
                    neighbors[6] = field[left  + (y * width)];    //Left
                    neighbors[7] = field[left  + (up * width)];   //Top Left
                    if(field_temp[x + y * width] == nullptr)
                        field_temp[x + y * width] = emp::NewPtr<CELL>(func_update(*field[x + y*width], neighbors)); 
                    else
                        *(field_temp[x + y * width]) = func_update(*field[x + y*width], neighbors); 
                }
            }
            //GroupUpdate();
            for(size_t i = 0; i < field.size(); i++){
                *(field[i]) = *(field_temp[i]);
            }
        }
        //Populate randomly
        void Populate(){
            for(size_t i = 0; i < field.size(); i++){
                field[i] = emp::NewPtr<CELL>(func_spawn(*random_ptr));
                field_temp[i] = emp::NewPtr<CELL>(*field[i]);
            }
            if(doGrouping)
                GroupStart();
        }
        //Populate with specific data
        void Populate(emp::vector<CELL>& data){
            //emp::assert(data.size() == width * height, "Data must have width * height elements!");
            for(int i = 0; i < field.size(); i++){
                field[i] = emp::NewPtr<CELL>(data[i]);
                field_temp[i] = emp::NewPtr<CELL>(*field[i]);
            }
            if(doGrouping)
                GroupStart();
        }
        //Populate with one value
        void Populate(CELL val){
            for(size_t i = 0; i < field.size(); i++){
                field[i] = emp::NewPtr<CELL>(val);
                field_temp[i] = emp::NewPtr<CELL>(*field[i]);
            }
            if(doGrouping)
                GroupStart();
        }

        void Print(std::ostream& stream = std::cout){
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    stream << func_print(*field[x + (width*y)]);
                }
                stream << "\n";
            }
        }    

        void PrintGroup(std::ostream& stream = std::cout){
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    stream << x << "x" << y << ": ";
                    for(long group : groupData[x + y*width])
                        stream << group << " ";
                    stream << "\n";
                }
            }
        }
        
        void PrintGroupFlat(std::ostream& stream = std::cout){
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    if(groupData[x + y * width].empty())
                        stream << ".";
                    else    
                        stream << *groupData[x + y*width].begin(); 
                }
                stream << "\n";
            }
        }
 
        void GroupUpdate(){
            /*
            if(!doGrouping)
                return;
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    int i = x + y * width;
                    groupData_temp[i].clear();
                    if(func_alive(*field_temp[i])){
                        int left =  (x != 0) ? x - 1 : width - 1; 
                        int right = (x != width - 1) ? x + 1 : 0;
                        int up =    (y != 0) ? y - 1 : height - 1;
                        int down =  (y != height - 1) ? y + 1 : 0;
                        groupData_temp[i].insert(groupData[x + up * height].begin(), 
                            groupData[x+up*height].end()); 
                        groupData_temp[i].insert(groupData[right + up * height].begin(), 
                            groupData[right+up*height].end()); 
                        groupData_temp[i].insert(groupData[right + y * height].begin(), 
                            groupData[right+y*height].end()); 
                        groupData_temp[i].insert(groupData[right + down * height].begin(), 
                            groupData[right+down*height].end()); 
                        groupData_temp[i].insert(groupData[x + down * height].begin(), 
                            groupData[x+down*height].end()); 
                        groupData_temp[i].insert(groupData[left + down * height].begin(), 
                            groupData[left+down*height].end()); 
                        groupData_temp[i].insert(groupData[left + y * height].begin(), 
                            groupData[left+y*height].end()); 
                        groupData_temp[i].insert(groupData[left + up * height].begin(), 
                            groupData[left+up*height].end()); 
                        if(groupData_temp[i].find(0) != groupData_temp[i].end())
                            groupData_temp[i].erase(0);
                    }
                }
            }
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    int i = x + y * width;
                    if(!func_alive(*field_temp[i])) //Skip "dead" cells
                        continue;
                    if(groupData_temp[i].find(0) != groupData_temp[i].end()) //Already checked
                        continue;
                    std::set<int> cells;
                    std::set<long> values;
                    GroupUpdateHelper(x, y, field_temp[i], cells, values);
                    for(int i : cells){
                        groupData_temp[i].clear();
                        groupData_temp[i].insert(0); //0 is our 'I have been checked' flag
                        for(long val : values){
                            groupData_temp[i].insert(val);
                        }
                    }
                }
            }
            for(size_t i = 0; i < groupData.size(); i++){
                groupData[i] = std::move(groupData_temp[i]);
            }
            */
        }
 
        void GroupUpdateHelper(size_t x, size_t y, cell_t B, std::set<int>& cells, std::set<long>& values){
            /*
            int i = x + y * width;
            //Skip cells that have already been checked
            if(cells.find(i) != cells.end())                 
                return;
            if(!func_alive(*field_temp[i])) //Skip "dead" cells
                return;
            if(func_cmp(*B, *field_temp[i])){
                cells.insert(i);
                for(long val : groupData_temp[i]){
                    values.insert(val);
                }
                int left =  (x != 0) ? x - 1 : width - 1; 
                int right = (x != width - 1) ? x + 1 : 0;
                int up =    (y != 0) ? y - 1 : height - 1;
                int down =  (y != height - 1) ? y + 1 : 0;
                GroupUpdateHelper(x,     up,   field_temp[i], cells, values); //Top
                GroupUpdateHelper(right, up,   field_temp[i], cells, values); //Top Right
                GroupUpdateHelper(right, y,    field_temp[i], cells, values); //Right
                GroupUpdateHelper(right, down, field_temp[i], cells, values); //Bottom Right
                GroupUpdateHelper(x,     down, field_temp[i], cells, values); //Bottom
                GroupUpdateHelper(left,  down, field_temp[i], cells, values); //Bottom Left
                GroupUpdateHelper(left,  y,    field_temp[i], cells, values); //Left
                GroupUpdateHelper(left,  up,   field_temp[i], cells, values); //Top Left
            }
            */
        }
 
        void GroupStartHelper(int x, int y, long val, cell_t B){
            int i = x + y * width; 
            if(!func_alive(*field[i])) //Skip "dead" cells
                return;
            if(!groupData[i].empty()) //Skip cells that have already been filled
                return;
            if(func_cmp(*B, *field[i])){
                groupData[i].insert(val);    
                int left =  (x - 1 + width) % width; 
                int right = (x + 1 + width) % width;
                int up =    (y - 1 + height) % height;
                int down =  (y + 1 + height) % height;
                GroupStartHelper(x,     up,   val,  field[i]); //Top
                GroupStartHelper(right, up,   val,  field[i]); //Top Right
                GroupStartHelper(right, y,    val,  field[i]); //Right
                GroupStartHelper(right, down, val,  field[i]); //Bottom Right
                GroupStartHelper(x,     down, val,  field[i]); //Bottom
                GroupStartHelper(left,  down, val,  field[i]); //Bottom Left
                GroupStartHelper(left,  y,    val,  field[i]); //Left
                GroupStartHelper(left,  up,   val,  field[i]); //Top Left
            }
        }
 
        void GroupStart(){
            long cur = 1;
            groupData.resize(field.size());
            //groupData_temp.resize(field.size());
            for(size_t i = 0; i < groupData.size(); i++){
                groupData[i].clear();  
            }
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    size_t i = x + y * width;
                    if(!func_alive(*field[i])) //Skip "dead" cells
                        continue;
                    if(!groupData[i].empty()) //Skip cells that have already been filled
                        continue;
                    size_t left =  (x - 1 + width) % width; 
                    size_t right = (x + 1 + width) % width;
                    size_t up =    (y - 1 + height) % height;
                    size_t down =  (y + 1 + height) % height;
                    GroupStartHelper(x,     up,   cur, field[i]); //Top
                    GroupStartHelper(right, up,   cur, field[i]); //Top Right
                    GroupStartHelper(right, y,    cur, field[i]); //Right
                    GroupStartHelper(right, down, cur, field[i]); //Bottom Right
                    GroupStartHelper(x,     down, cur, field[i]); //Bottom
                    GroupStartHelper(left,  down, cur, field[i]); //Bottom Left
                    GroupStartHelper(left,  y,    cur, field[i]); //Left
                    GroupStartHelper(left,  up,   cur, field[i]); //Top Left
                groupData[i].insert(cur);
                cur++;
                }
            }
            maxGroup = cur-1;
        }

        void CalculateGroupCentroids(std::ostream& stream){
            /*
            int xSum = 0;
            int ySum = 0;
            int count = 0;
            for(long g = 1; g <= maxGroup; g++){                
                for(int y = 0; y < height; y++){
                    for(int x = 0; x < width; x++){
                        if(groupData[x + y*width].find(g) != groupData[x+y*width].end()){
                            xSum += x;
                            ySum += y;
                            count++;
                        }
                    }
                }
                stream << g << " -> (" << ((float)xSum) / count << ", " << ((float)ySum) / count << ")" << std::endl;
                xSum = 0;
                ySum = 0;
                count = 0;
            }
            */
        }        

        void LoadFromFile(std::istream& stream, emp::vector<CELL>& data){
            std::string line;
            int idx = 0;
            while(std::getline(stream, line)){
                if(idx >= width * height)
                    return;
                for(char c : line){
                    if(c != '\n'){
                        data[idx] = func_from_char(c);
                        idx++;
                    }
                }
            }
        }

        void AddSubsurface(emp::vector<CELL>& dest, emp::vector<CELL>& src, int x, int y, int w){
            int h = src.size()/w;
            if(x + (y * width) + w * (h * width) > dest.size()){
                std::cout << "Must fit within dest vector!" << std::endl;
                return;
            }
            for(int j = 0; j < h; j++){
                for(int i = 0; i < w; i++){
                    dest[(x + y*width) + (i + j*width)] = src[i + j*w];
                }    
            } 
        }
        
        /*
        void AddSubsurface(emp::vector<CELL>& src, int x, int y, int w){
            int h = src.size()/w;
            if((x + w) + ((y+h-1) * width) > field.size()){
                std::cout << x << " " << y << " " << w << " " << h << std::endl; 
                std::cout << (x + w) + ((y+h) * width) << " > " << field.size(); 
                std::cout << "Must fit within dest vector!" << std::endl;
                return;
            }
            for(int j = 0; j < h; j++){
                for(int i = 0; i < w; i++){
                    int idx = (x + y*width) + (i + j*width);
                    *(field[idx]) = src[i + j*w];
                }    
            } 
        }
        */        

        void AddSubsurface(std::vector<CELL>& src, int x, int y, int w){
            int h = src.size()/w;
            if((x + w) + ((y+h-1) * width) > field.size()){
                std::cout << x << " " << y << " " << w << " " << h << std::endl; 
                std::cout << (x + w) + ((y+h) * width) << " > " << field.size(); 
                std::cout << "Must fit within dest vector!" << std::endl;
                return;
            }
            for(int j = 0; j < h; j++){
                for(int i = 0; i < w; i++){
                    int idx = (x + y*width) + (i + j*width);
                    *(field[idx]) = src[i + j*w];
                }    
            } 
        }

        void Reset(){
            CELL base = func_get_base();
            for(size_t i = 0; i < field.size(); i++){
                if(field[i] != nullptr){
                    //field[i].Delete();
                    *(field[i]) = base;
                }
                else
                    field[i] = emp::NewPtr<CELL>(base);
                if(field_temp[i] != nullptr){
                    //field_temp[i].Delete();
                    (*field_temp[i]) = base;
                }
                else
                    field_temp[i] = emp::NewPtr<CELL>(base);
            }
        }
                
        void Clear(){
            for(size_t i = 0; i < field.size(); i++){
                if(field[i] != nullptr){
                    field[i].Delete();
                    field[i] = nullptr; 
                }
                if(field_temp[i] != nullptr){
                    field_temp[i].Delete();
                    field_temp[i] = nullptr; 
                }
            }
        }

        void ShiftRight(){
            size_t xp = 0;
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    xp = (x - 1 + width) % width;
                    if(field_temp[x + y*width] == nullptr)
                        field_temp[x + y*width] = emp::NewPtr<CELL>(*field[xp + y*width]); 
                    else
                        *(field_temp[x + y*width]) = *(field[xp + y*width]); 
                }
            }
            for(size_t i = 0; i < field.size(); i++){
                *(field[i]) = *(field_temp[i]);
            }
        }        
        
        void ShiftDown(){
            size_t yp = 0;
            for(size_t y = 0; y < height; y++){
                yp = (y - 1 + height) % height;
                for(size_t x = 0; x < width; x++){
                    if(field_temp[x + y*width] == nullptr)
                        field_temp[x + y*width] = emp::NewPtr<CELL>(*field[x + yp*width]); 
                    else
                        *(field_temp[x + y*width]) = *(field[x + yp*width]); 
                }
            }
            for(size_t i = 0; i < field.size(); i++){
                *(field[i]) = *(field_temp[i]);
            }
        }        

        bool GetIsOnRight(){
            for(size_t y = 0; y < height; y++){
                if(*(field[(width - 1) + y * width]) == 1){
                    return true;
                }
            }
            return false;   
        } 
           
        bool GetIsOnBottom(){
            for(size_t x = 0; x < width; x++){
                if(*(field[x + (height - 1) * width]) == 1){
                    return true;
                }
            }
            return false;   
        }        

        void ClearRightAndBottom(){
            size_t count = 0;
            while(GetIsOnRight()){
                ShiftRight();
                count++;
                if(count >= width)
                    break;
            }
            count = 0;
            while(GetIsOnBottom()){
                ShiftDown();
                count++;
                if(count >= height)
                    break;
            }
        }

        Rect GetBounds(bool clear = true){
            if(clear)
                ClearRightAndBottom();
            size_t startX = width + 1;
            size_t startY = height + 1;
            size_t maxX = 0;
            size_t maxY = 0;
            for(size_t y = 0; y < height; y++){
                for(size_t x = 0; x < width; x++){
                    if(*(field[x + y * width]) == 1){
                        if(x < startX)
                            startX = x;
                        if(y < startY)
                            startY = y;
                        if(x > maxX)
                            maxX = x;
                        if(y > maxY)
                            maxY = y;
                    }
                }
            }
            if((maxX == 0 && startX != 0) || (maxY == 0 && startY != 0))
                return Rect(0,0,0,0);
            return Rect(startX, startY, maxX - startX + 1, maxY - startY + 1);
        }
        
        std::vector<CELL> GetMinimumVec(){
            Rect bounds = GetBounds(); 
            std::vector<CELL> vec;
            vec.resize(bounds.w * bounds.h, CELL());
            int idx = 0;
            for(size_t y = bounds.y; y < bounds.y + bounds.h; y++){
                for(size_t x = bounds.x; x < bounds.x + bounds.w; x++){
                    vec[idx] = *(field[x + y * width]);
                    idx++; 
                }
            }
            return vec;
        }
 
        void Center(){
            ClearRightAndBottom();
            Rect bounds = GetBounds();
            std::vector<CELL> vec = GetMinimumVec();
            CELL base = func_get_base();
            for(size_t i = 0; i < field.size(); i++){
                *(field[i]) = base;
            }
            size_t startX = (width / 2) - (bounds.w / 2);
            size_t startY = (height / 2) - (bounds.h / 2);
            for(size_t y = 0; y < bounds.h; y++){
                for(size_t x = 0; x < bounds.w; x++){
                    *(field[startX + x + (startY + y) * width]) = vec[x + y * bounds.w];
                }
            } 
        }

        size_t RelativeComparison(std::vector<CELL>& vec, size_t w, size_t h, bool clear = true){
            if(clear)
                ClearRightAndBottom();
            Rect selfBounds = GetBounds(false);
            bool useSelfXLim = selfBounds.w < w;
            bool useSelfYLim = selfBounds.h < h;
            size_t count = 0;
            for(size_t y = 0; y < ((useSelfYLim) ? selfBounds.h : h); y++){
                for(size_t x = 0; x < ((useSelfXLim) ? selfBounds.w : w); x++){
                    //std::cout << x << " " << y << std::endl;
                    //std::cout << selfBounds.GetString() << std::endl;
                    //std::cout << *(field[(selfBounds.x + x) + (selfBounds.y + y) * width]) << std::endl;
                    //std::cout << vec[x + y * w] << std::endl;
                    //std::cout << x << " " << y << std::endl;
                    if(*(field[(selfBounds.x + x) + (selfBounds.y + y) * width]) ==
                        vec[x + y * w])
                        count++;
                }
            }
            return count; 
        }

        //Note: This assumes w and h are smaller than the actual width and height
        bool RelativeComparisonPerfect(std::vector<CELL>& vec, size_t w, size_t h, bool clear=true){
            if(clear)
                ClearRightAndBottom();
            Rect selfBounds = GetBounds(false);
            return ((double)RelativeComparison(vec, w, h, clear) == w * h) && 
                (selfBounds.w == w && selfBounds.h == h);
        }

    
        size_t DoesRepeat(size_t maxUpdates = 10, bool canMove = true, bool mustMove = false){
            Center();
            Rect bounds = GetBounds();
            std::vector<CELL> vec = GetMinimumVec();
            for(size_t i = 0 ; i < maxUpdates; i++){
                Update();
                if(RelativeComparisonPerfect(vec, bounds.w, bounds.h, true)){
                    Rect newBounds = GetBounds(false);
                    if(mustMove){
                        if(newBounds.x != bounds.x || newBounds.y != bounds.y)
                            return i + 1;
                    } else if(!canMove){
                        if(newBounds.x == bounds.x && newBounds.y == bounds.y)
                            return i + 1;
                    }
                    else
                        return i + 1;
                }
            }
            return 0;
        }

        //Note: This requires a GroupStart prior
        CellularAutomaton<CELL> Quarantine(size_t index){
            CellularAutomaton<CELL> ca(*this);
            ca.Reset();
            for(size_t i = 0; i < field.size(); i++){
                if(!groupData[i].empty()){
                    if(*(groupData[i].begin()) == index)
                        *(ca.field[i]) = *(field[i]);
                }
            }
            return ca;
        }
        
        //Note: This requires a GroupStart prior
        std::vector<size_t> CheckAllRepeating(size_t maxUpdates = 10, bool canMove = true, bool mustMove = false){
            std::vector<size_t> vec;
            std::vector<CellularAutomaton<CELL>> quarantines;
            for(size_t i = 1; i <= maxGroup ; i++){
                if(Quarantine(i).DoesRepeat(maxUpdates, canMove, mustMove) != 0){
                    vec.push_back(i);
                }
            }
            return vec;
        }

        //Getters and Setters
        void SetUpdateFunc(const func_update_t & func){ func_update = func; }
        void SetSpawnFunc(const func_spawn_t & func){ func_spawn = func; }
        void SetPrintFunc(const func_print_t & func){ func_print = func; }
        void SetCompareFunc(const func_cmp_t & func){ func_cmp = func; } 
        void SetAliveFunc(const func_alive_t & func){ func_alive = func; }
        void SetFromCharFunc(const func_from_char_t & func){ func_from_char = func; }
        void SetGetBaseFunc(const func_get_base_t & func){ func_get_base = func; }
        void SetDoGrouping(bool b){ doGrouping = b; }


        void SetFunctions(CAFunctionStruct<CELL>& str){
            SetUpdateFunc(str.func_update);
            SetSpawnFunc(str.func_spawn);
            SetPrintFunc(str.func_print);
            SetCompareFunc(str.func_cmp);
            SetAliveFunc(str.func_alive);
            SetFromCharFunc(str.func_from_char);
            SetGetBaseFunc(str.func_get_base);
        }
        const emp::vector<cell_t>& GetField(){ return field; }
        const size_t GetWidth(){ return width; }
        const size_t GetHeight(){ return height; }
        const CELL GetAt(size_t x, size_t y){
            return *field[x + y * width];
        }
        void SetAt(size_t x, size_t y, CELL val){
            if(field[x + y * width] != nullptr)
                *(field[x + y * width]) = val;
            else
                field[x + y * width] = emp::NewPtr<CELL>(val);
        }
    };
    
}
