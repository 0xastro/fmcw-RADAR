 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 %
 % (C) Copyright 2018 Texas Instruments, Inc.
 %
 %  Redistribution and use in source and binary forms, with or without
 %  modification, are permitted provided that the following conditions
 %  are met:
 %
 %  Redistributions of source code must retain the above copyright
 %  notice, this list of conditions and the following disclaimer.
 %
 %  Redistributions in binary form must reproduce the above copyright
 %  notice, this list of conditions and the following disclaimer in the
 %  documentation and/or other materials provided with the
 %  distribution.
 %
 %  Neither the name of Texas Instruments Incorporated nor the names of
 %  its contributors may be used to endorse or promote products derived
 %  from this software without specific prior written permission.
 %
 %  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 %  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 %  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 %  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 %  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 %  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 %  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 %  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 %  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 %  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 %  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Provide (a) The binary file saved from the PC-GUI 
% AND     (b) The corresponding Configuration file used to run the GUI 

% Variables saved in the Workspace
% rangeProfile_cplx [rows x columns] : Contains the complex Range-FFT values from the
%                     range-bins corresponsing to the values specified   
%                     in the cfg file. The rows contains the range bins while the  
%                     columns are the chirps/frames. 1st chirp of each
%                     frame is saved.

% vitalSignsDemo_OutputStats: Contains Processed values Outputed from 
%                             the EVM. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fclose all;close all;clear all;clc

% Directory name
dir_name = 'C:\DataDirectory\';

% Binary Data
file_name_bin = 'dataOutputFromEVM.bin';

% Configuration file used to collect the data
cliCfgFileDir  = dir_name;
cliCfgFileName = 'xwr1642_profile_VitalSigns_20fps_Back.cfg';

global TOTAL_PAYLOAD_SIZE_BYTES;

file_path_bin = strcat(dir_name, file_name_bin);
fid_bin = fopen(file_path_bin,'rb');
dataBin = fread(fid_bin,'uint8');
fclose(fid_bin);
cliCfgFilePath = strcat(dir_name, cliCfgFileName);
ParamsStruct= parseCfgFile(cliCfgFilePath);
[TOTAL_PAYLOAD_SIZE_BYTES, indexVitalSignsOutput] = calculate_PayLoadSize(ParamsStruct);
NUM_SAMPLES_EVM = TOTAL_PAYLOAD_SIZE_BYTES;

numChirpsFile = length(dataBin)/NUM_SAMPLES_EVM;
dataAll = reshape(dataBin,[NUM_SAMPLES_EVM numChirpsFile]);
dataEVM = dataAll;

samplingInterval = 1e-3*ParamsStruct.frameCfg.framePeriodicity;  % Frame Periodicity
Fs = 1/samplingInterval;   % Sampling Rate
timeAxis = samplingInterval:samplingInterval:size(dataEVM,2)*samplingInterval;
rangeBinSize_meter = ParamsStruct.dataPath.rangeBinSize_meter;
rangeStartMeters   = ParamsStruct.vitalSignsParams.rangeStartMeters;
rangeEndMeters   = ParamsStruct.vitalSignsParams.rangeEndMeters;
rangeAxis = rangeStartMeters:rangeBinSize_meter:rangeEndMeters;
%%
INDEX_RANGE_PROFILE_START = indexVitalSignsOutput.INDEX_RANGE_PROFILE_START;
numRangeBinsSentUART = ParamsStruct.dataPath.numRangeBinProcessed; 
rangeProfile = dataEVM(INDEX_RANGE_PROFILE_START:(INDEX_RANGE_PROFILE_START + 4*numRangeBinsSentUART)-1,:);
% Convert uint8 to uint16 
rangeProfile = rangeProfile(1:2:end,:)+ rangeProfile(2:2:end,:)*2^8;
% Convert from the 2's complement Form
for col=1:size(rangeProfile,2)
    for i=1:size(rangeProfile,1)
    if (rangeProfile(i,col) >2^15)
    rangeProfile(i,col) = rangeProfile(i,col) - 2^16;
    end
    end
end

% Convert to Complex Format
rangeProfile_cplx = rangeProfile(1:2:end,:) + j*rangeProfile(2:2:end,:);
% Extract the Phase from a range-bin of interest
row_index = 12;
ant1_Ph  = angle(rangeProfile_cplx(row_index,:));
ant1_UnWrapPh = unwrap(ant1_Ph);

