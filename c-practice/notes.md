## OS Notes
### C
signed vs unsigned -> val has a sign (unsigned assume positive)

memory address of var 
&varName

You should also note that &myAge is often called a "pointer". A pointer basically stores the memory address of a variable as its value. To print pointer values, we use the %p format specifier.

**
 int myAge = 43;     // An int variable
int* ptr = &myAge;  // A pointer variable, with the name ptr, that stores the address of myAge

// Output the value of myAge (43)
printf("%d\n", myAge);

// Output the memory address of myAge (0x7ffe5367e044)
printf("%p\n", &myAge);

// Output the memory address of myAge with the pointer (0x7ffe5367e044)
printf("%p\n", ptr);
**
dereference a pointer, gets the value store at memory address of pointer
printf("%d\n", *ptr);

get memory addy of each indice in arr
for (i = 0; i < 4; i++) {
  printf("%p\n", &myNumbers[i]);
}

** arrays
in c, name off arr is pointer in arr to indice 0

** fork
returns:
-1 = error
0 = child proc
> 0 = parent (fork returns pid of child)

** pipes

