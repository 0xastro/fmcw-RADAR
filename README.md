# fmcw-RADAR
[mmWave based fmcw radar design files] based on AWR1843 chip operating at 76-GHz to 81-GHz.

### Objective
Test & Evaluate the performance of TI mmWave Radar system using the AWR1843 EVM development board.

In this system we detect specifically objects within close proximity the radar and generally evaluate different ranges profiles short, medium and long for further application.

We use TI AWR1843 EVM, algorithms run on-board the single-chip device to create Range-Azimuth and Range-Elevation heat-maps, then performs object detection with CFAR, angle of arrival estimation and clustering on configured range rows in the heatmaps.

### Future Work
Porting the system to be deployed in Localization and exploration services.
> DARPA competition
