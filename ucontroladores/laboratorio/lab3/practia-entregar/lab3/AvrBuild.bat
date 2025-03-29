@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\ucontroladores\laboratorio\lab3\labels.tmp" -fI -W+ie -C V3 -o "C:\ucontroladores\laboratorio\lab3\Lab3.hex" -d "C:\ucontroladores\laboratorio\lab3\Lab3.obj" -e "C:\ucontroladores\laboratorio\lab3\Lab3.eep" -m "C:\ucontroladores\laboratorio\lab3\Lab3.map" "C:\ucontroladores\laboratorio\lab3\Lab3.asm"
