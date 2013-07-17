A=imread('peppers.png');
%A=imread('fabric.png');
A2=imresize(A,[484,724]);
imshow(A2);
size(A2)
B=zeros(484,724);
B(1:2:end,1:2:end)=A2(1:2:end,1:2:end,2);
B(2:2:end,2:2:end)=A2(2:2:end,2:2:end,2);
B(1:2:end,2:2:end)=A2(1:2:end,2:2:end,1);
B(2:2:end,1:2:end)=A2(1:2:end,2:2:end,3);
%%
Bd=double(B/255);
Bd2p2=Bd.^(2.2);
B2p2 = uint8(255*Bd2p2);
imshow(uint8(B2p2));
%%
B = 4*double(B2p2); % make it 10 bit input
% write out ascii values
fid = fopen('inputGRBG10bitg2p2.pgm','wt');
fprintf(fid,'P2 %3d %3d %4d\n',size(B,2),size(B,1),1023);
for r=1:size(B,1)
    for c=1:size(B,2)
        fprintf(fid,'%4d ',B(r,c));
    end;
    fprintf(fid,'\n');
end;
fclose(fid);
%imwrite(uint16(B),'inputGRBG10bit.pgm','Encoding','ASCII','MaxValue',1023);
