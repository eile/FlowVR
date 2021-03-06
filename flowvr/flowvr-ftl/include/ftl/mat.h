/******* COPYRIGHT ************************************************
*                                                                 *
*                             FlowVR                              *
*                       Template Library                          *
*                                                                 *
*-----------------------------------------------------------------*
* COPYRIGHT (C) 2003-2011                by                       *
* INRIA.  ALL RIGHTS RESERVED.                                    *
*                                                                 *
* This source is covered by the GNU LGPL, please refer to the     *
* COPYING file for further information.                           *
*                                                                 *
*-----------------------------------------------------------------*
*                                                                 *
*  Original Contributors:                                         *
*    Jeremie Allard,                                              *
*    Clement Menier.                                              *
*                                                                 * 
*******************************************************************
*                                                                 *
* File: include/ftl/mat.h                                         *
*                                                                 *
* Contacts: 20/09/2005 Clement Menier <clement.menier.fr>         *
*                                                                 *
******************************************************************/
#ifndef FTL_MAT_H
#define FTL_MAT_H

#include "vec.h"

namespace ftl
{

template <int L, int C, class real=float>
class Mat : public fixed_array<Vec<C,real>,L>
{
 public:

  static Type::Type getType() { return (Type::Type)Type::matrix(Type::get(real()),C,L); }

  enum { N = L*C };

  typedef Vec<C,real> Line;
  typedef Vec<L,real> Col;

  Mat()
  {
  }

  /// Specific constructor with a single line.
  Mat(Line r1)
  {
    BOOST_STATIC_ASSERT(L == 1);
    this->elems[0]=r1;
  }

  /// Specific constructor with 2 lines.
  Mat(Line r1, Line r2)
  {
    BOOST_STATIC_ASSERT(L == 2);
    this->elems[0]=r1;
    this->elems[1]=r2;
  }

  /// Specific constructor with 3 lines.
  Mat(Line r1, Line r2, Line r3)
  {
    BOOST_STATIC_ASSERT(L == 3);
    this->elems[0]=r1;
    this->elems[1]=r2;
    this->elems[2]=r3;
  }

  /// Specific constructor with 4 lines.
  Mat(Line r1, Line r2, Line r3, Line r4)
  {
    BOOST_STATIC_ASSERT(L == 4);
    this->elems[0]=r1;
    this->elems[1]=r2;
    this->elems[2]=r3;
    this->elems[3]=r4;
  }
  
  /// Constructor from an array of elements (stored per line).
  template<typename real2>
  explicit Mat(const real2* p)
  {
    std::copy(p, p+N, this->begin()->begin());
  }

  /// Assignment from an array of elements (stored per line).
  void operator=(const real* p)
  {
    std::copy(p, p+N, this->begin()->begin());
  }

  /// Assignment from a matrix of different size.
 template<int L2, int C2,typename real2> void operator=(const Mat<L2,C2,real2>& m)
  {
    std::copy(m.begin(), m.begin()+(L>L2?L2:L), this->begin());
  }

  /// Sets each element to 0.
  void clear()
  {
    for (int i=0;i<L;i++)
      this->elems[i].clear();
  }

  /// Sets each element to r.
  void fill(real r)
  {
    for (int i=0;i<L;i++)
      this->elems[i].fill(r);
  }

  /// Read-only access to line i.
  const Line& line(int i) const
  {
    return this->elems[i];
  }

  /// Copy of column j.
  Col col(int j) const
  {
    Col c;
    for (int i=0;i<L;i++)
      c[i]=this->elems[i][j];
    return c;
  }

  /// Write access to line i.
  Line& operator[](int i)
  {
    return this->elems[i];
  }

  /// Read-only access to line i.
  const Line& operator[](int i) const
  {
    return this->elems[i];
  }

  /// Write acess to line i.
  Line& operator()(int i)
  {
    return this->elems[i];
  }

  /// Read-only access to line i.
  const Line& operator()(int i) const
  {
    return this->elems[i];
  }

  /// Write access to element (i,j).
  real& operator()(int i, int j)
  {
    return this->elems[i][j];
  }

  /// Read-only access to element (i,j).
  const real& operator()(int i, int j) const
  {
    return this->elems[i][j];
  }

  /// Cast into a standard C array of lines (read-only).
  const Line* lptr() const
  {
    return this->elems;
  }

  /// Cast into a standard C array of lines.
  Line* lptr()
  {
    return this->elems;
  }

