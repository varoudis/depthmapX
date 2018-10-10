// genlib - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2018, Christian Sailer

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <stdexcept>

namespace depthmapX {
template<typename T> class BaseMatrix {
protected:
    BaseMatrix(size_t rows, size_t columns ){
        m_data = new T[rows * columns];
        m_rows = rows;
        m_columns = columns;
    }

    BaseMatrix<T>(const BaseMatrix<T> &other) : BaseMatrix<T>(other.rows, other,columns){
        std::copy(m_data, other.begin(), other.end());
    }

    BaseMatrix<T>(BaseMatrix<T> &&other): m_data(other.m_data), m_rows(other.m_rows), m_columns(other.m_columns){
        other.m_data = nullptr;
        other.m_rows = 0;
        other.m_columns =0;
    }

public:
    virtual ~BaseMatrix<T>(){
        delete[] m_data;
    }

    virtual T & operator()(size_t row, size_t column) = 0;
    virtual T const & operator()(size_t row, size_t column) const = 0;

    T* begin(){
        return m_data;
    }

    T* end(){
        return m_data + size();
    }

    T const * begin() const {
        return m_data;
    }

    T const * end() const {
        return m_data + size();
    }

    size_t size() const {
        return m_rows * m_columns;
    }

    size_t rows() const {
        return m_rows;
    }

    size_t columns() const {
        return m_columns;
    }

protected:
    T* m_data;
    size_t m_rows;
    size_t m_columns;

    void access_check( size_t row, size_t column) const {
        if ( row < 0 || row >= m_rows ){
            throw std::out_of_range("row out of range");
        }
        if ( column < 0 || column >= m_columns){
            throw std::out_of_range("column out of range");
        }

    }
};

template<typename T> class RowMatrix : public BaseMatrix<T>{
public:
    RowMatrix(size_t rows, size_t columns) : BaseMatrix<T>(rows, columns){}
    RowMatrix(RowMatrix const & other) : BaseMatrix(other){}
    RowMatrix(RowMatrix && other) : BaseMatrix(other){}

    T & operator () (size_t row, size_t column){
        access_check(row, column);
        return m_data[column + row * m_columns];
    }

    T const & operator ()(size_t row, size_t column) const {
        access_check(row, column);
        return m_data[column + row * m_columns];
    }
};

}
