% Calculate integration of given function using Gaussien method and
% Using Legendre function as orthogonal polynomials.
% func      : The function to be integrated.
% l_bndr    : left integral boundary
% r_bndr    : right integral bouddary
% precision : required precision, default is 1e-6.
% nr_lg_nodes   : number of used Legendre nodes used in integration, default is 4.
% enable_print  : true for print internal product, default is false.
function y=gl_int(func, l_bndr, r_bndr, precision, nr_lg_nodes, enable_print)

if nargin<3 error('Too few parameters.');
elseif nargin==3 precision=1e-6; nr_lg_nodes=4; enable_print=false;
elseif nargin==4 nr_lg_nodes=4; enable_print=false;
elseif nargin==5 enable_print=false;
elseif nargin>6 error('Too many parameters.');
end

MAX_LEGEND_NODES = 10;
if nr_lg_nodes>MAX_LEGEND_NODES
    warning('MAXIMUM is 10, using default 4');
    nr_lg_nodes = 4;
end
lg_nodes={[0],[-0.5773502692,0.5773502692],[-0.7745966692,0,0.7745966692],[-0.8611363116,-0.3399810436,0.3399810436,0.8611363116],[-0.9061798459,-0.5384693101,0,0.5384693101,0.9061798459],[-0.9324695142,-0.6612093865,-0.2386191861,0.2386191861,0.6612093865,0.9324695142], [-0.9491079123,-0.7415311856,-0.4058451514,0,0.4058451514,0.7415311856,0.9491079123],[-0.9602898565,-0.7966664774,-0.5255324099,-0.1834346425,0.1834346425,0.5255324099,0.7966664774,0.9602898565],[-0.9681602395,-0.8360311073,-0.6133714327,-0.3242534234,0,0.3242534234,0.6133714327,0.8360311073,0.9681602395], [-0.9739065285,-0.8650633667,-0.6794095683,-0.4338953941,-0.1488743390,0.1488743390,0.4338953941,0.6794095683,0.8650633667,0.9739065285]};
lg_coefs={[2],[1,1],[5/9,8/9,5/9], [0.3478548451,0.6521451549,0.6521451549,0.3478548451], [0.2369268851,0.4786286705,0.56+8/900,0.4786286705,0.2369268851],[0.1713244924,0.3607615731,0.4679139346,0.4679139346,0.3607615731,0.1713244924],[0.1294849662,0.2797053915,0.3818300505,0.4179591837,0.3818300505,0.2797053915,0.1294849662],[0.1012285363,0.2223810345,0.3137066459,0.3626837834,0.3626837834,0.3137066459,0.2223810345,0.1012285363],[0.0812743884,0.1806481607,0.2606106964,0.3123470770,0.3302393550,0.3123470770,0.2606106964,0.1806481607,0.0812743884],[0.0666713443,0.1494513492,0.2190863625,0.2692667193,0.2955242247,0.2955242247,0.2692667193,0.2190863625,0.1494513492,0.0666713443]};
lg_tbl=struct('lg_nodes', lg_nodes, 'lg_coefs', lg_coefs);  % structual array of Legendre nodes and Legendre coefficients

MAX_DIVIDE=16;      % Max. 16 times of sub-divide
DivideCount=0;

n=nr_lg_nodes;

StepCount=2;        % initial value by divide the integral sect into 2.
StepLength=(r_bndr - l_bndr)*0.5;

y_temp=0;
for ii=1:n
    y_temp=y_temp+lg_tbl(n).lg_coefs(ii)*(feval(func, ((l_bndr+l_bndr+StepLength)+StepLength*lg_tbl(n).lg_nodes(ii))*0.5)+feval(func, ((l_bndr+StepLength+r_bndr)+StepLength*lg_tbl(n).lg_nodes(ii))*0.5));
end
y_temp=y_temp*StepLength*0.5;   % calculate the initial value by divide the primary integral sect into 2.

if enable_print     % for debug only
    display(y_temp)
end

while DivideCount<=MAX_DIVIDE
    
    StepCount=StepCount*2;          % divide the integral sect using binary dividing method.
    StepLength = StepLength*0.5;
    
    y=0;
    new_l_bndr=l_bndr;
    new_r_bndr=l_bndr+StepLength;
    for ii=1:StepCount              % for each integral sect
        
        temp=0;
        for jj=1:n                  % calculate the integration using n-node Gaussien-Legendre integration
            temp=temp+lg_tbl(n).lg_coefs(jj)*feval(func, ((new_l_bndr+new_r_bndr)+StepLength*lg_tbl(n).lg_nodes(jj))*0.5);
        end
        temp=temp*StepLength*0.5;
                
        new_l_bndr = new_l_bndr+StepLength;     % adjust integral boundary
        new_r_bndr = new_r_bndr+StepLength;
        
        y = y+temp;                 % calculate y
    end
    
    cur_error = abs(y-y_temp)/(1+abs(y));   % calculate current error.
    
        if (enable_print==true)             % if enable_print is true
            display(DivideCount)
            display(y)
            display(cur_error)
        end
        
    if cur_error<=precision                 % if required precision is achieved break calculation
        break;
    else
        y_temp=y;
        DivideCount=DivideCount+1;          % else continue to calculate
        continue;
    end
end

if DivideCount>MAX_DIVIDE                   % if exceeds the maximum loop count
    warning('Maximum loop executed, required precision not achieved.');
end
