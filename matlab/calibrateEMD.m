function [H, V, R] = calibrateEMD(hRaw, vRaw, rRaw, hCoeff, vCoeff, rCoeff)

H = hCoeff(1) + hCoeff(2)*hRaw + hCoeff(3)*vRaw + hCoeff(4)*hRaw.^2 + hCoeff(5)*vRaw.^2 + hCoeff(6)*hRaw.*vRaw;
V = vCoeff(1) + vCoeff(2)*hRaw + vCoeff(3)*vRaw + vCoeff(4)*hRaw.^2 + vCoeff(5)*vRaw.^2 + vCoeff(6)*hRaw.*vRaw;

R = rCoeff*rRaw;
