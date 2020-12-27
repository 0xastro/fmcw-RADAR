/**
 *   @file  gtrack.h
 *
 *   @brief
 *      This is the header file for the GTRACK Algorithm
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

/** @mainpage GTRACK Algorithm
 *	This code is an implementation of Group TRACKing algorithm.<br>
 *	The algorithm is designed to track multiple targets, where each target is represented by a set of measurement points.<br> 
 *	Each measurement point carries detection informations, for example, range, azimuth, elevation (for 3D option), and radial velocity.<br> 
 *	Instead of tracking individual reflections, the algorithm predicts and updates the location and dispersion properties of the group.<br>
 *	The group is defined as the set of measurements (typically, few tens; sometimes few hundreds) associated with a real life target.<br>
 *	Algorithm supports tracking targets in two or three dimensional spaces as a build time option:<br>
 * 	- When built with 2D option, algorithm inputs range/azimuth/doppler information and tracks targets in 2D cartesian space.
 * 	- When built with 3D option, algorithm inputs range/azimuth/elevation/doppler information and tracks targets in 3D cartesian space.
 *
 *  ### Input/output
 *	- Algorithm inputs the Point Cloud. For example, few hundreds of individual measurements (reflection points).<br>
 *	- Algorithm outputs a Target List. For example, an array of few tens of target descriptors. Each descriptor carries a set of properties for a given target.<br>
 *	- Algorithm optionally outputs a Target Index. If requested, this is an array of target IDs associated with each measurement.<br>
 *
 *  ### Features
 *	- Algorithm uses extended Kalman Filter to model target motion in Cartesian coordinates.<br>
 *	- Algorithm supports constant velocity and constant acceleartion models.<br>
 *	- Algorithm uses 3D/4D Mahalanobis distances as gating function and Max-likelihood criterias as scoring function to associate points with an existing track.<br>
 *
 *  ## External API
 *  Application includes the following algorithm header
 *  @code
    #include <ti/alg/gtrack.h>
 *  @endcode
 *	All resources are allocated at create time during the \ref gtrack_create call. <br>
 *	All resources are freed at delete time time during the \ref gtrack_delete call. <br>
 *  Application is expected to implement the design pattern as described in the pseudo code bellow:
 *  @code
    h = gtrack_create(params);                      // Creates an instance of the algorithm with a desired configuration
    while(running) {
        gtrack_step(h, pointCloud, &targetList);    // Runs a single step of the given alrorithm instance with input point cloud data
    }
    gtrack_delete(h);                               // Delete the algorithm instance
 *  @endcode
 *  ### Dependencies
 *  Library is platform independent. To port the library, platform shall implement few functions that Library abstracts, see \ref GTRACK_Dependencies
 *
 *  ## Internal Architecture of the GTRACK Algorithm
 *  Algorithm is implemented with two internal software sublayers: Module and Units(s). <br>
 *  Application can create multiple Module instances with different configuration parameters (for example, to track different classes of targets).<br>
 *  Each Module instance creates amd manages multiple units.<br>
 *  Each Unit represents a single tracking object.<br> 
 *  Units inherit configuration parameters from the parent Module.<br>
 *  ### Module level
 *  The imlementation of \ref gtrack_create function creates a module instance and pre-allocates resources for a maximum number of units.<br> 
 *  The imlementation of \ref gtrack_step function calls one single round of module functions as illustrated in the pseudo code below:
 *  @code
    gtrack_step(h, pointCloud, &targetList) {
        gtrack_modulePredict(h,...);
        gtrack_moduleAssociate(h,...);
        gtrack_moduleAllocate(h,...);
        gtrack_moduleUpdate(h,...);
        gtrack_moduleReport(h,...);
    }
 *  @endcode
 *  The imlementation of \ref gtrack_delete function returns all the resources back to the system
 *  ### Unit level
 *  Units are created during \ref gtrack_moduleAllocate calls.<br>
 *  The resources pre-allocated at module create time are assigned to a new unit.<br>
 *  All active units are called during each parent module function step calls.<br>
 *  As an example, the pseudo code below illustrates the implementation of the predict function:
 *  @code
    gtrack_modulePredict(h,...) {
        for(each active unit) {
            gtrack_unit_predict(unit, ...);
        }
    }
 *  @endcode
 *  Units are deleted during \ref gtrack_moduleUpdate calls. The resources are returned back to the parent module.
 */
