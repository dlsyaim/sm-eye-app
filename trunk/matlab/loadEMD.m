function [t, H, V, T, R, ACC, TARGET] = loadEMD()
% load emd file format
% [t, H, V, T, R, ACC, TARGET] = loadEMD();
% t : M by 1. sampling time
% H, V, T, R: M by 2. Horizontal, Vertical, Torsional, Pupil Radius of Both
% Eyes
% ACC : M by 5. 2 Acceleration, 3 gyro signals
% Target: Target position(Saccade, Pursuit, etc) or Velocity(OKN)


% get file name
[filename, pathname] = uigetfile('*.emd', 'Pick an EMD');

%file open
fid = fopen(fullfile(pathname, filename));

% read calibration coeff data
hCoeff = zeros(6,2);
vCoeff = zeros(6,2);
rCoeff = zeros(1,2);
%horizontal
fseek(fid, 440, 'bof');
hCoeff(:,1) = fread(fid, 6, 'double')';
vCoeff(:,1) = fread(fid, 6, 'double')';
rCoeff(:,1) = fread(fid, 1, 'double')';
%vertical
fseek(fid, 552, 'bof');
hCoeff(:,2) = fread(fid, 6, 'double')';
vCoeff(:,2) = fread(fid, 6, 'double')';
rCoeff(:,2) = fread(fid, 1, 'double')';

% read raw data
fseek(fid, 1024, 'bof');
dat = fread(fid, inf, 'double')';

%file close
fclose(fid);

count = length(dat)/15;
t = (0:count-1)/120;
H = zeros(count,2);
V = zeros(count,2);
R = zeros(count,2);
T = zeros(count,2);
ACC = zeros(count,5);
TARGET = zeros(count,2);

% read raw data
H(:,1) = dat(1:15:end);
H(:,2) = dat(2:15:end);
V(:,1) = dat(3:15:end);
V(:,2) = dat(4:15:end);
T(:,1) = dat(5:15:end);
T(:,2) = dat(6:15:end);
R(:,1) = dat(7:15:end);
R(:,2) = dat(8:15:end);
ACC(:,1) = dat(9:15:end);
ACC(:,2) = dat(10:15:end);
ACC(:,3) = dat(11:15:end);
ACC(:,4) = dat(12:15:end);
ACC(:,5) = dat(13:15:end);

TARGET(:,1) = dat(14:15:end);
TARGET(:,2) = dat(15:15:end);

%calibrate
[H(:,1), V(:,1), R(:,1)] = calibrateEMD(H(:,1), V(:,1), R(:,1), hCoeff(:,1), vCoeff(:,1), rCoeff(:,1));
[H(:,2), V(:,2), R(:,2)] = calibrateEMD(H(:,2), V(:,2), R(:,2), hCoeff(:,2), vCoeff(:,2), rCoeff(:,2));
