# Version: 14 september 2024
### Cache
Command
`sudo perf stat ./main`

Result:
```
Performance counter stats for './main':

           229.12 msec task-clock                       #    0.994 CPUs utilized
                1      context-switches                 #    4.365 /sec
                0      cpu-migrations                   #    0.000 /sec
            78192      page-faults                      #  341.276 K/sec
        623253445      cycles                           #    2.720 GHz
        486502429      instructions                     #    0.78  insn per cycle
         83829449      branches                         #  365.882 M/sec
           224006      branch-misses                    #    0.27% of all branches

      0.230470406 seconds time elapsed

      0.068747000 seconds user
      0.161759000 seconds sys
```

### Valgrind
Command:
`valgrind --leak-check=full --show-leak-kinds=all -s ./main > doc.md`

Result:
```
==18051== Memcheck, a memory error detector
==18051== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==18051== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==18051== Command: ./main
==18051==
==18051==
==18051== HEAP SUMMARY:
==18051==     in use at exit: 320,000,032 bytes in 2 blocks
==18051==   total heap usage: 5 allocs, 3 frees, 320,008,696 bytes allocated
==18051==
==18051== 160,000,016 bytes in 1 blocks are still reachable in loss record 1 of 2
==18051==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==18051==    by 0x109219: main (main.c:100)
==18051==
==18051== 160,000,016 bytes in 1 blocks are still reachable in loss record 2 of 2
==18051==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==18051==    by 0x109417: main (main.c:149)
==18051==
==18051== LEAK SUMMARY:
==18051==    definitely lost: 0 bytes in 0 blocks
==18051==    indirectly lost: 0 bytes in 0 blocks
==18051==      possibly lost: 0 bytes in 0 blocks
==18051==    still reachable: 320,000,032 bytes in 2 blocks
==18051==         suppressed: 0 bytes in 0 blocks
==18051==
==18051== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
