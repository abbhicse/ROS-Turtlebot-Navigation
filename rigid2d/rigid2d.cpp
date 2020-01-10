
#include "rigid2d.hpp"
#include <iostream>

namespace rigid2d
{

// public

std::ostream & operator<<(std::ostream & os, const Vector2D & v)
{
  os << "x component: " <<  v.x << " y component: " << v.y << '\n';
  return os;
}


std::istream & operator>>(std::istream & is, Vector2D & v)
{
  std::cout << "Enter x component" << std::endl;
  is >> v.x;

  std::cout << "Enter y component" << std::endl;
  is >> v.y;

  return is;
}



Transform2D::Transform2D()
{
  theta = 0.0;
  ctheta = 1.0;
  stheta = 0.0;
  x = 0.0;
  y = 0.0;
}


Transform2D::Transform2D(const Vector2D & trans)
{
  theta = 0.0;
  ctheta = 0.0;
  stheta = 0.0;
  x = trans.x;
  y = trans.y;
}


Transform2D::Transform2D(double radians)
{
  theta = radians;
  ctheta = std::cos(theta);
  stheta = std::sin(theta);
  x = 0.0;
  y = 0.0;
}


Transform2D::Transform2D(const Vector2D & trans, double radians)
{
  theta = radians;
  ctheta = std::cos(theta);
  stheta = std::sin(theta);
  x = trans.x;
  y = trans.y;
}


Vector2D Transform2D::operator()(Vector2D v) const
{
  Vector2D v_new;
  v_new.x = ctheta * v.x - stheta * v.y;
  v_new.y = stheta * v.x + ctheta * v.y;

  return v_new;
}


Transform2D Transform2D::inv() const
{
  // create new transform
  Transform2D trans2d(theta, ctheta, stheta, x, y);

  // R^T flip sign in sin
  trans2d.stheta = -1.0 * stheta;

  // Vector2D v;
  // v.x = this->x;
  // v.y = this->y;
  //
  // v = this->operator()(v);

  // p' = -R^T * p
  trans2d.x = -(trans2d.ctheta * x - trans2d.stheta * y);
  trans2d.y = -(trans2d.stheta * x + trans2d.ctheta * y);
  // trans2d.x = v.x;
  // trans2d.y = v.y;

  return trans2d;
}



// private

Transform2D::Transform2D(double theta, double ctheta, double stheta, double x, double y)
{
  this->theta = theta;
  this->ctheta = ctheta;
  this->stheta = stheta;
  this->x = x;
  this->y = y;
}


Transform2D & Transform2D::operator*=(const Transform2D & rhs)
{
  // multiply rotations
  double c_new = ctheta * rhs.ctheta - stheta * rhs.stheta;
  double s_new = stheta * rhs.ctheta + ctheta * rhs.stheta;

  // multiply rotation by translation
  double x_new = ctheta * rhs.x - stheta * rhs.y + x;
  double y_new = stheta * rhs.x + ctheta * rhs.y + y;

  // set new transform
  ctheta = c_new;
  stheta = s_new;
  theta = std::acos(ctheta);

  x = x_new;
  y = y_new;

  return *this;
}


// end class


std::ostream & operator<<(std::ostream & os, const Transform2D & tf)
{
  os << "degrees: " << rad2deg(tf.theta) << " x: " << tf.x << " y: " << tf.y << "\n";
  return os;
}


std::istream & operator>>(std::istream & is, Transform2D & tf)
{
  double deg_angle;
  Vector2D v;

  std::cout << "Enter anlge in degrees" << std::endl;
  is >> deg_angle;

  std::cout << "Enter x component" << std::endl;
  is >> v.x;

  std::cout << "Enter y component" << std::endl;
  is >> v.y;

  Transform2D tf_temp(v, deg2rad(deg_angle));
  tf = tf_temp;

  return is;
}


Transform2D operator*(Transform2D lhs, const Transform2D & rhs)
{
  // get access to lhs private members
  return lhs.operator*=(rhs);
}



} // end namespace












// end file