  /// Cast into a standard C array of elements (stored per line) (read-only).
  const real* ptr() const
  {
    return this->elems[0].ptr();
  }

  /// Cast into a standard C array of elements (stored per line).
  real* ptr()
  {
    return this->elems[0].ptr();
  }

  /// Special access to first line.
  Line& x() { BOOST_STATIC_ASSERT(L >= 1); return this->elems[0]; }
  /// Special access to second line.
  Line& y() { BOOST_STATIC_ASSERT(L >= 2); return this->elems[1]; }
  /// Special access to third line.
  Line& z() { BOOST_STATIC_ASSERT(L >= 3); return this->elems[2]; }
  /// Special access to fourth line.
  Line& w() { BOOST_STATIC_ASSERT(L >= 4); return this->elems[3]; }

  /// Special access to first line (read-only).
  const Line& x() const { BOOST_STATIC_ASSERT(L >= 1); return this->elems[0]; }
  /// Special access to second line (read-only).
  const Line& y() const { BOOST_STATIC_ASSERT(L >= 2); return this->elems[1]; }
  /// Special access to thrid line (read-only).
  const Line& z() const { BOOST_STATIC_ASSERT(L >= 3); return this->elems[2]; }
  /// Special access to fourth line (read-only).
  const Line& w() const { BOOST_STATIC_ASSERT(L >= 4); return this->elems[3]; }

  /// Set matrix to identity.
  void identity()
  {
    BOOST_STATIC_ASSERT(L == C);
    clear();
    for (int i=0;i<L;i++)
      this->elems[i][i]=1;
  }

  /// Set matrix as the transpose of m.
  void transpose(const Mat<C,L,real> &m)
  {
    for (int i=0;i<L;i++)
      for (int j=0;j<C;j++)
	this->elems[i][j]=m[j][i];
  }

  /// Transpose current matrix.
  void transpose()
  { BOOST_STATIC_ASSERT(L == C);
    for (int i=0;i<L;i++)
      for (int j=i+1;j<C;j++)
      {
	real t = this->elems[i][j];
	this->elems[i][j] = this->elems[j][i];
	this->elems[j][i] = t;
      }
  }

  // LINEAR ALGEBRA

  /// Matrix multiplication operator.
  template <int P>
  Mat<L,P,real> operator*(const Mat<C,P,real>& m) const
  {
    Mat<L,P,real> r;
    for(int i=0;i<L;i++)
      for(int j=0;j<P;j++)
      {
	r[i][j]=(*this)[i][0] * m[0][j];
	for(int k=1;k<C;k++)
	  r[i][j] += (*this)[i][k] * m[k][j];
      }
    return r;
  }

  /// Matrix addition operator.
  Mat<L,C,real> operator+(const Mat<L,C,real>& m) const
  {
    Mat r;
    for(int i = 0; i < L; i++)
      r[i] = (*this)[i] + m[i];
    return r;
  }

  /// Matrix subtraction operator.
  Mat<L,C,real> operator-(const Mat<L,C,real>& m) const
  {
    Mat r;
    for(int i = 0; i < L; i++)
      r[i] = (*this)[i] - m[i];
    return r;
  }
 
  /// Matrix negation
  Mat<L,C,real> operator-() const
  {
    Mat r;
    for(int i = 0; i < L; i++)
      r[i] = -(*this)[i];
    return r;
  }

  /// Multiplication operator Matrix * Column.
  Col operator*(const Line& v) const
  {
    Col r;
    for(int i=0;i<L;i++)
    {
      r[i]=(*this)[i][0] * v[0];
      for(int j=1;j<C;j++)
	r[i] += (*this)[i][j] * v[j];
    }
    return r;
  }

  /// Scalar multiplication operator.
  Mat<L,C,real> operator*(real f) const
  {
    Mat<L,C,real> r;
    for(int i=0;i<L;i++)
      for(int j=0;j<C;j++)
	r[i][j] = (*this)[i][j] * f;
    return r;
  }

  /// Scalar multiplication assignment operator.
  void operator *=(real r)
  {
    for(int i=0;i<L;i++)
      this->elems[i]*=r;
  }

  /// Addition assignment operator.
  void operator +=(const Mat<L,C,real>& m)
  {
    for(int i=0;i<L;i++)
      this->elems[i]+=m[i];
  }