#ifndef GTRACK_H
#define GTRACK_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


#if !defined (GTRACK_2D) && !defined (GTRACK_3D)
#define GTRACK_2D
#endif

/** @defgroup GTRACK_ALG      GTRACK Algorithm Library
@brief
*	Library implementation of tracking algorithm
*/

/** @defgroup GTRACK_External	Externals
@ingroup GTRACK_ALG
*/

/** @defgroup GTRACK_Dependencies	Dependencies
@ingroup GTRACK_ALG
@brief
*	Functions used by GTRACK modules. Shall be implemented by porting platform layers 
*/

/** @defgroup GTRACK_Internal	Internals
@ingroup GTRACK_ALG
*/

/**
@defgroup GTRACK_ALG_EXTERNAL_FUNCTION            External Functions
@ingroup GTRACK_External
@brief
*	External functional API, called by Application
*/

/**
@defgroup GTRACK_ALG_EXTERNAL_DATA_STRUCTURE      External Data Structures
@ingroup GTRACK_External
@brief
*   Data structures exposed to Application
*/

/**
@defgroup GTRACK_ALG_MODULE_FUNCTION    Module Level Functions
@ingroup GTRACK_Internal
@brief
*   Internal MODULE level API, called by External Functions
*/

/**
@defgroup GTRACK_ALG_UNIT_FUNCTION    Unit Level Functions
@ingroup GTRACK_Internal
@brief
*   Internal UNIT level API, called by MODULE
*/
/**
@defgroup GTRACK_ALG_UTILITY_FUNCTION	Utility Functions
@ingroup GTRACK_Internal
@brief
*   Utility functions used internally
*/

/**
@defgroup GTRACK_ALG_MATH_FUNCTION	Math Functions
@ingroup GTRACK_Internal
@brief
*   Math functions used internally
*/

/**
@defgroup GTRACK_ALG_INTERNAL_DATA_STRUCTURE	Internal Data Structures
@ingroup GTRACK_Internal
@brief
*   Internal data structures
*/

/**
@defgroup GTRACK_ERROR_CODE            Algorithm Error codes
@ingroup GTRACK_External
@brief
*   Error codes reported to Application
@{ */

/**
 * @brief   Base error code for GTRACK algorithm
 */
#define GTRACK_ERRNO_BASE		(-8000)

/**
 * @brief   Error code: No errors
 */
#define GTRACK_EOK				(0)

/**
 * @brief   Error Code: Invalid argument
 */
#define GTRACK_EINVAL			(GTRACK_ERRNO_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define GTRACK_EINUSE			(GTRACK_ERRNO_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define GTRACK_ENOTIMPL			(GTRACK_ERRNO_BASE-3)

/**
 * @brief   Error Code: Out of memory
 */
#define GTRACK_ENOMEM			(GTRACK_ERRNO_BASE-4)

/** @}*/

/** 
@addtogroup GTRACK_ALG_EXTERNAL_DATA_STRUCTURE
@{ */

/**
 * @name Maximum supported configurations
 * @{ */
#define GTRACK_NUM_POINTS_MAX			(1000U) /**< @brief Defines maximum possible number of measurments point the algorithm will accept at configuration time */
#define GTRACK_NUM_TRACKS_MAX			(250U)	/**< @brief Defines maximum possible number of tracking target the algorithm will accept at configuration time */
/** @} */

/**
 * @name Target ID definitions
 * @{ 
 * @details
 *  Target IDs are uint8_t, with valid IDs ranging from 0 to 249. Values 250 to 252 are reserved for future use, other values as defined below
 */
