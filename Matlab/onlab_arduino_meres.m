% Önálló laboratórium
% Arduino mérési adatok feldolgozása

clear;
clc;
fprintf("Önálló laboratórium - mérés\n");

% Mérési adatok fájlja
fajlnev = 'onlab_arduino_meres_20180307.csv';
meres = csvread(fajlnev, 1, 0);
meres_db = length(meres);
fprintf("Mérések száma: %d\n", meres_db)

% Mérési adatok csoportosítása
t = meres(:,1); % mérés sorszáma (percenként egy mérés)
fenyerosseg = meres(:,2); % fényer?sség [lux]
legnyomas = meres(:,3); % légnyomás [hPa]
paratartalom = meres(:,5);  % páratartalom [%]
% H?mérsékletek
homerseklet_bmp180 = meres(:,4);  % BMP180 szenzor
homerseklet_dht11 = meres(:,6);  % DHT11
homerseklet_dht11_hic = meres(:,7);  % DHT11 heat index Celsiusban

%% Els? ábra: fényer?sség, légnyomás, páratartalom
figure(1)
%%% Légnyomás
subplot(3,1,1)
plot(t, legnyomas)
title('Legnyomas')
xlabel('ido [s]')
ylabel('legnyomas [hPa]')
%%% Fényer?sség
subplot(3,1,2)
plot(t, fenyerosseg)
title('Fenyerosseg')
xlabel('ido [s]')
ylabel('fenyerosseg [lux]')
%%% Páratartalom
subplot(3,1,3)
plot(t, paratartalom)
title('Paratartalom')
xlabel('ido [s]')
ylabel('paratartalom [%]')

%% Második ábra: h?mérsékletek
figure(2)
hold all
plot(t, homerseklet_bmp180)
plot(t, homerseklet_dht11)
plot(t, homerseklet_dht11_hic)
title('Homerseklet')
xlabel('ido [s]')
ylabel('homerseklet [°C]')
legend('homerseklet bmp180', 'homerseklet dht11', 'homerseklet dht11 hic')