  /// Determinant of the matrix.
  /// @attention determinant currently only implemented for 3x3 matrices.
  real determinant() const
  {
    BOOST_STATIC_ASSERT(L == 3);
    BOOST_STATIC_ASSERT(C == 3);
    return (*this)(0,0)*(*this)(1,1)*(*this)(2,2)
         + (*this)(1,0)*(*this)(2,1)*(*this)(0,2)
         + (*this)(2,0)*(*this)(0,1)*(*this)(1,2)
         - (*this)(0,0)*(*this)(2,1)*(*this)(1,2)
         - (*this)(1,0)*(*this)(0,1)*(*this)(2,2)
         - (*this)(2,0)*(*this)(1,1)*(*this)(0,2);
  }

  /// Invert matrix and stores the result in this matrix
  bool invert(const Mat<L,C,real>& m)
  {
    return invertMatrix(*this,m);
  }

  bool setAsInverseOf(const Mat<L,C,real>& m)
  {
	  return invertMatrix(*this,m);
  }

};

/// Matrix inversion (general case).
template<int N, class real>
extern __inline__ bool invertMatrix(Mat<N,N,real>& dest, const Mat<N,N,real>& from)
{
  int i, j, k;
  Vec<N,int> r, c, row, col;

  Mat<N,N,real> m1 = from;
  Mat<N,N,real> m2;
  m2.identity();

  for ( k = 0; k < N; k++ )
  {
    // Choosing the pivot
    real pivot = 0;
    for (i = 0; i < N; i++)
    {
      if (row[i])
	continue;
      for (j = 0; j < N; j++)
      {
	if (col[j])
	  continue;
	real t = m1[i][j]; if (t<0) t=-t;
	if ( t > pivot)
	{
	  pivot = t;
	  r[k] = i;
	  c[k] = j;
	}
      }
    }

    if (pivot <= 1e-10)
    {
      return false;
    }

    row[r[k]] = col[c[k]] = 1;
    pivot = m1[r[k]][c[k]];

    // Normalization
    m1[r[k]] /= pivot; m1[r[k]][c[k]] = 1;
    m2[r[k]] /= pivot;

    // Reduction
    for (i = 0; i < N; i++)
    {
      if (i != r[k])
      {
	real f = m1[i][c[k]];
	m1[i] -= m1[r[k]]*f; m1[i][c[k]] = 0;
	m2[i] -= m2[r[k]]*f;
      }
    }
  }

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      if (c[j] == i)
	row[i] = r[j];

  for ( i = 0; i < N; i++ )
    dest[i] = m2[row[i]];

  return true;
}

/// Matrix inversion (special case 3x3).
template<class real>
extern __inline__ bool invertMatrix(Mat<3,3,real>& dest, const Mat<3,3,real>& from)
{
  real det=from.determinant();

  if ( -1e-10<=det && det<=1e-10)
    return false;

  dest(0,0)= (from(1,1)*from(2,2) - from(2,1)*from(1,2))/det;
  dest(1,0)= (from(1,2)*from(2,0) - from(2,2)*from(1,0))/det;
  dest(2,0)= (from(1,0)*from(2,1) - from(2,0)*from(1,1))/det;
  dest(0,1)= (from(2,1)*from(0,2) - from(0,1)*from(2,2))/det;
  dest(1,1)= (from(2,2)*from(0,0) - from(0,2)*from(2,0))/det;
  dest(2,1)= (from(2,0)*from(0,1) - from(0,0)*from(2,1))/det;
  dest(0,2)= (from(0,1)*from(1,2) - from(1,1)*from(0,2))/det;
  dest(1,2)= (from(0,2)*from(1,0) - from(1,2)*from(0,0))/det;
  dest(2,2)= (from(0,0)*from(1,1) - from(1,0)*from(0,1))/det;
  
  return true;
}

/// Matrix inversion (special case 2x2).
template<class real>
extern __inline__ bool invertMatrix(Mat<2,2,real>& dest, const Mat<2,2,real>& from)
{
  real det=from(0,0)*from(1,1)-from(0,1)*from(1,0);

  if ( -1e-10<=det && det<=1e-10)
    return false;

  dest(0,0)=  from(1,1)/det;
  dest(0,1)= -from(0,1)/det;
  dest(1,0)= -from(1,0)/det;
  dest(1,1)=  from(0,0)/det;
  
  return true;
}

// Special operations for transformation matrices

template<class real, class real2>
extern __inline__ Vec<3,real> transform(const Mat<3,4,real2>& mat, const Vec<3,real>& p)
{
  return Vec<3,real>(
  mat[0][0]*p[0] + mat[0][1]*p[1] + mat[0][2]*p[2] + mat[0][3],
  mat[1][0]*p[0] + mat[1][1]*p[1] + mat[1][2]*p[2] + mat[1][3],
  mat[2][0]*p[0] + mat[2][1]*p[1] + mat[2][2]*p[2] + mat[2][3]
  );
}

template<class real>
extern __inline__ Vec<3,real> transform(const Mat<4,4,real>& mat, const Vec<3,real>& p)
{
  return Vec<3,real>(
  mat[0][0]*p[0] + mat[0][1]*p[1] + mat[0][2]*p[2] + mat[0][3],
  mat[1][0]*p[0] + mat[1][1]*p[1] + mat[1][2]*p[2] + mat[1][3],
  mat[2][0]*p[0] + mat[2][1]*p[1] + mat[2][2]*p[2] + mat[2][3]
  );
}

template <class real>
extern __inline__ Mat<3,4,real> transform(const Mat<3,4,real>& m1, const Mat<3,4,real>& m2)
{
  Mat<3,4,real> mat;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 4; j++) {
      mat[i][j] = m1[i][0]*m2[0][j] + m1[i][1]*m2[1][j] + m1[i][2]*m2[2][j];
    }
    mat[i][3] += m1[i][3];
  }
  return mat;
}

