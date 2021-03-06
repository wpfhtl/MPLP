#ifndef MPLP_MATRIX_H
#define MPLP_MATRIX_H

#include <iostream>
#include <fstream>

#include <MPLP/mplp_config.h>

namespace mplpLib {

class Matrix;

class Vec
{
public:
    MPLPIndexType m_size;
    double *m_dat;
    double *m_ep;
    Vec(MPLPIndexType size)
    {
        m_size = size;
        if (size==0) return;
        m_dat= new double[size];
        m_ep = &m_dat[size];
    }
    ~Vec();
    inline MPLPIndexType size() {return m_size;}
    double & operator[](MPLPIndexType i) {return m_dat[i];}
    Vec & operator=(double val);
    Vec & operator=(Vec & v);
    Vec & operator*=(double val);
    Vec & operator/=(double val);
    Vec & operator+=(double val);
    Vec & operator-=(double val);
    Vec & operator+=(Vec & v);
    Vec & operator*=(Vec & v);
    Vec & operator/=(Vec & v);
    void MatMult(Matrix & m, Vec & v);
    void OuterProd(Vec &v, Matrix &m);   // m should be size this->Size,v.Size
    Vec & operator-=(Vec & v);
    Vec & BackDiv(Vec &v);
    double Max();
    double Min();
    void MaxMin(double &mx, double &mn);
    double Sum();
    /*  Vec & Log();
  Vec & Exp();*/
    /*  double KL_Dist(Vec & v);*/
    Vec & Inv();
    void SetSize(MPLPIndexType n);
    void Rand();
    void Read(char *fname);
    void Write(char *fname);
    void Print();
};


class Matrix
{
public:
    MPLPIndexType m_nrows;
    MPLPIndexType m_ncols;
    bool m_bDelete;
    double *m_raw_dat;
    double *m_ep;
    double **m_rows;
    Matrix(MPLPIndexType rows =0,  MPLPIndexType cols = 0);
    Matrix(MPLPIndexType rows, MPLPIndexType cols,double *p_dat);
    ~Matrix();
    void SetSize(MPLPIndexType rows, MPLPIndexType cols);
    inline MPLPIndexType nrows() {return m_nrows;}
    inline MPLPIndexType ncols() {return m_ncols;}
    inline double & operator()(MPLPIndexType r,MPLPIndexType c) {return m_rows[r][c];}
    Matrix & BackDiv(Matrix &m);
    Matrix & operator+=(Matrix &m);
    Matrix & operator-=(Matrix &m);
    Matrix & operator/=(Matrix &m);
    Matrix & operator*=(Matrix &m);
    void VecMult(Vec &m, Vec & v);
    Matrix & operator*=(double val);
    Matrix & operator/=(double val);
    Matrix & operator+=(double val);
    Matrix & operator=(double val);
    Matrix & operator=(Matrix &m);
    void Rand();
    void Read(char *fname);
    void Write(char *fname);
    void Exp();
    /*  double KL_Dist(Matrix & p);
  void Log();*/
    double Sum();
    double *GetRow(MPLPIndexType i) {return m_rows[i];}
    void GetCol(MPLPIndexType i, Vec &v);
    void GetRow(MPLPIndexType i, Vec &v);
    void SetRow(MPLPIndexType i, Vec & v);
    double SumRow(MPLPIndexType i);
    double SumCol(MPLPIndexType i);
    void SumRows(Vec &v);
    void SumCols(Vec & v);
    void ScaleRow(MPLPIndexType i,double s);
    // Scale the i'th row by v[i]
    Matrix & ScaleRowsByVec(Vec &v);
    Matrix & DivRowsByVec(Vec &v);
    void Transpose( Matrix & dst);
    void Print();
};

class SparseMatrix
{
public:
    double *m_dat;
    MPLPIndexType *m_rind;
    MPLPIndexType *m_cind;
    MPLPIndexType m_nrows;
    MPLPIndexType m_ncols;
    MPLPIndexType m_nelem;
    double m_thr;

    double GetElem(MPLPIndexType i, MPLPIndexType & row, MPLPIndexType & col) {row=m_rind[i]; col=m_cind[i]; return m_dat[i];}
    //	double KL_Dist(Matrix &p);
    void Read(char *fname, double thr);
    double SumRow(MPLPIndexType i);
    double SumCol(MPLPIndexType i);
};

inline double dot(double *a, double *b, MPLPIndexType n)
{
    double s=0;
    double *ap,*bp;
    double *aend=&a[n];
    for (ap=&a[0],bp=&b[0]; ap<aend; ap++, bp++)
        s+=*ap*(*bp);
    return s;
}

void mult_trans1(Matrix & A, Matrix & B, Matrix & C);
void mult_trans2(Matrix & A, Matrix & B, Matrix & C);
void mult_trans3(Matrix & A, Matrix & B, Matrix & C);
void CalcModel(Matrix & phis, Matrix & psis, Vec & a, Vec &b, Matrix & Model);
void CalcModelTrans(Matrix & phis, Matrix & psis, Vec & a, Vec &b, Matrix & Model);
void MatMaxEnt( Matrix & f_val, Matrix & f_exp, Matrix & p_proj, MPLPIndexType ep, Matrix & lambdas, Vec & zs);
void ConjGradMaxEnt( Matrix & f_val, Matrix & f_exp, Matrix & p_proj, MPLPIndexType ep, Matrix & lambdas, Vec & zs);
void MakeCondDistX(Matrix & pygx, Vec & px);
void VecMaxEnt( Matrix & f_val, Matrix & f_exp, Vec & p0, MPLPIndexType ep, Matrix & lambdas, Vec & zs, bool brows, double maxentthr);
double dot(double *a, double *b, MPLPIndexType n);
void ConjGradMaxEntVec( Matrix & f_val, Matrix & ref_p, Matrix & p_proj, MPLPIndexType ep, Matrix & lambdas, Vec & zs, bool brow);

} // namespace mplpLib

#endif