figure(1);clf
subplot(311);imagesc(abs(rangeProfile_cplx));
xlabel('Frame Number');ylabel('Range Bins')
title('Magnitude of the Range Profiles')
subplot(312);
plot(abs(rangeProfile_cplx(:,10))); grid on
title('A selected column of the Range Profile')
subplot(313);plot(ant1_UnWrapPh);
ylabel('Radians')
title({strcat('rangeIndex = ',num2str(row_index)), 'Phase'})
grid on

%% Extract Paremeters from the EVM output

vitalSignsDemo_OutputStats = parseDataEVM(dataEVM, indexVitalSignsOutput);
outGlobalCount     = vitalSignsDemo_OutputStats.outGlobalCount;
outPhase           = vitalSignsDemo_OutputStats.outPhase;
outBreathWfm       = vitalSignsDemo_OutputStats.outBreathWfm;
outHeartWfm        = vitalSignsDemo_OutputStats.outHeartWfm;
outHeartRate_FFT   = vitalSignsDemo_OutputStats.outHeartRate_FFT;
outBreathRate_FFT  = vitalSignsDemo_OutputStats.outBreathRate_FFT;
outBreathRate_Peak = vitalSignsDemo_OutputStats.outBreathRate_Peak;
outHeartRate_Peak  = vitalSignsDemo_OutputStats.outHeartRate_Peak;
outConfidenceMetricHeart = vitalSignsDemo_OutputStats.outConfidenceMetricHeart;
outConfidenceMetricBreath = vitalSignsDemo_OutputStats.outConfidenceMetricBreath;
outEnergyBreath = vitalSignsDemo_OutputStats.outEnergyBreath;
outEnergyHeart  = vitalSignsDemo_OutputStats.outEnergyHeart;

figure(2);
plot(outPhase);grid on
title('Phase values (from the EVM)')

figure(9);
subplot(211);plot(outGlobalCount);title('Frame Number saved from the EVM')
subplot(212);plot(diff(outGlobalCount)); title('If not zero indicates that frames have been missed in saving the data')


