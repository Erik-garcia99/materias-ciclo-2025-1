#------------------------------------------------------------------------------------



# Basic Imports
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.model_selection import train_test_split #serpara los datos 
from sklearn.metrics import confusion_matrix, classification_report #estos los regulatado las claisigfacion y la matriz de confusion
from sklearn.preprocessing import StandardScaler #normalizar los datos 

#------------------------------------------------------------------------------------


def to_classlabel(z):
    return z.argmax(axis = 1) #etiqeuta en la columna



#------------------------------------------------------------------------------------

#y susecion de numeros, 
def one_hot_encode(y):
    n_class = np.unique(y).shape[0] #cunatas clases tiene Y 
    y_encode = np.zeros((y.shape[0], n_class)) #inicialzia la tabla, rengloes(instancias ) columnas numero de clases
    for idx, val in enumerate(y):
        y_encode[idx, val] = 1.0
    return y_encode




#------------------------------------------------------------------------------------

# Define Accuracy
def accuracy(y_true, y_pred):
    acc = np.sum(y_true == y_pred) / len(y_true)
    return acc #presicion 




#------------------------------------------------------------------------------------



class Softmax_Regression_AdamD():
    
    def __init__(self, lambda_param=0.01):
        self.theta = None
        self.lambda_param = lambda_param  # Parámetro de regularización L2
        
    def _softmax(self, Z):
        """Implementación manual de softmax con estabilidad numérica"""
        # Estabilidad numérica: restar el máximo para evitar overflow
        shiftZ = Z - np.max(Z, axis=1, keepdims=True)
        expZ = np.exp(shiftZ)
        return expZ / np.sum(expZ, axis=1, keepdims=True)

    def _loss(self, Y, h, theta):
        """Función de pérdida de entropía cruzada categórica con regularización L2"""
        epsilon = 1e-10  # Para evitar log(0)
        m = Y.shape[0]
        # Pérdida de entropía cruzada
        loss = -np.sum(Y * np.log(np.clip(h, epsilon, 1 - epsilon))) / m
        # Término de regularización L2 (excluir sesgo)
        reg_term = (self.lambda_param / (2 * m)) * np.sum(theta[1:, :]**2)
        return loss + reg_term

    def gradient(self, A, Y, h, theta):
        """Calcula el gradiente con regularización L2"""
        m = A.shape[0]
        grad = (1 / m) * A.T @ (h - Y)
        # Aplicar regularización solo a los pesos (no al sesgo)
        grad[1:, :] += (self.lambda_param / m) * theta[1:, :]
        return grad

    def fit(self, A, y, learning_rate=0.001, beta1=0.9, beta2=0.999, 
            epsilon=1e-8, epochs=100, batch_size=32, show_step=10, 
            stopping_threshold=1e-6, verbose=False):
        """
        Entrenamiento con optimizador AdamD
        
        Parámetros:
        A: Matriz de diseño
        y: Etiquetas
        learning_rate: Tasa de aprendizaje (alpha)
        beta1, beta2: Parámetros de momento
        epsilon: Constante de estabilidad numérica
        epochs: Número máximo de épocas
        batch_size: Tamaño del lote (1=online, len(y)=batch, otro=minilotes)
        show_step: Frecuencia para mostrar progreso
        stopping_threshold: Umbral para parada temprana
        verbose: Mostrar información durante el entrenamiento
        """
        # Convertir y a one-hot encoding
        Y = one_hot_encode(y)
        n_classes = Y.shape[1]
        n_features = A.shape[1]
        n_obs = A.shape[0]
        
        # Inicializar parámetros
        self.theta = np.random.randn(n_features, n_classes) * 0.01
        m = np.zeros_like(self.theta)  # Primer momento
        v = np.zeros_like(self.theta)  # Segundo momento
        t = 0  # Contador de pasos
        
        previous_loss = np.inf
        epoch_losses = []
        
        for epoch in range(epochs):
            # Barajar datos
            permutation = np.random.permutation(n_obs)
            A_shuffled = A[permutation]
            Y_shuffled = Y[permutation]
            
            epoch_loss = 0.0
            n_batches = n_obs // batch_size
            residual = n_obs % batch_size
            total_batches = n_batches + (1 if residual != 0 else 0)
            
            for batch_idx in range(total_batches):
                t += 1
                start = batch_idx * batch_size
                end = start + batch_size
                
                # Manejar el último lote si es más pequeño
                if batch_idx == total_batches - 1 and residual != 0:
                    end = start + residual
                
                A_batch = A_shuffled[start:end]
                Y_batch = Y_shuffled[start:end]
                
                # Calcular softmax y pérdida
                Z_batch = A_batch @ self.theta
                S_batch = self._softmax(Z_batch)
                loss_batch = self._loss(Y_batch, S_batch, self.theta)
                epoch_loss += loss_batch * len(Y_batch)
                
                # Calcular gradiente
                grad = self.gradient(A_batch, Y_batch, S_batch, self.theta)
                
                # Actualización AdamD
                m = beta1 * m + (1 - beta1) * grad
                v = beta2 * v + (1 - beta2) * (grad**2)
                
                # Calcular tasa de aprendizaje adaptativa
                alpha_t = learning_rate * np.sqrt(1 - beta2**t)
                
                # Actualizar parámetros
                self.theta -= alpha_t * m / (np.sqrt(v) + epsilon)
            
            # Pérdida promedio por muestra
            epoch_loss /= n_obs
            epoch_losses.append(epoch_loss)
            
            # Parada temprana
            if epoch > 0 and abs(previous_loss - epoch_loss) < stopping_threshold:
                if verbose:
                    print(f"Early stopping at epoch {epoch}")
                break
                
            previous_loss = epoch_loss
            
            if verbose and epoch % show_step == 0:
                print(f'Epoch: {epoch}, Loss: {epoch_loss:.6f}')
        
        return epoch_losses
                
    # def predict(self, A):
    #     Z = A @ self.theta
    #     S = self._softmax(Z)
    #     return np.argmax(S, axis=1)

    def predict_proba(self, A):
        Z = A @ self.theta
        return self._softmax(Z)




