//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KGRID_H
#define BLACKOS_KGRID_H

#include "Eigen/Dense"
#include "Kwindow.h"
#include <string>
#include <iomanip>
#include <sstream>


double pi = 3.14159265359;


namespace BlackOSDisplay{

    /// BlackOS Grid Object
    template <typename dataType, int rows, int cols> class Kgrid : public Kwindow  {
    private:
        const std::string _name;
        Eigen::Matrix<dataType, rows, cols> _matrix;
        WINDOW * _win;
        std::vector<int> _size;
        int _xAlign = 0;
        int _yAlign = 0;
        Eigen::Vector2i _position;
        int _precision;
        int _vPadding{2};
        bool _showGrid = true;

        int _highlightedRow{-1};
        int _highlightedCol{-1};
        const size_t _id; // TODO: need a NODE MAP TO NAVIGATE BETWEEN MENUS
        mutable int m_startAnim, m_finishAnim;
        virtual void setWin() override{
            _win = newwin(_size[0], _size[1], _position[0], _position[1]);
        }
        ///
        void write(Eigen::Matrix<dataType, rows, cols> &data) {
            int expectedSize = rows * cols;
            if (data.size() != expectedSize) {
                throw std::exception();
            }
            _matrix = data;
        }
        
    public:
        
        /// display Kgrid
        virtual void display() override {
            /// TODO MAKE GRID
            
            setAnimation(0,0);// TODO: not in use
            
            keypad(_win, true);
            int selection;
            int highlightedRow = 0;
            int highlightedCol = 0;

            while (true) {
                
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j){
                        if (j == highlightedCol && i == highlightedRow){
                            wattron(_win, A_REVERSE);
                            }
                    std::stringstream stream;
                        stream << std::setprecision(_precision)<< std::fixed <<_matrix.coeff(i, j);
                    stream.precision(_precision);
                    auto str = stream.str();
                        if(_showGrid){
                            mvwprintw(_win, i*_vPadding, (_precision+5)*j, ("|" + str + "|").c_str());
                        } else {
                    mvwprintw(_win, i*_vPadding, (_precision+3)*j, str.c_str());
                        }
                        wattroff(_win, A_REVERSE);
                    }
                }
                selection = wgetch(_win);
                switch (selection) {
                    case KEY_UP:
                        highlightedRow--;
                        if (highlightedRow == - 1) {
                            highlightedRow = 0;
                        }
                        break;
                    case KEY_DOWN:
                        highlightedRow++;
                        if (highlightedRow == rows) {
                            highlightedRow = rows - 1;
                        }
                        break;
                    case KEY_LEFT:
                        highlightedCol--;
                        if (highlightedCol == - 1) {
                            highlightedCol = 0;
                        }
                        break;
                    case KEY_RIGHT:
                        highlightedCol++;
                        if (highlightedCol == cols) {
                            highlightedCol = cols - 1;
                        }
                        break;
                    default:
                        break;
                }
                if (selection == 10) {
                    break;
                }
            }
            _highlightedRow = highlightedRow;
            _highlightedCol = highlightedCol;
            clear();
            wrefresh(_win);
        }
        /// return Window Object
        virtual WINDOW * window() const override{
            return this->_win;
        }
        /// set animation on start and finish
        virtual void setAnimation(const int &start, const int &finish) const override{
            m_startAnim = start;
            m_finishAnim = finish;
        }
        /// set style of BlackOS Window border
        virtual void setBorderStyle(const int &ch = 0) override{
            wborder(_win, ch, ch, ch, ch, ch, ch, ch, ch);
            wrefresh(_win);
        }
        /// set style of BlackOS Window border
        virtual void setBorderStyle(const int &L, const int &R,
                                    const int &T, const int &B,const int &TL, const int &TR,
                                    const int &BL, const int &BR) override {
            wborder(_win, L, R, T, B, TL, TR, BL, BR);
            wrefresh(_win);
        }
        ///
        virtual void setLabel(const std::string &label) const override{
            int labellocy = _size[0] - 1;
            int labellocx = _size[1] - (3 + (int)label.length());
            mvwaddstr(_win, labellocy, labellocx, label.c_str());
        }
        ///
        virtual std::vector<int> maxSize() const override {
            int yMax, xMax;
            getmaxyx(_win, yMax, xMax);
            std::vector<int> size{yMax, xMax};
            return size;
        }
        virtual std::string winType() const override{ return "Kgrid";}
        virtual std::string name() const override{return this->_name;}
        ///
        Kgrid(std::string &name,Eigen::Matrix<dataType, rows, cols> &data, std::vector<int> size)
                : _name(name), _id(0){
                    this->write(data);
                    _size = size;
                    setWin();
                }
        ///
        Eigen::Matrix<dataType, rows, cols> matrix() const { return _matrix; }
        ///
        size_t getID() const {
            return this->_id;
        }
        ///
        int centreX() const {
            return _size[1]/2;
        }
        int centreY() const {
            return _size[0]/2;
        }
        void setPrecision(int precision){
            _precision  = precision;
        }
        void setGrid(bool show){
            _showGrid = show;
        }
    };
} // namespace BlackOSDisplay

#endif //BLACKOS_KGRID_H
