%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%      (C) Copyright 2016 Texas Instruments, Inc.
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions
%  are met:
%
%    Redistributions of source code must retain the above copyright
%    notice, this list of conditions and the following disclaimer.
%
%    Redistributions in binary form must reproduce the above copyright
%    notice, this list of conditions and the following disclaimer in the
%    documentation and/or other materials provided with the
%    distribution.
%
%    Neither the name of Texas Instruments Incorporated nor the names of
%    its contributors may be used to endorse or promote products derived
%    from this software without specific prior written permission.
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
function [] = read_serial_port_and_plot_object_location(comportSnum, dim, record_options)
global  load_parking_assist load_point_cloud_srr load_point_cloud_usrr load_clusters load_trackers view_range use_perspective_projection;
global  platformType MAX_NUM_OBJECTS OBJ_STRUCT_SIZE_BYTES MAX_NUM_CLUSTERS;
global  CLUSTER_STRUCT_SIZE_BYTES TOTAL_PAYLOAD_SIZE_BYTES MAX_NUM_TRACKERS;
global TRACKER_STRUCT_SIZE_BYTES STATS_SIZE_BYTES bytevec_log readUartFcnCntr;
global ELEV_VIEW EXIT_KEY_PRESSED BYTE_VEC_ACC_MAX_SIZE bytevecAcc;
global BYTES_AVAILABLE_FCN_CNT BYTES_AVAILABLE_FLAG bytevecAccLen StatsInfo;
global activeFrameCPULoad interFrameCPULoad guiCPULoad guiProcTime;
global loggingEnable fidLog recordingHandle;


% The SRR demo only works on the 1642.
platformType = hex2dec('a1642');

MMWDEMO_UART_MSG_DETECTED_POINTS    = 1;
MMWDEMO_UART_MSG_CLUSTERS           = 2;
MMWDEMO_UART_MSG_TRACKED_OBJ        = 3;
MMWDEMO_UART_MSG_PARKING_ASSIST     = 4;
MMWDEMO_UART_MSG_STATS = 6;

load_point_cloud_srr = 1;
load_point_cloud_usrr = 1;
load_clusters  = 1;
load_trackers = 1;
view_range = 0;
load_parking_assist = 0;
view_range_curr = view_range;
%% Initialize some constants.
MAX_NUM_OBJECTS = 200;
OBJ_STRUCT_SIZE_BYTES = 10;
MAX_NUM_CLUSTERS = 24;
CLUSTER_STRUCT_SIZE_BYTES = 8;
MAX_NUM_TRACKERS = 24;
TRACKER_STRUCT_SIZE_BYTES = 12;
STATS_SIZE_BYTES = 16;
bytevec_log = zeros(0,1,'single');
readUartFcnCntr = 0;
ELEV_VIEW = 3;
EXIT_KEY_PRESSED = 0;
BYTES_AVAILABLE_FLAG = 0;
BYTES_AVAILABLE_FCN_CNT = 32*8;
BYTE_VEC_ACC_MAX_SIZE  = 2^16;
bytevecAcc = zeros(BYTE_VEC_ACC_MAX_SIZE,1);
bytevecAccLen = 0;

%% Some more Initialisations
StatsInfo.interFrameProcessingTime = 0;
StatsInfo.transmitOutputTime = 0;
StatsInfo.interFrameProcessingMargin = 0;
StatsInfo.interChirpProcessingMargin = 0;
StatsInfo.interFrameCPULoad = 0;
StatsInfo.activeFrameCPULoad = 0;

activeFrameCPULoad = zeros(100,1,'single');
interFrameCPULoad = zeros(100,1,'single');
guiCPULoad = zeros(100,1,'single');
view_range = 0;
guiProcTime = 0;

displayUpdateCntr   = 0;
timeout_ctr         = 0;
bytevec_cp_max_len  = 2^15;
bytevec_cp          = zeros(bytevec_cp_max_len,1,'uint8');
bytevec_cp_len      = 0;
packetNumberPrev    = 0;
loggingEnable       = 0;
fidLog              = 0;
use_perspective_projection = 0;
prev_use_perspective_projection = use_perspective_projection;

%% Parse CLI parameters
Params = generate_params_for_SRR();

%% Configure monitoring UART port
sphandle = configureSport(comportSnum);

