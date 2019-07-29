close all
clc
clear all

%% Parameters
nSides = 5;
RingRadius = 110/2;
FrameOffset = 50.5; % For the camera frame's longest dimension
Tolerance = 1.75; % How deep should the frame sit in the ring
ZOffset = 15; % Doesn't matter
AngleOffset = 90;
FinalRadius = RingRadius + FrameOffset/2 - Tolerance;

%% Generate
[PGonX, PGonY, Angles] = nsidedpoly_generator(nSides, FinalRadius, 0, 0, 18*pi/180); % The 18 deg rotation makes one of the sides parallel to the x-axis
[CX, CY, ~] = nsidedpoly_generator(1000, RingRadius, 0, 0, 0);
[FCX, FCY, ~] = nsidedpoly_generator(1000, FinalRadius, 0, 0, 0);
plot([PGonX; PGonX(1)], [PGonY; PGonY(1)]), hold on
plot(CX, CY), hold on
plot(FCX, FCY), hold on
plot(0, 0, 'rx')
axis equal

% Print
Output = [Angles * 180 / pi - AngleOffset, PGonX, PGonY, repmat(ZOffset, 1, nSides)']

%% Calculate angles of vectors from center to vertices

%% nsidedpoly generator function
function [X, Y, Angles] = nsidedpoly_generator(nSides, Radius, OffsetX, OffsetY, RotAngle)
    N = 0:(nSides-1);
    Angles = 2*pi*N/nSides + RotAngle;
    X = Radius * cos(Angles) + OffsetX;
    Y = Radius * sin(Angles) + OffsetY;
    X = X(:);
    Y = Y(:);
    Angles = Angles(:);
end
