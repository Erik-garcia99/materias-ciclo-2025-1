@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\ucontroladores\listado\labels.tmp" -fI -W+ie -C V3 -o "C:\ucontroladores\listado\listado1.hex" -d "C:\ucontroladores\listado\listado1.obj" -e "C:\ucontroladores\listado\listado1.eep" -m "C:\ucontroladores\listado\listado1.map" "C:\ucontroladores\listado\listado1.asm"
