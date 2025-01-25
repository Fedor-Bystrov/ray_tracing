export module models;

namespace RT::Models {

export struct Circle {
  int x;
  int y;
  int r;
};

export struct Ray {
  int x_start;
  int y_start;
  double angle;
  double dx;  // Precomputed cosine
  double dy;  // Precomputed sine
};

}  // namespace RT::Models