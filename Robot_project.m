% p1=serialport('COM5',9600);
% p2=serialport('COM3',9600);
% if (readline(p2) == '1')
%     fprintf(p1,'1')
%     pause(2)
% end
% if (readline(p2) == '0')
%     fprintf(p1, '0')
%     pause(2)
% end
% Set the serial port object
s1 = serial('COM4');
s2 = serial('COM3');
set(s1, 'Timeout', 0.5);
set(s2, 'Timeout', 0.5);
% Setting serial Port Parameters
% open the port
set(s1, 'BaudRate', 9600);
set(s2, 'BaudRate', 9600);
fopen(s1);
fopen(s2);
s=10;
%Read and display the data sent from Arduino
while(true)
    pause(1);
    if(s == 10)
        pause(1);
fprintf(s1,'1');
s=11;
pause(10);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
    end
if (str2num(data1) == 2)
    fprintf(s2,'2');
    pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
%     disp("opens")
elseif(str2num(data1) == 4)
    fprintf(s2,'4');
    pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
elseif(str2num(data1) == 3)
    fprintf(s1,'3');
        pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
    elseif(str2num(data1) == 7)
    fprintf(s1,'7');
        pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
    elseif(str2num(data1) == 8)
    fprintf(s2,'8');
        pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
elseif(str2num(data1) == 0)
    fprintf(s2,'10');
    pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
end
    if (str2num(data2) == 3)
    fprintf(s1,'3');
        pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
elseif(str2num(data2) == 5)
    fprintf(s1,'5');
    fprintf(s2,'6');
        pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
elseif(str2num(data2) == 9)
    fprintf(s1,'9');
        pause(2);
    data1 = fscanf(s1);
    data2 = fscanf(s2);
    while(isempty(data1) == 1 && isempty(data2) == 1)
            data1 = fscanf(s1);
            data2 = fscanf(s2);  
    end
    end
end

% close the port
fclose(s1);
delete(s1);
clear s1;
fclose(s2);
delete(s2);
clear s2;

