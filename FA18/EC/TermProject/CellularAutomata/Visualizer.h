#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "./CellularAutomaton.h"

namespace empCA{
    template <class CELL>
    class Visualizer{
        public:
            using func_from_color_t = std::function<CELL(size_t, size_t, size_t)>;
            using func_get_color_t = std::function<uint32_t(CELL)>;
            using func_mouse_t = std::function<CELL()>;
        public:
            Visualizer():
                window(sf::VideoMode(1,1), "Cellular Automata Visualizer", sf::Style::Default){
            }

            ~Visualizer(){
            }
            void Init(size_t _screenWidth, size_t _screenHeight, CellularAutomaton<CELL>* ca){
                screenWidth = _screenWidth;
                screenHeight = _screenHeight;
                window.setSize(sf::Vector2u(screenWidth, screenHeight));
                sf::FloatRect area(0,0,screenWidth, screenHeight);
                window.setView(sf::View(area));
                window.setKeyRepeatEnabled(false);
                automaton = ca;
                numCols = automaton->GetWidth();
                numRows = automaton->GetHeight();
                cellWidth = screenWidth / numCols;
                cellHeight = screenHeight / numRows;
                paused = true;
                testVec.resize(9,0);
                testVec[0] = 1;
                testVec[1] = 1;
                testVec[2] = 1; //###
                testVec[3] = 0; //..#
                testVec[4] = 0; //.#.
                testVec[5] = 1;
                testVec[6] = 0;
                testVec[7] = 1;
                testVec[8] = 0;
            }
            
            void Run(){
                while(window.isOpen()){
                    HandleInput();
                    Render();
                    if(!paused){
                        automaton->Update();
                        if(time.asSeconds() > 0)
                            sf::sleep(time);
                    }
                }
            }
            void SaveImage(std::string filename){
                sf::Image img;
                img.create(numCols, numRows, sf::Color::Black);
                CELL val;
                for(size_t j = 0; j < numRows; j++){
                    for(size_t i = 0; i < numCols; i++){
                        val = automaton->GetAt(i,j);
                        if(colorMap.find(val) == colorMap.end()){
                            if(func_get_color != NULL){
                                uint32_t col = func_get_color(val);
                                uint8_t r = (col & (255 << 24)) >> 24;
                                uint8_t g = (col & (255 << 16)) >> 16;
                                uint8_t b = (col & (255 << 8)) >> 8;
                                uint8_t a = col & 255;
                                colorMap[val] = sf::Color(r,g,b,a);
                            }
                            else{
                                colorMap[val] = colors[colorIdx];
                                colorIdx = (colorIdx+1) % colors.size();
                            }
                        }
                        img.setPixel(i,j,colorMap[val]);
                    }
                }
                img.saveToFile(filename.c_str());
            }
            void LoadImage(std::string filename){
                sf::Image img;
                img.loadFromFile(filename);
                emp::vector<CELL> tmpVec;
                tmpVec.resize(numRows * numCols);
                sf::Color col;
                for(size_t y = 0; y < numRows; y++)
                {
                    for(size_t x = 0; x < numCols; x++)
                    {
                        col = img.getPixel(x,y);
                        tmpVec[x + y * numCols] = func_from_color(col.r, col.g, col.b);
                    }
                }
                automaton->Reset();
                automaton->AddSubsurface(tmpVec, 0, 0, numCols);
            }        

            void SetColor(CELL key, size_t r, size_t g, size_t b){
                colorMap[key] = sf::Color(r,g,b);
            }            
            void SetFromColorFunc(func_from_color_t& func){
                func_from_color = func;
            }
            void SetGetColorFunc(func_get_color_t& func){
                func_get_color = func;
            }
            void SetLeftMouseFunc(func_mouse_t& func){
                func_mouse_left = func;
            }
            void SetRightMouseFunc(func_mouse_t& func){
                func_mouse_right = func;
            }
            void SetMiddleMouseFunc(func_mouse_t& func){
                func_mouse_middle = func;
            }
            void SetFunctions(CAFunctionStruct<CELL>& str){
                SetFromColorFunc(str.func_from_color);
                SetGetColorFunc(str.func_get_color);
                SetLeftMouseFunc(str.func_mouse_left);
                SetRightMouseFunc(str.func_mouse_right);
                SetMiddleMouseFunc(str.func_mouse_middle);
            }
            void SetTimestep(float sec){
                time = sf::seconds(sec);
            }

        protected:
            sf::RenderWindow window;
            size_t screenWidth, screenHeight;
            size_t cellWidth, cellHeight;
            size_t numCols, numRows;
            sf::Event event;
            CellularAutomaton<CELL>* automaton;   
            std::map<CELL, sf::Color> colorMap;
            emp::vector<sf::Color> colors = {
                sf::Color::Black, 
                sf::Color::White,
                sf::Color::Red,
                sf::Color::Blue,
                sf::Color::Yellow};
            int colorIdx = 0;
            bool paused = true;
            sf::Vector2f cursorPos;
            sf::Vector2i mousePos;
            sf::Time time;

            std::vector<unsigned char> testVec;
    
