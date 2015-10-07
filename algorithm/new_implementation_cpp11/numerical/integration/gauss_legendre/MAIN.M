x=linspace(0,10,100);
y0=zeros(1,length(x));
y1=zeros(1, length(x));

for ii=1:length(x)
    y0(ii)=test(x(ii));
    y1(ii)=gl_int('test', 0, x(ii));
end

figure
plot(x, y0)
figure
plot(x, y1)
figure
plotyy(x, y0, x, y1)
