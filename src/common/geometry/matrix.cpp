/*****************************************************************************
	Emp

    Copyright (C) 2018  German Molina (germolinal@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include "./matrix.h"
#include <iostream>
#include <stdexcept>
#include "../utilities/io.h"
#include "tbb/tbb.h"


#define NROWS data.size()
#define NCOLS data[0].size()

Matrix::Matrix()
{
    data = std::vector< std::vector <float> >(1);
    data[0] = std::vector <float>(1);
}

Matrix::Matrix(size_t nrows, size_t ncols)
{
    data = std::vector< std::vector <float> >(nrows,std::vector <float>(ncols,0.0f));
}


void Matrix::print() const
{
    size_t nrows = NROWS;
    size_t ncols = NCOLS;
    
    for (size_t row = 0; row < nrows; row++) {
        for (int col = 0; col < ncols; col++) {
            std::cout << data[row][col] << "\t";
        }
        std::cout << "\n";
    }    
    std::cout << "--------\n";
    
}

size_t Matrix::ncols() const
{
    return NCOLS;
}

size_t Matrix::nrows() const
{
    return NROWS;
}


void Matrix::resize(size_t nrows, size_t ncols)
{
    data.resize(nrows);
    for(size_t nrow = 0; nrow < nrows; nrow++ ){
        data[nrow].resize(ncols,0.0);
    }
}

bool Matrix::multiply(const Matrix * m, Matrix * res) const
{
    // Check size consistency with m
    if (NCOLS != m->nrows())
        throw std::invalid_argument("Size mismatch between matrices when trying to multiply()");
    
    const size_t ncols = m->ncols();
    const size_t aux = m->nrows();
    
    // Check size consistency with res
    if(res->ncols() != m->ncols() || res->nrows() != NROWS){
        WARN(msg, "Size mismatch between resulting matrix and factors... resizing results");
        res->resize(NROWS,m->ncols());
    }
    
    // Multiply
    const size_t nrows = NROWS;
    
    // First for --> in rows
    tbb::parallel_for(tbb::blocked_range<size_t>(0, nrows),
                      [=](const tbb::blocked_range<size_t>& r1) {
                          for (size_t row = r1.begin(); row != r1.end(); ++row) {
                              
                              // Second for --> in cols
                              tbb::parallel_for(tbb::blocked_range<size_t>(0, ncols),
                                [=](const tbb::blocked_range<size_t>& r2) {
                                    for (size_t col = r2.begin(); col != r2.end(); ++col) {
                                        
                                        float v = 0;
                                        for (int i = 0; i < aux; i++) {
                                            v += (getElement(row,i) * m->getElement(i,col)); // CHECK THIS!
                                        }
                                        res->setElement(row,col,v);
                                        
                                        
                                }
                            },
                            tbb::auto_partitioner()
                            ); // end of loop in col
                              
                              
                          }
                      },
                      tbb::auto_partitioner()
      ); // end of loop in row
    
    return true;
}


bool Matrix::multiplyRowToColumn( const Matrix * vec, size_t row, size_t col, Matrix * res) const
{
    if(vec->nrows() <= row)
        throw std::invalid_argument("Trying to multiply by an exceeding row ("+ std::to_string(row) +") in multiplyRowToColumn()");
    
    if ( 1 != vec->ncols())
        throw std::invalid_argument("vector needs to have only one column multiplyRowToColumn()");
    
    // Check size consistency with m
    if (NCOLS != vec->nrows())
        throw std::invalid_argument("Size mismatch between matrices when trying to multiply()");
    
    // Check size consistency with res
    if(res->ncols() <= col || res->nrows() != NROWS){
        WARN(msg, "Size mismatch between resulting matrix and factors... resizing results");
        res->resize(NROWS,col);
    }
    
    // Multiply
    const size_t nrows = NROWS;
    const auto vecValue = vec->getElement(row,0);
                       
    // Loop in rows
    tbb::parallel_for(tbb::blocked_range<size_t>(0, nrows),
                      [=](const tbb::blocked_range<size_t>& r1) {
                          for (size_t i = r1.begin(); i != r1.end(); ++i) {
                              
                              float v = getElement(i,row) * vecValue;
                              res->setElement(i,col,v);
                              
                          }
                      },
                      tbb::auto_partitioner()
                      );// end of loop in rows

    
    return true;
}

bool Matrix::multiplyToColumn( const Matrix * vec, size_t col, Matrix * res) const
{
    if ( 1 != vec->ncols())
        throw std::invalid_argument("vector needs to have only one column multiplyToColumn()");
    
    // Check size consistency with m
    if (NCOLS != vec->nrows())
        throw std::invalid_argument("Size mismatch between matrices when trying to multiply()");
    
    // Check size consistency with res
    if(res->ncols() <= col || res->nrows() != NROWS){
        WARN(msg, "Size mismatch between resulting matrix and factors... resizing results");
        res->resize(NROWS,col);
    }
    
    // Multiply
    const size_t ncols = NCOLS;
    const size_t nrows = NROWS;
    
    // Loop in rows
    tbb::parallel_for(tbb::blocked_range<size_t>(0, nrows),
                      [=](const tbb::blocked_range<size_t>& r1) {
                          for (size_t row = r1.begin(); row != r1.end(); ++row) {
                              
                              float v = 0;
                              for (int i = 0; i < ncols; i++) {
                                  v += (getElement(row,i) * vec->getElement(i,0));
                              }
                              res->setElement(row,col,v);
              
          }
      },
      tbb::auto_partitioner()
      );// end of loop in rows
    
    
    return true;
}

void Matrix::setElement(size_t row, size_t col, float value)
{
    if ( row >= NROWS || col >= NCOLS )
        throw std::invalid_argument("Trying to set element out of range in MATRIX");
    
    data[row][col] = value;
}


float Matrix::getElement(size_t row, size_t col) const
{
    if ( row >= NROWS || col >= NCOLS )
        throw std::invalid_argument("Trying to get element out of range in MATRIX");
    
    return data[row][col];
}

#undef NROWS 
#undef NCOLS