            func_from_color_t func_from_color;
            func_get_color_t func_get_color;
            func_mouse_t func_mouse_left;
            func_mouse_t func_mouse_right;
            func_mouse_t func_mouse_middle;
        protected:
            void HandleInput(){
                auto t = std::time(nullptr);
                auto tm = *std::localtime(&t);
                std::ostringstream oss;
                while(window.pollEvent(event)){
                    if(event.type == sf::Event::Closed)
                        window.close();
                    else if(event.type == sf::Event::KeyPressed){
                        switch(event.key.code){
                            case(sf::Keyboard::Escape):
                                window.close();
                                break;
                            case(sf::Keyboard::Space):
                                paused = !paused; 
                                break;
                            case(sf::Keyboard::Return):
                                automaton->Update();
                                break;
                            case(sf::Keyboard::P):
                                for(int i = 0; i < 10; i++)
                                    automaton->Update();
                                break;
                            case(sf::Keyboard::R):
                                automaton->Reset();
                                break;
                            case (sf::Keyboard::N):
                                automaton->ShiftRight();
                                break;
                            case (sf::Keyboard::M):
                                automaton->ShiftDown();
                                break;
                            case (sf::Keyboard::B):
                                std::cout << 
                                    automaton->RelativeComparison(testVec, 3, 3) << 
                                    std::endl;
                                break;
                            case (sf::Keyboard::V):
                                std::cout << 
                                    automaton->RelativeComparisonPerfect(testVec, 3, 3) << 
                                    std::endl;
                                break;
                            case (sf::Keyboard::C):
                                std::cout << automaton->GetBounds().GetString() << std::endl;
                                break;
                            case (sf::Keyboard::Num1):
                                std::cout << automaton->DoesRepeat(20, false) << std::endl;
                                break;
                            case (sf::Keyboard::Num2):
                                std::cout << automaton->DoesRepeat(20, true) << std::endl;
                                break;
                            case (sf::Keyboard::Num3):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                break;
                            case (sf::Keyboard::Num4):
                                automaton->GroupStart();
                                automaton->Quarantine(1);
                                break;
                            case (sf::Keyboard::Num5):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                for(size_t val :  automaton->CheckAllRepeating(20, false)){
                                    std::cout << val << " ";
                                }
                                std::cout << std::endl;
                                break;
                            case (sf::Keyboard::Num6):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                for(size_t val :  automaton->CheckAllRepeating(20, true)){
                                    std::cout << val << " ";
                                }
                                std::cout << std::endl;
                                break;
                            case (sf::Keyboard::Num7):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                for(size_t val :  automaton->CheckAllRepeating(20, true, true)){
                                    std::cout << val << " ";
                                }
                                std::cout << std::endl;
                                break;
                            case (sf::Keyboard::Num8):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                std::cout << automaton->CheckAllRepeating(20, false).size();
                                std::cout << std::endl;
                                break;
                            case (sf::Keyboard::Num9):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                std::cout << automaton->CheckAllRepeating(20, true).size();
                                std::cout << std::endl;
                                break;
                            case (sf::Keyboard::Num0):
                                automaton->GroupStart();
                                automaton->PrintGroupFlat();
                                std::cout << automaton->CheckAllRepeating(20, true).size();
                                std::cout << std::endl;
                                break;
                            case (sf::Keyboard::X):
                                automaton->Center();
                                automaton->Print();
                                std::cout << std::endl;
                                break;
                            case(sf::Keyboard::S):
                                //Getting datetime string via
                                //https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
                                oss << std::put_time(&tm, "%d_%m_%Y_%H_%M_%S") << ".png";
                                SaveImage(oss.str());
                                break;
                            default:
                                break;
                        }
                    }
                }
                mousePos = sf::Mouse::getPosition(window);
                cursorPos.x = mousePos.x - (mousePos.x % cellWidth);
                cursorPos.y = mousePos.y - (mousePos.y % cellHeight);
                if(mousePos.y >= 0 && mousePos.y < (int)screenHeight 
                        && mousePos.x >= 0 && mousePos.x <= (int)screenWidth){
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                        automaton->SetAt(cursorPos.x / cellWidth, cursorPos.y / cellHeight,
                            func_mouse_left());
                    }
                    else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                        automaton->SetAt(cursorPos.x / cellWidth, cursorPos.y / cellHeight,
                            func_mouse_right());
                    }
                    else if(sf::Mouse::isButtonPressed(sf::Mouse::Middle)){
                        automaton->SetAt(cursorPos.x / cellWidth, cursorPos.y / cellHeight,
                            func_mouse_middle());
                    }
                }
            }
            void Render(){
                sf::RectangleShape rect(sf::Vector2f(cellWidth, cellHeight));
                window.clear();
                CELL val;
                for(size_t j = 0; j < numRows; j++){
                    for(size_t i = 0; i < numCols; i++){
                        val = automaton->GetAt(i,j);
                        if(colorMap.find(val) == colorMap.end()){
                            if(func_get_color != NULL){
                                uint32_t col = func_get_color(val);
                                uint8_t r = (col & (255 << 24)) >> 24;
                                uint8_t g = (col & (255 << 16)) >> 16;
                                uint8_t b = (col & (255 << 8)) >> 8;
                                uint8_t a = col & 255;
                                colorMap[val] = sf::Color(r,g,b,a);
                            }
                            else{
                                colorMap[val] = colors[colorIdx];
                                colorIdx = (colorIdx+1) % colors.size();
                            }
                        }
                        rect.setFillColor(colorMap[val]);
                        rect.setPosition(sf::Vector2f(i * cellWidth, j * cellHeight));
                        window.draw(rect);
                    }
                }
                window.display();
                rect.setFillColor(sf::Color(0,150,200,255));
                rect.setPosition(cursorPos);
            } 
    }; 
}
