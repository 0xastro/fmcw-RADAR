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

function guiMonitor = gui_initializer(Params, dim, show_params)
    global MAX_NUM_CLUSTERS
    global  load_parking_assist load_point_cloud_srr load_point_cloud_usrr load_clusters load_trackers view_range use_perspective_projection; 

    if nargin < 3
        show_params = 1; 
    end
    
    mversion = version('-release'); 
    mversion = str2num(mversion(1:4)); 
    if mversion > 2015
        show_legend = 0; 
    else
        show_legend = 1;
    end
    
    %% Setup the main figure
    guiMonitor.detectedObjects = 1; 
    guiMonitor.stats = 0;
    figHnd = figure(1);
    clf(figHnd);
    set(figHnd,'Name','FMCW Radar Visualization. TeCIP, Scuola Superiore SantAnna. © Texas Instruments, Inc ','NumberTitle','off','GraphicsSmoothing','off')

    warning off MATLAB:HandleGraphics:ObsoletedProperty:JavaFrame
    jframe=get(figHnd,'javaframe');
    jIcon=javax.swing.ImageIcon('tecip.jpeg');
    jframe.setFigureIcon(jIcon);
    % set(figHnd, 'MenuBar', 'none');
    set(figHnd, 'Color', [0.8 0.8 0.8]);
    set(figHnd, 'KeyPressFcn', @myKeyPressFcn);
    set(figHnd, 'CloseRequestFcn', @myCloseRequestFcn);

    % set(figHnd,'ResizeFcn',@Resize_clbk);

    pause(0.00001);
    set(jframe,'Maximized',1); 
    pause(0.00001);

    %% Display chirp params
    if show_params == 1
        displayChirpParams(Params);
    end
    guiMonitor.figHnd = figHnd;

    %% Initalize figures
    % X, Y plot
    guiMonitor.detectedObjectsFigHnd = subplot(3,5, [1 2 3 6 7 8 11 12 13]);
    hold on
    axis equal                    
    axis([-dim.max_dist_x dim.max_dist_x 0 dim.max_dist_y])
    xlabel('Distance along lateral axis (meters)');                  
    ylabel('Distance along longitudinal axis (meters)');
    
    % Populate the plots. 
    guiMonitor.detectedObjectsPlotHndA = plot(inf,inf,'g.', 'Marker', '.','MarkerSize',20); hold on;
    guiMonitor.trackedObjPlotHnd = plot(inf,inf,'g.', 'Marker', 'd','MarkerSize',13 ); 
    guiMonitor.detectedObjectsPlotHndB = plot(inf,inf,'c.', 'Marker', '.','MarkerSize',14);
    guiMonitor.clustersPlotHndA = plot(inf*ones(6*MAX_NUM_CLUSTERS,1),inf*ones(6*MAX_NUM_CLUSTERS,1),'c', 'LineWidth',2); 
    guiMonitor.clustersPlotHndB = plot(inf*ones(6*MAX_NUM_CLUSTERS,1),inf*ones(6*MAX_NUM_CLUSTERS,1),'c', 'LineWidth',2); 
    guiMonitor.parkingAssistRangeBinsHnd = plot(inf,inf,'g', 'LineWidth', 2, 'Color', [1 0.5 1]); 
    t = linspace(pi/6,5*pi/6,128);
    plotSemiCircularGrid(dim.max_dist_y);
    title('X-Y Scatter Plot')
    set(gca,'Color',[0 0 0.5]);

    %% R, Rd plot
    guiMonitor.detectedObjectsRngDopFigHnd = subplot('Position',[0.618 0.625 0.375 0.3]);
    hold off
    hold on
    set(gca,'Color',[0 0 0.5]);
    axis([0 dim.max_dist_y -dim.max_vel dim.max_vel])
    xlabel('Range (meters)');
    ylabel('Doppler (m/s)');
    title('Doppler-Range Plot');
    set(gca,'Xcolor',[0.5 0.5 0.5]);
    set(gca,'Ycolor',[0.5 0.5 0.5]);
    guiMonitor.detectedObjectsRngDopPlotHndA = plot(inf,inf,'g.', 'Marker', '.','MarkerSize',10); hold on;
    guiMonitor.trackedObjRngDop = plot(inf,inf,'g.', 'Marker', 'd','MarkerSize',13); hold on;
    guiMonitor.detectedObjectsRngDopPlotHndB = plot(inf,inf,'c.', 'Marker', '.','MarkerSize',10);
    clustersPlotHnd = plot(inf*ones(6*MAX_NUM_CLUSTERS,1),inf*ones(6*MAX_NUM_CLUSTERS,1),'c', 'LineWidth',2); 
    parkingAssistRangeBinsHnd = plot(inf,inf,'g', 'LineWidth', 2, 'Color', [1 0.5 1]); 
    if show_legend
        lgnd = legend('SRR obj', 'MRR/SRR Tracks', 'USRR obj', 'Clusters', 'Park Assist Grid','Location','NorthEastOutside');
        set(lgnd,'color','white');
    end
    
    plotRectGrid(dim.max_dist_y, dim.max_vel);
    
    %% Y, Z plot
    guiMonitor.detectedObjectsYZFigHnd = subplot('Position',[0.618 0.260 0.375 0.3]);
    hold off
    hold on
    set(gca,'Color',[0 0 0.5]);
    axis equal                    
    axis([0 dim.max_dist_y -dim.max_dist_z/4 dim.max_dist_z]) 
    xlabel('Distance along longitudinal axis (meters) ');
    ylabel('Distance along height axis (meters) ');
    title('Y-Z Plot');
    set(gca,'Xcolor',[0.5 0.5 0.5]);
    set(gca,'Ycolor',[0.5 0.5 0.5]);
    guiMonitor.detectedObjectsPlotYZHndA = plot(inf,inf,'g.', 'Marker', '.','MarkerSize',20); hold on;
    guiMonitor.detectedObjectsPlotYZHndB = plot(inf,inf,'c.', 'Marker', '.','MarkerSize',14);
    
    if show_legend
        lgnd = legend('MRR/SRR obj',  'USRR obj','Location','NorthEastOutside');
        set(lgnd,'color','white');
    end
    
    % plotRectGrid(dim.max_dist_y, dim.max_vel);

    %% GUI filtering options
    step = 0.15; offset = 0.05; halfStep = 0.2; height = 0.075; 
    thickness = 0.3; h_thickness = 0.5; width = step; chkbxwidth = step/5;

    h_ui = uipanel('Parent',figHnd,'Title','Display Options.','FontSize',12, 'Units', 'normalized', 'Position',[0.618 0.10 0.274 0.1], 'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized', 'Style','text','Position',[offset (height+h_thickness) width thickness],'String',        'Near View', 'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','checkbox','Position',[(halfStep + offset) (height+h_thickness+0.002) chkbxwidth thickness],'Value',view_range,'Callback',@select_viewRange,'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized', 'Style','text','Position',[(2*step + offset) (height+h_thickness) width thickness],'String',        'Parking ', 'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','checkbox','Position',[(2*step + halfStep + offset) (height+h_thickness+0.002) chkbxwidth thickness],'Value',load_parking_assist,'Callback',@select_parking_assist,'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized', 'Style','text','Position',[(4*step + offset) (height+h_thickness) width thickness],'String',        'USRR Cloud', 'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','checkbox','Position',[(4*step + halfStep + offset) (height+h_thickness+0.002) chkbxwidth thickness],'Value',load_point_cloud_usrr,'Callback',@select_pointcloud_usrr,'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized', 'Style','text','Position',[offset height width thickness],'String',        'SRR Cloud', 'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','checkbox','Position',[(halfStep + offset) (height+0.002) chkbxwidth thickness],'Value',load_point_cloud_srr,'Callback',@select_pointcloud_srr,'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','text','Position',[(2*step + offset) height width thickness],'String',        'Clusters','BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','checkbox','Position',[(2*step + halfStep + offset) (height+0.002) chkbxwidth thickness],'Value',load_clusters,'Callback',@select_clusters,'BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','text','Position',[(4*step + offset) height width thickness],'String',        'Trackers','BackgroundColor', [0.8 0.8 0.8]);
    uicontrol('Parent',h_ui,'Units', 'normalized','Style','checkbox','Position',[(4*step  + halfStep + offset) (height+0.002) chkbxwidth thickness],'Value',load_trackers,'Callback',@select_trackers,'BackgroundColor', [0.8 0.8 0.8]);
    
    if (guiMonitor.stats == 1)
         guiMonitor.statsFigHnd = subplot(3,3, 6);
         guiMonitor.statsPlotHnd = plot(zeros(100,3));
         figIdx = figIdx + 1;
         hold on;
         xlabel('frames');                  
         ylabel('% CPU Load');
         axis([0 100 0 100])
         title('Active and Interframe CPU Load')
         plot([0 0 0; 0 0 0])
         legend('Interframe', 'Active frame', 'GUI')
    end

