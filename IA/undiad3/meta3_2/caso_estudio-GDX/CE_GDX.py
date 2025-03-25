import numpy as np

# objective Extended Rosenbrock function
'''calcula el valor de la funcion objetivo, la funcion tiene un minumo en el punto (1,1,....,1)
n representa el nuermo de dimensiones del problema 
'''
def objfcn(x):
    # Minima -> f=0 at (1,.....,1)
    n = len(x) # n par
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2] = 10.0 * (x[l1] - (x[l2])**2.0)
    z[l1] = 1.0 - x[l2]
    f = z.T @ z
    return f[0,0]

# Extended Rosenbrock gradient function
def objfcngrad(x):
    n = len(x) # n even
    Jz = np.zeros((n,n))
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2] = 10.0 * (x[l1] - (x[l2])**2.0)
    z[l1] = 1.0 - x[l2]

    for i in range(n//2):
        Jz[2*i, 2*i]     = -20.0 * x[2*i, 0]
        Jz[2*i, 2*i+1]   = 10.0
        Jz[2*i+1, 2*i]   = -1.0

    gX = 2.0 * Jz.T @ z
    return gX

def objfcnjac(x):
    # Extended Rosenbrock Jacobian Function
    n = len(x) # n even
    Jz = np.zeros((n,n))
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2]=10.0*(x[l1]-(x[l2])**2.0)
    z[l1]=1.0-x[l2]
    for i in range(n//2):
        Jz[2*i,2*i] = -20.0*x[2*i]
        Jz[2*i,2*i+1] = 10.0
        Jz[2*i+1,2*i] = -1.0
    gX = 2.0*Jz.T @ z
    normgX = np.linalg.norm(gX)
    return z, Jz, normgX

def trainGDX(n, x_range, max_iter, show, lr_init=1e-3, momentum=0.9, lr_dec=0.7, lr_inc=1.05, max_perf_inc=1.04, tol=1e-8):
    # Inicialización de x usando distribución uniforme
    #vector de parametros inicalizando con valores uniformes en el reango especificado. 
    x = np.random.uniform(x_range[0], x_range[1], size=(n, 1))
    
    performace = objfcn(x) #valor iniciar de la funcion objetivo 
    gX = objfcngrad(x) # gradiente incial 
    delta_x = np.zeros((n,1))
    lr = lr_init
    trayectoria = [x.copy()] # guarda los esstados de x en cada iteracion, utiliza la funcion .copy(), para no alterar los demas valores anteriores a la iteracion actual 
    grad_norm = np.linalg.norm(gX) #norma del gradiente inical, se calcula la norma euclidiana del vector x actual durante la optimización

    for epoch in range(max_iter+1):
        if show > 0 and (epoch % show == 0 or epoch == 0):
            print(f"Iter {epoch}: f(x) = {performace:.4e} |delta_f| = {grad_norm:.4e} lr = {lr:.2e}")

        #condicion de parada. 
        if grad_norm < tol:
            print(f"norma gradiente < {tol:.1e}")
            break

        # Actualizar delta_x usando momentum
        delta_x = momentum * delta_x - (1 - momentum) * lr * gX

        # Guardar el estado actual antes de actualizar
        x_prev = x.copy()
        performace_prev = performace
        delta_x_prev = delta_x.copy()

        # Actualizar x
        x = x_prev + delta_x
        performace = objfcn(x)
        gX = objfcngrad(x)
        grad_norm = np.linalg.norm(gX)

        # Verificar si el rendimiento ha aumentado demasiado
        if performace / performace_prev > max_perf_inc:
            # Revertir los cambios
            x = x_prev
            performace = performace_prev
            delta_x = delta_x_prev
            # Reducir la tasa de aprendizaje
            lr *= lr_dec
            # Recalcular el gradiente
            gX = objfcngrad(x)
            grad_norm = np.linalg.norm(gX)
        else:
            # Si el rendimiento ha mejorado, aumentar la tasa de aprendizaje
            if performace < performace_prev:
                lr *= lr_inc

        trayectoria.append(x.copy())

    return x, performace, epoch, trayectoria


#lecutra esta OK
def lectura():

    import os
    ruta = "C:/Users/erikG/OneDrive/Documents/ciclo2025-1/IA/undiad3/meta3_2/data/challenge00_syntheticdataset22.txt"


    X_data = []
    Y_data = []
    
    with open(ruta, 'r') as archivo:
        for linea in archivo:
            linea_limpia = linea.strip()
            if not linea_limpia: continue
            
            partes = linea_limpia.split('/')
            if len(partes) != 2: continue
            
            x_vals = list(map(float, partes[0].split()))
            y_vals = list(map(float, partes[1].split()))
            
            if len(x_vals) != 2 or len(y_vals) != 2: continue
            
            X_data.append(x_vals)
            Y_data.append(y_vals)
    
    # Convertir a matrices numpy y agregar columna de unos solo a X
    X_matrix = np.array(X_data)
    unos = np.ones((X_matrix.shape[0], 1))
    A = np.hstack((X_matrix, unos))  # Matriz de diseño [X | 1]
    Y = np.array(Y_data)
    
    return A, Y

#hipotesis esta OK
def hipotesis(A, THETA):
    return A @ THETA


def funcion_costo(Y, Y_hat):

    E= Y-Y_hat #calcula el error que seria XI matriz de erroes distribuida normalemnte 
    E_vector_F= E.flatten(order='F') #matriz vectorizada en forma de fila
    E_vector_C=E.flatten(order='F').reshape(-1, 1) #matriz vectroizada en manera de columna
    
    SEE = E_vector_F @ E_vector_C #multiplicacion para calcular el costo
    
    return E, E_vector_F, E_vector_C, SEE


def fcnGrad(A,E):

    gradiete= -2*(A.T @ E)
    return gradiete

def fcnJcbn():

    print()

    
def main():

    A,Y = lectura()

    #A y Y son matrices
    #A es X las entradas y Y las salidas estunadas no son las hipoteticas 

    print(A) #matriz de diseño 
    print("\n")
    print(Y) #matriz de salidas estimadas
    print("\n")
    
    phi = A.shape[1]
    print(type(phi))
    theta = np.random.randn(phi, Y.shape[1]) #matriz
    print("tipo de dato de theta", type(theta),"\ndatos: \n", theta )
    Y_hip= hipotesis(A,theta)

    print(type(Y_hip))

    #termino de error 

    xi,xi_F,xi_C, SEE = funcion_costo(Y, Y_hip)
    print("funcion costo\n  ")
    print(type(xi)) # XI es una matriz 
    print(xi)

    print("\n")
    print(type(xi_F)) # XI es una matriz 
    print(xi_F)

    
    print(type(xi_C)) # XI es una matriz 
    print(xi_C)

    print("\n")
    print(type(SEE))
    print(SEE)

    print("funcion gradinte")
    gdX= fcnGrad(A, xi)

    print(type(gdX))
    print(gdX)





main()

