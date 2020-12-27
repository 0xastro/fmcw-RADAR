#ifndef _TESTLIMITS_H
#define _TESTLIMITS_H
#ifdef SUBSYS_MSS
    #ifdef GTRACK_3D
        /* Acceptable Resource Limits for 3D Tracker @ R4F */
        #define LIMIT_PROG_MEMORY_SIZE          20000   /* shall take < 20k of proggram memory */
        #define LIMIT_DATA_MEMORY_SIZE_MODULE   3200    /* 3.2k data memory for module */
        #define LIMIT_DATA_MEMORY_SIZE_UNIT     1300    /* 1.3k of data memory per unit, so 20 tracks shall take < 3.2k+20*1.3 = 29.2k data memory  */
        #define LIMIT_USEC_PER_TRACK_AVE        250     /* shall take < 250us per track at average */
        #define LIMIT_USEC_PER_TRACK_MAX        350     /* shall take < 350us per track at maximum */
    #else
        /* Acceptable Resource Limits for 2D Tracker @ R4F */
        #define LIMIT_PROG_MEMORY_SIZE          18000   /* shall take  < 18k of program memory */
        #define LIMIT_DATA_MEMORY_SIZE_MODULE   2800    /* 2.8k data memory for module */
        #define LIMIT_DATA_MEMORY_SIZE_UNIT     800     /* 0.8k of data memory per unit, so 20 tracks shall take < 2.8k+20*0.8 = 18.8k data memory  */
        #define LIMIT_USEC_PER_TRACK_AVE        200     /* shall take < 200us per track at average */
        #define LIMIT_USEC_PER_TRACK_MAX        250     /* shall take < 250us per track at maximum */
    #endif
#else
    #ifdef GTRACK_3D
        /* Acceptable Resource Limits for 3D Tracker @ C67 */
        #define LIMIT_PROG_MEMORY_SIZE          31000   /* shall take < 31k of proggram memory */
        #define LIMIT_DATA_MEMORY_SIZE_MODULE   3200    /* 3.2k data memory for module */
        #define LIMIT_DATA_MEMORY_SIZE_UNIT     1300    /* 1.3k of data memory per unit, so 20 tracks shall take < 3.2k+20*1.3 = 29.2k data memory  */
        #define LIMIT_USEC_PER_TRACK_AVE       	120     /* shall take < 120us per track at average */
        #define LIMIT_USEC_PER_TRACK_MAX        200     /* shall take < 200us per track at maximum */
    #else
        /* Acceptable Resource Limits for 2D Tracker @ C67 */
        #define LIMIT_PROG_MEMORY_SIZE          29000   /* shall take  < 29k of program memory */
        #define LIMIT_DATA_MEMORY_SIZE_MODULE   2800    /* 3k data memory for module */
        #define LIMIT_DATA_MEMORY_SIZE_UNIT     800     /* 0.8k of data memory per unit, so 20 tracks shall take < 2.8k+20*0.8 = 18.8k data memory  */
        #define LIMIT_USEC_PER_TRACK_AVE        80     	/* shall take < 80us per tarck at average */
        #define LIMIT_USEC_PER_TRACK_MAX        120     /* shall take < 120us per track at maximum */
    #endif
#endif
#endif