#define GTRACK_ID_POINT_TOO_WEAK            (253U)   /**< @brief Point is not associated, is too weak */
#define GTRACK_ID_POINT_BEHIND_THE_WALL     (254U)   /**< @brief Point is not associated, is behind the wall */
#define GTRACK_ID_POINT_NOT_ASSOCIATED      (255U)   /**< @brief Point is not associated, noise */
/** @} */

/**
 * @name Benchmarking results
 * @{  
 * @details
 *  During runtime execution, tracking step function can optionally return cycle counts for the sub-functions defined below
 *  Each count is 32bit unsigned integer value representing a timestamp of free runing clock 
 */
#define GTRACK_BENCHMARK_SETUP              (0U)    /**< @brief Cycle count at step setup */
#define GTRACK_BENCHMARK_PREDICT            (1U)    /**< @brief Cycle count after predict function */
#define GTRACK_BENCHMARK_ASSOCIATE          (2U)    /**< @brief Cycle count after associate function */
#define GTRACK_BENCHMARK_ALLOCATE           (3U)    /**< @brief Cycle count after allocate function */
#define GTRACK_BENCHMARK_UPDATE             (4U)    /**< @brief Cycle count after update function */
#define GTRACK_BENCHMARK_REPORT             (5U)    /**< @brief Cycle count after report function */

#define GTRACK_BENCHMARK_SIZE               (GTRACK_BENCHMARK_REPORT)    /**< @brief Size of benchmarking array */
/** @} */

#ifdef GTRACK_3D
#include "include/gtrack_3d.h"
#else
#include "include/gtrack_2d.h"
#endif

/**
 * @name Boundary boxes
 * @{  
 * @details
 *  Application can configure tracker with scene boundries. Boundaries are defined as a boxes.
 */
#define GTRACK_MAX_BOUNDARY_BOXES           (2U)    /**< @brief Maximum number of boundary boxes. Points outside of boundary boxes are ignored */
#define GTRACK_MAX_STATIC_BOXES             (2U)    /**< @brief Maximum number of static boxes. Targets inside the static box can persist longer */
/** @} */

/**
 * @brief
 *  GTRACK Box Structure
 *
 * @details
 *  The structure defines the box element used to describe the scenery 
 */
typedef struct {
    /**  @brief   Left boundary, m */
	float x1;
    /**  @brief   Right boundary, m */
	float x2;
    /**  @brief   Near boundary, m */
	float y1;
    /**  @brief   Far boundary, m */
	float y2;
    /**  @brief   Bottom boundary, m */
	float z1;
    /**  @brief   Top boundary, m */
	float z2;
} GTRACK_boundaryBox; 
/**
 * @brief
 *  GTRACK Gate Limits
 *
 * @details
 *  The structure describes the limits the gating function will expand
 */
typedef struct {
    /**  @brief   Width limit, m */
	float width;
    /**  @brief   Depth limit, m */
	float depth;
    /**  @brief   Heigth limit, m */
	float height;
    /**  @brief   Radial velocity limit, m/s */
	float vel;
} GTRACK_gateLimits; 


/**
 * @brief 
 *  GTRACK Update Function Parameters
 *
 * @details
 *  The structure describes default standard deviation values applied when no variance information provided in the point Cloud
 */
typedef struct {
    /**  @brief Expected standard deviation of measurements in target length dimension*/
    float widthStd;
    /**  @brief Expected standard deviation of measurements in target length dimension*/
    float depthStd;
    /**  @brief Expected standard deviation of measurements in target width dimension*/
    float heightStd;
    /**  @brief Expected standard deviation of measurements of target radial velocity */
    float dopplerStd;
} GTRACK_varParams;

/**
 * @brief 
 *  GTRACK Scene Parameters
 *
 * @details
 *  This set of parameters describes the scenery. <br>
 *  It allows user to configure the tracker with expected boundaries, and areas of static behavior. <br>
 *  User can define up to \ref GTRACK_MAX_BOUNDARY_BOXES boundary boxes, and up to \ref GTRACK_MAX_STATIC_BOXES static boxes. <br>
 *  Boxes coordinates are in meters, sensor is assumed at (0, 0) of Cartesian (X, Y) space.
 */
