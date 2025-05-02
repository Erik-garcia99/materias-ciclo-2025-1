@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\ucontroladores\XD\labels.tmp" -fI -W+ie -C V3 -o "C:\ucontroladores\XD\random.hex" -d "C:\ucontroladores\XD\random.obj" -e "C:\ucontroladores\XD\random.eep" -m "C:\ucontroladores\XD\random.map" "C:\ucontroladores\XD\random.asm"
