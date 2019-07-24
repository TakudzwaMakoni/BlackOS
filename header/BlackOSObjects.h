//
// Created by Takudzwa Makoni on 2019-07-20.
//

#ifndef BLACKOS_BLACKOSOBJECTS_H
#define BLACKOS_BLACKOSOBJECTS_H

#include "Eigen/dense"
#include "ncurses.h"
#include "BlackOSScripts.h"

#include <string>
#include <vector>
#include <memory>

namespace BlackOSObjects{

    typedef void (*directive)();

    class Kfield {

    public:
        Kfield(std::string &name,  std::string &message, directive script);

        std::string name() const;
        directive script() const;
        std::string message() const;

    private:
        const std::string m_name;
        const directive m_script;
        const std::string m_message;
    };


    class Kmenu {

    public:
        Kmenu(std::string &name, std::vector<Kfield> &fields, std::vector<int> &size);

        std::vector<Kfield> fields() const;
        std::vector<int> size() const;
        std::string name() const;

    private:
        const std::vector<Kfield> m_fields;
        const std::vector<int> m_size;
        const std::string m_name;
    };

    class Kwindow {

    public:
        explicit Kwindow( const std::vector<int> &size, std::unique_ptr<Kmenu> &menu,
                          int &xAlignment , int &yAlignment);

        WINDOW *window() const;
        void setAnimation(int start, int finish);
        static void cursor(int code);

        void display();


        ~Kwindow();

    private:
        WINDOW *m_window;
        int m_start, m_end;
      std::unique_ptr<Kmenu>  m_menu;
      const std::vector<int> &m_size;
      const int m_xAlignment;
      const int m_yAlignment;

    };

template <typename T , int rows ,int cols>
class Kgrid{
private:

  const std::string m_name;
  Eigen::Matrix<T, rows, cols> m_matrix;

public:
  Kgrid(std::string &name, Eigen::Matrix<T, rows, cols> matrix ) : m_name(name), m_matrix(matrix){}
  Eigen::Matrix<T, rows, cols> matrix() const{
    return m_matrix;
  }
  void write(std::vector<T> &data){
    int expectedSize = rows * cols;
    if(data.size()!= expectedSize){
      throw std::exception("invalid size for matrix " + m_name);
    }
    Eigen::Map<Eigen::Matrix<T, rows, cols>> matrix(&data);
    m_matrix = matrix;
  }
  void display(){

  }
};

} // Namespace BlackOSObjects

#endif //BLACKOS_BLACKOSOBJECTS_H
