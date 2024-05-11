#ifndef MATHEMATICS_R
# define MATHEMATICS_R

# include "Rational.h"
# include "Matrix.h"

struct s_Rational	*ft_exp_Rational(struct s_Rational *x);
struct s_Rational	*ft_ln_Rational(struct s_Rational *x);
struct s_Rational	*ft_sin_Rational(struct s_Rational *x);
struct s_Rational	*ft_cos_Rational(struct s_Rational *x);
struct s_Rational	*ft_tan_Rational(struct s_Rational *x);
struct s_Rational	*ft_sinh_Rational(struct s_Rational *x);
struct s_Rational	*ft_cosh_Rational(struct s_Rational *x);
struct s_Rational	*ft_tanh_Rational(struct s_Rational *x);
struct s_Rational	*ft_sqrt_Rational(struct s_Rational *x);
struct s_Rational	*ft_radians_Rational(struct s_Rational *x);
struct s_Rational	*ft_degrees_Rational(struct s_Rational *x);
struct s_Rational	*ft_norm_Matrix(struct s_Matrix *x);

#endif
