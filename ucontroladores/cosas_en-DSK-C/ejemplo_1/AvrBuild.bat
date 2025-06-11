@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\ucontroladore\ejemplo_1\labels.tmp" -fI -W+ie -C V3 -o "C:\ucontroladore\ejemplo_1\ejemplo.hex" -d "C:\ucontroladore\ejemplo_1\ejemplo.obj" -e "C:\ucontroladore\ejemplo_1\ejemplo.eep" -m "C:\ucontroladore\ejemplo_1\ejemplo.map" "C:\ucontroladore\ejemplo_1\ejemplo.asm"