typedef struct {
    /**  @brief Number of scene boundary boxes. If defined (numBoundaryBoxes > 0), only points within the boundary box(s) can be associated with tracks */
    uint8_t             numBoundaryBoxes;
    /**  @brief Scene boundary boxes */
    GTRACK_boundaryBox  boundaryBox[GTRACK_MAX_BOUNDARY_BOXES];
    /**  @brief Number of scene static boxes. If defined (numStaticBoxes > 0), only targets within the static box(s) can persist as static */
    uint8_t             numStaticBoxes;
    /**  @brief Scene static boxes */
    GTRACK_boundaryBox  staticBox[GTRACK_MAX_STATIC_BOXES];
} GTRACK_sceneryParams;


/**
 * @brief 
 *  GTRACK Gating Function Parameters
 *
 * @details
 *  The structure describes gating function parameters
 */
typedef struct {
    /**  @brief   Volume of the gating function */
	float		volume;
    /**  @brief   Gating function limits */
    union {
	    GTRACK_gateLimits	limits;
        float limitsArray[4];
    };
} GTRACK_gatingParams;

/**
 * @brief 
 *  GTRACK Tracking Management Function Parameters
 *
 * @details
 *  The structure describes the thresholds for state changing counters
 */
typedef struct {
    /**  @brief  DETECTION => ACTIVE threshold */
	uint16_t det2actThre;
    /**  @brief  DETECTION => FREE threshold */
    uint16_t det2freeThre;

    /**  @brief  ACTIVE => FREE threshold */
    uint16_t active2freeThre;
    /**  @brief  STATIC => FREE threshold */
    uint16_t static2freeThre;
    /**  @brief  EXIT ZONE => FREE threshold */
	uint16_t exit2freeThre;
} GTRACK_stateParams;


/**
 * @brief 
 *  GTRACK Allocation Function Parameters
 *
 * @details
 *  The structure describes the thresholds used in Allocation function
 */
typedef struct {
    /**  @brief  Minimum total SNR */
    float snrThre;
    /**  @brief  Minimum total SNR when behind another target */
    float snrThreObscured;
    /**  @brief  Minimum initial velocity, m/s */
    float velocityThre;
    /**  @brief  Minimum number of points in a set */
	uint16_t pointsThre;
    /**  @brief  Maximum squared distance between points in a set */
    float	maxDistanceThre;
    /**  @brief  Maximum velocity delta between points in a set */
    float	maxVelThre;
} GTRACK_allocationParams;

/**
 * @brief 
 *  GTRACK Unrolling Parameters
 *
 * @details
 *  The structure describes the filtering parameters used to switch unrolling states
 */
typedef struct {
    /**  @brief  Range rate filtering alpha */
	float alpha;
    /**  @brief  Range rate filtering confidence */
	float confidence;
} GTRACK_unrollingParams;


/**
 * @brief 
 *  GTRACK Advanced Parameters
 *
 * @details
 *  The structure describes advanced configuration parameters
 */
typedef struct {
	/**  @brief  Pointer to gating parameters */
	GTRACK_gatingParams *gatingParams;
    /**  @brief  Pointer to allocation parameters */
	GTRACK_allocationParams *allocationParams;
    /**  @brief  Pointer to unrolling parameters */
	GTRACK_unrollingParams *unrollingParams;
    /**  @brief  Pointer to tracking state parameters */
	GTRACK_stateParams *stateParams;
    /**  @brief  Pointer to measurements variation parameters */
	GTRACK_varParams   *variationParams;
    /**  @brief  Pointer to scenery parameters */
	GTRACK_sceneryParams *sceneryParams;
} GTRACK_advancedParameters;



/**
 * @brief 
 *  GTRACK State Vector
 *
 * @details
 *  Defines State vector options 
 *		2D - Depreciated, not supported		
 *		2DA - Supported
 *		3D, 3DA - Not supported (Future)
 */