return



function select_pointcloud_srr(popup,event)
global load_point_cloud_srr;
    load_point_cloud_srr = (popup.Value); 
return

function select_pointcloud_usrr(popup,event)
global load_point_cloud_usrr;
    load_point_cloud_usrr = (popup.Value); 
return

function select_clusters(popup,event)
global load_clusters;
    load_clusters = (popup.Value); 
return

function select_trackers(popup,event)
global load_trackers;
    load_trackers = (popup.Value); 
return

function select_viewRange(popup,event)
global view_range;
    view_range = (popup.Value); 
return

function select_parking_assist(popup,event)
global load_parking_assist;
    load_parking_assist = (popup.Value); 

return
function select_perspective(popup, event)
    global use_perspective_projection 
    use_perspective_projection = popup.Value;
return


function myKeyPressFcn(hObject, event)
    global EXIT_KEY_PRESSED PAUSE_KEY_PRESSED RECORD_KEY_PRESSED
    if lower(event.Key) == 'q'
        EXIT_KEY_PRESSED  = 1;
    elseif lower(event.Key) == 'p'
        PAUSE_KEY_PRESSED  = 1;
    elseif lower(event.Key) == 'r'
        RECORD_KEY_PRESSED == 1;
    end
return

function myCloseRequestFcn(hObject, event)
    global EXIT_KEY_PRESSED
    EXIT_KEY_PRESSED  = 1;
    
return