%% Parses the data from the EVM to extract out the relevant outputs in  vitalSignsDemo_OutputStats
%  dataEVM: Data saved from the EVM 
%  indexVitalSignsOutput:  Index positions of the vital signs Demo Output. 
%                          These index positions are hardcoded in the
%                          firmware on the EVM (subjected to change in later versions)
% vitalSignsDemo_OutputStats : Output structure containing different output values
function [vitalSignsDemo_OutputStats] = parseDataEVM(dataEVM, indexVitalSignsOutput)
vitalSignsDemo_OutputStats = [];
INDEX_GLOBAL_COUNT = indexVitalSignsOutput.INDEX_GLOBAL_COUNT;
INDEX_RANGE_BIN_PHASE = indexVitalSignsOutput.INDEX_RANGE_BIN_PHASE;
INDEX_RANGE_BIN_VALUE = indexVitalSignsOutput.INDEX_RANGE_BIN_VALUE;
INDEX_PHASE = indexVitalSignsOutput.INDEX_PHASE;
INDEX_BREATHING_WAVEFORM = indexVitalSignsOutput.INDEX_BREATHING_WAVEFORM;
INDEX_HEART_WAVEFORM = indexVitalSignsOutput.INDEX_HEART_WAVEFORM;
INDEX_BREATHING_RATE_FFT = indexVitalSignsOutput.INDEX_BREATHING_RATE_FFT;
INDEX_HEART_RATE_EST_FFT = indexVitalSignsOutput.INDEX_HEART_RATE_EST_FFT;
INDEX_BREATHING_RATE_PEAK = indexVitalSignsOutput.INDEX_BREATHING_RATE_PEAK;
INDEX_HEART_RATE_EST_PEAK = indexVitalSignsOutput.INDEX_HEART_RATE_EST_PEAK;
INDEX_CONFIDENCE_METRIC_BREATH = indexVitalSignsOutput.INDEX_CONFIDENCE_METRIC_BREATH;
INDEX_CONFIDENCE_METRIC_HEART = indexVitalSignsOutput.INDEX_CONFIDENCE_METRIC_HEART;
INDEX_ENERGYWFM_BREATH = indexVitalSignsOutput.INDEX_ENERGYWFM_BREATH;
INDEX_ENERGYWFM_HEART = indexVitalSignsOutput.INDEX_ENERGYWFM_HEART;

    for ii=1:size(dataEVM,2)

    temp16 = typecast(uint8([dataEVM(:,ii)]),'uint16');
    temp32 = typecast(uint8([dataEVM(:,ii)]),'uint32');
    tempFloat = typecast(uint8([dataEVM(:,ii)]),'single');
   
   globalCountTemp =  dataEVM([INDEX_GLOBAL_COUNT: INDEX_GLOBAL_COUNT+3],ii);
   outGlobalCount = typecast(uint8([globalCountTemp]),'uint32');
   
   vitalSignsDemo_OutputStats.outGlobalCount(ii)    = outGlobalCount;
   vitalSignsDemo_OutputStats.outPhase(ii)          = tempFloat(INDEX_PHASE);
   vitalSignsDemo_OutputStats.outBreathWfm(ii)      = tempFloat(INDEX_BREATHING_WAVEFORM );
   vitalSignsDemo_OutputStats.outHeartWfm(ii)       = tempFloat(INDEX_HEART_WAVEFORM );
   vitalSignsDemo_OutputStats.outHeartRate_FFT(ii)  = tempFloat(INDEX_HEART_RATE_EST_FFT );
   vitalSignsDemo_OutputStats.outBreathRate_FFT(ii) = tempFloat(INDEX_BREATHING_RATE_FFT );
   vitalSignsDemo_OutputStats.outBreathRate_Peak(ii)= tempFloat(INDEX_BREATHING_RATE_PEAK );
   vitalSignsDemo_OutputStats.outHeartRate_Peak(ii) = tempFloat(INDEX_HEART_RATE_EST_PEAK );
   vitalSignsDemo_OutputStats.outConfidenceMetricHeart(ii)  = tempFloat(INDEX_CONFIDENCE_METRIC_HEART );
   vitalSignsDemo_OutputStats.outConfidenceMetricBreath(ii) = tempFloat(INDEX_CONFIDENCE_METRIC_BREATH );
   vitalSignsDemo_OutputStats.outEnergyBreath(ii) = tempFloat(INDEX_ENERGYWFM_BREATH);
   vitalSignsDemo_OutputStats.outEnergyHeart(ii)  = tempFloat(INDEX_ENERGYWFM_HEART);
   end
end
    
function [INDEX_OUT] = TRANSLATE_INDEX(LENGTH_OFFSET, INDEX_IN)
INDEX_OUT  = (LENGTH_OFFSET + INDEX_IN*4)/4;
end    

function [TOTAL_PAYLOAD_SIZE_BYTES, indexVitalSignsOutput] = calculate_PayLoadSize(ParamsStruct)

LENGTH_MAGIC_WORD_BYTES        = 8; % Length of Magic Word appended to the UART packet from the EVM
LENGTH_DEBUG_DATA_OUT_BYTES    = 128;   % VitalSignsDemo_OutputStats size
LENGTH_HEADER_BYTES            = 40;   % Header + Magic Word
LENGTH_TLV_MESSAGE_HEADER_BYTES = 8;
MMWDEMO_OUTPUT_MSG_SEGMENT_LEN = 32;

   %Calculate monitoring packet size
   numRangeBinProcessed = ParamsStruct.dataPath.numRangeBinProcessed;  
   TOTAL_PAYLOAD_SIZE_BYTES = LENGTH_HEADER_BYTES;
   TOTAL_PAYLOAD_SIZE_BYTES = TOTAL_PAYLOAD_SIZE_BYTES+...
              LENGTH_TLV_MESSAGE_HEADER_BYTES + (4*numRangeBinProcessed);
   TOTAL_PAYLOAD_SIZE_BYTES = TOTAL_PAYLOAD_SIZE_BYTES+...
              LENGTH_TLV_MESSAGE_HEADER_BYTES+LENGTH_DEBUG_DATA_OUT_BYTES;
    
    % Padding 
    if (mod(TOTAL_PAYLOAD_SIZE_BYTES,MMWDEMO_OUTPUT_MSG_SEGMENT_LEN)~=0)
        paddingFactor = ceil(TOTAL_PAYLOAD_SIZE_BYTES/ MMWDEMO_OUTPUT_MSG_SEGMENT_LEN);
        TOTAL_PAYLOAD_SIZE_BYTES = MMWDEMO_OUTPUT_MSG_SEGMENT_LEN*paddingFactor;
    end
    