typedef enum
{
    /**  @brief   2D motion model with constant velocity. State vector has four variables S={X,Y, Vx,Vy} */
	GTRACK_STATE_VECTORS_2DV = 0,
    /**  @brief   2D motion model with constant acceleration. State vector has six variables S={X,Y, Vx,Vy, Ax,Ay} */
	GTRACK_STATE_VECTORS_2DA,
    /**  @brief   3D motion model with constant velocity. State vector has six variables S={X,Y,Z, Vx,Vy,Vz} */
	GTRACK_STATE_VECTORS_3DV,
    /**  @brief   3D motion model with constant acceleration. State vector has nine variables S={X,Y,Z, Vx,Vy,Vz, Ax,Ay,Az} */
	GTRACK_STATE_VECTORS_3DA
} GTRACK_STATE_VECTOR_TYPE;


/**
 * @brief 
 *  GTRACK Verbose Level
 *
 * @details
 *  Defines Algorithm verboseness level 
 */
typedef enum
{
    /**  @brief   NONE */
	GTRACK_VERBOSE_NONE = 0,
    /**  @brief   ERROR Level, only errors are reported */
	GTRACK_VERBOSE_ERROR,
    /**  @brief   WARNING Level, errors and warnings are reported */
	GTRACK_VERBOSE_WARNING,
    /**  @brief   DEBUG Level, errors, warnings, and state transitions are reported */
	GTRACK_VERBOSE_DEBUG,
    /**  @brief   MATRIX Level, previous level plus all intermediate computation results are reported */
	GTRACK_VERBOSE_MATRIX,
    /**  @brief   MAXIMUM Level, maximum amount of details are reported */
	GTRACK_VERBOSE_MAXIMUM
} GTRACK_VERBOSE_TYPE;


/**
 * @brief 
 *  GTRACK Configuration
 *
 * @details
 *  The structure describes the GTRACK algorithm configuration options. 
 */
typedef struct
{
    /**  @brief   State Vector Type */
    GTRACK_STATE_VECTOR_TYPE stateVectorType;
    /**  @brief   Verboseness Level */
    GTRACK_VERBOSE_TYPE verbose;
    /**  @brief   Maximum Number of Measurement Points per frame. Up to \ref GTRACK_NUM_POINTS_MAX supported */
	uint16_t maxNumPoints;
    /**  @brief   Maximum Number of Tracking Objects. Up to \ref GTRACK_NUM_TRACKS_MAX supported */
	uint16_t maxNumTracks;

    /**  @brief   Expected target radial velocity at the moment of detection, m/s */
    float initialRadialVelocity;
    /**  @brief   Maximum radial velocity reported by sensor +/- m/s */
	float maxRadialVelocity;
    /**  @brief   Radial Velocity resolution, m/s */
	float radialVelocityResolution;
    /**  @brief   Maximum expected target acceleration in lateral (X), longitudinal (Y), and vertical (Z) directions, m/s2 */
    float maxAcceleration[3];
    /**  @brief   Frame rate, ms */
    float deltaT;

    /**  @brief   Advanced parameters, set to NULL for defaults */
    GTRACK_advancedParameters *advParams;

} GTRACK_moduleConfig;

#define GTRACK_STATE_VECTOR_SIZE sizeof(GTRACK_state_vector_pos_vel_acc)/sizeof(float)
#define GTRACK_MEASUREMENT_VECTOR_SIZE sizeof(GTRACK_measurement_vector)/sizeof(float)

/**
 * @brief 
 *  GTRACK Measurement point
 *
 * @details
 *  The structure describes measurement point format
 */
typedef struct
{
    union {
    	/**  @brief   Measurement vector */
        GTRACK_measurement_vector vector;
        float array[GTRACK_MEASUREMENT_VECTOR_SIZE];
    };
	/**  @brief   Range detection SNR, linear */
    float snr;
} GTRACK_measurementPoint;

typedef union {
    /**  @brief   Measurement vector */
    GTRACK_measurement_vector vector;
    float array[GTRACK_MEASUREMENT_VECTOR_SIZE];
} GTRACK_measurementUnion;


