[imdata, map] = imread('C:\\Users\\Beans\\Desktop\\BRIDGE MAN\\Senior Year\\Fall Semester\\Senior Project\\Project\\MattXXXXXXXXXX-2015\\Tree.pgm', 'PGM');
imdata = double(imdata);
z = [];
for i = 1:size(imdata, 1)
    x = imdata(i, :);
    y = fwht(x, 2^ceil(log2(size(imdata, 1))), 'hadamard');
    disp(y);
    z = [z; y];
end
z = mat2gray(z);