***************************************************************************
* Readme.txt
***************************************************************************

The directory contains the following test files:-

----------------------------------------------------------
Serialize (serialize.cpp)
----------------------------------------------------------

The binary file saved from the Data Card GUI stores the data in the order the FPGA receives it.

XWR14xx:-
There are 4 Lanes. So the first sample of 16 on lane 0 then 16 bit on lane 1 , lane 2 , lane 3.
Then the next sample of 16bit on the lane 0, lane 1,lane 2 , lane 3 etc.

XWR16xx/XWR18xx/XWR68xx:-
There are 2 Lanes. So the first sample of 16 on lane 0 then 16 bit on lane 1. Then the
next sample of 16bit on the lane 0, lane 1 etc.

This will basically imply that the data sent from the XWR1xx is not in the same format
as when it is received. The serializer provides a simple implementation which serializes the data
discounting the LVDS lanes. The source file is provided as is and can be compiled for the PC
using any native compiler.

For Example:-
gcc serialize.cpp -o serialize.out

The following is the usage:-
serializer.out <Number of LVDS Lanes> <Input Raw File> <Output Serial File>

----------------------------------------------------------
Data Card (data_card.cpp)
----------------------------------------------------------
Once the data has been serialized. The Data Card File Parser can be used to display the contents of
the serialized binary file into human readable form. The HSI Header provides many helper functions
which can be used to parse the header. The goal behind the file is to showcase to users how the
received binary data can be parsed to access the CP, ADC, CQ and User Data.

Compiler Options:-
- Build with one of the following definitions:-
    - _LITTLE_ENDIAN or
    - _BIG_ENDIAN
  To select the endianess of the system.

- Ensure that the $(MMWAVE_SDK_INSTALL_PATH) is there in the "Include Directory"
  The file refers to the hsiprotocol.h which refers to the HSI Protocol helper functions

For Example:-
gcc -D_LITTLE_ENDIAN -I$MMWAVE_SDK_INSTALL_PATH data_card.cpp -o data_card.out

The following is the usage:-
datacard.out <Serial File>

The serial file is the output of the serializer described above.