recordingHandle.enable = record_options.record;
if recordingHandle.enable == 1
    recordingHandle.size = 2^28;
    recordingHandle.indx = 1;
    recordingHandle.array = zeros(recordingHandle.size,1,'uint8');
end

% Initialize the GUI.
guiMonitor = gui_initializer(Params, dim,0);
magicNotOkCntr=0;
% Every packet from the AWR1642 has the following barker code at the beginning.
barker_code = char([2 1 4 3 6 5 8 7]);
%-------------------- Main Loop ------------------------
while (~EXIT_KEY_PRESSED)
    %Read bytes from the UART port. 
    readUartCallbackFcn(sphandle, 0);
    
	% If bytes are available, append the new bytes to bytevec_cp
    if BYTES_AVAILABLE_FLAG == 1
        BYTES_AVAILABLE_FLAG = 0;
        %fprintf('bytevec_cp_len, bytevecAccLen = %d %d \n',bytevec_cp_len, bytevecAccLen)
        if (bytevec_cp_len + bytevecAccLen) < bytevec_cp_max_len
            bytevec_cp(bytevec_cp_len+1:bytevec_cp_len + bytevecAccLen) = bytevecAcc(1:bytevecAccLen);
            bytevec_cp_len = bytevec_cp_len + bytevecAccLen;
            bytevecAccLen = 0;
        else
            fprintf('Error: Buffer overflow, bytevec_cp_len, bytevecAccLen = %d %d \n',bytevec_cp_len, bytevecAccLen)
            bytevecAccLen = 0;
            bytevec_cp_len = 0;
        end
    end
    
    try 
    bytevecStr = (bytevec_cp);
    magicOk = 0;
	% if the bytevecStr is atleast as large as the header, check if it contains the header. 
    if (bytevec_cp_len > 72) && (size(bytevecStr,2) == 1)
        startIdx = strfind(bytevecStr', barker_code);
    else
        startIdx = [];
    end
    if ~isempty(startIdx)
        if startIdx(1) > 1
            bytevec_cp(1: bytevec_cp_len-(startIdx(1)-1)) = bytevec_cp(startIdx(1):bytevec_cp_len);
            bytevec_cp_len = bytevec_cp_len - (startIdx(1)-1);
        end
        if bytevec_cp_len < 0
            fprintf('Error: %d %d \n',bytevec_cp_len, bytevecAccLen)
            bytevec_cp_len = 0;
        end
        
        packetlenNum = single(bytevec_cp(8+4+[1:4]));
        totalPacketLen = sum(packetlenNum .* [1 256 65536 16777216]');
        if bytevec_cp_len >= totalPacketLen
            magicOk = 1;
        else
            magicOk = 0;
        end
    end
    
    byteVecIdx = 0;
    if(magicOk == 1)
        tStart = tic;
        bytevec_cp_flt = single(bytevec_cp);
        % Extract the header. 
		[Header, byteVecIdx] = getHeader(bytevec_cp_flt, byteVecIdx);
        sfIdx = Header.subframeNumber+1;
        if (sfIdx > 2) || (Header.numDetectedObj > MAX_NUM_OBJECTS)
            continue;
        end
        detObj.numObj = 0;
        clusterObj.numObj = 0;
        trackedObj.numObj = 0;
        
        % Extract each of the TLVs (type length value) in the current message. 
        for tlvIdx = 1:Header.numTLVs
            [tlv, byteVecIdx] = getTlv(bytevec_cp_flt, byteVecIdx);
            switch tlv.type
                case MMWDEMO_UART_MSG_DETECTED_POINTS
                    if tlv.length >= OBJ_STRUCT_SIZE_BYTES
                        [detObj, byteVecIdx] = getDetObj(bytevec_cp_flt, ...
                            byteVecIdx, ...
                            tlv.length);
                    end
                case MMWDEMO_UART_MSG_CLUSTERS
                    if tlv.length >= CLUSTER_STRUCT_SIZE_BYTES
                        [clusterObj, byteVecIdx] = getClusters(bytevec_cp_flt, ...
                            byteVecIdx, ...
                            tlv.length);
                    end
                case MMWDEMO_UART_MSG_TRACKED_OBJ
                    if tlv.length >= TRACKER_STRUCT_SIZE_BYTES
                        [trackedObj, byteVecIdx] = getTrackers(bytevec_cp_flt, byteVecIdx, tlv.length);
                    end
                    
                case MMWDEMO_UART_MSG_PARKING_ASSIST
                    [parkingAssistRangeBins, byteVecIdx] = getParkingAssistBins(bytevec_cp_flt, byteVecIdx, tlv.length);
                    
                case MMWDEMO_UART_MSG_STATS
                    [StatsInfo, byteVecIdx] = getStatsInfo(bytevec_cp_flt, byteVecIdx);
                    %fprintf('StatsInfo: %d, %d, %d %d \n', StatsInfo.interFrameProcessingTime, StatsInfo.transmitOutputTime, StatsInfo.interFrameProcessingMargin, StatsInfo.interChirpProcessingMargin);
                    displayUpdateCntr = displayUpdateCntr + 1;
                    interFrameCPULoad = [interFrameCPULoad(2:end); StatsInfo.interFrameCPULoad];
                    activeFrameCPULoad = [activeFrameCPULoad(2:end); StatsInfo.activeFrameCPULoad];
                    guiCPULoad = [guiCPULoad(2:end); 100*guiProcTime/Params(1).frameCfg.framePeriodicity];
                    if displayUpdateCntr == 40
                        UpdateDisplayTable(Params);
                        displayUpdateCntr = 0;
                    end
                otherwise
            end
        end
        
        byteVecIdx = Header.totalPacketLen;
        
        if ((Header.frameNumber - packetNumberPrev) ~= 1) && (packetNumberPrev ~= 0)
            fprintf('Error: Packets lost: %d, current frame num = %d \n', (Header.frameNumber - packetNumberPrev - 1), Header.frameNumber)
        end
        packetNumberPrev = Header.frameNumber;
        
        % Display
        % 1. Detected objects
        if sfIdx == 1
            if (detObj.numObj > 0) && load_point_cloud_srr
                set(guiMonitor.detectedObjectsPlotHndA, 'Xdata', detObj.x, 'Ydata', detObj.y);
                set(guiMonitor.detectedObjectsRngDopPlotHndA, 'Xdata', detObj.range, 'Ydata', detObj.doppler);
            else
                set(guiMonitor.detectedObjectsPlotHndA, 'Xdata', inf, 'Ydata', inf);
                set(guiMonitor.detectedObjectsRngDopPlotHndA, 'Xdata', inf, 'Ydata', inf);
            end
        else
            if (detObj.numObj > 0) && load_point_cloud_usrr
                set(guiMonitor.detectedObjectsPlotHndB, 'Xdata', detObj.x, 'Ydata', detObj.y);
                set(guiMonitor.detectedObjectsRngDopPlotHndB, 'Xdata', detObj.range, 'Ydata', detObj.doppler);
                set(guiMonitor.detectedObjectsPlotYZHndB, 'Xdata', detObj.y, 'Ydata', detObj.z);
 
            else
                set(guiMonitor.detectedObjectsPlotHndB, 'Xdata', inf, 'Ydata', inf);
                set(guiMonitor.detectedObjectsRngDopPlotHndB, 'Xdata', inf, 'Ydata', inf);
                set(guiMonitor.detectedObjectsPlotYZHndB, 'Xdata', inf, 'Ydata', inf);
            end
        end
        
        % 2. Clusters.
        if sfIdx == 2
            if (clusterObj.numObj > 0) && load_clusters
                set(guiMonitor.clustersPlotHndB, 'Xdata', clusterObj.x_loc, 'Ydata', clusterObj.y_loc);
            else
                set(guiMonitor.clustersPlotHndB, 'Xdata', inf, 'Ydata', inf);
            end
        end
        
        % 3. Tracking
        if (trackedObj.numObj > 0) && load_trackers
            set(guiMonitor.trackedObjPlotHnd, 'Xdata', trackedObj.x, 'Ydata', trackedObj.y);
            set(guiMonitor.trackedObjRngDop, 'Xdata', trackedObj.range, 'Ydata', trackedObj.doppler);
            set(guiMonitor.clustersPlotHndA, 'Xdata', trackedObj.clusters_x_loc, 'Ydata', trackedObj.clusters_y_loc);
        else
            if sfIdx == 1
                set(guiMonitor.trackedObjPlotHnd, 'Xdata', inf, 'Ydata', inf);
                set(guiMonitor.trackedObjRngDop,  'Xdata', inf, 'Ydata', inf);
                set(guiMonitor.clustersPlotHndA,  'Xdata', inf, 'Ydata', inf);
            end
        end
        
        % 4. Parking Assist
        if sfIdx == 2
            if load_parking_assist
                set(guiMonitor.parkingAssistRangeBinsHnd, 'Xdata', parkingAssistRangeBins.x,'Ydata', parkingAssistRangeBins.y);
            else
                set(guiMonitor.parkingAssistRangeBinsHnd, 'Xdata', inf,'Ydata', inf);
            end
        end
        
        guiProcTime = round(toc(tStart) * 1e3);
    else
        magicNotOkCntr = magicNotOkCntr + 1;
    end
    
    %Remove processed data
    if byteVecIdx > 0
        shiftSize = byteVecIdx;
        bytevec_cp = bytevec_cp(shiftSize+1:bytevec_cp_len);
        bytevec_cp_len = bytevec_cp_len - shiftSize;
        if bytevec_cp_len < 0
            fprintf('Error: bytevec_cp_len < bytevecAccLen, %d %d \n', bytevec_cp_len, bytevecAccLen)
            bytevec_cp_len = 0;
        end
    end
    
    if bytevec_cp_len > (bytevec_cp_max_len * 7/8)
        bytevec_cp_len = 0;
    end
        
    % Update near view. 
	if (view_range_curr ~= view_range)
        if view_range == 0
            range_depth_tmp = dim.max_dist_y;
            range_width_tmp = dim.max_dist_x;
            dopplerRange_tmp = dim.max_vel;
        else
            range_depth_tmp = dim.max_dist_y/4;
            range_width_tmp = dim.max_dist_x/4;
            dopplerRange_tmp = dim.max_vel/4;
        end
        view_range_curr = view_range;
        
        subplot(guiMonitor.detectedObjectsFigHnd);
        axis([-range_width_tmp range_width_tmp 0 range_depth_tmp]);
        subplot(guiMonitor.detectedObjectsRngDopFigHnd);
        axis([0 range_depth_tmp -dopplerRange_tmp dopplerRange_tmp]);
        subplot(guiMonitor.detectedObjectsYZFigHnd);
        axis([0 range_depth_tmp -dim.max_dist_z/4 dim.max_dist_z ]) 
    
    end
    
    if use_perspective_projection ~= prev_use_perspective_projection
        camproj(guiMonitor.detectedObjectsFigHnd, 'perspective');
        campos(guiMonitor.detectedObjectsFigHnd, [0,0,dim.max_dist_y/2]);
        camtarget([0,dim.max_dist_y*0.33,0]);
        prev_use_perspective_projection = use_perspective_projection;
    end
    tIdleStart = tic;
        
    if(toc(tIdleStart) > 2*Params(1).frameCfg.framePeriodicity/1000)
        timeout_ctr=timeout_ctr+1;
        tIdleStart = tic;
    end
    
    if magicNotOkCntr > 3000 
        
        fclose(sphandle); %close com port (or file)
        magicNotOkCntr  =  0; 
        displayUpdateCntr   = 0;
        timeout_ctr         = 0;
        bytevec_cp_max_len  = 2^15;
        bytevec_cp          = zeros(bytevec_cp_max_len,1,'uint8');
        bytevec_cp_len      = 0;
        packetNumberPrev    = 0;
        loggingEnable       = 0;
        fidLog              = 0;
        use_perspective_projection = 0;

        sphandle = configureSport(comportSnum);
    end
    catch
        disp('Recovering...');
        
        if byteVecIdx > 0
            shiftSize = byteVecIdx;
            bytevec_cp = bytevec_cp(shiftSize+1:bytevec_cp_len);
            bytevec_cp_len = bytevec_cp_len - shiftSize;
            if bytevec_cp_len < 0
                fprintf('Error: bytevec_cp_len < bytevecAccLen, %d %d \n', bytevec_cp_len, bytevecAccLen)
                bytevec_cp_len = 0;
            end
        end
    end
	%    
    pause(0.003);
    
end
%close and delete handles before exiting
close(guiMonitor.figHnd); % close figure
fclose(sphandle); %close com port (or file)
delete(guiMonitor.figHnd);
if record_options.record == 1
    fid = fopen(record_options.filename_rec,'w');
    fwrite(fid,recordingHandle.array(1:recordingHandle.indx),'uint8');
    fclose(fid);
end


return

function [] = readUartCallbackFcn(obj, event)
global bytevecAcc;
global bytevecAccLen;
global readUartFcnCntr;
global BYTES_AVAILABLE_FLAG
global BYTE_VEC_ACC_MAX_SIZE
global EXIT_KEY_PRESSED
global recordingHandle;

bytesToRead = get(obj,'BytesAvailable');
if(bytesToRead == 0)
    return;
end

[bytevec, byteCount] = fread(obj, bytesToRead, 'uint8');

if bytevecAccLen + length(bytevec) < BYTE_VEC_ACC_MAX_SIZE * 3/4
    bytevecAcc(bytevecAccLen+1:bytevecAccLen+byteCount) = bytevec;
    bytevecAccLen = bytevecAccLen + byteCount;
else
    bytevecAccLen = 0;
end

if recordingHandle.enable
    recordingHandle.array(recordingHandle.indx+1:recordingHandle.indx+byteCount) = bytevec;
    recordingHandle.indx = recordingHandle.indx + byteCount;
    if (recordingHandle.indx  + 16384 > recordingHandle.size)
        % Exit as soon as the recording buffer is close to being full.
        EXIT_KEY_PRESSED = 1;
    end
end

readUartFcnCntr = readUartFcnCntr + 1;
BYTES_AVAILABLE_FLAG = 1;
return

function [] = dispError()
disp('error!');
return




function [y] = pow2roundup (x)
y = 1;
while x > y
    y = y * 2;
end
return





function [Header, idx] = getHeader(bytevec, idx)
idx = idx + 8; %Skip magic word
word = [1 256 65536 16777216]';
Header.version = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.totalPacketLen = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.platform = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.frameNumber = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.timeCpuCycles = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.numDetectedObj = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.numTLVs = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
Header.subframeNumber = sum(bytevec(idx+[1:4]) .* word);
idx = idx + 4;
return

function [tlv, idx] = getTlv(bytevec, idx)
word = [1 256 65536 16777216]';
tlv.type = sum(bytevec(idx+(1:4)) .* word);
idx = idx + 4;
tlv.length = sum(bytevec(idx+(1:4)) .* word);
idx = idx + 4;
return

function [detObj, idx] = getDetObj(bytevec, idx, tlvLen)
global OBJ_STRUCT_SIZE_BYTES;
detObj =[];
detObj.numObj = 0;
len_bytevec = length(bytevec);
if len_bytevec < idx + 4; 
    idx = len_bytevec;
    return;
end
if tlvLen > 0
    %Get detected object descriptor
    word = [1 256]';
    detObj.numObj = sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    xyzQFormat = 2^sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    invXyzQFormat = 1/xyzQFormat;
    %Check if the array can be fulfilled. 
    if len_bytevec < idx + detObj.numObj*OBJ_STRUCT_SIZE_BYTES
        detObj.numObj = 0; 
        idx = len_bytevec;
        return;
    end
    bytes = bytevec(idx+(1:detObj.numObj*OBJ_STRUCT_SIZE_BYTES));
    idx = idx + detObj.numObj*OBJ_STRUCT_SIZE_BYTES;
    
    bytes = reshape(bytes, OBJ_STRUCT_SIZE_BYTES, detObj.numObj);
    detObj.doppler = (bytes(1,:)+bytes(2,:)*256);
    detObj.peakVal = bytes(3,:)+bytes(4,:)*256; %peak value (16-bit=> so constructed from 2 bytes)
    
    detObj.x = bytes(5,:)+bytes(6,:)*256;
    detObj.y = bytes(7,:)+bytes(8,:)*256;
    detObj.z = bytes(9,:)+bytes(10,:)*256;
    detObj.x( detObj.x > 32767) =  detObj.x( detObj.x > 32767) - 65536;
    detObj.y( detObj.y > 32767) =  detObj.y( detObj.y > 32767) - 65536;
    detObj.z( detObj.z > 32767) =  detObj.z( detObj.z > 32767) - 65536;
    detObj.doppler(detObj.doppler > 32767) = detObj.doppler(detObj.doppler > 32767) -65536;
    detObj.x = detObj.x*invXyzQFormat;
    detObj.y = detObj.y*invXyzQFormat;
    detObj.z = -(detObj.z*invXyzQFormat);
    detObj.doppler = detObj.doppler*invXyzQFormat;
    detObj.range = sqrt(detObj.y.*detObj.y + detObj.x.*detObj.x + detObj.z.*detObj.z);
    
%     disp([detObj.x;detObj.y;detObj.z]);
end
return

function [clusterObj, idx] = getClusters(bytevec, idx, tlvLen)
global CLUSTER_STRUCT_SIZE_BYTES;
clusterObj =[];
clusterObj.numObj = 0;

len_bytevec = length(bytevec);
if len_bytevec < idx +4; 
    idx = len_bytevec;
    return;
end
if tlvLen > 0
    %Get detected object descriptor
    word = [1 256]';
    clusterObj.numObj = sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    onebyXyzQFormat = 1/2^sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    
    %Get detected array of detected objects
    if len_bytevec < idx + (1:clusterObj.numObj*CLUSTER_STRUCT_SIZE_BYTES); 
        clusterObj.numObj = 0;
        idx = len_bytevec;
        return;
    end
    bytes = bytevec(idx+(1:clusterObj.numObj*CLUSTER_STRUCT_SIZE_BYTES));
    idx = idx + clusterObj.numObj*CLUSTER_STRUCT_SIZE_BYTES;
    
    bytes = reshape(bytes, CLUSTER_STRUCT_SIZE_BYTES, clusterObj.numObj);
    x = bytes(1,:)+bytes(2,:)*256;
    y = bytes(3,:)+bytes(4,:)*256;
    x( x > 32767) =  x( x > 32767) - 65536;
    y( y > 32767) =  y( y > 32767) - 65536;
    x = x.*onebyXyzQFormat;
    y = y.*onebyXyzQFormat;
    
    x_size = bytes(5,:)+bytes(6,:)*256;
    y_size = bytes(7,:)+bytes(8,:)*256;
    x_size = x_size.*onebyXyzQFormat;
    y_size = y_size.*onebyXyzQFormat;
    area = 4*x_size.*y_size;
    
    % avoid printing large clusters. 
    x_size(area > 20) = inf;
    
    x_loc = (repmat(x',[1,6])) + x_size'*([-1 1 1 -1 -1 inf]);
    y_loc = (repmat(y',[1,6])) + y_size'*([-1 -1 1 1 -1 inf]);
    
    x_loc = x_loc';
    y_loc = y_loc';
    
    clusterObj.x_loc = x_loc(:);
    clusterObj.y_loc = y_loc(:);
    
end
return

function [detObj, idx] = getTrackers(bytevec, idx, tlvLen)
global TRACKER_STRUCT_SIZE_BYTES ;
detObj =[];
detObj.numObj = 0;
len_bytevec = length(bytevec);
if len_bytevec < idx +4; 
    idx = len_bytevec;
    return;
end

if tlvLen > 0
    %Get detected object descriptor
    word = [1 256]';
    detObj.numObj = sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    xyzQFormat = 2^sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    onebyXyzQFormat = 1/xyzQFormat;
    %Get detected array of detected objects
    if len_bytevec < idx + (1:detObj.numObj*TRACKER_STRUCT_SIZE_BYTES); 
        detObj.numObj = 0;
        idx = len_bytevec;
        return;
    end
    bytes = bytevec(idx+(1:detObj.numObj*TRACKER_STRUCT_SIZE_BYTES ));
    idx = idx + detObj.numObj*TRACKER_STRUCT_SIZE_BYTES ;
    
    bytes = reshape(bytes, TRACKER_STRUCT_SIZE_BYTES , detObj.numObj);
    detObj.x = bytes(1,:)+bytes(2,:)*256;
    detObj.y = bytes(3,:)+bytes(4,:)*256;
    detObj.x( detObj.x > 32767) =  detObj.x( detObj.x > 32767) - 65536;
    detObj.y( detObj.y > 32767) =  detObj.y( detObj.y > 32767) - 65536;
    detObj.x = detObj.x*onebyXyzQFormat;
    detObj.y = detObj.y*onebyXyzQFormat;
    
    detObj.vx = bytes(5,:)+bytes(6,:)*256;
    detObj.vy = bytes(7,:)+bytes(8,:)*256;
    detObj.vx( detObj.vx > 32767) =  detObj.vx( detObj.vx > 32767) - 65536;
    detObj.vy( detObj.vy > 32767) =  detObj.vy( detObj.vy > 32767) - 65536;
    detObj.vx = detObj.vx*onebyXyzQFormat;
    detObj.vy = detObj.vy*onebyXyzQFormat;
    x_size = bytes(9,:)+bytes(10,:)*256;
    y_size = bytes(11,:)+bytes(12,:)*256;
    
    x_size = x_size.*onebyXyzQFormat;
    y_size = y_size.*onebyXyzQFormat;
    
    x_loc = (repmat(detObj.x',[1,6])) + x_size'*([-1 1 1 -1 -1 inf]);
    y_loc = (repmat((detObj.y+y_size)',[1,6])) + y_size'*([-1 -1 1 1 -1 inf]);
    
    x_loc = x_loc';
    y_loc = y_loc';
    detObj.clusters_x_loc = x_loc(:);
    detObj.clusters_y_loc = y_loc(:);
    
    detObj.range = sqrt(detObj.y.*detObj.y + detObj.x.*detObj.x);
    detObj.doppler = (detObj.vy.*detObj.y + detObj.vx.*detObj.x)./detObj.range;
    
end
return

function [PA, idx] = getParkingAssistBins(bytevec, idx, tlvLen)
PARKING_ASSIST_BIN_SIZE_BYTES = 2;
detObj =[];
detObj.numObj = 0;
PA.x = [];
PA.y = [];
len_bytevec = length(bytevec);
if len_bytevec < idx +4; 
    idx = len_bytevec;
    return;
end
if tlvLen > 0
    %Get detected object descriptor
    word = [1 256]';
    detObj.numObj = sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    xyzQFormat = 2^sum(bytevec(idx+(1:2)) .* word);
    idx = idx + 2;
    oneByxyzQFormat = 1/xyzQFormat;
    
    %Get detected array of detected objects
    if len_bytevec < idx + (1:detObj.numObj*PARKING_ASSIST_BIN_SIZE_BYTES); 
        idx = len_bytevec;
        return;
    end
    bytes = bytevec(idx+(1:detObj.numObj*PARKING_ASSIST_BIN_SIZE_BYTES ));
    idx = idx + detObj.numObj*PARKING_ASSIST_BIN_SIZE_BYTES;
    
    bytes = reshape(bytes, PARKING_ASSIST_BIN_SIZE_BYTES, detObj.numObj);
    range = ((bytes(1,:)+bytes(2,:)*256)*oneByxyzQFormat);
    
    range = fftshift(range);
    
    range = [range range(1)];
    
    xl = linspace(-1,1,detObj.numObj+2);
    yl = sqrt(1 - (xl.*xl));
    x_1 = range.*xl(1:end-1);
    x_2 = range.*xl(2:end);
    y_1 = range.*yl(1:end-1);
    y_2 = range.*yl(2:end);
    
    x = [x_1; x_2]; y = [y_1; y_2];
    
    PA.x = x(2:end-1);
    PA.y = y(2:end-1);
end
return

function [StatsInfo, idx] = getStatsInfo(bytevec, idx)
    word = [1 256 65536 16777216]';
    StatsInfo.interFrameProcessingTime = sum(bytevec(idx+(1:4)) .* word);
    idx = idx + 4;
    StatsInfo.transmitOutputTime = sum(bytevec(idx+(1:4)) .* word);
    idx = idx + 4;
    StatsInfo.interFrameProcessingMargin = sum(bytevec(idx+(1:4)) .* word);
    idx = idx + 4;
    StatsInfo.interChirpProcessingMargin = sum(bytevec(idx+(1:4)) .* word);
    idx = idx + 4;
    StatsInfo.activeFrameCPULoad = sum(bytevec(idx+(1:4)) .* word);
    idx = idx + 4;
    StatsInfo.interFrameCPULoad = sum(bytevec(idx+(1:4)) .* word);
    idx = idx + 4;
return


function [sphandle] = configureSport(comportSnum)
    global BYTES_AVAILABLE_FCN_CNT;

    if ~isempty(instrfind('Type','serial'))
        disp('Serial port(s) already open. Re-initializing...');
        delete(instrfind('Type','serial'));  % delete open serial ports.
    end
    comportnum_str = ['COM' num2str(comportSnum)];
    sphandle = serial(comportnum_str,'BaudRate',921600);
    set(sphandle,'InputBufferSize', 2^16);
    set(sphandle,'Timeout',10);
    set(sphandle,'ErrorFcn',@dispError);
    set(sphandle,'BytesAvailableFcnMode','byte');
    set(sphandle,'BytesAvailableFcnCount', 2^16+1);%BYTES_AVAILABLE_FCN_CNT);
    set(sphandle,'BytesAvailableFcn',@readUartCallbackFcn);
    fopen(sphandle);
return