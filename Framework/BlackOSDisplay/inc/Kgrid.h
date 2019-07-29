//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KGRID_H
#define BLACKOS_KGRID_H

#include "Eigen/Dense"
#include "Kwindow.h"
#include <string>

namespace BlackOSDisplay{

    /// BlackOS Grid Object
    template <typename T, int rows, int cols> class Kgrid : public Kwindow  {
    private:
        const std::string m_name;
        Eigen::Matrix<T, rows, cols> m_matrix;
    public:

        Kgrid(std::string &name, Eigen::Matrix<T, rows, cols> matrix)
                : m_name(name), m_matrix(matrix) {}
        Eigen::Matrix<T, rows, cols> matrix() const { return m_matrix; }
        void write(std::vector<T> &data) {
            int expectedSize = rows * cols;
            if (data.size() != expectedSize) {
                throw std::exception("invalid size for matrix " + m_name);
            }
            Eigen::Map<Eigen::Matrix<T, rows, cols>> matrix(&data);
            m_matrix = matrix;
        }
    };
} // namespace BlackOSDisplay

#endif //BLACKOS_KGRID_H