#------------------------------------------------------------------------------------


class softMax_regresor_AdamD_Multiclass():
    def __init__(self, lambda_param=0.01, num_classes=6):
        self.models = []
        self.losses = []
        self.lambda_param = lambda_param
        self.num_classes = num_classes
        
    def fit(self, A, y, **fit_params):
        self.models = []
        self.losses = []
        
        for i in range(self.num_classes):
            y_binary = np.where(y == i, 1, 0)
            
            positive_indices = np.where(y_binary == 1)[0]
            negative_indices = np.where(y_binary == 0)[0]
            
            n_pos = len(positive_indices)
            
            if n_pos > 0:
                neg_selected = np.random.choice(negative_indices, size=n_pos, replace=False)
                balanced_indices = np.concatenate([positive_indices, neg_selected])
                np.random.shuffle(balanced_indices)
                
                A_balanced = A[balanced_indices]
                y_balanced = y_binary[balanced_indices]
            else:
                print(f"Advertencia: Clase {i} tiene 0 muestras positivas")
                A_balanced = A
                y_balanced = y_binary
                
            model = Softmax_Regression_AdamD(lambda_param=self.lambda_param)
            epoch_losses = model.fit(A_balanced, y_balanced, **fit_params)
            self.models.append(model)
            self.losses.append(epoch_losses)
    
    # def predict_proba(self, A):
    #     probas = []
    #     for model in self.models:
    #         # Usar función sigmoide directamente
    #         proba = 1 / (1 + np.exp(-np.dot(A, model.theta)))
    #         probas.append(proba)
    #     return np.array(probas).T

    def predict_proba(self, A):
        probas = []
        for model in self.models:
            # Obtener probabilidades softmax del modelo binario
            proba_matrix = model.predict_proba(A)
            # Para clasificación binaria (One-vs-Rest), tomar la columna de la clase positiva
            proba_class_i = proba_matrix[:, 1] if proba_matrix.shape[1] > 1 else proba_matrix[:, 0]
            probas.append(proba_class_i)
        # Apilar horizontalmente para obtener [muestras x clases]
        return np.column_stack(probas)

    
    def predict(self, A):
        probas = self.predict_proba(A)
        return np.argmax(probas, axis=1)
#------------------------------------------------------------------------------------



# # Read the data
# data = np.loadtxt('admisiones_dataset.txt',delimiter=',')
# inputs = data[:,0:2]
# idx = 2-data[:,2] #restamos el 1 para establecer el categorico, adminitivos - 1 no admitivos - 0 
# targets = np.array(idx, dtype=int)     # codificacion categorica
# # targets = one_hot_encode(labels)      # one hot encode to classlabel

try:
    df = pd.read_csv('dermatology.dat', header=None, na_values='?', delimiter=r'\s+')
    
    inputs = df.iloc[:, 0:34]
    targets = df.iloc[:, 34]
    
    age_mean = inputs[33].mean()
    inputs[33] = inputs[33].fillna(age_mean)
    
    inputs = inputs.astype(float)
    targets = targets.astype(int) - 1  # Convertir clases [1-6] a [0-5]

except Exception as e:
    print(f"Error al leer el archivo: {e}")
    exit()



#------------------------------------------------------------------------------------


