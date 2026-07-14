#ifndef CONJUNTO_H
#define CONJUNTO_H

template <typename T>
class Conjunto {
public:
Conjunto();  
~Conjunto();
void agregar (const T& e);  
void quitar (const T& e);  
bool pertenece (const T& e) const; 
int tamanio() const;  
void unionCon(const Conjunto<T>& otro);// o Conjunto<T> &

private:
struct Nodo {
        T dato;
        Nodo* sig;   
    };
Nodo* primero;
int cantidad;
};