/**
 * @brief 
 *  GTRACK target descriptor
 *
 * @details
 *  The structure describes target descriptorformat
 */
typedef struct
{
	/**  @brief   Tracking Unit Identifier */
	uint8_t uid;
	/**  @brief   Target Identifier */
	uint32_t tid;
	/**  @brief   State vector */
	float S[GTRACK_STATE_VECTOR_SIZE];
	/**  @brief   Group covariance matrix */
	float EC[GTRACK_MEASUREMENT_VECTOR_SIZE*GTRACK_MEASUREMENT_VECTOR_SIZE];
	/**  @brief   Gain factor */
	float G;
	/**  @brief   Estimated target dimensions: depth, width, [height], doppler */
	float dim[GTRACK_MEASUREMENT_VECTOR_SIZE];

} GTRACK_targetDesc;

extern void *gtrack_create(GTRACK_moduleConfig *config, int32_t *errCode);
extern void gtrack_step(void *handle, GTRACK_measurementPoint *point, GTRACK_measurement_vector *var, uint16_t mNum, GTRACK_targetDesc *t, uint16_t *tNum, uint8_t *mIndex, uint32_t *bench);
extern void gtrack_delete(void *handle);

/**
@} */

/* External dependencies */
/** @addtogroup GTRACK_Dependencies
 @{ */

/**
* @brief 
*  GTRACK calls this function to allocate memory. Expects the void pointer if allocation is sucessful, and NULL otherwise
*
*  @param[in]  numElements
*      Number of elements to allocate
*  @param[in]  sizeInBytes
*      Size of each element in bytes to allocate
*/
extern void *gtrack_alloc(uint32_t numElements, uint32_t sizeInBytes);

/**
* @brief 
*  GTRACK calls this function to free memory
*
*  @param[in]  pFree
*      Pointer to a memmory to free
*  @param[in]  sizeInBytes
*      Size of memory in bytes to free
*/
extern void gtrack_free(void *pFree, uint32_t sizeInBytes);

/* For Matlab MEX environment, redefine gtrack_log with mexPrintf */
#ifdef _MEX_
	#define gtrack_log(level, format, ...) mexPrintf(format, ##__VA_ARGS__);
	extern int mexPrintf(const char *format, ...);
#else
/**
* @brief 
*  GTRACK calls this function to log the events
*
*  @param[in]  level
*      Level is the event importance
*  @param[in]  format
*      Format is the variable size formated output
*/
	extern void gtrack_log(GTRACK_VERBOSE_TYPE level, const char *format, ...);
#endif
/**@}*/

/* This is inline implementation of gtrack_assert */
#ifdef _WIN32
#include <assert.h>
#define gtrack_assert(expression) assert(expression)
#endif

#if defined (SUBSYS_MSS) || defined (SUBSYS_DSS)
#include <ti/drivers/osal/DebugP.h>
#define gtrack_assert(expression) DebugP_assert(expression)
#endif

/* This is inline implementation of getCycleCount */
#ifdef _WIN32
#include <intrin.h>
static __inline uint32_t gtrack_getCycleCount(void){
  return (uint32_t)__rdtsc();
}
#endif

/* This defines boolean type for VS2012 and below */
#if defined _WIN32
#ifndef __bool_true_false_are_defined
#define	__bool_true_false_are_defined	1

#ifndef false
#define	false	0
#endif

#ifndef true
#define	true	1
#endif

#define	bool	_Bool
typedef unsigned char _Bool;

#endif
#endif

#ifdef SUBSYS_MSS
#define	far		/* nothing */
#define	near	/* nothing */
#if defined (__GNUC__) && !defined(__ti__)
static inline uint32_t gtrack_getCycleCount (void)
{
    uint32_t value;
    // Read CCNT Register
    asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));
    return value;
}
#else
#define gtrack_getCycleCount() __MRC(15, 0, 9, 13, 0)
#endif
#endif

#ifdef SUBSYS_DSS
#include <c6x.h>
#define  gtrack_getCycleCount() TSCL
#endif

#ifdef __cplusplus
}
#endif

#endif /* GTRACK_H */