# Split the data
# x_train,x_test,y_train,y_test = train_test_split(inputs,targets,test_size=0.40,random_state=1234) # test_size genreta entrenamiento y prueba 

# División de datos
x_train, x_test, y_train, y_test = train_test_split(
    inputs, targets, test_size=0.3, random_state=42
)




#------------------------------------------------------------------------------------
# Normalización
scaler = StandardScaler()
x_train = scaler.fit_transform(x_train)
x_test = scaler.transform(x_test)


#------------------------------------------------------------------------------------

#matrices de disenio, 
A_train = np.c_[np.ones(len(x_train)), x_train]
A_test  = np.c_[np.ones(len(x_test)), x_test]


#------------------------------------------------------------------------------------



# Parámetros para el constructor

#minilot4es
# Hiperparámetros para AdamD
lambda_param = 0.01  # Regularización L2
fit_params = {
    'learning_rate': 1e-4,
    'beta1': 0.9,
    'beta2': 0.999,
    'epsilon': 1e-8,
    'epochs': 1000,
    'batch_size': 128, 
    'show_step': 100,
    'stopping_threshold': 1e-6,
    'verbose': True
}


#online
# lambda_param = 0.01  # Regularización L2
# fit_params = {
#     'learning_rate': 1e-4,
#     'beta1': 0.9,
#     'beta2': 0.999,
#     'epsilon': 1e-8,
#     'epochs': 1000,
#     'batch_size': 1,  
#     'show_step': 100,
#     'stopping_threshold': 1e-4,
#     'verbose': True
# }


#lote completo
    # lambda_param = 0.01  # Regularización L2
    # fit_params = {
    #     'learning_rate': 1e-4,
    #     'beta1': 0.9,
    #     'beta2': 0.999,
    #     'epsilon': 1e-8,
    #     'epochs': 1000,
    #     'batch_size': len(y_train),  
    #     'show_step': 100,
    #     'stopping_threshold': 1e-4,
    #     'verbose': True
    # }
#-----------------------------------------------------------------------------------


# # Build and fit best LR model
# alpha = 0.01 #lr
# maxEpochs = 5000
# batch = 10 #minilotes
# show = 500 #view

# # Build model
# log_model = Logistic_Regression()
# # Fit Model
# theta, batch_loss, epoch_loss = log_model.fit(A_train, y_train, learning_rate=alpha, 
#                                 epochs=maxEpochs, batch_size=batch, show_step = show, verbose=True)


# Entrenamiento del modelo
model = softMax_regresor_AdamD_Multiclass(lambda_param=lambda_param, num_classes=6)
model.fit(A_train, y_train, **fit_params)
#------------------------------------------------------------------------------------



# Predicciones y evaluación
# Predicciones
train_pred = model.predict(A_train)
test_pred = model.predict(A_test)


#------------------------------------------------------------------------------------


# # Evaluación
# print(f"Modo de aprendizaje: Minilotes (tamaño {fit_params['batch_size']})")
# print(f"Regularización L2: lambda={lambda_param}")
# print(f"Épocas completadas: {len(epoch_losses)}/{fit_params['epochs']}")

#------------------------------------------------------------------------------------

# # Calculate accuracy
# train_acc = accuracy(y_train, train_pred)
# print(f'Accuracy on training set: {train_acc}')
#resultados finales
# Resultados en entrenamiento
print("\nRendimiento en entrenamiento (multiclase - AdamD):")
print(f'Accuracy: {accuracy(y_train, train_pred):.4f}')
print("\nMatriz de confusión:")
print(confusion_matrix(y_train, train_pred))
print("\nReporte de clasificación:")
print(classification_report(y_train, train_pred))

#------------------------------------------------------------------------------------




# Calculate metrics - son de entrenamiento 
# cm_train = confusion_matrix(y_train, train_pred)
# train_report = classification_report(y_train, train_pred)

# print("Performance on training set:\n")
# print(f'Confusion Matrix:\n {cm_train}\n')
# print(f'Classification Report:\n {train_report}')

# Resultados en prueba
print("\nRendimiento en prueba (multiclase - AdamD):")
print(f'Accuracy: {accuracy(y_test, test_pred):.4f}')
print("\nMatriz de confusión:")
print(confusion_matrix(y_test, test_pred))
print("\nReporte de clasificación:")
print(classification_report(y_test, test_pred))


#------------------------------------------------------------------------------------

# Gráfica de pérdida
plt.figure(figsize=(12, 6))
for i, loss_list in enumerate(model.losses):
    plt.plot(loss_list, label=f'Clase {i}')
plt.title('Evolución de la Pérdida por Época (AdamD Multiclase)')
plt.xlabel('Época')
plt.ylabel('Pérdida (Entropía Cruzada Binaria)')
plt.legend()
plt.grid(True)
plt.show()


