:: Setup output name
set BATDIR=%~dp0
set time2=%time: =0%
set time3=%time2:~0,2%%time2:~3,2%%time2:~6,2%
set date2=%date:/=%
set date3=%date2: =%
set OUTDIR=%date3%%time3%
mkdir %OUTDIR%

:: Start profiling
vsinstr.exe %1
vsperfcmd /start:trace /output:%1.vsp
%*
vsperfcmd /shutdown
vsperfreport %1.vsp /output:%OUTDIR% /summary:all

