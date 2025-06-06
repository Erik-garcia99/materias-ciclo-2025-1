/*



 /*


 el ADC tiene una resolucion de 10 bits

 donde el valor minimo es GND

 y el maximo valor repesentadoo en voltaje en AREF pin,

 opcionalmente AVCC o un voltaje de referencia interno de 1.1V o 2.56V pueden

 conectar al pin AREF escribiendo en los bit REFSn en el registro ADMUX









#include <avr/io.h>




void ADC_ini(void){


    //referncias al AREF PIN, ADC 9
    //adlar =0 , alineado a la derecha
    ADMUX |= (1<<REFS0) | (1<<MUX0);

    //PARA SELECCIONR EL ADC
    ADCSRB | (1<<MUX5);


    //seleccionamos el PS, como veo la unica opcion es el de 128 que nos da 125KHz,
    //el rango adecuado para el ADC trabaje, recorfando que el ADC los primeros 13
    //ciclos en cunato se prede es basura
    //y el ADC tarda cerca de 13 ciclos en realizar una conversion


    //habilidar el ADC, PS = 128
    ADCSRA |= (1<<ADEN) | (7<<ADPS0);


    DIDR2 |=1<<ADC9D; //deshabilitamos la parte digitial el pin asicado al ADC9


}


//leer lo que esta en el ADC

uint16_t read_adc(uint8_t channel){

    //inicar la converiron

    ADCSRA |= (1<<ADSC);

    //cunado se termina la conversion este bit se pone el 0 de nuevo porque lo que hace es realizar
    //una solo conversion, por lo que en cada conversion se debe de activar
    //por lo que ahora esperamos hasta que esta se termine

    while(ADCSRA & (1<<ADSC));


    uint8_t adc_high = ADCH;
    uint8_t adc_low = ADCL;

    return (adc_high << 8) | adc_low;




}







int main(void)
{

    // Insert code

    while(1)
    ;

    return 0;
}

*/


#include <avr/io.h>
#include <util/delay.h> // Para _delay_ms
#include <stdlib.h>     // Necesario para itoa()

// --- Definiciones de Frecuencia y Baud Rate ---
#ifndef F_CPU
#define F_CPU 16000000UL // Frecuencia del CPU, ej. 16 MHz para Arduino Mega
#endif

#define BAUD 9600 // Velocidad en baudios para la comunicación serial
#define UBRR_VALUE (((F_CPU / (16UL * BAUD))) - 1) // Calcula el valor para el registro UBRR

// --- Funciones de UART ---

void uart_init() {
    // Configurar la velocidad en baudios
    //UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    //UBRR0L = (unsigned char)UBRR_VALUE;

    UBRR0 =103;
    // Habilitar el transmisor (TX)
    UCSR0B = (1 << TXEN0) |(1<<RXEN0);



    //PARIDAD PAR - 1 STOP BIT - 8 BITS
    UCSR0C |= (1<<UPM01)| (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit_byte(unsigned char data) {
    // Esperar a que el buffer de transmisión de datos esté vacío
    while (!(UCSR0A & (1 << UDRE0)));
    // Poner el dato en el buffer y enviarlo
    UDR0 = data;
}

void uart_transmit_string(const char *str) {
    while (*str != '\0') {
        uart_transmit_byte(*str);
        str++;
    }
}

// --- Funciones de ADC ---

void setup_adc() {
    // Referencia: AVCC con capacitor en AREF (REFS0=1, REFS1=0)
    // Esto se logra con ADMUX = (1 << REFS0);
    // Alineación a la derecha (ADLAR=0) es el valor por defecto, no es necesario ajustarlo explícitamente si ya está en 0.
    ADMUX |= (1<<REFS0) | (1<<MUX0);

    //PARA SELECCIONR EL ADC
    ADCSRB | (1<<MUX5);

    // Prescalador: 128 (para 16MHz -> 125kHz ADC clock)
    // Habilitar ADC (ADEN=1)
    // Bits ADPS2, ADPS1, ADPS0 a 1 configuran el prescalador a 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Deshabilitar entradas digitales en el pin ADC0 para reducir ruido y consumo.
    DIDR2 |= (1 << ADC9D);
    // Si vas a usar otros pines como ADC1, etc., asegúrate de deshabilitar sus entradas digitales también.
    // DIDR0 |= (1 << ADC1D);
}

uint16_t read_adc(uint8_t channel) {
    // Seleccionar el canal de entrada analógica
    // MUX4:0 (bits 4:0 de ADMUX) para canales 0-7
    // MUX5 (bit 3 de ADCSRB) para canales 8-15
    /*
    if (channel >= 8) { // Si el canal es ADC8 a ADC15 (pin del puerto K)
        ADCSRB |= (1 << MUX5); // Habilita MUX5
        // Ajusta los 4 bits MUX de ADMUX para el canal específico (channel - 8)
        ADMUX = (ADMUX & 0xF0) | (channel - 8);
    } else { // Si el canal es ADC0 a ADC7
        ADCSRB &= ~(1 << MUX5); // Deshabilita MUX5
        // Ajusta los 4 bits MUX de ADMUX para el canal específico
        ADMUX = (ADMUX & 0xF0) | channel;
    }*/

    // Iniciar la conversión ADC
    ADCSRA |= (1 << ADSC);

    // Esperar a que la bandera ADSC se borre (la conversión ha terminado)
    while (ADCSRA & (1 << ADSC));

    // Leer el resultado de 10 bits. ADCL debe leerse primero para bloquear el buffer
    // y asegurar la integridad de los datos.
    uint8_t low_byte = ADCL;
    uint8_t high_byte = ADCH;

    // Combinar los bytes para obtener el valor de 10 bits
    return (high_byte << 8) | low_byte;
}

// --- Función Principal ---

int main() {
    setup_adc(); // Inicializa el ADC
    uart_init(); // Inicializa el UART

    char buffer[10]; // Buffer para almacenar la cadena de texto del valor ADC

     _delay_ms(100); // Pequeña espera
    while (1) {

        uint16_t adc_value = read_adc(0); // Lee el valor del canal ADC0

        // Convertir el valor entero del ADC a una cadena de texto (decimal)
        itoa(adc_value, buffer, 10);

        // Enviar el mensaje por UART
        uart_transmit_string("Valor ADC (Pin A0): ");
        uart_transmit_string(buffer);
        uart_transmit_string("\r\n"); // Enviar un retorno de carro y salto de línea para formatear

        _delay_ms(500); // Esperar 500 milisegundos antes de la siguiente lectura
    }

    return 0;
}
