To install the drivers for the moxa UPort 1130:
```
$ wget http://mtsc.moxa.com:8888/Software/HA/USB/UPort_1110_1130_1150/Linux/ver1.3.7/driv_linux_uport1p_v1.3.7_build_13071816.tgz
$ tar -xzvf driv_linux_uport1p_v1.3.7_build_13071816.tgz
$ cd driv_linux_uport1p_v1.3.7_build_13071816.tgz
$ sudo ./mxinstall
```

To use power button:
```
$ sudo cp powerbtn.sh /etc/acpi/
```
