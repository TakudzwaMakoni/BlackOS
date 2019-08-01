//
// Created by Takudzwa Makoni on 2019-07-27.
//


#include "../inc/Directives.h"
#include "../inc/Kmenu.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

namespace {
const int PADDING = 1;
//WINDOW * DEFAULT_WINDOW = newwin(0,0,0,0);

}

using namespace BlackOSDisplay;



Kmenu::Kmenu(const size_t &id, std::string &name, int sizeY, int sizeX ,int posY, int posX) : m_id(id) {

    m_size = {sizeY,sizeX};
    m_position = {posY,posX};
    setWin();

}

/// return Window Object
WINDOW * Kmenu::window() const {
  return _win;
}
/// return fields in Kmenu instance
std::vector<Kfield> Kmenu::fields() const { return m_fields; }
/// return window size of instance
std::vector<int> Kmenu::size() const { return m_size; }
/// return window centre x position
int Kmenu::centreX() const {
    return m_size[1]/2;
}
int Kmenu::centreY() const {
    return m_size[0]/2;
}
/// return type of Kwindow instance
std::string Kmenu::winType() const { return "Kmenu"; }
/// return name of Kwindow instance
std::string Kmenu::name() const { return m_name; }
/// switch for cursor blinking
void Kmenu::cursor(const int &code) const { curs_set(code); }
/// set style of corners of BlackOS Window
void Kmenu::setCornerStyle(const int &ch) {
    wborder(_win, 0, 0, 0, 0, (int)ch, (int)ch, (int)ch, (int)ch);
}
/// set style for each corner of BlackOS Window
void Kmenu::setCornerStyle(const int &ch1, const int &ch2,
                           const int &ch3, const int &ch4) {
    wborder(_win, 0, 0, 0, 0, ch1, ch2, ch3, ch4);
    wrefresh(_win);
}
///set border style
void Kmenu::setBorderStyle(const int &ch){
    box(_win, ch, ch);
}
void Kmenu::setFields(const std::vector<Kfield> &fields){
    this->m_fields = fields;
}
/// get selected field
Kfield Kmenu::getSelectedField() const {
    return this->m_fields[_highlighted];}
/// set corner label of BlackOS Window
void Kmenu::setLabel(const std::string &label) const {
    int labellocy = m_size[0] - 1;
    int labellocx = m_size[1] - (3 + (int)label.length());
    mvwaddstr(_win, labellocy, labellocx, label.c_str());
}
size_t Kmenu::getID() const {return this->m_id;}
/// return the maximum size of the local window
std::vector<int> Kmenu::maxSize() const {
    int yMax, xMax;
    getmaxyx(_win, yMax, xMax);
    std::vector<int> size{yMax, xMax};
    return size;
}
/// set the position of the menu inside the local window
void Kmenu::setPos(Eigen::Vector2i &v){

}
/// set animation codes for window start and finish
void Kmenu::setAnimation(const int &start, const int &finish) const {
  m_startAnim = start;
  m_finishAnim = finish;
}
/// get position of window top left corner
Eigen::Vector2i Kmenu::position() const { return m_position;}
/// set the private window to a window
void Kmenu::setWin() {
  _win = newwin(m_size[0], m_size[1], m_size[2], m_size[3]);
}
/// add child display object to this instances private window
void Kmenu::addDisplayObj(BlackOSDisplay::Kwindow obj) const {
  std::vector<int> childSize = m_size;
  std::for_each(childSize.begin(), childSize.end(), [](const int &i){return i - PADDING;});
  //WINDOW * win = subwin(m_privateWin, childSize[0],childSize[1],childSize[2],childSize[3]);
}
/// display the menu onto private window
void Kmenu::display(){
setAnimation(0,0);
  cursor(0);

   setLabel(m_name);

    int yMax, xMax;
    getmaxyx(_win, yMax, xMax);
    move(yMax / 2, xMax / 2);

    keypad(_win, true);
    box(_win, 0, 0);

    int selection;
    int highlighted = 0;
    int numOfFields = m_fields.size();

    while (true) {
        for (int i = 0; i < numOfFields; i++) {
            if (i == highlighted)
                wattron(_win, A_REVERSE);

            int yAlign = 0;
            int xAlign = 0;
            int left, right, top, bottom, v_centre, h_centre;

            // left align
            left = 1;

            // right align
            int longest_string_len = 0;
            std::for_each(m_fields.begin(), m_fields.end(), [&longest_string_len](const Kfield &field){

                std::string str = field.name();

                int len = str.length();
                if (len > longest_string_len)
                    longest_string_len = len;

            });
            right = xMax - longest_string_len - 1;

            // centre align
            v_centre = i + (yMax / 2) - numOfFields;
            h_centre = (xMax - m_fields[i].name().length()) / 2;

            // top align
            top =  i + 1;
            bottom = yMax - 2  - numOfFields + i;

            if(m_xAlignment == 1 )
                xAlign = right;
            else if(m_xAlignment == 0)
                xAlign = h_centre;
            else if(m_xAlignment == -1)
                xAlign = left;

            if(m_yAlignment == 1)
                yAlign = top;
            else if(m_yAlignment == 0)
                yAlign = v_centre;
            else if (m_yAlignment == -1)
                yAlign = bottom;

            mvwprintw(_win, yAlign, xAlign, m_fields[i].name().c_str());
            wattroff(_win, A_REVERSE);
        }
        selection = wgetch(_win);

        switch (selection) {
            case KEY_UP:
                highlighted--;
                if (highlighted == -1) {
                    highlighted = 0;
                }
                break;
            case KEY_DOWN:
                highlighted++;
                if (highlighted == m_fields.size()) {
                    highlighted = (int)m_fields.size() - 1;
                }
            default:
                break;
        }
        if (selection == 10) {
            break;
        }
        
    }
    _highlighted = highlighted;
    clear();
    wrefresh(_win);

}

Kmenu::~Kmenu(){
    
    delwin(_win);
}


