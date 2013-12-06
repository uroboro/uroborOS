#ifndef BASE_H
#define BASE_H

// Defines

#define ACTIVO_ALTO 1
#define ACTIVO_BAJO 0

#define ON 1
#define OFF 0

#define ENTRADA 0
#define SALIDA 1

//end Defines

// Macros

#define getbit(puerto, pin) \
( ((puerto >> pin) & 0x01)? ON:OFF )

#define setbit(puerto, pin, activo) \
puerto = ( (activo == ACTIVO_ALTO)? (puerto | (0x01 << pin)):(puerto & ~(0x01 << pin)) ) 

// end Macros