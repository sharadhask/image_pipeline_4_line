function yuv=rgb2yuv(rgb)
% usage
%        rgb=yuv2rgb(yuv)
% formula from wikipedia
% 
RGBd = double(rgb);
rgb2yuvmat = [66 129 25
              -38 -74 112
              112 -94 -18];
[H,W,p]=size(rgb);
YUV = zeros(size(rgb));
for r=1:H
    for c=1:W
        rgbvec = squeeze(RGBd(r,c,:));
        yuvvec = rgb2yuvmat*rgbvec(:);
        YUV(r,c,1) = floor((yuvvec(1)+128)/256); 
        YUV(r,c,2) = floor((yuvvec(2)+128)/256); 
        YUV(r,c,3) = floor((yuvvec(3)+128)/256); 
    end;
end;

YUV(:,:,1) = YUV(:,:,1)+16;
YUV(:,:,2) = YUV(:,:,2)+128;
YUV(:,:,3) = YUV(:,:,3)+128;

YUV(YUV<0) = 0;
YUV(YUV>255) = 255;
yuv = uint8(YUV);
