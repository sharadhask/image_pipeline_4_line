function rgb=yuv2rgb(yuv)
% usage
%        rgb=yuv2rgb(yuv)
% formula from wikipedia
% 
YUVd = double(yuv);
YUVd(:,:,1)=YUVd(:,:,1)-16;
YUVd(:,:,2)=YUVd(:,:,2)-128;
YUVd(:,:,3)=YUVd(:,:,3)-128;
% coefficients come from 65536*inv(rgb2yuv) matrix
RGB(:,:,1)=floor((298*YUVd(:,:,1)-YUVd(:,:,2)+409*YUVd(:,:,3)+128)/256);
RGB(:,:,2)=floor((298*YUVd(:,:,1)-100*YUVd(:,:,2)-210*YUVd(:,:,3)+128)/256);
RGB(:,:,3)=floor((298*YUVd(:,:,1)+519*YUVd(:,:,2)+128)/256);
RGB(RGB>255)=255;
RGB(RGB<0)=0;
rgb=uint8(RGB);