template<class real>
extern __inline__ Vec<3,real> transformv(const Mat<3,4,real>& mat, const Vec<3,real>& p)
{
  return Vec<3,real>(
  mat[0][0]*p[0] + mat[0][1]*p[1] + mat[0][2]*p[2],
  mat[1][0]*p[0] + mat[1][1]*p[1] + mat[1][2]*p[2],
  mat[2][0]*p[0] + mat[2][1]*p[1] + mat[2][2]*p[2]
  );
}

template<class real>
extern __inline__ Vec<3,real> transformv(const Mat<4,4,real>& mat, const Vec<3,real>& p)
{
  return Vec<3,real>(
  mat[0][0]*p[0] + mat[0][1]*p[1] + mat[0][2]*p[2],
  mat[1][0]*p[0] + mat[1][1]*p[1] + mat[1][2]*p[2],
  mat[2][0]*p[0] + mat[2][1]*p[1] + mat[2][2]*p[2]
  );
}

typedef Mat<2,2,unsigned char> Mat2x2b;
typedef Mat<2,2,int> Mat2x2i;
typedef Mat<2,2,float> Mat2x2f;
typedef Mat<2,2,double> Mat2x2d;

typedef Mat<2,3,unsigned char> Mat2x3b;
typedef Mat<2,3,int> Mat2x3i;
typedef Mat<2,3,float> Mat2x3f;
typedef Mat<2,3,double> Mat2x3d;

typedef Mat<2,4,unsigned char> Mat2x4b;
typedef Mat<2,4,int> Mat2x4i;
typedef Mat<2,4,float> Mat2x4f;
typedef Mat<2,4,double> Mat2x4d;

typedef Mat<3,2,unsigned char> Mat3x2b;
typedef Mat<3,2,int> Mat3x2i;
typedef Mat<3,2,float> Mat3x2f;
typedef Mat<3,2,double> Mat3x2d;

typedef Mat<3,3,unsigned char> Mat3x3b;
typedef Mat<3,3,int> Mat3x3i;
typedef Mat<3,3,float> Mat3x3f;
typedef Mat<3,3,double> Mat3x3d;

typedef Mat<3,4,unsigned char> Mat3x4b;
typedef Mat<3,4,int> Mat3x4i;
typedef Mat<3,4,float> Mat3x4f;
typedef Mat<3,4,double> Mat3x4d;

typedef Mat<4,2,unsigned char> Mat4x2b;
typedef Mat<4,2,int> Mat4x2i;
typedef Mat<4,2,float> Mat4x2f;
typedef Mat<4,2,double> Mat4x2d;

typedef Mat<4,3,unsigned char> Mat4x3b;
typedef Mat<4,3,int> Mat4x3i;
typedef Mat<4,3,float> Mat4x3f;
typedef Mat<4,3,double> Mat4x3d;

typedef Mat<4,4,unsigned char> Mat4x4b;
typedef Mat<4,4,int> Mat4x4i;
typedef Mat<4,4,float> Mat4x4f;
typedef Mat<4,4,double> Mat4x4d;

// Creation of transformation matrices

template<class real> Mat<4,4,real> matrixTranslation(const Vec<3,real>& pos)
{
  Mat<4,4,real> m; m.identity();
  m[0][3] = pos[0];
  m[1][3] = pos[1];
  m[2][3] = pos[2];
  return m;
}


