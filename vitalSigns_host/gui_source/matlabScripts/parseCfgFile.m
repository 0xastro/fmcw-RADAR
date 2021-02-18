function [P] = parseCfgFile(cliCfgFilePath)

%Read Configuration file
cliCfgFileId = fopen(cliCfgFilePath, 'r');
if cliCfgFileId == -1
    fprintf('File %s not found!\n', cliCfgFilePath);
    return
else
    fprintf('Opening configuration file %s ...\n', cliCfgFilePath);
end
cliCfg=[];
tline = fgetl(cliCfgFileId);
k=1;
while ischar(tline)
    cliCfg{k} = tline;
    tline = fgetl(cliCfgFileId);
    k = k + 1;
end

    P=[];
    for k=1:length(cliCfg)
        C = strsplit(cliCfg{k});
        if strcmp(C{1},'channelCfg')
            P.channelCfg.txChannelEn = str2num(C{3});
            P.dataPath.numTxAzimAnt = bitand(bitshift(P.channelCfg.txChannelEn,0),1) +...
                                      bitand(bitshift(P.channelCfg.txChannelEn,-2),1);
            P.dataPath.numTxElevAnt = bitand(bitshift(P.channelCfg.txChannelEn,-1),1);
            P.channelCfg.rxChannelEn = str2num(C{2});
            P.dataPath.numRxAnt = bitand(bitshift(P.channelCfg.rxChannelEn,0),1) +...
                                  bitand(bitshift(P.channelCfg.rxChannelEn,-1),1) +...
                                  bitand(bitshift(P.channelCfg.rxChannelEn,-2),1) +...
                                  bitand(bitshift(P.channelCfg.rxChannelEn,-3),1);
            P.dataPath.numTxAnt = P.dataPath.numTxElevAnt + P.dataPath.numTxAzimAnt;
                                
        elseif strcmp(C{1},'dataFmt')
        elseif strcmp(C{1},'profileCfg')
            P.profileCfg.startFreq = str2num(C{3});
            P.profileCfg.idleTime =  str2num(C{4});
            P.profileCfg.rampEndTime = str2num(C{6});
            P.profileCfg.freqSlopeConst = str2num(C{9});
            P.profileCfg.numAdcSamples = str2num(C{11});
            P.profileCfg.digOutSampleRate = str2num(C{12}); %uints: ksps
        elseif strcmp(C{1},'chirpCfg')
        elseif strcmp(C{1},'frameCfg')
            P.frameCfg.chirpStartIdx = str2num(C{2});
            P.frameCfg.chirpEndIdx = str2num(C{3});
            P.frameCfg.numLoops = str2num(C{4});
            P.frameCfg.framePeriodicity = str2num(C{6});
        elseif strcmp(C{1},'guiMonitor')
          P.guiMonitor.flag1 =  str2num(C{2});
          P.guiMonitor.flag2 = str2num(C{3});    
          P.guiMonitor.rangeAzimuthHeatMap = str2num(C{4});
          P.guiMonitor.rangeDopplerHeatMap = str2num(C{5});
        
        elseif strcmp(C{1},'vitalSignsCfg')
          P.vitalSignsParams.rangeStartMeters =  str2num(C{2});
          P.vitalSignsParams.rangeEndMeters = str2num(C{3});    
          P.vitalSignsParams.winLenBreath = str2num(C{4});
          P.vitalSignsParams.winLenHeart  = str2num(C{5});
        end
        
    end
    P.dataPath.numChirpsPerFrame = (P.frameCfg.chirpEndIdx -...
                                            P.frameCfg.chirpStartIdx + 1) *...
                                            P.frameCfg.numLoops;
    P.dataPath.numDopplerBins = P.dataPath.numChirpsPerFrame / P.dataPath.numTxAnt;
    P.dataPath.numRangeBins = 2^nextpow2(P.profileCfg.numAdcSamples);
    P.dataPath.rangeResolutionMeters = 3e8 * P.profileCfg.digOutSampleRate * 1e3 /...
                     (2 * P.profileCfg.freqSlopeConst * 1e12 * P.profileCfg.numAdcSamples);
    P.dataPath.rangeIdxToMeters = 3e8 * P.profileCfg.digOutSampleRate * 1e3 /...
                     (2 * P.profileCfg.freqSlopeConst * 1e12 * P.dataPath.numRangeBins);
    P.dataPath.dopplerResolutionMps = 3e8 / (2*P.profileCfg.startFreq*1e9 *...
                                        (P.profileCfg.idleTime + P.profileCfg.rampEndTime) *...
                                        1e-6 * P.dataPath.numDopplerBins * P.dataPath.numTxAnt);
   P.dataPath.chirpDuration_us = (1e3*P.profileCfg.numAdcSamples)/(P.profileCfg.digOutSampleRate);
   freqSlopeConst_temp = 48*P.profileCfg.freqSlopeConst* 2^26 * 1e3/((3.6*1e9)*900);  % To match the C-code 
   P.dataPath.chirpBandwidth_kHz = (freqSlopeConst_temp)*(P.dataPath.chirpDuration_us );
   
   numTemp = (P.dataPath.chirpDuration_us)*(P.profileCfg.digOutSampleRate)*(3e8) ;
   denTemp = 2*(P.dataPath.chirpBandwidth_kHz);
   P.dataPath.rangeMaximum =  numTemp/(denTemp*1e9);
   P.dataPath.rangeBinSize_meter = P.dataPath.rangeMaximum/(P.dataPath.numRangeBins);
   rangeStart_Index = floor(P.vitalSignsParams.rangeStartMeters/P.dataPath.rangeBinSize_meter);
   rangeEnd_Index   = floor(P.vitalSignsParams.rangeEndMeters/P.dataPath.rangeBinSize_meter);
   P.dataPath.lambdaCenter_mm = (3e8/(P.profileCfg.startFreq))/1e6;
   P.dataPath.rangeStart_Index = rangeStart_Index;
   P.dataPath.rangeEnd_Index = rangeEnd_Index;
   P.dataPath.numRangeBinProcessed = rangeEnd_Index - rangeStart_Index + 1;
   fclose(cliCfgFileId);
end



