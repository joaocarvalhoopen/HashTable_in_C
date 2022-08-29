# HashTable in C
Just to test my current C programming chopsticks :-)

## Description
This is a working implementation of a HashTable or Hashmap or Associative Arrays made in the good ... old C programming language. <br>
This implementation, that I made, isn't optimized for speed, I just wanted to put a lot of stuff in it. <br>
Like I said, just to test my current chopsticks ... they are fine hehehe <br>

## Valgrind
After the second GitHub commit I analyzed the program with Valgrind, and it didn't had any dynamic (heap) memory problem. <br>
<br>

The command to execute Valgrind was: <br>

```
> valgrind -v --tool=memcheck --leak-check=full --log-file=valgrind.log ./a.out

> cat valgrind.log | less
```

<br>
The final output of Valgrind was: <br>

```
..........
..........
==16956== HEAP SUMMARY:
==16956==     in use at exit: 0 bytes in 0 blocks
==16956==   total heap usage: 265 allocs, 265 frees, 8,006,033 bytes allocated
==16956== 
==16956== All heap blocks were freed -- no leaks are possible
==16956== 
==16956== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## License
MIT Open Source License

## Have fun
Best regards, <br>
João Nuno Carvalho <br>