template<class real> Mat<4,4,real> matrixScale(const Vec<3,real>& scale)
{
  Mat<4,4,real> m; m.identity();
  m[0][0] = scale[0];
  m[1][1] = scale[1];
  m[2][2] = scale[2];
  return m;
}

template<class real> Mat<4,4,real> matrixScale(const real scale)
{
  Mat<4,4,real> m; m.identity();
  m[0][0] = scale;
  m[1][1] = scale;
  m[2][2] = scale;
  return m;
}

template<class real=float> class Quat_;
typedef Quat_<float> Quat;
typedef Quat_<double> Quatd;

template<class real> Mat<4,4,real> matrixRotation(const Quat_<real>& rot)
{
  Mat<3,3,real> r; rot.toMatrix(&r);
  Mat<4,4,real> m; m.identity();
  m = r;
  return m;
}
template<class real> Mat<4,4,real> matrixRotation(real ang, const Vec<3,real>& axis)
{
  Quat_<real> q; q.fromDegreeAngAxis(ang, axis);
  return matrixRotation(q);
}


template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos, const Quat_<real>& rot, const Vec<3,real>& scale)
{
  return matrixTranslation(pos) * matrixRotation(rot) * matrixScale(scale);
}
template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos, const Quat_<real>& rot, real scale=1.0f)
{
  return matrixTranslation(pos) * matrixRotation(rot) * matrixScale(scale);
}
template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos, real ang, const Vec<3,real>& axis, const Vec<3,real>& scale)
{
  return matrixTranslation(pos) * matrixRotation(ang,axis) * matrixScale(scale);
}
template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos, real ang, const Vec<3,real>& axis, real scale=1.0f)
{
  return matrixTranslation(pos) * matrixRotation(ang,axis) * matrixScale(scale);
}
template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos, const Vec<3,real>& scale)
{
  return matrixTranslation(pos) * matrixScale(scale);
}
template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos, real scale)
{
  return matrixTranslation(pos) * matrixScale(scale);
}
template<class real> Mat<4,4,real> matrixTransform(const Vec<3,real>& pos)
{
  return matrixTranslation(pos);
}

template<class real> Mat<4,4,real> matrixOrthoProjection( real left, real right, real bottom, real top, real near, real far )
{
	Mat<4,4,real> pproj;

	if( left == right or top == bottom or near == far )
	{
		pproj.identity();
		return pproj; // invalid matrix
	}

	pproj.clear();

	pproj[0][0] =  real(2) / (right-left);
	pproj[1][1] =  real(2) / (top - bottom);
	pproj[2][2] =  real(-2) / (far-near);
	pproj[3][3] =  real(1);

	pproj[0][3] = - ( (right+ left  ) / (right- left  ) );
	pproj[1][3] = - ( (top  + bottom) / (top  - bottom) );
	pproj[2][3] = - ( (far  + near  ) / (far  - near  ) );

	return pproj;
}

template<class real> Mat<4,4,real> matrixPerspectiveProjection( real fovy, real aspect, real znear, real zfar )
{
	Mat<4,4,real> proj;
	proj.clear();

	real fov    = real(1.0) / ::tan(((fovy/real(180.0)) * real(3.14159265358979323846264338327) )/ real(2.0) );

	proj[0][0] = fov/aspect;
	proj[1][1] = fov;
	proj[2][2] = (zfar + znear) / (znear - zfar);
	proj[2][3] = ( real(2) * znear * zfar ) / (znear - zfar);
	proj[3][2] =   real(-1);
	return proj;
}

/**
 * @brief create a lookAt matrix in world space(!), that means it can be applied on a
 *        'camera' metaphor. In case you want to apply that to a CG setup,
 *        supply a rotationalPart matrix and multiply that with -eyePos.
 * @param eyePos3D the position of the eye in world space
 * @param center3D the center to look at from eyePos
 * @param up3D normalized(!) vector for the up-direction (not co-planar to (center3D - eyePos))
 * @param rotationalPart the 4x4 matrix with a 3x3 embedding of only the rotational part
 * @result a matrix that can be set for a 'world-space-camera' metaphor
 */
