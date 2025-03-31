import numpy as np

def objfcn(x):
    # Minima -> f=0 at (1,.....,1)
    n = len(x) # n par
    z = np.zeros((n, 1))
    l2 = np.array(range(0, n, 2)) # índices pares
    l1 = np.array(range(1, n, 2)) # índices impares
    
    # Asegurarse de que las dimensiones sean compatibles
    z[l2] = 10.0 * (x[l1][:, np.newaxis] - (x[l2][:, np.newaxis])**2.0)
    z[l1] = 1.0 - x[l2][:, np.newaxis]
    
    f = z.T @ z
    return f[0, 0]

def objfcnjac(x): 
    # Extended Rosenbrock Jacobian Function 
    n = len(x) # n even 
    Jz = np.zeros((n, n)) 
    z = np.zeros((n, 1)) 
    l2 = np.array(range(0, n, 2)) # índices pares 
    l1 = np.array(range(1, n, 2)) # índices impares 
    
    # Asegurarse de que las dimensiones sean compatibles
    #agregre la funcion newaxis porque estaba tenido probelmas con las deimensiones de los arreglos
    z[l2] = 10.0 * (x[l1][:, np.newaxis] - (x[l2][:, np.newaxis])**2.0)
    z[l1] = 1.0 - x[l2][:, np.newaxis]

    for i in range(n // 2): 
        Jz[2*i, 2*i]     = -20.0 * x[2*i]
        Jz[2*i, 2*i+1]   = 10.0 
        Jz[2*i+1, 2*i]   = -1.0 

    gX = 2.0 * Jz.T @ z 
    normgX = np.linalg.norm(gX) 
    return z, Jz, normgX 


# def objfcn(x):
#     # Minima -> f=0 at (1,.....,1)
#     n = len(x) # n par
#     z = np.zeros((n,1))
#     l2 = np.array(range(0,n,2)) # indice par
#     l1 = np.array(range(1,n,2)) # indice impar
#     z[l2] = 10.0 * (x[l1] - (x[l2])**2.0)
#     z[l1] = 1.0 - x[l2]
#     f = z.T @ z
#     return f[0,0]

# def objfcnjac(x): 
# # Extended Rosenbrock Jacobian Function 
#     n = len(x) # n even 
#     Jz = np.zeros((n,n)) 
#     z = np.zeros((n,1)) 
#     l2 = np.array(range(0,n,2)) # indice par 
#     l1 = np.array(range(1,n,2)) # indice impar 
#     z[l2]=10.0*(x[l1]-(x[l2])**2.0) 
#     z[l1]=1.0-x[l2] 
#     for i in range(n//2): 
#         Jz[2*i,2*i]     = -20.0*x[2*i] 
#         Jz[2*i,2*i+1]   = 10.0 
#         Jz[2*i+1,2*i]   = -1.0 

#     gX = 2.0*Jz.T @ z 
#     normgX = np.linalg.norm(gX) 
#     return z, Jz, normgX 



def trainLM(n,x_range, max_iter, show, lr_init=1e-3, lr_dec=0.1, lr_inc=10.0, gamma_max=1e10, tol=1e-8, verbose=False):


    x = x0.copy() # vector inical, se actualiza durante las iteraciones 
    performance = [] #alamaena el valor de al funcion en cada iteracion 
    gamma = lr_init

    for epoch in range(max_iter+1):
        try:

            F, J, norm_grad = objfcnjac(x) #recibe el jacobian, los residuos y la norma del graditen. 
            #la jacobiana contiene las derivadas parciales con respecto a cada varibale del vector x
            #norm_grad es la norma euclidiana del gradiente de la funcion objetivo.

            #F hace referemic al vector de residuos o desviaciones de la funcion objetivo de rosenbrock

            current_performance = objfcn(x)
            performance.append(current_performance)

            if verbose and (show > 0) and (epoch % show == 0):
                x_norm = np.linalg.norm(x) # se está usando para calcular la norma euclidiana (también conocida como norma L2) del vector x, se obtiene una visión más completa del estado del algoritmo en cada iteración
                print(f"Iter {epoch:5d}: f(x) = {current_performance:.4e} | "
                    f"||grad|| = {norm_grad:.4e} | "
                    f"gama = {gamma:.4e} | "
                    f"||x_norm|| = {x_norm:.4e}")

            if norm_grad < tol:
                if verbose:
                    print(f"Converged at iteration {epoch}")
                return x, performance
                #aproximado a la segunda derivada - estabilizackion numerica
            A = J.T @ J + gamma * np.eye(len(x))
            # se utiliza para crear una matriz identidad de tamaño len(x) x len(x), haciendo que A sea invertible. La matriz identidad es una matriz cuadrada con unos en la diagonal principal y ceros en el resto de posiciones.

            # @ multiplicacion de matriz

            #representa el  gradiente negativo de la funcion objetivo, el negativo indica que quermos minimizar la funcion 
            b = -J.T @ F
            delta = np.linalg.solve(A, b) # utilizado para resolver el sistema de ecuaciones lineales. donde A es una matriz cuadrada y b un arreglo unideimencional o matriz bidimensional 
            '''La solución delta obtenida de esta función representa el paso óptimo o actualización a aplicar a los parámetros actuales x para minimizar la función objetivo. Este paso es crucial en el proceso iterativo del algoritmo de Levenberg-Marquardt, ya que determina cómo deben ajustarse los parámetros para acercarse al mínimo de la función de manera más eficiente.'''
            x_new = x + delta.flatten()  # Asegurar que delta tenga la misma forma que x
            new_performance = objfcn(x_new)

            if new_performance < current_performance:
                x = x_new
                gamma *= lr_dec
            else:
                gamma *= lr_inc

        except np.linalg.LinAlgError:
            #si hay un error numero aunemnta gamma y reintenta
            gamma *= lr_inc
            continue

        if gamma > gamma_max:
            if verbose:
                print("Gamma reached maximum value")
            break

    return x, performance



opcion=0

while(opcion != 5 ):

    print("1 - 10 variables - rango [-10,10]")
    print("5 - salir")

    if(opcion==1):

        
        n = 1000
        x0 = np.random.uniform(-10, 10, n)

        solution, performance = trainLM(
        x0, 
        x_range=(-10,10),
        max_iter=5000,
        show=1,  # Mostrar cada 1000 iteraciones
        verbose=True,
        tol=1e-3  # Tolerancia más estricta para alta precisión
        )   

        print(f"n={n} performance final ={performance[-1]:.2e}")
        print(solution)

    elif(opcion==2):
        print("simon")


