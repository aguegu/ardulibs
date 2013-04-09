/* http://www.rotomotion.com/downloads/tilt.c
 *
 * -*- indent-tabs-mode:T; c-basic-offset:8; tab-width:8; -*- vi: set ts=8:
 * $Id: tilt.c,v 1.1 2003/07/09 18:23:29 john Exp $
 *
 * 1 dimensional tilt sensor using a dual axis accelerometer
 * and single axis angular rate gyro.  The two sensors are fused
 * via a two state Kalman filter, with one state being the angle
 * and the other state being the gyro bias.
 *
 * Gyro bias is automatically tracked by the filter.  This seems
 * like magic.
 *
 * Please note that there are lots of comments in the functions and
 * in blocks before the functions.  Kalman filtering is an already complex
 * subject, made even more so by extensive hand optimizations to the C code
 * that implements the filter.  I've tried to make an effort of explaining
 * the optimizations, but feel free to send mail to the mailing list,
 * autopilot-devel@lists.sf.net, with questions about this code.
 *
 *
 * (c) 2003 Trammell Hudson <hudson@rotomotion.com>
 *
 *************
 *
 *  This file is part of the autopilot onboard code package.
 *
 *  Autopilot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Autopilot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Autopilot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * KalmanFilter.h
 *
 *  Created on: Sep 15, 2012
 *      Author: Weihong Guan (aGuegu)
 *
 *  update the Kalman Filter to a c++ class
 */

#ifndef KALMANFILTER_H_
#define KALMANFILTER_H_

#include "Arduino.h"

class KalmanFilter
{
public:
	KalmanFilter(float dt);
	virtual ~KalmanFilter();
	void state_update(const float q_m);
	void kalman_update(const float ax_m, const float az_m);

	float getAngle() const;
	float getQBias() const;
	float getRate() const;

private:
	/*
	 * Our update rate.  This is how often our state is updated with
	 * gyro rate measurements.
	 */
	const float _dt;

	/*
	 * Our covariance matrix.  This is updated at every time step to
	 * determine how well the sensors are tracking the actual state.
	 */
	float _p[2][2];

	/*
	 * Our two states, the angle and the gyro bias.  As a byproduct of computing
	 * the angle, we also have an unbiased angular rate available.   These are
	 * read-only to the user of the module.
	 */
	float _angle;
	float _q_bias;
	float _rate;

	/*
	 * R represents the measurement covariance noise.  In this case,
	 * it is a 1x1 matrix that says that we expect 0.3 rad jitter
	 * from the accelerometer.
	 */
	static const float R_angle = 0.3;

	/*
	 * Q is a 2x2 matrix that represents the process covariance noise.
	 * In this case, it indicates how much we trust the acceleromter
	 * relative to the gyros.
	 */
	static const float Q_angle = 0.001;
	static const float Q_gyro = 0.003;
};

#endif /* KALMANFILTER_H_ */