template<class real> Mat<4,4,real> matrixLookAt( const Vec<3,real> &eyePos3D,
		                                         const Vec<3,real> &center3D,
		                                         const Vec<3,real> &up3D,
		                                         Mat<4,4,real> *rotationalPart = NULL )
{
	Vec<3,real> forward, side, up;
	Mat<4,4,real> m2, result;

	forward = center3D - eyePos3D;
	forward.normalize();

	side = cross( forward, up3D );
	side.normalize();

	up = cross( side, forward );

	m2[0][0] = side[0];
	m2[0][1] = side[1];
	m2[0][2] = side[2];
	m2[0][3] = real(0.0);

	m2[1][0] = up[0];
	m2[1][1] = up[1];
	m2[1][2] = up[2];
	m2[1][3] = real(0.0);

	m2[2][0] = -forward[0];
	m2[2][1] = -forward[1];
	m2[2][2] = -forward[2];
	m2[2][3] = real(0.0);

	m2[3][0] = m2[3][1] = m2[3][2] = real(0.0);
	m2[3][3] = real(1.0);

	// that is the part that does the trick to move the view
	// to 'world-space-camera'
	Mat<4,4,real> m;
	m.invert(m2);

	// copy rotational part now
	// from the original matrix, as the user probably needs this
	// more than the inverted one ;)
	if(rotationalPart)
		*rotationalPart = m2;

	// move to eye position (note that this is positive eyePos3D
	// as the cam moves in world-space)
	m2 = matrixTranslation( eyePos3D ) *  m;

	return m2;
}

// Assignement from typed data

namespace Type
{

//template<int L, int C, class real>
//inline Type get(const Mat<L,C,real>& m)
//{
//  return (Type)matrix(get(m[0][0]),C,L);
//}

template <int L, int C, typename real>
class Assign< Mat<L,C,real> >
{
public:
static bool do_assign(Mat<L,C,real>& dest, int type, const void* data)
{
  if (isString(type))
  {
    const char* src = (const char*) data;
    const char* end = src + size(type);
    bool res = true;
    int x = 0;
    int y = 0;
    while (y < L && src < end)
    {
      if (*src==' ' || *src==',') ++src;
      else
      {
	const char* s0 = src;
	if (x==0 && *s0 == '{')
	{
	  ++s0;
	  if (y==0 && s0<end && *s0 == '{') ++s0;
	}
	do
	  ++src;
	while (src < end && *src != ' ' && *src != ',');
	int s = src-s0;
	if (src==end && src[-1]=='}') --s;
	bool eol = false;
	if (s>0 && s0[s-1]=='}')
	{
	  eol = true;
	  --s;
	}
	if (s>0)
	{
	  res &= assign(dest[y][x], buildString(s), s0);
	  ++x;
	}
	if (eol || x == C)
	{
	  x = 0;
	  ++y;
	}
      }
    }
    return res;
  }
  else if (isMatrix(type))
  {
    int eSize = elemSize(type);
    int eType = toSingle(type);
    int c = nx(type); int r=0;
    int l = ny(type);
    if (c>C) { r=c-C; c=C; }
    if (l>L) l=L;
    bool res = true;
    for (int y=0;y<l;y++)
    {
      for (int x=0;x<c;x++)
      {
	res &= assign(dest[y][x],eType,data);
	data = ((const char*)data)+eSize;
      }
      data = ((const char*)data)+eSize*r;
    }
    return res;
  }
  else
  {
    // Default implementation: read one value and fill the matrix with it
    real r;
    if (!assign(r,type,data)) return false;
    dest.fill(r);
    return true;
  }
}
};

} // namespace Type

} // namespace ftl

// iostream

template <int L, int C, typename real>
std::ostream& operator<<(std::ostream& o, const ftl::Mat<L,C,real>& m)
{
  o << '<' << m[0];
  for (int i=1; i<L; i++)
    o << ',' << m[i];
  o << '>';
  return o;
}

template <int L, int C, typename real>
std::istream& operator>>(std::istream& in, ftl::Mat<L,C,real>& m)
{
  int c;
  c = in.peek();
  while (c==' ' || c=='\n' || c=='<')
  {
    in.get();
    c = in.peek();
  }
  in >> m[0];
  for (int i=1; i<L; i++)
  {
    c = in.peek();
    while (c==' ' || c==',')
    {
      in.get();
      c = in.peek();
    }
    in >> m[i];
  }
  c = in.peek();
  while (c==' ' || c=='\n' || c=='>')
  {
    in.get();
    c = in.peek();
  }
  return in;
}

/// Scalar matrix multiplication operator.
template <int L, int C, typename real>
ftl::Mat<L,C,real> operator*(real r, const ftl::Mat<L,C,real>& m)
{
  return m*r;
}

#endif
