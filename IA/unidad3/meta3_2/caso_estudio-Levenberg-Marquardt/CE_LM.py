import numpy as np

# def objfcn(x):
#     # Minima -> f=0 at (1,.....,1)
#     n = len(x) # n par
#     z = np.zeros((n, 1))
#     l2 = np.array(range(0, n, 2)) # índices pares
#     l1 = np.array(range(1, n, 2)) # índices impares
    
#     # Asegurarse de que las dimensiones sean compatibles
#     z[l2] = 10.0 * (x[l1][:, np.newaxis] - (x[l2][:, np.newaxis])**2.0)
#     z[l1] = 1.0 - x[l2][:, np.newaxis]
    
#     f = z.T @ z
#     return f[0, 0]

# def objfcnjac(x): 
#     # Extended Rosenbrock Jacobian Function 
#     n = len(x) # n even 
#     Jz = np.zeros((n, n)) 
#     z = np.zeros((n, 1)) 
#     l2 = np.array(range(0, n, 2)) # índices pares 
#     l1 = np.array(range(1, n, 2)) # índices impares 
    
#     # Asegurarse de que las dimensiones sean compatibles
#     #agregre la funcion newaxis porque estaba tenido probelmas con las deimensiones de los arreglos
#     z[l2] = 10.0 * (x[l1][:, np.newaxis] - (x[l2][:, np.newaxis])**2.0)
#     z[l1] = 1.0 - x[l2][:, np.newaxis]

#     for i in range(n // 2): 
#         Jz[2*i, 2*i]     = -20.0 * x[2*i]
#         Jz[2*i, 2*i+1]   = 10.0 
#         Jz[2*i+1, 2*i]   = -1.0 

#     gX = 2.0 * Jz.T @ z 
#     normgX = np.linalg.norm(gX) 
#     return z, Jz, normgX 

########################################################################
################################################################################
#funciones para la meta 3.2

#funcion de lectura OK
#funcion de la hipotesis OK
#funcoin del costo OK
#funcion del gradiente

#lecutra esta OK
#usamos la que no los serpara
def lectura(URL):
    X_data = []
    Y_data = []
    with open(URL, 'r') as archivo:
        for linea in archivo:
            linea_limpia = linea.strip()
            if not linea_limpia:
                continue
            # Separar por espacios y dividir en predictores (primeros 2) y respuestas (últimos 2)
            valores = list(map(float, linea_limpia.split()))
            if len(valores) != 4:
                continue  # Cada línea debe tener 4 números: x1, x2, y1, y2
            X_data.append(valores[:2])  # Primeros dos: predictores
            Y_data.append(valores[2:])  # Últimos dos: respuestas
    
    # Construir matriz de diseño A = [X | 1]
    X_matrix = np.array(X_data)
    unos = np.ones((X_matrix.shape[0], 1))
    A = np.hstack((X_matrix, unos))
    Y = np.array(Y_data)
    return A, Y




#hipotesis esta OK
def hipotesis(A, THETA):
    return A @ THETA


def funcion_costo(Y, Y_hat, A):

    E= Y-Y_hat #calcula el error que seria XI matriz de erroes distribuida normalemnte 

    E_vector_F= E.flatten(order='F') #matriz vectorizada en forma de fila
    E_vector_C=E.flatten(order='F').reshape(-1, 1) #matriz vectroizada en manera de columna
    
    #SEE = E_vector_F @ E_vector_C #multiplicacion para calcular el costo

    SEE =np.sum(E**2)
    g=Y.shape[0] #entradas
    m=Y.shape[1] #salidas

    MSE = SEE/(g*m)
    RMSE = np.sqrt(MSE)     
    return E, SEE, MSE, RMSE


def fcnGrad(A,E):
    return -2*A.T @ E



def calcular_r2(Y_real, Y_pred):
    ss_res = np.sum((Y_real - Y_pred)**2)
    ss_tot = np.sum((Y_real - np.mean(Y_real, axis=0))**2)
    return 1 - (ss_res / ss_tot)


def calcular_jacobiana(A, m):
    q, g = A.shape
    J = np.zeros((q*m, g*m))
    
    # Construir matriz diagonal por bloques
    for j in range(m):
        fila_inicio = j*q
        col_inicio = j*g
        J[fila_inicio:fila_inicio+q, col_inicio:col_inicio+g] = -A
        
    return J



def objfcn_reg(theta_vec, A, Y):
    g = A.shape[1]
    m = Y.shape[1]
    theta = theta_vec.reshape((g, m), order='F')
    Y_hat = hipotesis(A,theta)
    E = Y - Y_hat
    return np.sum(E**2)


def objfcnjac_reg(theta_vec, A, Y):
    g = A.shape[1]
    m = Y.shape[1]
    theta = theta_vec.reshape((g, m), order='F')
    Y_hat = hipotesis(A,theta)
    E = Y - Y_hat
    residuals = E.flatten(order='F')
    J = calcular_jacobiana(A, m)
    gX = 2.0 * J.T @ residuals
    normgX = np.linalg.norm(gX)
    return residuals, J, normgX




