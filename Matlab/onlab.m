clear;
clc;

data = csvread('onlab.csv', 1, 0);

t = data(:,1); p = data(:,2); T = data(:,3); lux = data(:,4);

legnyomas = [t, p];
homerseklet = [t, T];
fenyerosseg = [t, lux];

figure(1)
subplot(3,1,1)
plot(t, p)
title('Legnyomas')
xlabel('ido [s]')
ylabel('nyomas [hPa]')

subplot(3,1,2)
plot(t, T)
title('Homerseklet')
xlabel('ido [s]')
ylabel('homerseklet [°C]')

subplot(3,1,3)
plot(t, lux)
title('Fenyerosseg')
xlabel('ido [s]')
ylabel('fenyerosseg [lux]')

figure(2)
hold all
plot(t, p)
plot(t, T+750)
