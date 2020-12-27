/**
 *   @file  gtrack_2d.h
 *
 *   @brief
 *      This is the internal header file for GTRACK Algorithm
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef GTRACK_2D
#ifndef GTRACK_2D_H__
#define GTRACK_2D_H__

/**
 * @brief 
 *  GTRACK Measurement vector
 * @details
 *  The structure defines tracker measurement vector format
 */
typedef struct
{
    /**  @brief   Range, m */
	float range;
    /**  @brief   Angle, rad */
	float angle;
    /**  @brief   Radial velocity, m/s */
	float doppler;
} GTRACK_measurement_vector;

/**
 * @brief 
 *  GTRACK position
 * @details
 *  The structure defines a position in cartesian space
 */
typedef struct
{
    /**  @brief   Target lateral position, m */
    float posX;
    /**  @brief   Targte horizontal position, m */
    float posY;
} GTRACK_cartesian_position;

/**
 * @brief 
 *  GTRACK State vector, 2DV configuration
 * @details
 *  The structure defines tracker state vector format when target position and velocity are tracked
 */
typedef struct
{
    /**  @brief   Target lateral position, m */
    float posX;
    /**  @brief   Targte horizontal position, m */
    float posY;
    /**  @brief   Target lateral velocity, m/s */
    float velX;
    /**  @brief   Target horizontal velocity, m/s */
    float velY;
} GTRACK_state_vector_pos_vel;

/**
 * @brief 
 *  GTRACK State vector, 2DA configuration
 * @details
 *  The structure defines tracker state vector format when target position, velocity and acceleration are tracked
 */
typedef struct
{
    /**  @brief   Target lateral position, m */
    float posX;
    /**  @brief   Targte horizontal position, m */
    float posY;
    /**  @brief   Target lateral velocity, m/s */
    float velX;
    /**  @brief   Target horizontal velocity, m/s */
    float velY;
    /**  @brief   Target lateral acceleration, m/s2 */
    float accX;
    /**  @brief   Target horizontal acceleration, m/s2 */
    float accY;
} GTRACK_state_vector_pos_vel_acc;
#endif
#endif