##################################################################################

def trainLM(x0,A,Y, max_iter, show, lr_init=1e-3, lr_dec=0.1, lr_inc=10.0, gamma_max=1e10, tol=1e-8, verbose=False):


    x = x0.copy() # vector inical, se actualiza durante las iteraciones 
    performance = [] #alamaena el valor de al funcion en cada iteracion 
    gamma = lr_init

    for epoch in range(max_iter):
        try:

            resisual, J, norm_grad = objfcnjac_reg(x,A,Y)
            current_performance=objfcn_reg(x,A,Y)
            performance.append(current_performance)

            if verbose and (epoch % show ==0):
                
                print(f"Iter {epoch:5d}: costo = {current_performance:.4e} | ||grad|| ={norm_grad:.4e} | gamma = {gamma:.4e}")

            if norm_grad < tol:
                if verbose:
                    print(f"Converged at iteration {epoch}")
                return x, performance

            A_lm = J.T @ J + gamma * np.eye(len(x))
            b = -J.T @ resisual
            delta = np.linalg.solve(A_lm, b) # utilizado para resolver el sistema de ecuaciones lineales. donde A es una matriz cuadrada y b un arreglo unideimencional o matriz bidimensional 
           
            x_new = x + delta  # Asegurar que delta tenga la misma forma que x
            new_performance = objfcn_reg(x_new,A,Y)

            if new_performance < current_performance:
                x = x_new
                gamma *= lr_dec
            else:
                gamma *= lr_inc

            if gamma > gamma_max:
                if verbose:
                    print("Gamma reached maximum value")
                break

        except np.linalg.LinAlgError:
            gamma *= lr_inc
            continue

        if gamma > gamma_max:
            if verbose:
                print("Gamma reached maximum value")
            break

    return x, performance


def main():


    URL="data/challenge00_syntheticdataset22.txt"
 


    A,Y = lectura(URL)


    print(A) #matriz de diseño 
    print("\n")
    print(Y) #matriz de salidas estimadas
    print("\n")

    g = A.shape[1]
    m = Y.shape[1]
    
    # Inicializar parámetros
    Theta_initial = np.zeros((g, m))
    x0 = Theta_initial.flatten(order='F')
    
    # Entrenar modelo
    theta_opt, performance = trainLM(x0, A, Y, max_iter=1000, show=100, tol=1e-6)
    
    # Resultados
    Theta_opt = theta_opt.reshape((g, m), order='F')
    Y_hat = A @ Theta_opt
    r2 = calcular_r2(Y, Y_hat)
    print("\nCoeficientes optimos:")
    print(Theta_opt)
    print(f"\nR_2: {r2:.4f}")





main()


# opcion=0

# while(opcion != 5 ):

#     print("1 - 10 variables - rango [-10,10]")
#     print("2 - 100 variables - rango [-10,10]")
#     print("3 - 1000 variables - rango [-10,10]")
#     print("4 - 1000 variables - rango [-100,100]")
#     print("5 - salir")
#     opcion=int(input(": "))

#     if(opcion==1):

        
#         n = 10
#         x0 = np.random.uniform(-10, 10, n)

#         solution, performance = trainLM(
#         x0, 
#         x_range=(-10,10),
#         max_iter=10000,
#         show=1000,  # Mostrar cada 1000 iteraciones
#         verbose=True,
#         tol=1e-3  # Tolerancia más estricta para alta precisión
#         )   

#         print(f"n={n} performance final ={performance[-1]:.2e}")
#         print("solucion :", solution)
#         #print(solution)

#     elif(opcion==2):
#         n = 100
#         x0 = np.random.uniform(-10, 10, n)

#         solution, performance = trainLM(
#         x0, 
#         x_range=(-10,10),
#         max_iter=10000,
#         show=1000,  # Mostrar cada 1000 iteraciones
#         verbose=True,
#         tol=1e-3  # Tolerancia más estricta para alta precisión
#         )   

#         print(f"n={n} performance final ={performance[-1]:.2e}")
#         print("solucion :", solution)

#     elif(opcion==3):
#         n = 1000
#         x0 = np.random.uniform(-10, 10, n)

#         solution, performance = trainLM(
#         x0, 
#         x_range=(-10,10),
#         max_iter=5000,
#         show=1000,  # Mostrar cada 1000 iteraciones
#         verbose=True,
#         tol=1e-3  # Tolerancia más estricta para alta precisión
#         )   

#         print(f"n={n} performance final ={performance[-1]:.2e}")
#         print("solucion :", solution[:100],"\n.......\n", solution[900:])

#     elif(opcion==4):
#         n = 10000
#         x0 = np.random.uniform(-100, 100, n)

#         solution, performance = trainLM(
#         x0, 
#         x_range=(-100,1000),
#         max_iter=5000,
#         show=1000,  # Mostrar cada 1000 iteraciones
#         verbose=True,
#         tol=1e-3  # Tolerancia más estricta para alta precisión
#         )   

#         print(f"n={n} performance final ={performance[-1]:.2e}")
#         print("solucion :", solution[:100],"\n.......\n", solution[900:])

