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




opcion=0

while(opcion != 5 ):

    print("1 - 10 variables - rango [-10,10]")
    print("2 - 100 variables - rango [-10,10]")
    print("3 - 1000 variables - rango [-10,10]")
    print("4 - 1000 variables - rango [-100,100]")
    print("5 - salir")
    opcion=int(input(": "))

    if(opcion==1):

        n = 10
        x_opt, perf, epochs, trajectory = trainGDX(
        n, 
        x_range=(-10, 10),
        max_iter=100000,
        show=1000,
        lr_init=1e-3)

        print(f"n={n} performance final = {perf:.4e}")
        print("solution: ",x_opt)
        # print(x_opt)

    elif(opcion==2):
        n = 100
        x_opt, perf, epochs, trajectory = trainGDX(
        n, 
        x_range=(-10, 10),
        max_iter=100000,
        show=1000,
        lr_init=1e-3)

        print(f"n={n} performance final = {perf:.4e}")
        print("solution: ",x_opt)


    elif(opcion==3):
        n = 1000
        x_opt, perf, epochs, trajectory = trainGDX(
        n, 
        x_range=(-10, 10),
        max_iter=100000,
        show=10000,
        lr_init=1e-3)

        print(f"n={n} performance final = {perf:.4e}")
        print("solution: ",x_opt[:100],"\n.......\n", x_opt[900:])

    elif(opcion==4):
        n = 1000
        x_opt, perf, epochs, trajectory = trainGDX(
        n, 
        x_range=(-100, 100),
        max_iter=1000000,
        show=10000,
        lr_init=1e-3)

        print(f"n={n} performance final = {perf:.4e}")
        print("solution: ",x_opt[:50],"\n.......\n", x_opt[950:])