LENGTH_OFFSET_BYTES   = LENGTH_HEADER_BYTES  - LENGTH_MAGIC_WORD_BYTES + LENGTH_TLV_MESSAGE_HEADER_BYTES
%% This structure defines the index location of various outputs in radar packet
%  Note: The index Is subjected to change  

LENGTH_OFFSET = LENGTH_OFFSET_BYTES + LENGTH_TLV_MESSAGE_HEADER_BYTES;

indexVitalSignsOutput.INDEX_GLOBAL_COUNT             =     21; % Start Byte Number of Global Frame Count in the Header
indexVitalSignsOutput.INDEX_RANGE_BIN_PHASE          =     TRANSLATE_INDEX(LENGTH_OFFSET,1);
indexVitalSignsOutput.INDEX_RANGE_BIN_VALUE          =     TRANSLATE_INDEX(LENGTH_OFFSET,2)
indexVitalSignsOutput.INDEX_PHASE                    =     TRANSLATE_INDEX(LENGTH_OFFSET,5)
indexVitalSignsOutput.INDEX_BREATHING_WAVEFORM       =     TRANSLATE_INDEX(LENGTH_OFFSET,6)
indexVitalSignsOutput.INDEX_HEART_WAVEFORM           =     TRANSLATE_INDEX(LENGTH_OFFSET,7)
indexVitalSignsOutput.INDEX_HEART_RATE_EST_FFT       =     TRANSLATE_INDEX(LENGTH_OFFSET,8)
indexVitalSignsOutput.INDEX_HEART_RATE_EST_FFT_4Hz   =     TRANSLATE_INDEX(LENGTH_OFFSET,9)
indexVitalSignsOutput.INDEX_HEART_RATE_EST_xCorr     =     TRANSLATE_INDEX(LENGTH_OFFSET,10)
indexVitalSignsOutput.INDEX_HEART_RATE_EST_PEAK      =     TRANSLATE_INDEX(LENGTH_OFFSET,11)
indexVitalSignsOutput.INDEX_BREATHING_RATE_FFT       =     TRANSLATE_INDEX(LENGTH_OFFSET,12)
indexVitalSignsOutput.INDEX_BREATHING_RATE_xCorr     =     TRANSLATE_INDEX(LENGTH_OFFSET,13)
indexVitalSignsOutput.INDEX_BREATHING_RATE_PEAK      =     TRANSLATE_INDEX(LENGTH_OFFSET,14)

indexVitalSignsOutput.INDEX_CONFIDENCE_METRIC_BREATH =     TRANSLATE_INDEX(LENGTH_OFFSET,15)
indexVitalSignsOutput.INDEX_CONFIDENCE_METRIC_HEART  =     TRANSLATE_INDEX(LENGTH_OFFSET,17)
indexVitalSignsOutput.INDEX_CONFIDENCE_METRIC_HEART_4Hz =  TRANSLATE_INDEX(LENGTH_OFFSET,18)
indexVitalSignsOutput.INDEX_CONFIDENCE_METRIC_HEART_xCorr = TRANSLATE_INDEX(LENGTH_OFFSET,19)
indexVitalSignsOutput.INDEX_ENERGYWFM_BREATH         =     TRANSLATE_INDEX(LENGTH_OFFSET,20)
indexVitalSignsOutput.INDEX_ENERGYWFM_HEART          =     TRANSLATE_INDEX(LENGTH_OFFSET,21)

indexVitalSignsOutput.INDEX_MOTION_DETECTION         =      TRANSLATE_INDEX(LENGTH_OFFSET,22)
indexVitalSignsOutput.INDEX_RESERVED_2               =     TRANSLATE_INDEX(LENGTH_OFFSET,21)
indexVitalSignsOutput.INDEX_RESERVED_3               =     TRANSLATE_INDEX(LENGTH_OFFSET,22)
indexVitalSignsOutput.INDEX_RESERVED_4               =     TRANSLATE_INDEX(LENGTH_OFFSET,23)
indexVitalSignsOutput.INDEX_RANGE_PROFILE_START      =     (LENGTH_HEADER_BYTES + LENGTH_DEBUG_DATA_OUT_BYTES ...
                                                           + 2*LENGTH_TLV_MESSAGE_HEADER_BYTES)  +  1
 
end
