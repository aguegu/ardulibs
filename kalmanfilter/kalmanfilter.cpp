/*
 * KalmanFilter.cpp
 *
 *  Created on: Sep 15, 2012
 *      Author: agu
 */

#include "kalmanfilter.h"

KalmanFilter::KalmanFilter(float dt) :
		_dt(dt)
{
	_p[0][0] = 1.;
	_p[0][1] = 0.;
	_p[1][0] = 0.;
	_p[1][1] = 1.;
}

KalmanFilter::~KalmanFilter()
{
	// TODO Auto-generated destructor stub
}

/*
 * state_update is called every dt with a biased gyro measurement
 * by the user of the module.  It updates the current angle and
 * rate estimate.
 *
 * The pitch gyro measurement should be scaled into real units, but
 * does not need any bias removal.  The filter will track the bias.
 *
 * Our state vector is:
 *
 *	X = [ angle, gyro_bias ]
 *
 * It runs the state estimation forward via the state functions:
 *
 *	Xdot = [ angle_dot, gyro_bias_dot ]
 *
 *	angle_dot	= gyro - gyro_bias
 *	gyro_bias_dot	= 0
 *
 * And updates the covariance matrix via the function:
 *
 *	Pdot = A*P + P*A' + Q
 *
 * A is the Jacobian of Xdot with respect to the states:
 *
 *	A = [ d(angle_dot)/d(angle)     d(angle_dot)/d(gyro_bias) ]
 *	    [ d(gyro_bias_dot)/d(angle) d(gyro_bias_dot)/d(gyro_bias) ]
 *
 *	  = [ 0 -1 ]
 *	    [ 0  0 ]
 *
 * Due to the small CPU available on the microcontroller, we've
 * hand optimized the C code to only compute the terms that are
 * explicitly non-zero, as well as expanded out the matrix math
 * to be done in as few steps as possible.  This does make it harder
 * to read, debug and extend, but also allows us to do this with
 * very little CPU time.
 *
 * input: Pitch gyro measurement */
void KalmanFilter::state_update(const float q_m)
{
	/* Unbias our gyro */
	const float q = q_m - _q_bias;

	/*
	 * Compute the derivative of the covariance matrix
	 *
	 *	Pdot = A*P + P*A' + Q
	 *
	 * We've hand computed the expansion of A = [ 0 -1, 0 0 ] multiplied
	 * by P and P multiplied by A' = [ 0 0, -1, 0 ].  This is then added
	 * to the diagonal elements of Q, which are Q_angle and Q_gyro.
	 */
	const float p_dot[4] =
	{ Q_angle - _p[0][1] - _p[1][0], /* 0,0 */
	-_p[1][1], /* 0,1 */
	-_p[1][1], /* 1,0 */
	Q_gyro /* 1,1 */
	};

	/* Store our unbias gyro estimate */
	_rate = q;

	/*
	 * Update our angle estimate
	 * angle += angle_dot * dt
	 *       += (gyro - gyro_bias) * dt
	 *       += q * dt
	 */
	_angle += q * _dt;

	/* Update the covariance matrix */
	_p[0][0] += p_dot[0] * _dt;
	_p[0][1] += p_dot[1] * _dt;
	_p[1][0] += p_dot[2] * _dt;
	_p[1][1] += p_dot[3] * _dt;
}

/*
 * kalman_update is called by a user of the module when a new
 * accelerometer measurement is available.  ax_m and az_m do not
 * need to be scaled into actual units, but must be zeroed and have
 * the same scale.
 *
 * This does not need to be called every time step, but can be if
 * the accelerometer data are available at the same rate as the
 * rate gyro measurement.
 *
 * For a two-axis accelerometer mounted perpendicular to the rotation
 * axis, we can compute the angle for the full 360 degree rotation
 * with no linearization errors by using the arctangent of the two
 * readings.
 *
 * As commented in state_update, the math here is simplified to
 * make it possible to execute on a small microcontroller with no
 * floating point unit.  It will be hard to read the actual code and
 * see what is happening, which is why there is this extensive
 * comment block.
 *
 * The C matrix is a 1x2 (measurements x states) matrix that
 * is the Jacobian matrix of the measurement value with respect
 * to the states.  In this case, C is:
 *
 *	C = [ d(angle_m)/d(angle)  d(angle_m)/d(gyro_bias) ]
 *	  = [ 1 0 ]
 *
 * because the angle measurement directly corresponds to the angle
 * estimate and the angle measurement has no relation to the gyro
 * bias.
 *
 * input:  X acceleration, Z acceleration
 */
