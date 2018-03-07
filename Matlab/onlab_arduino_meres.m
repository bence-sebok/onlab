% �n�ll� laborat�rium
% Arduino m�r�si adatok feldolgoz�sa

clear;
clc;
fprintf("�n�ll� laborat�rium - m�r�s\n");

% M�r�si adatok f�jlja
fajlnev = 'onlab_arduino_meres_20180307.csv';
meres = csvread(fajlnev, 1, 0);
meres_db = length(meres);
fprintf("M�r�sek sz�ma: %d\n", meres_db)

% M�r�si adatok csoportos�t�sa
t = meres(:,1); % m�r�s sorsz�ma (percenk�nt egy m�r�s)
fenyerosseg = meres(:,2); % f�nyer?ss�g [lux]
legnyomas = meres(:,3); % l�gnyom�s [hPa]
paratartalom = meres(:,5);  % p�ratartalom [%]
% H?m�rs�kletek
homerseklet_bmp180 = meres(:,4);  % BMP180 szenzor
homerseklet_dht11 = meres(:,6);  % DHT11
homerseklet_dht11_hic = meres(:,7);  % DHT11 heat index Celsiusban

%% Els? �bra: f�nyer?ss�g, l�gnyom�s, p�ratartalom
figure(1)
%%% L�gnyom�s
subplot(3,1,1)
plot(t, legnyomas)
title('Legnyomas')
xlabel('ido [s]')
ylabel('legnyomas [hPa]')
%%% F�nyer?ss�g
subplot(3,1,2)
plot(t, fenyerosseg)
title('Fenyerosseg')
xlabel('ido [s]')
ylabel('fenyerosseg [lux]')
%%% P�ratartalom
subplot(3,1,3)
plot(t, paratartalom)
title('Paratartalom')
xlabel('ido [s]')
ylabel('paratartalom [%]')

%% M�sodik �bra: h?m�rs�kletek
figure(2)
hold all
plot(t, homerseklet_bmp180)
plot(t, homerseklet_dht11)
plot(t, homerseklet_dht11_hic)
title('Homerseklet')
xlabel('ido [s]')
ylabel('homerseklet [�C]')
legend('homerseklet bmp180', 'homerseklet dht11', 'homerseklet dht11 hic')
