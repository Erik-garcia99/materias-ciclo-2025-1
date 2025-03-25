#include <stdio.h>

int main()
{
    // Definimos la variable ecuacion
    int ecuacion;
    printf("si");
    // Ciclos anidados para x, y, z
    for (int x = 1; x < 256; x++)
    {
        for (int y = 1; y < 256; y++)
        {
            for (int z = 1; z < 256; z++)
            {
                // Ecuación corregida
                ecuacion = x * (6 + (4 * y) + (7 * y * z)) + 10;

                // Condición para verificar si ecuacion es igual a 1648
                if (ecuacion == 16000000)
                {
                    // Imprime los valores de x, y, z cuando la condición se cumple
                    printf("x = %d, y = %d, z = %d\n", x, y, z);
                }
            }
        }
    }

    return 0;
}