void KalmanFilter::kalman_update(const float ax_m, const float az_m)
{
	/* Compute our measured angle and the error in our estimate */
	const float angle_m = atan2(-az_m, ax_m);
	const float angle_err = angle_m - _angle;

	/*
	 * C_0 shows how the state measurement directly relates to
	 * the state estimate.
	 *
	 * The C_1 shows that the state measurement does not relate
	 * to the gyro bias estimate.  We don't actually use this, so
	 * we comment it out.
	 */
	const float c0 = 1;
	/* const float		C_1 = 0; */

	/*
	 * PCt<2,1> = P<2,2> * C'<2,1>, which we use twice.  This makes
	 * it worthwhile to precompute and store the two values.
	 * Note that C[0,1] = C_1 is zero, so we do not compute that
	 * term.
	 */
	const float pct0 = c0 * _p[0][0]; /* + C_1 * P[0][1] = 0 */
	const float pct1 = c0 * _p[1][0]; /* + C_1 * P[1][1] = 0 */

	/*
	 * Compute the error estimate.  From the Kalman filter paper:
	 *
	 *	E = C P C' + R
	 *
	 * Dimensionally,
	 *
	 *	E<1,1> = C<1,2> P<2,2> C'<2,1> + R<1,1>
	 *
	 * Again, note that C_1 is zero, so we do not compute the term.
	 */
	const float e = R_angle + c0 * pct0 /*	+ C_1 * PCt_1 = 0 */;

	/*
	 * Compute the Kalman filter gains.  From the Kalman paper:
	 *
	 *	K = P C' inv(E)
	 *
	 * Dimensionally:
	 *
	 *	K<2,1> = P<2,2> C'<2,1> inv(E)<1,1>
	 *
	 * Luckilly, E is <1,1>, so the inverse of E is just 1/E.
	 */
	const float k0 = pct0 / e;
	const float k1 = pct1 / e;

	/*
	 * Update covariance matrix.  Again, from the Kalman filter paper:
	 *
	 *	P = P - K C P
	 *
	 * Dimensionally:
	 *
	 *	P<2,2> -= K<2,1> C<1,2> P<2,2>
	 *
	 * We first compute t<1,2> = C P.  Note that:
	 *
	 *	t[0,0] = C[0,0] * P[0,0] + C[0,1] * P[1,0]
	 *
	 * But, since C_1 is zero, we have:
	 *
	 *	t[0,0] = C[0,0] * P[0,0] = PCt[0,0]
	 *
	 * This saves us a floating point multiply.
	 */
	const float t0 = pct0; /* C_0 * P[0][0] + C_1 * P[1][0] */
	const float t1 = c0 * _p[0][1]; /* + C_1 * P[1][1]  = 0 */

	_p[0][0] -= k0 * t0;
	_p[0][1] -= k0 * t1;
	_p[1][0] -= k1 * t0;
	_p[1][1] -= k1 * t1;

	/*
	 * Update our state estimate.  Again, from the Kalman paper:
	 *
	 *	X += K * err
	 *
	 * And, dimensionally,
	 *
	 *	X<2> = X<2> + K<2,1> * err<1,1>
	 *
	 * err is a measurement of the difference in the measured state
	 * and the estimate state.  In our case, it is just the difference
	 * between the two accelerometer measured angle and our estimated
	 * angle.
	 */
	_angle += k0 * angle_err;
	_q_bias += k1 * angle_err;
}

float KalmanFilter::getAngle() const
{
	return _angle;
}

float KalmanFilter::getQBias() const
{
	return _q_bias;
}

float KalmanFilter::getRate() const
{
	return _rate;
}

