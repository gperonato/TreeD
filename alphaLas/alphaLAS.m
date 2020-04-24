% Create a mesh from a LAS dataset using ?-shapes
% Copyright ?2017 Ecole polytechnique f?d?rale de Lausanne - EPFL
% Laboratory of Interdisciplinary Performance in Design - LIPID
% @license GNU GPLv3 https://www.gnu.org/licenses/gpl-3.0.en.html

% Author: Giuseppe Peronato, <giuseppe.peronato@epfl.ch>
% Latest modified on 19.07.2017

% Based on a script by Matthew Parkan - EPFL LASIG
% Original concept implemented in solar energy assessments:
% "3D-modeling of vegetation from LiDAR point clouds and assessment of its impact on fa?ade solar irradiation"
% http://dx.doi.org/10.5194/isprs-archives-XLII-2-W2-67-2016

% Dependencies:
% - Digital Forestry Toolbox (DFT) by Matthew Parkan
%   https://github.com/mparkan/Digital-Forestry-Toolbox
% - WriteOBJ function by Giuseppe Peronato

clc
clear 
close all
opengl hardware

% Set parameters

% DFT directory
addpath(genpath('C:\Users\giupe250\Documents\MATLAB\mparkan-Digital-Forestry-Toolbox-01e26d8\scripts\')) %path to DFT scripts

% IO
idir = 'C:\Users\giupe250\Data\LiDAR_High\Vegetation_tiles\cleaned_100cm\'; %directory with input *.las files
odir = 'C:\Users\giupe250\Data\LiDAR_High\Vegetation_tiles_OBJ\cleaned_100cm\'; %directory with output *.obj files
prefix = 'tile'; %optional prefix to output files

% Alpha-shape https://en.wikipedia.org/wiki/Alpha_shape
radius = 0.75; %alpha-radius
holethreshold = 0; % area of the largest hole to fill
regionthreshold = 1; % minimum volume

% LiDAR
class = 5; % LiDAR class: class 5  usually corresponds to high vegetation


%Script
files = dir(strcat(idir,'*.las')); % list the *.las files in the input directory

%iterate over each las file
for i = 1:length(files) %loop over all files
    filename = strrep(files(i).name,'.las',''); %filename without extension
    display(sprintf('Converting file %s', filename));
    
    opt.input.file.points = strcat(idir,filename,'.las'); %input *.las fle
    

    pc = LASread(opt.input.file.points); %pointcloud

    xyz = [pc.record.x, pc.record.y, pc.record.z];
    classification = pc.record.classification;
    
    clear pc;
    
    idxl_veg = ismember(classification, class); %class 5  usually corresponds to high vegetation

    shp = alphaShape(xyz(idxl_veg,:), ...
        radius, ...
        'HoleThreshold', holethreshold, ...
        'RegionThreshold', regionthreshold);

    [tri, pts] = boundaryFacets(shp); %Extract the boundaries of the hulls

    WriteOBJ(pts,tri,strcat(odir,prefix,filename,'.obj'),"Vegetation");
    
    clear opt;
    %clear xyz;
    clear classification;
    %clear idxl_veg;
    clear shp;
    clear pts;
    clear tri

    
    display(sprintf('Conversion of file %s completed.\n', filename));
end
